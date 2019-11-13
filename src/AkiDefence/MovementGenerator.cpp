#include"MovementGenerator.h"
#include"../core/Constant.h"
std::string MovementGenerator::printMovement(Movement movement)
{
	std::stringstream ss;
	const std::string dir[4] = { "↑","↓","←","→" };
	if (movement.type == MovementType::Move)
	{
		if (movement.direction == 4)
			ss << "原地不动";
		else
			ss << "向" << dir[movement.direction] << "移动";
	}
	else if (movement.type == MovementType::Throw)
		ss << "向" << dir[movement.direction] << "投出红薯，距离为" << movement.distance;
	else if (movement.type == MovementType::Get)
		ss << "从" << dir[movement.direction] << "拿一个红薯";
	else if (movement.type == MovementType::Put)
		ss << "向" << dir[movement.direction] << "放一个红薯";
	else if (movement.type == MovementType::Eat)
		ss << "吃掉一个红薯";
	return ss.str();
}
std::vector<Movement>MovementGenerator::getEffectiveMovement(GameInfo state,int who)
{
	std::vector<Movement> res;
	res.reserve(8);
	const int side = who;
	Movement movement= Movement::createMovementStay();
	if (state.player[side].hp <= 0)
	{
		res.push_back(movement);
		return res;
	}
	for (int i = 0; i <= 4; ++i)
	{
		if (i == 4)
		{
			res.push_back(Movement::createMovementStay());
			break;
		}
		movement.direction = i;
		Vec2i newPos = state.player[side].position + cns::delta[i];
		if (cns::outofRange(newPos, state.map.size()))
			continue;
		if (state.map[newPos.x][newPos.y].type == GridType::Wall)
			continue;
		if (state.map[newPos.x][newPos.y].type == GridType::Tree)
			continue;
		if (state.map[newPos.x][newPos.y].type == GridType::Pile)
		{
			if (side == SHIZUHA)
			{
				movement.type = MovementType::Move;
				res.push_back(movement);
			}
			if (state.player[side].have<state.bucketVolume[side])
			{
				movement.type = MovementType::Get;
				res.push_back(movement);
			}
			continue;
		}
		if (state.map[newPos.x][newPos.y].type == GridType::Shrine)
		{
			if (state.player[side].have&&side==MARISA)
			{
				movement.type = MovementType::Put;
				res.push_back(movement);
			}
			continue;
		}
		//判断启动的机关
		std::vector<int> activeColor;
		for (int p = 0; p < state.playerCount; ++p)
		{
			if (state.player[p].hp > 0)
			{
				if (state.map[state.player[p].position.x][state.player[p].position.y].type == GridType::Trigger)
					activeColor.push_back(state.map[state.player[p].position.x][state.player[p].position.y].tag);
			}
		}
		bool colorActived = false;
		for (auto color : activeColor)
			if (color == state.map[newPos.x][newPos.y].tag)
				colorActived = true;
		bool someOneStandOnThere = false;
		for (int p = 0; p < state.playerCount; ++p)
		{
			if (state.player[p].hp > 0 && state.player[p].position == newPos)
			{
				someOneStandOnThere = true;
				break;
			}
		}
		if (state.map[newPos.x][newPos.y].type == GridType::MoveableWall && !someOneStandOnThere && colorActived)
			continue;
		movement.type = MovementType::Move;
		res.push_back(movement);
	}
	bool shizuhaInPile = (side == SHIZUHA)&&(state.map[state.player[SHIZUHA].position.x][state.player[SHIZUHA].position.y].type == GridType::Pile);
	if (state.player[side].have|| shizuhaInPile)
	{
		//吃红薯
		if (state.player[side].hp < state.maxHp[side])
		{
			movement.type = MovementType::Eat;
			res.push_back(movement);
		}
		//投掷红薯
		std::vector<Vec2i> usefulPosition;
		for (int p = 0; p < state.playerCount; ++p)
		{
			if (state.player[p].hp<=0||p == side)continue;
			for (int i = 0; i <= 4; ++i)
			{
				Vec2i newPos = state.player[p].position + cns::delta[i];
				if (!cns::outofRange(newPos, state.map.size())&&\
					state.map[newPos.x][newPos.y].type != GridType::Wall&&\
					state.map[newPos.x][newPos.y].type != GridType::Shrine&&\
					state.map[newPos.x][newPos.y].type != GridType::Tree\
					)
				{
					bool haveNotAdded = true;
					for (auto item : usefulPosition)
						if (item == newPos)
						{
							haveNotAdded = false;
							break;
						}
					if (haveNotAdded)
						usefulPosition.push_back(newPos);
				}
					
			}
		}
		//前两层循环为投到哪个地方
		for (int j = 0; j < 4; ++j)
		{
			for (int k = 1; k <= 2; ++k)
			{
				if (shizuhaInPile&&k == 2)continue;
				Vec2i tarPos = state.player[side].position + cns::delta[j] * k;
				for (int i = 0; i < usefulPosition.size(); ++i)
				{
					if (usefulPosition[i] == tarPos)
					{
						movement.type = MovementType::Throw;
						movement.direction = j;
						movement.distance = k;
						res.push_back(movement);
					}
				}
			}
		}
	}
	return res;
}
/*
std::vector<Movement> MovementGenerator::getMovementForMarisa(GameInfo state)
{
	std::vector<Movement> res;
	Movement movement;
	for (int i = 0; i <= 4; ++i)
	{
		if (i == 4)
		{
			res.push_back(Movement::createMovementStay());
			break;
		}
		movement.direction = i;
		Vec2i newPos = state.marisa.position + cns::delta[i];
		if (cns::outofRange(newPos, state.map.size()))
			continue;
		if (state.map[newPos.x][newPos.y].type == GridType::Wall)
			continue;
		if (state.map[newPos.x][newPos.y].type == GridType::Pile)
		{
			if (!state.marisa.have)
			{
				movement.type = MovementType::Get;
				res.push_back(movement);
			}
			continue;
		}
		if (state.map[newPos.x][newPos.y].type == GridType::Shrine)
		{
			if (state.marisa.have)
			{
				movement.type = MovementType::Put;
				res.push_back(movement);
			}
			continue;
		}
		//判断启动的机关
		int64_t colorUp = 0;
		if (state.minoriko.hp > 0)
		{
			if (state.map[state.minoriko.position.x][state.minoriko.position.y].type == GridType::Trigger)
				colorUp |= state.map[state.minoriko.position.x][state.minoriko.position.y].tag;
		}
		if (state.map[state.marisa.position.x][state.marisa.position.y].type == GridType::Trigger)
			colorUp |= state.map[state.marisa.position.x][state.marisa.position.y].tag;
		if (state.map[newPos.x][newPos.y].type == GridType::MoveableWall && (state.map[newPos.x][newPos.y].tag & colorUp))
			continue;
		movement.type = MovementType::Move;
		res.push_back(movement);
	}
	if (state.marisa.have)
	{
		//吃红薯
		if (state.marisa.hp < state.marisaMaxHp)
		{
			movement.type = MovementType::Eat;
			res.push_back(movement);
		}
		//投掷红薯
		for (int i = 0; i <= 4; ++i)
		{
			Vec2i newPos = state.minoriko.position + cns::delta[i];
			bool ok = false;
			for (int j = 0; j < 4; ++j)
			{
				for (int k = 1; k <= 2; ++k)
				{
					Vec2i tarPos = state.marisa.position + cns::delta[j] * k;
					if (newPos == tarPos)
					{
						ok = true;
						movement.type = MovementType::Throw;
						movement.direction = j;
						movement.distance = k;
						res.push_back(movement);
						break;
					}
				}
				if (ok)break;
			}
		}
	}
	return res;
}
std::vector<Movement> MovementGenerator::getMovementForMinoriko(GameInfo state)
{
	std::vector<Movement> res;
	Movement movement = Movement::createMovementStay();
	if (state.minoriko.hp <= 0)
	{
		res.push_back(movement);
		return res;
	}
	for (int i = 0; i <= 4; ++i)
	{
		if (i == 4)
		{
			res.push_back(Movement::createMovementStay());
			continue;
		}
		movement.direction = i;
		Vec2i newPos = state.minoriko.position + cns::delta[i];
		if (cns::outofRange(newPos, state.map.size()))
			continue;
		if (state.map[newPos.x][newPos.y].type == GridType::Wall)
			continue;
		if (state.map[newPos.x][newPos.y].type == GridType::Pile)
		{
			if (!state.minoriko.have)
			{
				movement.type = MovementType::Get;
				res.push_back(movement);
			}
			continue;
		}
		if (state.map[newPos.x][newPos.y].type == GridType::Shrine)
		{
			if (state.minoriko.have)
			{
				movement.type = MovementType::Put;
				res.push_back(movement);
			}
			continue;
		}
		//判断启动的机关
		int64_t colorUp = 0;
		if (state.minoriko.hp > 0)
		{
			if (state.map[state.minoriko.position.x][state.minoriko.position.y].type == GridType::Trigger)
				colorUp |= state.map[state.minoriko.position.x][state.minoriko.position.y].tag;
		}
		if (state.map[state.marisa.position.x][state.marisa.position.y].type == GridType::Trigger)
			colorUp |= state.map[state.marisa.position.x][state.marisa.position.y].tag;
		if (state.map[newPos.x][newPos.y].type == GridType::MoveableWall && (state.map[newPos.x][newPos.y].tag & colorUp))
			continue;
		movement.type = MovementType::Move;
		res.push_back(movement);
	}
	if (state.minoriko.have)
	{
		//吃红薯
		if (state.minoriko.hp < state.minorikoMaxHp)
		{
			movement.type = MovementType::Eat;
			res.push_back(movement);
		}
		//投掷红薯
		for (int i = 0; i <= 4; ++i)
		{
			Vec2i newPos = state.marisa.position + cns::delta[i];
			bool ok = false;
			for (int j = 0; j < 4; ++j)
			{
				for (int k = 1; k <= 2; ++k)
				{
					Vec2i tarPos = state.minoriko.position + cns::delta[j] * k;
					if (newPos == tarPos)
					{
						ok = true;
						movement.type = MovementType::Throw;
						movement.direction = j;
						movement.distance = k;
						res.push_back(movement);
						break;
					}
				}
				if (ok)break;
			}
		}
	}
	return res;
}
*/
#include"Game.h"
#include"../core/Constant.h"
#include<iostream>
GameInfo Game::getInfo(int who)
{
	GameInfo res;
	res.minoriko = minoriko;
	res.marisa = marisa;
	res.round = round;
	res.roundLimit = roundLimit;
	res.map = map;
	res.score = score;
	res.who = who;
	res.collisionDamage = collisionDamage;
	res.attackDamage = attackDamage;
	res.trapDamage = trapDamage;
	res.recoverHp = recoverHp;
	res.marisaMaxHp=marisaMaxHp;
	res.minorikoMaxHp= minorikoMaxHp;
	return res;
}
const char gridStr[10] = { ' ','*','S','#','.','@','_' };
void Game::printInfo()
{
	auto info = this->getInfo(0);
	//cout << (info.who ? "雾雨魔理沙" : "秋穰子") << endl;
	cout << "<时间> 第" << info.round << "秒，还剩下" << info.roundLimit - info.round << "秒" << endl;
	cout << "<得分> 已搬运:" << info.score << endl;
	cout << "<参数> 击中:" << -info.attackDamage << " 碰撞:"<<-info.collisionDamage<<" 机关:"<<-info.trapDamage<<" 回复:"<<info.recoverHp<<endl;
	cout << "<HP>   穰子" << info.minoriko.hp << "/" << info.minorikoMaxHp << " 魔理沙" << info.marisa.hp << "/" << info.marisaMaxHp << endl;
	int64_t colorUp = 0;
	if (info.minoriko.hp>0)
		if (info.map[info.minoriko.position.x][info.minoriko.position.y].type == GridType::Trigger)
			colorUp |= info.map[info.minoriko.position.x][info.minoriko.position.y].tag;
	if (info.map[info.marisa.position.x][info.marisa.position.y].type == GridType::Trigger)
		colorUp |= info.map[info.marisa.position.x][info.marisa.position.y].tag;
	for (int i = 0; i < info.map.size().x; ++i)
	{
		for (int j = 0; j < info.map.size().y; ++j)
		{
			if (i == info.marisa.position.x&&j == info.marisa.position.y)
			{
				cout << (info.marisa.have ? 'M' : 'm');
			}
			else if (i == info.minoriko.position.x&&j == info.minoriko.position.y)
			{
				cout << (info.minoriko.have ? 'A' : 'a');

			}
			else
			{
				char out = gridStr[(int)info.map[i][j].type];
				if (info.map[i][j].type == GridType::Trap && (info.map[i][j].tag & colorUp))
					out = '^';
				if (info.map[i][j].type == GridType::MoveableWall && (info.map[i][j].tag & colorUp))
					out = '#';
				cout << out;
			}
		}
		cout << endl;
	}
}
void Game::setMovementForMinoriko(const Movement& movement)
{
	movementMinoriko = movement;
}
void Game::setMovementForMarisa(const Movement& movement)
{
	movementMarisa = movement;
}
void Game::roundFinish()
{
	if (minoriko.hp <= 0)
	{
		movementMinoriko = Movement::createMovementStay();
		minoriko.position = Vec2i(-1, -1);
	}
	//吃红薯补血
	if (movementMinoriko.type == MovementType::Eat)
	{
		minoriko.hp += recoverHp;
		if (minoriko.hp > minorikoMaxHp)
			minoriko.hp = minorikoMaxHp;
		minoriko.have = false;
	}
	if (movementMarisa.type == MovementType::Eat)
	{
		marisa.hp += recoverHp;
		if (marisa.hp > marisaMaxHp)
			marisa.hp = marisaMaxHp;
		marisa.have = false;
	}
	//判断启动的机关
	int64_t colorUp = 0;
	if (minoriko.hp > 0)
	{
		if (map[minoriko.position.x][minoriko.position.y].type == GridType::Trigger)
			colorUp |= map[minoriko.position.x][minoriko.position.y].tag;
	}
	if (map[marisa.position.x][marisa.position.y].type == GridType::Trigger)
		colorUp |= map[marisa.position.x][marisa.position.y].tag;
	//试图移动
	Vec2i newPosMinoriko= minoriko.position,newPosMarisa=marisa.position;
	if (minoriko.hp > 0&&movementMinoriko.type == MovementType::Move)
		newPosMinoriko = minoriko.position + cns::delta[movementMinoriko.direction];
	if (movementMarisa.type == MovementType::Move)
		newPosMarisa = marisa.position + cns::delta[movementMarisa.direction];
	//判断二人碰撞移动失败
	if ((newPosMarisa == minoriko.position&&newPosMinoriko == marisa.position)|| newPosMarisa== newPosMinoriko)
	{
		marisa.hp -= collisionDamage;
		minoriko.hp -= collisionDamage;
		newPosMarisa = marisa.position;
		newPosMinoriko = minoriko.position;
	}
	//判断越界和撞墙移动失败
	bool moveFaildMarisa = false;
	if (cns::outofRange(newPosMarisa,map.size()))
		moveFaildMarisa = true;
	if (!moveFaildMarisa&&map[newPosMarisa.x][newPosMarisa.y].type == GridType::Wall)
		moveFaildMarisa = true;
	if (!moveFaildMarisa&&map[newPosMarisa.x][newPosMarisa.y].type == GridType::Shrine)
		moveFaildMarisa = true;
	if (!moveFaildMarisa&&map[newPosMarisa.x][newPosMarisa.y].type == GridType::Pile)
		moveFaildMarisa = true;
	if (!moveFaildMarisa&&map[newPosMarisa.x][newPosMarisa.y].type == GridType::MoveableWall&&(map[newPosMarisa.x][newPosMarisa.y].tag& colorUp))
		moveFaildMarisa = true;
	if(moveFaildMarisa)
		newPosMarisa = marisa.position;
	bool moveFaildMinoriko= false;
	if (cns::outofRange(newPosMinoriko, map.size()))
		moveFaildMinoriko = true;
	if (!moveFaildMinoriko&&map[newPosMinoriko.x][newPosMinoriko.y].type == GridType::Wall)
		moveFaildMinoriko = true;
	if (!moveFaildMinoriko&&map[newPosMinoriko.x][newPosMinoriko.y].type == GridType::Shrine)
		moveFaildMinoriko = true;
	if (!moveFaildMinoriko&&map[newPosMinoriko.x][newPosMinoriko.y].type == GridType::Pile)
		moveFaildMinoriko = true;
	if (!moveFaildMinoriko&&map[newPosMinoriko.x][newPosMinoriko.y].type == GridType::MoveableWall && (map[newPosMinoriko.x][newPosMinoriko.y].tag & colorUp))
		moveFaildMinoriko = true;
	if (moveFaildMinoriko)
		newPosMinoriko = minoriko.position;
	minoriko.position = newPosMinoriko;
	marisa.position = newPosMarisa;
	//投掷红薯扣血
	if (movementMinoriko.type == MovementType::Throw&&movementMinoriko.distance > 0 && movementMinoriko.distance <=2&&minoriko.have)
	{
		Vec2i pos = minoriko.position+cns::delta[movementMinoriko.direction] * movementMinoriko.distance;
		if (marisa.position == pos)
			marisa.hp -= attackDamage;
		minoriko.have = false;
	}
	if (movementMarisa.type == MovementType::Throw&&movementMarisa.distance > 0 && movementMarisa.distance <= 2 && marisa.have)
	{
		Vec2i pos = marisa.position+cns::delta[movementMarisa.direction] * movementMarisa.distance;
		if (minoriko.position == pos)
			minoriko.hp -= attackDamage;
		marisa.have = false;
	}
	//红薯的放下和拿起
	if (movementMinoriko.type == MovementType::Get&&minoriko.hp>0)
	{
		Vec2i pos = minoriko.position+cns::delta[movementMinoriko.direction];
		if (!cns::outofRange(pos, map.size()))
		{
			if (map[pos.x][pos.y].type == GridType::Pile&&(map[pos.x][pos.y].tag > 0 || map[pos.x][pos.y].tag == -1))
			{
				if (map[pos.x][pos.y].tag != -1)
					map[pos.x][pos.y].tag -= minoriko.have ? 0 : 1;
				minoriko.have = true;

			}
		}
	}
	if (movementMarisa.type == MovementType::Get&&marisa.hp>0)
	{
		Vec2i pos = marisa.position + cns::delta[movementMarisa.direction];
		if (!cns::outofRange(pos, map.size()))
		{
			if (map[pos.x][pos.y].type == GridType::Pile&&(map[pos.x][pos.y].tag > 0 || map[pos.x][pos.y].tag == -1))
			{
				if (map[pos.x][pos.y].tag != -1)
					map[pos.x][pos.y].tag -= marisa.have ? 0 : 1;
				marisa.have = true;
			}
		}
	}
	if (movementMinoriko.type == MovementType::Put&&minoriko.have&&minoriko.hp>0)
	{
		Vec2i pos = minoriko.position +cns::delta[movementMinoriko.direction];
		if (!cns::outofRange(pos, map.size()))
		{
			if (map[pos.x][pos.y].type == GridType::Shrine&&(map[pos.x][pos.y].tag > 0 || map[pos.x][pos.y].tag == -1))
			{
				if (map[pos.x][pos.y].tag != -1)
					map[pos.x][pos.y].tag--;
				minoriko.have = false;
				score++;
			}
		}
	}
	if (movementMarisa.type == MovementType::Put&&marisa.have&&marisa.hp>0)
	{
		Vec2i pos = marisa.position +cns::delta[movementMarisa.direction];
		if (!cns::outofRange(pos, map.size()))
		{
			if (map[pos.x][pos.y].type == GridType::Shrine&&(map[pos.x][pos.y].tag > 0 || map[pos.x][pos.y].tag == -1))
			{
				if (map[pos.x][pos.y].tag != -1)
					map[pos.x][pos.y].tag--;
				marisa.have = false;
				score++;
			}
		}
	}
	//判断启动的机关
	colorUp = 0;
	if (minoriko.hp > 0)
	{
		if (map[minoriko.position.x][minoriko.position.y].type == GridType::Trigger)
			colorUp |= map[minoriko.position.x][minoriko.position.y].tag;
	}
	if (map[marisa.position.x][marisa.position.y].type == GridType::Trigger)
		colorUp |= map[marisa.position.x][marisa.position.y].tag;
	//机关扣血
	if (minoriko.hp>0&&map[minoriko.position.x][minoriko.position.y].type == GridType::Trap && (map[minoriko.position.x][minoriko.position.y].tag & colorUp))
		minoriko.hp -= trapDamage;
	if (marisa.hp > 0 && map[marisa.position.x][marisa.position.y].type == GridType::Trap && (map[marisa.position.x][marisa.position.y].tag & colorUp))
		marisa.hp -= trapDamage;
	if (minoriko.hp <= 0)
	{
		minoriko.position = Vec2i(-1, -1);
		minoriko.hp = 0;
	}
	if (marisa.hp <= 0)
	{
		marisa.hp = 0;
		gameOver = true;
	}
	round++;
	if (round == roundLimit)
		gameOver = true;
}
void Game::newGame(const GameInfo& startState)
{
	minoriko = startState.minoriko;
	marisa = startState.marisa;
	round = startState.round;
	roundLimit = startState.roundLimit;
	map = startState.map;
	score = startState.score;
	collisionDamage = startState.collisionDamage;
	attackDamage = startState.attackDamage;
	trapDamage = startState.trapDamage;
	recoverHp = startState.recoverHp;
	minorikoMaxHp= startState.minorikoMaxHp;
	marisaMaxHp = startState.marisaMaxHp;
	gameOver = false;
}
bool Game::timeUp()
{
	return round == roundLimit;
}
bool Game::gameIsOver()
{
	return gameOver;
}
int Game::getScore()
{
	return score;
}
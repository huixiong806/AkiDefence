#include"Game.h"
#include"../core/Constant.h"
#include<iostream>
#include<set>
#include<cassert>
GameInfo Game::getInfo()
{
	GameInfo res;
	res.player = player;
	res.round = round;
	res.roundLimit = roundLimit;
	res.map = map;
	res.score = score;
	res.collisionDamage = collisionDamage;
	res.attackDamage = attackDamage;
	res.trapDamage = trapDamage;
	res.recoverHp = recoverHp;
	res.bucketVolume=bucketVolume;
	for (int p = 0; p < playerCount; ++p)
		res.maxHp[p] = maxHp[p];
	res.playerCount = playerCount;
	return res;
}
const char gridStr[10] = { ' ','*','$','#','.','@','_' ,'T'};
void Game::printInfo()
{
	auto info = this->getInfo();
	//cout << (info.who ? "雾雨魔理沙" : "秋穰子") << endl;
	std::cout << "<时间> 第" << info.round << "秒，还剩下" << info.roundLimit - info.round << "秒" << std::endl;
	std::cout << "<得分> 已搬运:" << info.score << std::endl;
	std::cout << "<参数> 击中:" << -info.attackDamage << " 碰撞:"<<-info.collisionDamage<<" 机关:"<<-info.trapDamage<<" 回复:"<<info.recoverHp<< std::endl;
	std::cout << "<HP>   ";
	for (int p = 0; p < playerCount; ++p)
		std::cout <<cns::playerName[p]<< info.player[p].hp << "/" << info.maxHp[p]<<" ";
	std::cout <<std::endl << "<篮筐> ";
	for (int p = 0; p < playerCount; ++p)
		std::cout << cns::playerName[p]<< info.player[p].have << "/" << info.bucketVolume[p] << " ";
	std::cout << std::endl;
	int64_t colorUp = 0;
	if (info.player[MINORIKO].hp>0)
		if (info.map[info.player[MINORIKO].position.x][info.player[MINORIKO].position.y].type == GridType::Trigger)
			colorUp |= info.map[info.player[MINORIKO].position.x][info.player[MINORIKO].position.y].tag;
	if (info.map[info.player[MARISA].position.x][info.player[MARISA].position.y].type == GridType::Trigger)
		colorUp |= info.map[info.player[MARISA].position.x][info.player[MARISA].position.y].tag;
	for (int i = 0; i < info.map.size().x; ++i)
	{
		for (int j = 0; j < info.map.size().y; ++j)
		{
			char out = ' ';
			bool player = false;
			for (int p = 0; p < playerCount; ++p)
			{
				if (info.player[p].hp > 0)
				{
					if (i == info.player[p].position.x&&j == info.player[p].position.y)
					{
						player = true;
						const char playerSingal[4] = { 'a','m','s','c' };
						out = (info.player[p].have ? playerSingal[p] - 'a' + 'A' : playerSingal[p]);
						break;
					}
				}
			}
			if(!player)
			{
				out = gridStr[(int)info.map[i][j].type];
				if (info.map[i][j].type == GridType::Trap && (info.map[i][j].tag & colorUp))
					out = '^';
				if (info.map[i][j].type == GridType::MoveableWall && (info.map[i][j].tag & colorUp))
					out = '#';
			}
			std::cout << out;
		}
		std::cout << std::endl;
	}
}
void Game::setMovement(const Movement& movement,int side)
{
	this->movement[side] = movement;
}
void Game::setPlayer(const Player& player, int side)
{
	this->player[side] = player;
}
const Player& Game::getPlayerConst(int side)
{
	return this->player[side];
}
void Game::fastRoundFinishForTwoPlayer()noexcept
{
	if (player[MINORIKO].hp <= 0)
	{
		movement[MINORIKO] = Movement::createMovementStay();
		player[MINORIKO].position = Vec2i(-9, -9);
	}
	//吃红薯补血
	for (int p = 0; p<playerCount; ++p)
	{
		if (movement[p].type == MovementType::Eat&&player[p].have>0)
		{
			player[p].hp += recoverHp;
			if (player[p].hp > maxHp[p])
				player[p].hp = maxHp[p];
			player[p].have--;
		}
	}
	//判断启动的机关
	int activeColor=0;
	for (int p = 0; p < playerCount; ++p)
		if (player[p].hp > 0 && map[player[p].position.x][player[p].position.y].type == GridType::Trigger)
			activeColor = (map[player[p].position.x][player[p].position.y].tag);
	//判断所有MovementType为Move的非法操作，将movement改为静止不动
	for (int p = 0; p < playerCount; ++p)
	{
		if (movement[p].type != MovementType::Move)
			continue;
		Vec2i newPos = player[p].position + cns::delta[movement[p].direction];
		bool actionFailed = false;
		if (cns::outofRange(newPos, map.size()))
			actionFailed = true;
		else if (map[newPos.x][newPos.y].type == GridType::Wall)
			actionFailed = true;
		else if (map[newPos.x][newPos.y].type == GridType::Tree)
			actionFailed = true;
		else if (map[newPos.x][newPos.y].type == GridType::Shrine)
			actionFailed = true;
		else if (map[newPos.x][newPos.y].type == GridType::Pile)
			actionFailed = true;
		else if (map[newPos.x][newPos.y].type == GridType::MoveableWall && player[p ^ 1].position != newPos &&\
			activeColor == map[newPos.x][newPos.y].tag)
				actionFailed = true;
		if (actionFailed)movement[p] = Movement::createMovementStay();
	}
	//判断碰撞
	Vec2i newPos[2] = { Vec2i(),Vec2i() };
	for (int p = 0; p < playerCount; ++p)
		if (movement[p].type == MovementType::Move)
			newPos[p] = player[p].position + cns::delta[movement[p].direction];
		else newPos[p] = player[p].position;
	if ((player[MARISA].position == newPos[MINORIKO] && player[MINORIKO].position == newPos[MARISA]) || \
		newPos[MARISA] == newPos[MINORIKO])
	{
		newPos[MARISA] = player[MARISA].position;
		newPos[MINORIKO] = player[MINORIKO].position;
		player[MARISA].hp -= collisionDamage;
		player[MINORIKO].hp -= collisionDamage;
	}
	//更新位置
	for (int p = 0; p < playerCount; ++p)
		if (player[p].hp > 0)
			player[p].position = newPos[p];
	//大树造成的血量改变
	for (int p = 0; p < playerCount; ++p)
	{
		if (player[p].hp <= 0)continue;
		for (int i = 0; i < 4; ++i)
		{
			auto treePos = player[p].position + cns::delta[i];
			if (!cns::outofRange(treePos, map.size()) && map[treePos.x][treePos.y].type == GridType::Tree)
			{
				player[p].hp += map[treePos.x][treePos.y].tag;
				if (player[p].hp > maxHp[p])
					player[p].hp = maxHp[p];
			}
		}
	}
	//投掷红薯扣血
	if (movement[MINORIKO].type == MovementType::Throw&& movement[MINORIKO].distance > 0 && movement[MINORIKO].distance <= 2 && player[MINORIKO].have>0)
	{
		Vec2i pos = player[MINORIKO].position + cns::delta[movement[MINORIKO].direction] * movement[MINORIKO].distance;
		if (player[MARISA].position == pos)
			player[MARISA].hp -= attackDamage;
		player[MINORIKO].have--;
	}
	if (movement[MARISA].type == MovementType::Throw&& movement[MARISA].distance > 0 && movement[MARISA].distance <= 2 && player[MARISA].have>0)
	{
		Vec2i pos = player[MARISA].position + cns::delta[movement[MARISA].direction] * movement[MARISA].distance;
		if (player[MINORIKO].position == pos)
			player[MINORIKO].hp -= attackDamage;
		player[MARISA].have--;
	}
	//判断启动的机关
	activeColor = 0;
	for (int p = 0; p < playerCount; ++p)
		if (player[p].hp > 0 && map[player[p].position.x][player[p].position.y].type == GridType::Trigger)
			activeColor = (map[player[p].position.x][player[p].position.y].tag);
	//机关扣血
	for (int p = 0; p < playerCount; ++p)
		if (player[p].hp > 0 && map[player[p].position.x][player[p].position.y].type == GridType::Trap &&\
			activeColor == map[player[p].position.x][player[p].position.y].tag)
			player[p].hp -= trapDamage;
	//红薯的放下和拿起
	for (int p = 0; p < playerCount; ++p)
	{
		if (player[p].hp <= 0)continue;
		if (movement[p].type == MovementType::Get && player[p].have< bucketVolume[p])
		{
			Vec2i pos = player[p].position + cns::delta[movement[p].direction];
			if (!cns::outofRange(pos, map.size()) && map[pos.x][pos.y].type == GridType::Pile)
				player[p].have++;
		}
		else if (movement[p].type == MovementType::Put && player[p].have>0)
		{
			Vec2i pos = player[p].position + cns::delta[movement[p].direction];
			if (!cns::outofRange(pos, map.size()) && map[pos.x][pos.y].type == GridType::Shrine)
			{
				player[p].have--;
				score++;
			}
		}
	}
	for (int p = 0; p < playerCount; ++p)
		if (player[p].hp <= 0)
		{
			player[p].position = Vec2i(-9, -9);
			player[p].hp = 0;
		}
	if (player[MARISA].hp <= 0)
		gameOver = true;
	round++;
	if (round >= roundLimit)
		gameOver = true;
}
void Game::roundFinish()
{
	for (int p = 0; p < playerCount; ++p)
	{
		if (player[p].hp <= 0)
		{
			movement[p] = Movement::createMovementStay();
			player[p].position = Vec2i(-1, -1);
		}
	}
	//吃红薯补血
	for (int p=0;p<playerCount;++p)
	{
		if (movement[p].type == MovementType::Eat)
		{
			if ((p == SHIZUHA && map[player[p].position.x][player[p].position.y].type == GridType::Pile))
			{
				player[p].hp += recoverHp;
				if (player[p].hp > maxHp[p])
					player[p].hp = maxHp[p];
			}
			else if(player[p].have>0)
			{
				player[p].hp += recoverHp;
				if (player[p].hp > maxHp[p])
					player[p].hp = maxHp[p];
				player[p].have--;
			}
		}
	}
	struct node
	{
		Vec2i pos;
		int gap;//0表示非间隙，1表示下方间隙，2表示右方间隙
		node() { pos = Vec2i(0, 0), gap = 0; }
		node(Vec2i pos_, int gap_) :pos(pos_), gap(gap_) {}
		bool operator<(const node& rhs)const
		{
			if (gap < rhs.gap)return true;
			if (gap > rhs.gap)return false;
			if (pos.x < rhs.pos.x)return true;
			if (pos.x > rhs.pos.x)return false;
			return pos.y < rhs.pos.y;
		}
		bool operator ==(const node& rhs)const
		{
			return pos == rhs.pos&&gap == rhs.gap;
		}
	};
	std::vector<std::pair<node, int>> collisionTest;
	int index = 0;
	bool moveFailed[4] = { 0,0,0,0 };
	//判断启动的机关
	std::vector<int> activeColor;
	for (int p = 0; p < playerCount; ++p)
	{
		if (player[p].hp > 0)
		{
			if (map[player[p].position.x][player[p].position.y].type == GridType::Trigger)
				activeColor.push_back(map[player[p].position.x][player[p].position.y].tag);
		}
	}
	//判断所有MovementType为Move的非法操作，将movement改为静止不动
	for (int p = 0; p < playerCount; ++p)
	{
		if (movement[p].type != MovementType::Move)
			continue;
		Vec2i newPos = player[p].position + cns::delta[movement[p].direction];
		bool actionFailed = false;
		if (cns::outofRange(newPos, map.size()))
			actionFailed = true;
		if (!actionFailed&&map[newPos.x][newPos.y].type == GridType::Wall)
			actionFailed = true;
		if (!actionFailed&&map[newPos.x][newPos.y].type == GridType::Tree)
			actionFailed = true;
		if (!actionFailed&&map[newPos.x][newPos.y].type == GridType::Shrine)
			actionFailed = true;
		if (!actionFailed&&map[newPos.x][newPos.y].type == GridType::Pile&&p != SHIZUHA)
			actionFailed = true;
		bool someOneStandOnThere=false;
		for (int p = 0; p < playerCount; ++p)
		{
			if (player[p].hp > 0 && player[p].position == newPos)
			{
				someOneStandOnThere = true;
				break;
			}
		}
		bool colorActived = false;
		for (auto color : activeColor)
			if (color == map[newPos.x][newPos.y].tag)
				colorActived = true;
		if (!actionFailed&&map[newPos.x][newPos.y].type == GridType::MoveableWall &&!someOneStandOnThere && colorActived)
			actionFailed = true;
		if (actionFailed)
		{
			movement[p] = Movement::createMovementStay();
		}
	}
	
	Vec2i newPos[4] = { Vec2i(),Vec2i(),Vec2i(),Vec2i() };
	//首先判断因为途中碰撞而移动失败的情况
	for (int p = 0; p < playerCount; ++p)
	{
		if (player[p].hp > 0 && movement[p].type == MovementType::Move)
		{
			newPos[p] = player[p].position + cns::delta[movement[p].direction];
			node gap;
			//4为静止不动
			if (movement[p].direction != 4)
			{
				gap.pos = movement[p].direction % 2 ? Vec2i(player[p].position) : newPos[p];
				gap.gap = 1 + (movement[p].direction >> 1);
				1 << p;
				int i = 0;
				for (i = 0; i < collisionTest.size(); ++i)
				{
					if (collisionTest[i].first == gap)
					{
						collisionTest[i].second |= (1<<p);
						break;
					}
				}
				if (i == collisionTest.size())
				{
					collisionTest.push_back({ gap,1 << p });
				}
			}
		}
	}
	for (auto& gap : collisionTest)
		if (gap.second != cns::lowbit(gap.second))//x!=lowbit(x)等价于x里为1的bit不止一个
			for (int p = 0; p < playerCount; ++p)
			{
				if ((1 << p)&gap.second)
				{
					moveFailed[p] = true;
					player[p].hp -= collisionDamage;
				}
			}
	collisionTest.clear();
	//然后判断在格子上碰撞而移动失败的情况
	while(true)
	{
		bool noCollision = true;
		//站的位置加入vector
		for (int p = 0; p < playerCount; ++p)
		{
			if (player[p].hp > 0)
			{
				if(movement[p].type == MovementType::Move && !moveFailed[p])
					newPos[p] = player[p].position + cns::delta[movement[p].direction];
				else newPos[p] = player[p].position;
				int i=0;
				for (i = 0; i < collisionTest.size(); ++i)
				{
					if (collisionTest[i].first == node(newPos[p], false))
					{
						collisionTest[i].second |= (1 << p);
						break;
					}
				}
				if (i == collisionTest.size())
				{
					collisionTest.push_back({ node(newPos[p],false),1 << p });
				}
			}
		}
		//对每个格子判断碰撞
		for (auto& grid : collisionTest)
		{
			if (grid.second != cns::lowbit(grid.second))//x!=lowbit(x)等价于x里为1的bit不止一个
			{
				noCollision = false;
				int size = 0,temp=grid.second;
				while (temp)
				{
					size++;
					temp -= cns::lowbit(temp);
				}
				for (int p = 0; p < playerCount; ++p)
				{
					if ((1 << p)&grid.second)
					{
						moveFailed[p] = true;
						player[p].hp -= collisionDamage*(size-1);
					}
				}
			}	
		}
		collisionTest.clear();
		if (noCollision)break;
	}
	//更新位置
	for (int p = 0; p < playerCount; ++p)
		if (player[p].hp > 0)
			if (movement[p].type == MovementType::Move && !moveFailed[p])
				player[p].position = player[p].position + cns::delta[movement[p].direction];
	//大树造成的血量改变和加倍减半影响
	int throwEffectForCirno=0;//表示不变。-1表示减半，+1表示加倍。
	{
		std::vector<Vec2i>redLeaftree;
		if (playerCount >= 3)
		{
			for (int i = 0; i < 4; ++i)
			{
				auto treePos = player[SHIZUHA].position + cns::delta[i];
				if (!cns::outofRange(treePos, map.size()) && map[treePos.x][treePos.y].type == GridType::Tree\
					&&map[treePos.x][treePos.y].tag!=0)
					redLeaftree.push_back(treePos);
			}
		}
		for (int p = 0; p < playerCount; ++p)
		{
			if (player[p].hp <= 0)continue;
			for (int i = 0; i < 4; ++i)
			{
				auto treePos = player[p].position + cns::delta[i];
				if (!cns::outofRange(treePos, map.size()) && map[treePos.x][treePos.y].type == GridType::Tree\
					&&map[treePos.x][treePos.y].tag != 0)
				{
					bool redLeaf = false;
					for (auto item : redLeaftree)
						if (treePos == item)
						{
							redLeaf = true;
							break;
						}
					if (p == SHIZUHA)
					{
						if (map[treePos.x][treePos.y].tag < 0)
							player[p].hp -= map[treePos.x][treePos.y].tag;
					}
					else player[p].hp += redLeaf ? -map[treePos.x][treePos.y].tag : map[treePos.x][treePos.y].tag;
					if (p == CIRNO&& !redLeaf)
					{
						if (map[treePos.x][treePos.y].tag < 0)
							throwEffectForCirno -=1;
						else throwEffectForCirno +=1;
					}
					if (player[p].hp > maxHp[p])
						player[p].hp = maxHp[p];
				}
			}
		}
	}
	bool shizuhaInPile = (map[player[SHIZUHA].position.x][player[SHIZUHA].position.y].type == GridType::Pile);
	//投掷红薯扣血和传递红薯
	//枚举投掷红薯的角色
	for (int p = 0; p < playerCount; ++p)
	{
		if (movement[p].type == MovementType::Throw&& movement[p].distance > 0 && movement[p].distance <= 2 && (player[p].have > 0||(p==SHIZUHA&&shizuhaInPile)))
		{
			//静叶在红薯仓库中不消耗红薯。
			if(!(p == SHIZUHA && shizuhaInPile))
				player[p].have--;
			Vec2i pos = player[p].position + cns::delta[movement[p].direction] * movement[p].distance;
			if(p == SHIZUHA && shizuhaInPile)
				pos = player[p].position + cns::delta[movement[p].direction];
			//枚举目标角色
			for (int t = 0; t < playerCount; ++t)
			{
				if (p == t)continue;
				if (player[t].position == pos)
				{
					if (cns::isFriend(p, t))
					{
						if (player[t].have < bucketVolume[t])
							player[t].have++;
					}
					else
					{
						if (p == CIRNO)
						{
							if(throwEffectForCirno>0)
								player[t].hp -= (attackDamage << throwEffectForCirno);
							else player[t].hp -= (attackDamage >> (-throwEffectForCirno));
						}	
						else player[t].hp -= attackDamage;
					}
					break;
				}
			}
		}
	}
	//判断启动的机关
	activeColor.clear();
	for (int p = 0; p < playerCount; ++p)
	{
		if (player[p].hp > 0)
		{
			if (map[player[p].position.x][player[p].position.y].type == GridType::Trigger)
				activeColor.push_back(map[player[p].position.x][player[p].position.y].tag);
		}
	}
	//机关扣血
	for (int p = 0; p < playerCount; ++p)
	{
		bool colorActived = false;
		for (auto color : activeColor)
			if (color == map[player[p].position.x][player[p].position.y].tag)
				colorActived = true;
		if (player[p].hp > 0 && map[player[p].position.x][player[p].position.y].type == GridType::Trap && colorActived)
			player[p].hp -= trapDamage;
	}
	//红薯的放下和拿起
	for (int p = 0; p < playerCount;++p)
	{
		if (player[p].hp <= 0)continue;

		if (movement[p].type == MovementType::Get && player[p].have< bucketVolume[p])
		{
			Vec2i pos = player[p].position + cns::delta[movement[p].direction];
			if (!cns::outofRange(pos, map.size()))
			{
				if (map[pos.x][pos.y].type == GridType::Pile)
				{
					bool cirnoIsNear = false;
					for (int i = 0; i < 4; ++i)
					{
						if (pos == player[CIRNO].position+cns::delta[i])
						{
							cirnoIsNear = true;
							break;
						}
					}
					if(p==CIRNO||!cirnoIsNear)
						player[p].have++;
				}
			}
		}
		if (movement[p].type == MovementType::Put && (player[p].have > 0 || (p == SHIZUHA && shizuhaInPile)))
		{
			Vec2i pos = player[p].position + cns::delta[movement[p].direction];
			if (!cns::outofRange(pos, map.size()))
			{
				if (map[pos.x][pos.y].type == GridType::Shrine)
				{
					if(!(p == SHIZUHA && shizuhaInPile))
						player[p].have--;
					score++;
				}
			}
		}
	}
	for (int p = 0; p < playerCount; ++p)
	{
		if (player[p].hp <= 0)
		{
			player[p].position = Vec2i(-1, -1);
			player[p].hp = 0;
		}
	}
	if (player[MARISA].hp <= 0)
		gameOver = true;
	round++;
	if (round >= roundLimit)
		gameOver = true;
}
void Game::newGame(const GameInfo& startState)
{
	map = startState.map;
	player = startState.player;
	maxHp = startState.maxHp;
	bucketVolume = startState.bucketVolume;
	score = startState.score;
	round = startState.round;
	roundLimit = startState.roundLimit;
	collisionDamage = startState.collisionDamage;
	attackDamage = startState.attackDamage;
	trapDamage = startState.trapDamage;
	recoverHp = startState.recoverHp;
	playerCount = startState.playerCount;
	gameOver = false;
}
void Game::restartGame(const GameInfo& startState)
{
	for (int p = 0; p < playerCount; ++p)
		player[p] = startState.player[p];
	round = startState.round;
	score = startState.score;
	gameOver = false;
}
void Game::setGameOver(bool gameover)
{
	gameOver= gameover;
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
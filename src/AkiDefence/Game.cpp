#include"Game.h"
#include"../core/Constant.h"
#include<iostream>
GameInfo Game::getInfo()
{
	GameInfo res;
	res.player[MINORIKO] = player[MINORIKO];
	res.player[MARISA] = player[MARISA];
	res.round = round;
	res.roundLimit = roundLimit;
	res.map = map;
	res.score = score;
	res.collisionDamage = collisionDamage;
	res.attackDamage = attackDamage;
	res.trapDamage = trapDamage;
	res.recoverHp = recoverHp;
	res.bucketVolume=bucketVolume;
	res.maxHp[MARISA] =maxHp[MARISA];
	res.maxHp[MINORIKO] = maxHp[MINORIKO];
	return res;
}
const char gridStr[10] = { ' ','*','S','#','.','@','_' };
void Game::printInfo()
{
	auto info = this->getInfo();
	//cout << (info.who ? "雾雨魔理沙" : "秋穰子") << endl;
	cout << "<时间> 第" << info.round << "秒，还剩下" << info.roundLimit - info.round << "秒" << endl;
	cout << "<得分> 已搬运:" << info.score << endl;
	cout << "<参数> 击中:" << -info.attackDamage << " 碰撞:"<<-info.collisionDamage<<" 机关:"<<-info.trapDamage<<" 回复:"<<info.recoverHp<<endl;
	cout << "<HP>   穰子" << info.player[MINORIKO].hp << "/" << info.maxHp[MINORIKO] << " 魔理沙" << info.player[MARISA].hp << "/" << info.maxHp[MARISA] << endl;
	cout << "<篮筐> 穰子" << info.player[MINORIKO].have << "/" << info.bucketVolume[MINORIKO] << " 魔理沙" << info.player[MARISA].have<< "/" << info.bucketVolume[MARISA] << endl;
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
			if (i == info.player[MARISA].position.x&&j == info.player[MARISA].position.y)
			{
				cout << (info.player[MARISA].have ? 'M' : 'm');
			}
			else if (i == info.player[MINORIKO].position.x&&j == info.player[MINORIKO].position.y)
			{
				cout << (info.player[MINORIKO].have ? 'A' : 'a');

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
void Game::roundFinish()
{
	if (player[MINORIKO].hp <= 0)
	{
		movement[MINORIKO] = Movement::createMovementStay();
		player[MINORIKO].position = Vec2i(-1, -1);
	}
	//吃红薯补血
	for (int p=0;p<playerCount;++p)
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
	int64_t colorUp = 0;
	for (int p = 0; p < playerCount; ++p)
	{
		if (player[p].hp > 0)
		{
			if (map[player[p].position.x][player[p].position.y].type == GridType::Trigger)
				colorUp |= map[player[p].position.x][player[p].position.y].tag;
		}
	}
	//试图移动
	Vec2i newPosMinoriko= player[MINORIKO].position,newPosMarisa= player[MARISA].position;
	if (player[MINORIKO].hp > 0&& movement[MINORIKO].type == MovementType::Move)
		newPosMinoriko = player[MINORIKO].position + cns::delta[movement[MINORIKO].direction];
	if (movement[MARISA].type == MovementType::Move)
		newPosMarisa = player[MARISA].position + cns::delta[movement[MARISA].direction];
	//判断二人碰撞移动失败
	if ((newPosMarisa == player[MINORIKO].position&&newPosMinoriko == player[MARISA].position)|| newPosMarisa== newPosMinoriko)
	{
		player[MARISA].hp -= collisionDamage;
		player[MINORIKO].hp -= collisionDamage;
		newPosMarisa = player[MARISA].position;
		newPosMinoriko = player[MINORIKO].position;
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
		newPosMarisa = player[MARISA].position;
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
		newPosMinoriko = player[MINORIKO].position;
	player[MINORIKO].position = newPosMinoriko;
	player[MARISA].position = newPosMarisa;
	//投掷红薯扣血
	if (movement[MINORIKO].type == MovementType::Throw&& movement[MINORIKO].distance > 0 && movement[MINORIKO].distance <=2&& player[MINORIKO].have>0)
	{
		Vec2i pos = player[MINORIKO].position+cns::delta[movement[MINORIKO].direction] * movement[MINORIKO].distance;
		if (player[MARISA].position == pos)
			player[MARISA].hp -= attackDamage;
		player[MINORIKO].have--;
	}
	if (movement[MARISA].type == MovementType::Throw&& movement[MARISA].distance > 0 && movement[MARISA].distance <= 2 && player[MARISA].have>0)
	{
		Vec2i pos = player[MARISA].position+cns::delta[movement[MARISA].direction] * movement[MARISA].distance;
		if (player[MINORIKO].position == pos)
			player[MINORIKO].hp -= attackDamage;
		player[MARISA].have--;
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
					player[p].have++;
				}
			}
		}
		if (movement[p].type == MovementType::Put && player[p].have>0)
		{
			Vec2i pos = player[p].position + cns::delta[movement[p].direction];
			if (!cns::outofRange(pos, map.size()))
			{
				if (map[pos.x][pos.y].type == GridType::Shrine)
				{
					player[p].have--;
					score++;
				}
			}
		}
	}
	//判断启动的机关
	colorUp = 0;
	for (int p = 0; p < playerCount; ++p)
	{
		if (player[p].hp > 0)
		{
			if (map[player[p].position.x][player[p].position.y].type == GridType::Trigger)
				colorUp |= map[player[p].position.x][player[p].position.y].tag;
		}
	}
	//机关扣血
	for (int p = 0; p < playerCount; ++p)
	{
		if (player[p].hp > 0 && map[player[p].position.x][player[p].position.y].type == GridType::Trap && (map[player[p].position.x][player[p].position.y].tag & colorUp))
			player[p].hp -= trapDamage;
	}
	if (player[MINORIKO].hp <= 0)
	{
		player[MINORIKO].position = Vec2i(-1, -1);
		player[MINORIKO].hp = 0;
	}
	if (player[MARISA].hp <= 0)
	{
		player[MARISA].hp = 0;
		gameOver = true;
	}
	round++;
	if (round == roundLimit)
		gameOver = true;
}
void Game::newGame(const GameInfo& startState)
{
	playerCount = startState.playerCount;
	for (int p = 0; p < playerCount; ++p)
		player[p] = startState.player[p];
	round = startState.round;
	roundLimit = startState.roundLimit;
	map = startState.map;
	score = startState.score;
	collisionDamage = startState.collisionDamage;
	attackDamage = startState.attackDamage;
	trapDamage = startState.trapDamage;
	recoverHp = startState.recoverHp;
	bucketVolume = startState.bucketVolume;
	maxHp[MINORIKO]= startState.maxHp[MINORIKO];
	maxHp[MARISA] = startState.maxHp[MARISA];
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
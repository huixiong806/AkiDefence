#include"Game.h"
#include"../core/Constant.h"
#include<iostream>
GameInfo Game::getInfo(int who)
{
	GameInfo res;
	res.player[MINORIKO] = player[MINORIKO];
	res.player[MARISA] = player[MARISA];
	res.round = round;
	res.roundLimit = roundLimit;
	res.map = map;
	res.score = score;
	res.who = who;
	res.collisionDamage = collisionDamage;
	res.attackDamage = attackDamage;
	res.trapDamage = trapDamage;
	res.recoverHp = recoverHp;
	res.maxHp[MARISA] =maxHp[MARISA];
	res.maxHp[MINORIKO] = maxHp[MINORIKO];
	return res;
}
const char gridStr[10] = { ' ','*','S','#','.','@','_' };
void Game::printInfo()
{
	auto info = this->getInfo(0);
	//cout << (info.who ? "����ħ��ɳ" : "�����") << endl;
	cout << "<ʱ��> ��" << info.round << "�룬��ʣ��" << info.roundLimit - info.round << "��" << endl;
	cout << "<�÷�> �Ѱ���:" << info.score << endl;
	cout << "<����> ����:" << -info.attackDamage << " ��ײ:"<<-info.collisionDamage<<" ����:"<<-info.trapDamage<<" �ظ�:"<<info.recoverHp<<endl;
	cout << "<HP>   ���" << info.player[MINORIKO].hp << "/" << info.maxHp[MINORIKO] << " ħ��ɳ" << info.player[MARISA].hp << "/" << info.maxHp[MARISA] << endl;
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
	//�Ժ���Ѫ
	for (int p=0;p<playerCount;++p)
	{
		if (movement[p].type == MovementType::Eat)
		{
			player[p].hp += recoverHp;
			if (player[p].hp > maxHp[p])
				player[p].hp = maxHp[p];
			player[p].have = false;
		}
	}
	//�ж������Ļ���
	int64_t colorUp = 0;
	for (int p = 0; p < playerCount; ++p)
	{
		if (player[p].hp > 0)
		{
			if (map[player[p].position.x][player[p].position.y].type == GridType::Trigger)
				colorUp |= map[player[p].position.x][player[p].position.y].tag;
		}
	}
	//��ͼ�ƶ�
	Vec2i newPosMinoriko= player[MINORIKO].position,newPosMarisa= player[MARISA].position;
	if (player[MINORIKO].hp > 0&& movement[MINORIKO].type == MovementType::Move)
		newPosMinoriko = player[MINORIKO].position + cns::delta[movement[MINORIKO].direction];
	if (movement[MARISA].type == MovementType::Move)
		newPosMarisa = player[MARISA].position + cns::delta[movement[MARISA].direction];
	//�ж϶�����ײ�ƶ�ʧ��
	if ((newPosMarisa == player[MINORIKO].position&&newPosMinoriko == player[MARISA].position)|| newPosMarisa== newPosMinoriko)
	{
		player[MARISA].hp -= collisionDamage;
		player[MINORIKO].hp -= collisionDamage;
		newPosMarisa = player[MARISA].position;
		newPosMinoriko = player[MINORIKO].position;
	}
	//�ж�Խ���ײǽ�ƶ�ʧ��
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
	//Ͷ�������Ѫ
	if (movement[MINORIKO].type == MovementType::Throw&& movement[MINORIKO].distance > 0 && movement[MINORIKO].distance <=2&& player[MINORIKO].have)
	{
		Vec2i pos = player[MINORIKO].position+cns::delta[movement[MINORIKO].direction] * movement[MINORIKO].distance;
		if (player[MARISA].position == pos)
			player[MARISA].hp -= attackDamage;
		player[MINORIKO].have = false;
	}
	if (movement[MARISA].type == MovementType::Throw&& movement[MARISA].distance > 0 && movement[MARISA].distance <= 2 && player[MARISA].have)
	{
		Vec2i pos = player[MARISA].position+cns::delta[movement[MARISA].direction] * movement[MARISA].distance;
		if (player[MINORIKO].position == pos)
			player[MINORIKO].hp -= attackDamage;
		player[MARISA].have = false;
	}
	//����ķ��º�����
	for (int p = 0; p < playerCount;++p)
	{
		if (player[p].hp <= 0)continue;
		if (movement[p].type == MovementType::Get)
		{
			Vec2i pos = player[p].position + cns::delta[movement[p].direction];
			if (!cns::outofRange(pos, map.size()))
			{
				if (map[pos.x][pos.y].type == GridType::Pile && (map[pos.x][pos.y].tag > 0 || map[pos.x][pos.y].tag == -1))
				{
					if (map[pos.x][pos.y].tag != -1)
						map[pos.x][pos.y].tag -= player[p].have ? 0 : 1;
					player[p].have = true;

				}
			}
		}
		if (movement[p].type == MovementType::Put && player[p].have)
		{
			Vec2i pos = player[p].position + cns::delta[movement[p].direction];
			if (!cns::outofRange(pos, map.size()))
			{
				if (map[pos.x][pos.y].type == GridType::Shrine && (map[pos.x][pos.y].tag > 0 || map[pos.x][pos.y].tag == -1))
				{
					if (map[pos.x][pos.y].tag != -1)
						map[pos.x][pos.y].tag--;
					player[p].have = false;
					score++;
				}
			}
		}
	}
	//�ж������Ļ���
	colorUp = 0;
	for (int p = 0; p < playerCount; ++p)
	{
		if (player[p].hp > 0)
		{
			if (map[player[p].position.x][player[p].position.y].type == GridType::Trigger)
				colorUp |= map[player[p].position.x][player[p].position.y].tag;
		}
	}
	//���ؿ�Ѫ
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
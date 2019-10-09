#pragma once
#include"./Player.hpp"
#include"./Map.hpp"
#include<string>
#include<sstream>
#include<vector>
enum class MovementType
{
	Move,
	Throw,
	Get,
	Put,
	Eat
};
class Movement
{
public:
	MovementType type;
	int direction;//��ʾ����0123�ֱ�Ϊ��������,4Ϊ����
	int distance;//��ʾ��·�Ĳ��������Ӻ���ľ���
	static Movement createMovementStay()
	{
		Movement movement;
		movement.type = MovementType::Move;
		movement.direction = 4;
		movement.distance = 0;
		return movement;
	}
};
struct GameInfo
{
	GridMap<Grid> map;
	Player minoriko;
	Player marisa;
	int score;
	int round;
	int roundLimit;
	int minorikoMaxHp;
	int marisaMaxHp;
	int collisionDamage;
	int attackDamage;
	int trapDamage;
	int recoverHp;
	int who;//0��ʾ��minoriko�ж���1��ʾ��marisa�ж�
};
class MovementGenerator
{
protected:
	std::string name;
public:
	std::string printMovement(Movement movement);
	virtual Movement generateMovement(GameInfo) = 0;
	std::vector<Movement>getMovementForMinoriko(GameInfo state);
	std::vector<Movement>getMovementForMarisa(GameInfo state);
	virtual void init(GameInfo info)=0;
	virtual std::string getName()
	{
		return name;
	}
};

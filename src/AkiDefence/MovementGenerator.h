#pragma once
#include"./Player.hpp"
#include"./Map.hpp"
#include<string>
#include<sstream>
#include<array>
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
	int direction;//表示方向，0123分别为上下左右,4为不动
	int distance;//表示走路的步数或者扔红薯的距离
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
	std::array<Player, 4> player;
	std::array<int, 4> maxHp;
	int score;
	int round;
	int roundLimit;
	int collisionDamage;
	int attackDamage;
	int trapDamage;
	int recoverHp;
	int who;//0表示该minoriko行动，1表示该marisa行动
	int playerCount;
};
class MovementGenerator
{
protected:
	std::string name;
public:
	std::string printMovement(Movement movement);
	virtual Movement generateMovement(GameInfo) = 0;
	std::vector<Movement>getEffectiveMovement(GameInfo state);
	//std::vector<Movement>getMovementForMinoriko(GameInfo state);
	//std::vector<Movement>getMovementForMarisa(GameInfo state);
	virtual void init(GameInfo info)=0;
	virtual std::string getName()
	{
		return name;
	}
};

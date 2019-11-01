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
	std::array<int16_t, 4> maxHp;
	std::array<int16_t, 4> bucketVolume;//装红薯的篮筐容量
	int16_t score;
	int32_t round;
	int32_t roundLimit;
	int16_t collisionDamage;
	int16_t attackDamage;
	int16_t trapDamage;
	int16_t recoverHp;
	int8_t playerCount;
};
class MovementGenerator
{
protected:
	std::string name;
public:
	std::string printMovement(Movement movement);
	virtual Movement generateMovement(GameInfo,int who) = 0;
	std::vector<Movement>getEffectiveMovement(GameInfo ,int who);
	//std::vector<Movement>getMovementForMinoriko(GameInfo state);
	//std::vector<Movement>getMovementForMarisa(GameInfo state);
	virtual void init(GameInfo info)=0;
	virtual std::string getName()
	{
		return name;
	}
};

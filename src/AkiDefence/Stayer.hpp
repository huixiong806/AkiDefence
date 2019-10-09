#pragma once
#include"./MovementGenerator.h"
class Stayer:public MovementGenerator
{
public:
	Movement generateMovement(GameInfo info);
};
Movement Stayer::generateMovement(GameInfo info)
{
	return Movement::createMovementStay();
}
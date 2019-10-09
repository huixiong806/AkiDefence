#pragma once
#include"../core/GirdMap.hpp"
enum class GridType
{
	Empty,        //空
	Pile,         //红薯堆
	Shrine,       //神社
	Wall,         //土墙
	Trap,         //机关
	MoveableWall, //活动墙
	Trigger,      //踏板
};
struct Grid
{
	
	GridType type;
	/*
	当type=5~7时表示color
	64位二进制形式存储，每位表示对应的色元，为0表示不含该色元，为1表示含有该色元
	对踏板和陷阱有效
	当type=1~2时表示剩余红薯数量
	*/
	int64_t tag;
};
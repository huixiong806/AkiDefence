#pragma once
#include"../core/GirdMap.hpp"
enum class GridType
{
	Empty,        //��
	Pile,         //�����
	Shrine,       //����
	Wall,         //��ǽ
	Trap,         //����
	MoveableWall, //�ǽ
	Trigger,      //̤��
};
struct Grid
{
	
	GridType type;
	/*
	��type=5~7ʱ��ʾcolor
	64λ��������ʽ�洢��ÿλ��ʾ��Ӧ��ɫԪ��Ϊ0��ʾ������ɫԪ��Ϊ1��ʾ���и�ɫԪ
	��̤���������Ч
	��type=1~2ʱ��ʾʣ���������
	*/
	int64_t tag;
};
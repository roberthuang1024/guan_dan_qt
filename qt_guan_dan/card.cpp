﻿#include"card.h"
bool operator==(const Card& card1, const Card& card2)
{
	if (card1.suit_ == card2.suit_ &&
		card1.point_ == card2.point_)
		return true;
	return false;
}
bool operator<(const Card& card1, const Card& card2)
{
	//将A转换为相对值比较
	double point1 = card1.point_;
	if (point1 == 1)point1 = 13.1;
	double point2 = card2.point_;
	if (point2 == 1)point2 = 13.1;

	if (point1 < point2)
		return true;
	if (point1 == point2 && card1.suit_ < card2.suit_)
		return true;
	return false;
}

bool operator>(const Card& card1, const Card& card2)
{
	if (card1 < card2)
		return false;
	if (card1 == card2)
		return false;
	return true;
}


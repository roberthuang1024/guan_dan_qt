#ifndef CARD_H_
#define CARD_H_
#include<iostream>
#include <string>
class Card
{
public:
	Card(int point,int suit):point_(point),suit_(suit){}
	friend bool operator==(const Card& card1, const Card& card2);

	int get_point() const { return point_; }
	int get_suit() const { return suit_; }

	//debug: print the card to DOS
	void print_card() const
	{
		std::string point_name[] = { "","A","2","3","4","5","6","7","8","9","10","J","Q","K","joker","JOKER"};
		std::string suit_name[] = { "♠","♥","♣","♦" };
		std::cout<<point_name[point_]<<suit_name[suit_]<<std::endl;
	}

private:
	// 点数 (2-10,A:1,J:11,Q:12,K:13,joker:14,JOKER:15)
	int point_;
	// 花色(0-3:♠♥♣♦，-1:大小王)
	int suit_;
};

#endif //CARD_H_


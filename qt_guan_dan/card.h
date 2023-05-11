#ifndef CARD_H_
#define CARD_H_
#include<iostream>
#include <string>
#include <qdebug.h>
class CardButton;
class Card
{
public:
	Card(int point, int suit,bool is_wild_card = false) :point_(point), suit_(suit), card_btn_(nullptr), is_wild_card_(is_wild_card) {}
	friend bool operator==(const Card& card1, const Card& card2);
	//绝对大小比较 (JOKER > joker > A > 2 > K > Q >...)
	friend bool operator<(const Card& card1, const Card& card2);
	friend bool operator>(const Card& card1, const Card& card2);

	int get_point() const { return point_; }
	int get_suit() const { return suit_; }
	CardButton* get_card_btn()const { return card_btn_; }
	void set_card_btn(CardButton* btn) { card_btn_ = btn; }

	//debug: print the card to DOS
	void print_card() const
	{
		std::string point_name[] = { "","A","2","3","4","5","6","7","8","9","10","J","Q","K","joker","JOKER" };
		std::string suit_name[] = { "♠","♥","♣","♦" };
		std::cout << point_name[point_] << suit_name[suit_] << std::endl;
	}

private:
	// 点数 (2-10,A:1,J:11,Q:12,K:13,joker:14,JOKER:15)
	int point_;
	// 花色(0-3:♠♥♣♦，-1:大小王)
	int suit_;
	//对应UI实现（optional）
	CardButton* card_btn_;
	//是否为逢人配替换牌（用于UI显示）
	bool is_wild_card_;
};

#endif //CARD_H_


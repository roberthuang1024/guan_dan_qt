﻿#ifndef PLAYER_WIDGET_H_
#define PLAYER_WIDGET_H_

#include"qwidget.h"
#include"sprite.h"
#include "qlabel.h"
#include"hand.h"
#include"card_btn.h"

class Player_widget :public QWidget
{
	Q_OBJECT
public:
	Player_widget(Hand* hand);
	//@brief 牌队恢复默认排序
	void sort_card_heap();
	//@brief 根据heap重新显示牌堆
	void update_card_heap_show();
	
	void set_hand(Hand* hand) { hand_ = hand; }

	//@brief 根据 Hand 刷新所有页面卡牌
	void update_all();

protected:
	void closeEvent(QCloseEvent* event);

private:
	//group等级的label
	QLabel* lb_rank_self_;
	QLabel* lb_rank_rival_;
	//玩家头像位置坐标
	const int SPR_PLAYER_X[4] = { 20,860,390,20 };
	const int SPR_PLAYER_Y[4] = { 370,130,15,130 };
	//对应玩家指针
	Hand* hand_;
	//该玩家排堆： vector<pair<是否为整理好的牌，该堆所有的牌>>
	std::vector<std::pair<bool, std::vector<Card>>> card_heaps_;
signals:
	void player_close();
	void delete_all_card_bottons();
};
#endif

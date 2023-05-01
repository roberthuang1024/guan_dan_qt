﻿#include "player_widget.h"
#include "qstring.h"
#include "status.h"
#include "sprite.h"
#include "qpushbutton.h"
#include "qlabel.h"
#include "button.h"
#include "hand.h"
#include <algorithm>

PlayerWidget::PlayerWidget(Hand* hand) :QWidget(), hand_(hand)
{
	int id = hand_->id_;
	show();
	/*****窗口基本设置****/
	//设置标题
	setWindowTitle(QString("玩家") + QString::number(id + 1));
	//背景色深灰色
	setPalette(QColor(35, 35, 35));
	//设置窗口大小
	setGeometry((id / 2) * 200, 30 + (id % 2) * 200, SCREEN_W, SCREEN_H);
	setFixedSize(SCREEN_W, SCREEN_H);

	/*****基本UI布局设置****/
	//显示级牌底边
	QString rsc_t = QString("img/label/rank_bg_group") + QString::number(id % 2) + QString(".png");
	new Sprite(15, 15, rsc_t, this, Sprite::Height, 65);

	auto set_label = [](QLabel* lb, int y) {
		lb->setStyleSheet("boder:none;");
		lb->setFont(QFont("微软雅黑", 12));
		lb->setGeometry(70, y, 30, 25);
		lb->show();
	};
	lb_rank_self_ = new QLabel("2", this);
	lb_rank_rival_ = new QLabel("2", this);
	set_label(lb_rank_self_, 15);
	set_label(lb_rank_rival_, 47);
	//显示玩家布局
	int id_t = id;
	for (int i = 0; i < 4; i++)
	{
		int id_;
		rsc_t = QString("img/label/player") + QString::number(id_t) + QString(".png");
		new Sprite(SPR_PLAYER_X[i], SPR_PLAYER_Y[i], rsc_t, this, Sprite::Height, 120);
		if (++id_t == 4) { id_t = 0; }
	}
	//显示按钮控件
	// 同花顺标签
	new Sprite(150, 512, "img/label/straight_flush.png", this, Sprite::Height, 20);
	// 同花顺按钮
	for (int i = 0; i < 4; i++)
	{
		auto path = "img/btn/straight_flush" + QString::number(i) + "0.png";
		straight_flush_btns_[i] = new Button(220 + i * 40, 505, path, this, Button::Height, 40);
		path = "img/btn/straight_flush" + QString::number(i) + "1.png";
		straight_flush_btns_[i]->set_pm(path, Button::Disabled);
	}
	//理牌按钮
	auto arrange_btn = new Button(700, 506, "img/btn/arrange0.png", this, Button::Height, 40);
	arrange_btn->set_pm("img/btn/arrange1.png", Button::Mode2);
	//自动整理牌堆，并显示
	sort_card_heap();
	//显示记录按钮
	auto btn_t = new Button(850, 10, "img/btn/record.png", this, Button::Height, 40);
	connect(btn_t, &Button::click_emit, btn_t, &Button::show_record);
}

void PlayerWidget::closeEvent(QCloseEvent* event)
{
	emit player_close();
}

void PlayerWidget::sort_card_heap()
{
	//无卡牌的情况
	if (card_heaps_.empty()) { return; }

	//获取全部卡牌并重排
	std::vector<Card>cards;
	for (auto i : card_heaps_)
	{
		cards.insert(cards.end(), i.second.begin(), i.second.end());
	}
	sort(cards.begin(), cards.end(), [](Card& c1, Card& c2) {return c2 > c1; });

	//由大到小分点数将卡牌加入到牌堆中
	card_heaps_.clear();
	card_heaps_.push_back(std::make_pair<bool, std::vector<Card>>(false, {}));
	int last_point = (cards.front()).get_point();
	for (auto i : cards)
	{
		//获取卡牌点数
		int point = i.get_point();
		//同种点数牌，加在统一堆
		if (point == last_point) {
			card_heaps_.back().second.push_back(i);
		}
		//不同点数牌，新建一堆
		else {
			card_heaps_.push_back(std::make_pair<bool, std::vector<Card>>(false, {}));
			card_heaps_.back().second.push_back(i);
			last_point = point;
		}
	}
	//更新牌堆显示
	update_card_heap_show();
}
void PlayerWidget::update_card_heap_show()
{
	emit delete_all_card_bottons();

	//相邻两张牌的偏移量
	//中心位置x:480,极限位置x:115
	int x_count = card_heaps_.size();
	int offset_x = min(100, ((480 - 115) * 2 - 100) / (x_count - 1));
	int offset_y = 35;

	//起始x位置，由左到右渲染
	int x = 480 - (offset_x * (x_count - 1) + 100) / 2.0;
	for (auto i : card_heaps_)
	{
		//初始y坐标，由上到下渲染
		int y = 360 - (i.second.size() - 1) * offset_y;
		for (auto& j : i.second)
		{
			//创建卡牌按钮
			auto card_btn = new CardButton(x, y, j, this);
			//连接卡牌与玩家窗口事件
			connect(card_btn, &CardButton::card_selected, this, &PlayerWidget::on_card_selected);
			connect(card_btn, &CardButton::card_unselected, this, &PlayerWidget::on_card_unselected);
			y += offset_y;
		}
		x += offset_x;
	}
}

void PlayerWidget::update_all()
{
	//清空原有heap
	card_heaps_.clear();
	card_heaps_.push_back(std::make_pair<int, std::vector<Card>>(false, {}));
	//根据Hand中的数据重建UI
	for (auto i : hand_->get_cards())
	{
		card_heaps_.back().second.push_back(i);
	}
	sort_card_heap();
}


void PlayerWidget::on_card_selected(CardButton* card_btn)
{
	//加入已选牌的牌堆
	selected_cards_.push_back(card_btn);
	//将已选card_btns转换为cards
	std::vector<Card>cards;
	for (auto i : selected_cards_)
	{
		cards.push_back(i->get_card());
	}
	//检查所选卡牌情况
	auto selected_info = hand_->check(cards);
	////非合法牌型
	//if (selected_info.first == -1)
	//{

	//}
}
void PlayerWidget::on_card_unselected(CardButton* card_btn)
{
	//从已选牌堆中删除
	remove(selected_cards_.begin(),selected_cards_.end(),card_btn);
}


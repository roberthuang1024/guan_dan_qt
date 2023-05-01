﻿#include"card_btn.h"
#include"player_widget.h"

CardButton::CardButton(int x, int y, const Card& card, PlayerWidget* parent) :
	Button(x, y,
		"img/card/" + QString::number(card.get_point()) + "_" + QString::number(card.get_suit()) + ".png",
		parent, Button::Size, 40), card_(card)
{
	//禁用常规按钮动画
	this->disable_animation();
	//设置选中图标
	set_pm("img/card/" + QString::number(card.get_point()) + "_" + QString::number(card.get_suit()) + "c.png", Mode2);
	//绑定自杀事件
	connect(parent,&PlayerWidget::delete_all_card_bottons, this, &CardButton::delete_self);
	//绑定点击事件
	connect(this, &CardButton::clicked, this, &CardButton::on_clicked);
}

void CardButton::delete_self()
{
	delete this;
}

void CardButton::on_clicked()
{
	//取消选中
	if (mode_ == Mode2) {
		emit card_unselected(this);
	}
	//如果未被选中，选中
	else {
		emit card_selected(this);
	}
	reverse_mode();
}



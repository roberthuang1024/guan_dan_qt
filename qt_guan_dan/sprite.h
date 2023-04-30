﻿#ifndef SPRITE_H_
#define SPRITE_H_
#include <qpushbutton.h>
#include <qimage.h>
#include <windows.h>
#include<QPropertyAnimation.h>

//自定义UI类
//继承QPushButton是因为QLabel图像缩放会有高DPI失真问题
class Sprite :public QPushButton
{
public:
	enum SizeMode { Size, Width, Height };
	//@para
	//img: 图片路径
	//parent:父 窗口
	//size: 图片大小，100为原始大小
	Sprite(int x, int y, QString img, QWidget* parent, SizeMode mode = Size, double size = 100);

	int get_w() { return w_; }
	int get_h() { return h_; }

protected:
	//实际长和宽
	int w_;
	int h_;

};
#endif // BUTTON_H_

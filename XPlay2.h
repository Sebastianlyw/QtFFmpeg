#pragma once

#include <QtWidgets/QWidget>
#include "ui_XPlay2.h"

class XPlay2 : public QWidget
{
	Q_OBJECT

public:
	XPlay2(QWidget *parent = Q_NULLPTR);
	~XPlay2();

	void timerEvent(QTimerEvent *e);

	void resizeEvent(QResizeEvent *e);

	void mouseDoubleClickEvent(QMouseEvent *e);
	void SetPause(bool isPause);
	
public slots:
	void OpenFile();
	void PlayOrPause();
	void SliderPress();
	void SliderRelease();
private:
	bool isSliderPress = false;
	Ui::XPlay2Class ui;
};

#pragma once

#include "titlewidget.h"
#include "gamewidget.h"
#include <QStackedWidget>

class ManagerWidget : public QStackedWidget
{
	Q_OBJECT

public:
	ManagerWidget(QWidget* parent = nullptr);

private:
	TitleWidget* m_titleWidget;
	GameWidget* m_gameWidget;
};
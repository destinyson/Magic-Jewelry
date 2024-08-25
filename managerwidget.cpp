#include "managerwidget.h"

ManagerWidget::ManagerWidget(QWidget* parent)
	: QStackedWidget(parent)
{
	resize(parent->size());
	setStyleSheet("background-color: black;");

	m_titleWidget = new TitleWidget(this);
	m_gameWidget = new GameWidget(this);

	addWidget(m_titleWidget);
	addWidget(m_gameWidget);

	setCurrentIndex(0);
}
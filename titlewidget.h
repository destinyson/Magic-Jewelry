#pragma once

#include "scenewidget.h"
#include "strwidget.h"
#include <QLabel>
#include <QTimer>
#include <QShowEvent>
#include <QHideEvent>
#include <QKeyEvent>

class TitleWidget : public SceneWidget
{
	Q_OBJECT

public:
	TitleWidget(QWidget* parent = nullptr);

protected:
	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;

private slots:
	void onTimeout();

private:
	void setLetterJewelry();

	QWidget* m_letterWidget;
	QVector<QLabel*> m_letterJewelryLabelVec;
	StrWidget* m_hiScoreTextWidget;
	StrWidget* m_hiScoreNumberWidget;

	QTimer* m_timer;

	int m_letterIdx;
	int m_jewelryIdx;
};
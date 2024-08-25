#pragma once

#include <QLabel>
#include <QWidget>
#include <QPixmap>
#include <QTimer>

class Prefab : public QLabel
{
	Q_OBJECT

public:
	Prefab(QPixmap pixmap, QWidget* parent = nullptr);

	void active();
	void deactive();

	virtual void init() = 0;

private:

	virtual void fixedUpdate() = 0;

	QTimer* m_timer;
};
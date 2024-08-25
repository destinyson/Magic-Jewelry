#include "scenery.h"
#include "common.h"

Moon::Moon(QWidget* parent)
	: Prefab(QPixmap(Common::moonImgPath), parent)
	, m_moveSpeed(2)
{}

void Moon::init()
{
	move(Common::gameMoonPosX, Common::height);
}

void Moon::fixedUpdate()
{
	if (pos().y() <= -height())
		move(pos().x(), Common::height);
	else
		move(pos().x(), pos().y() - m_moveSpeed);
}


Star::Star(int posOrder, int pixmapOrder, int speed, QWidget* parent)
	: Prefab(Common::starPixmapVec[pixmapOrder], parent)
	, m_posOrder(posOrder)
	, m_moveSpeed(speed)
{}

void Star::init()
{
	move(Common::gameStarPosX[m_posOrder], Common::height);
}

void Star::fixedUpdate()
{
	if (pos().y() <= -height())
		move(pos().x(), Common::height);
	else
		move(pos().x(), pos().y() - m_moveSpeed);
}


Flame::Flame(QWidget* parent)
	: Prefab(QPixmap(Common::flameImgPath[0]), parent)
	, m_pixmapOrder(1)
	, m_frameVal(20)
	, m_frameCount(0)
{}

void Flame::init()
{
	move(Common::gameFlamePos);
	setPixmap(QPixmap(Common::flameImgPath[0]));
}

void Flame::fixedUpdate()
{
	if (m_frameCount == m_frameVal)
	{
		QPixmap pixmap = QPixmap(Common::flameImgPath[m_pixmapOrder]);
		resize(pixmap.size());
		setPixmap(pixmap);
		m_pixmapOrder = (m_pixmapOrder + 1) % 2;

		m_frameCount = 0;
	}
	else
		++m_frameCount;
}

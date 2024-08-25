#include "prefab.h"
#include "common.h"

Prefab::Prefab(QPixmap pixmap, QWidget* parent)
	: QLabel(parent)
	, m_timer(new QTimer(this))
{
	resize(pixmap.size());
	setPixmap(pixmap);

	setAttribute(Qt::WA_TranslucentBackground, true);

	connect(m_timer, &QTimer::timeout, this, &Prefab::fixedUpdate);
}

void Prefab::active()
{
	m_timer->start(Common::mspf);
	init();
}

void Prefab::deactive()
{
	m_timer->stop();
}
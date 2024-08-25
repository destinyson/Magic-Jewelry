#include "titlewidget.h"
#include "managerwidget.h"
#include "common.h"

TitleWidget::TitleWidget(QWidget* parent)
	: SceneWidget(Qt::StrongFocus, parent)
	, m_timer(new QTimer(this))
	, m_letterIdx(0)
	, m_jewelryIdx(0)
{
	m_letterWidget = new QWidget(this);
	m_letterWidget->setGeometry(Common::titleLetterRect);

	m_hiScoreTextWidget = new StrWidget("HI SCORE", QColor::fromRgb(100, 176, 255), this);
	m_hiScoreTextWidget->move(Common::titleHiScoreTextPos);

	m_hiScoreNumberWidget = new StrWidget("0000000", QColor::fromRgb(255, 255, 255), this);
	m_hiScoreNumberWidget->move(Common::titleHiScoreNumberPos);

	connect(m_timer, &QTimer::timeout, this, &TitleWidget::onTimeout);
}


void TitleWidget::showEvent(QShowEvent* event)
{
	m_letterIdx = 0;
	m_jewelryIdx = 0;
	setLetterJewelry();

	m_hiScoreNumberWidget->changeStr(std::to_string(Common::hiScore).c_str());

	m_timer->start(500);

	QWidget::showEvent(event);
}

void TitleWidget::hideEvent(QHideEvent* event)
{
	m_timer->stop();

	QWidget::hideEvent(event);
}

void TitleWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Return)
	{
		ManagerWidget* parent = qobject_cast<ManagerWidget*>(parentWidget());
		parent->setCurrentIndex(1);
	}
}

void TitleWidget::onTimeout()
{
	if (m_jewelryIdx == 5)
	{
		m_jewelryIdx = 0;
		++m_letterIdx;
	}
	else
		++m_jewelryIdx;

	if (m_letterIdx == 7)
	{
		ManagerWidget* parent = qobject_cast<ManagerWidget*>(parentWidget());
		parent->setCurrentIndex(1);
	}
	else
		setLetterJewelry();
}

void TitleWidget::setLetterJewelry()
{
	if (m_jewelryIdx == 0)
	{
		for (auto& label : m_letterJewelryLabelVec)
			delete label;
		m_letterJewelryLabelVec.clear();

		auto letterPixel = Common::titleLetterPixel[m_letterIdx];
		for (int i = 0; i < 7; ++i)
		{
			for (int j = 0; j < 7; ++j)
			{
				if (letterPixel[i][j])
				{
					QLabel* label = new QLabel(m_letterWidget);
					label->setGeometry(Common::jewelryWidth * j, Common::jewelryHeight * i, Common::jewelryWidth, Common::jewelryHeight);
					m_letterJewelryLabelVec.emplace_back(label);
				}
			}
		}
	}

	for (auto& label : m_letterJewelryLabelVec)
	{
		label->setPixmap(Common::jewelryPixmapVec[m_jewelryIdx]);
		label->show();
	}
}
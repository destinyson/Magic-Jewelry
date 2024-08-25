#include "strwidget.h"
#include "common.h"

StrWidget::StrWidget(const char* str, const QColor& color, QWidget* parent, bool isTranslucent)
	: QWidget(parent)
	, m_color(color)
{
	int len = strlen(str);
	m_str = new char[len + 1];
	strcpy_s(m_str, len + 1, str);

	resize(len * Common::fontSize + (len - 1) * Common::fontMargin, Common::fontSize);
	for (int i = 0; i < len; ++i)
	{
		QLabel* label = new QLabel(this);
		label->setGeometry((Common::fontSize + Common::fontMargin) * i, 0, Common::fontSize, Common::fontSize);

		if (str[i] >= 'A' && str[i] < ' Z')
		{
			Common::applyColorFilter(Common::letterPixmapVec[str[i] - 'A'], color);
			label->setPixmap(Common::letterPixmapVec[str[i] - 'A']);
		}
		else if (str[i] >= '0' && str[i] <= '9')
		{
			Common::applyColorFilter(Common::digitPixmapVec[str[i] - '0'], color);
			label->setPixmap(Common::digitPixmapVec[str[i] - '0']);
		}
		else;

		label->setAttribute(Qt::WA_TranslucentBackground, isTranslucent);

		QLabel* marginLabel = new QLabel(this);
		marginLabel->setGeometry((Common::fontSize + Common::fontMargin) * i - Common::fontMargin, 0, Common::fontMargin, Common::fontSize);
		marginLabel->setAttribute(Qt::WA_TranslucentBackground, isTranslucent);

		m_labelVec.emplace_back(label);
	}
	
	if (isTranslucent)
		setAttribute(Qt::WA_TranslucentBackground, true);
	else
		setStyleSheet("background-color: black;");
}

StrWidget::~StrWidget() { delete[] m_str; }

void StrWidget::changeStr(const char* str)
{
	for (size_t idx = 0; idx < strlen(str); ++idx)
	{
		if (str[idx] != m_str[strlen(m_str) - strlen(str) + idx])
		{
			m_str[strlen(m_str) - strlen(str) + idx] = str[idx];
			if (str[idx] >= 'A' && str[idx] < ' Z')
			{
				Common::applyColorFilter(Common::letterPixmapVec[str[idx] - 'A'], m_color);
				m_labelVec[strlen(m_str) - strlen(str) + idx]->setPixmap(Common::letterPixmapVec[str[idx] - 'A']);
			}
			else if (str[idx] >= '0' && str[idx] <= '9')
			{
				Common::applyColorFilter(Common::digitPixmapVec[str[idx] - '0'], m_color);
				m_labelVec[strlen(m_str) - strlen(str) + idx]->setPixmap(Common::digitPixmapVec[str[idx] - '0']);
			}
			else;
		}
	}
}

void StrWidget::changeColor(QColor color)
{
	for (auto& label : m_labelVec)
	{
		QPixmap pixmap = std::move(label->pixmap());
		Common::applyColorFilter(pixmap, color);
		label->setPixmap(pixmap);
	}
}
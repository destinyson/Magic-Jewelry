#pragma once

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QColor>

class StrWidget : public QWidget
{
	Q_OBJECT

public:
	StrWidget(const char* str, const QColor& color, QWidget* parent = nullptr, bool isTranslucent = true);
	~StrWidget();

	void changeStr(const char* str);
	void changeColor(QColor color);

private:
	char* m_str;
	const QColor m_color;
	QVector<QLabel*> m_labelVec;
};
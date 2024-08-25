#pragma once

#include <QWidget>

class SceneWidget : public QWidget
{
	Q_OBJECT

public:
	SceneWidget(Qt::FocusPolicy policy, QWidget* parent = nullptr);
};
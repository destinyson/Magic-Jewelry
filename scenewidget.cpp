#include "scenewidget.h"

SceneWidget::SceneWidget(Qt::FocusPolicy policy, QWidget* parent)
	: QWidget(parent)
{
	resize(parent->size());

	setFocusPolicy(policy);
}
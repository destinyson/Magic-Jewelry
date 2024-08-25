#pragma once

#include "prefab.h"

class Moon : public Prefab
{
	Q_OBJECT

public:
	Moon(QWidget* parent = nullptr);

	void init() override;

private:
	void fixedUpdate() override;

	int m_moveSpeed;
};

class Star : public Prefab
{
	Q_OBJECT

public:
	Star(int posOrder, int pixmapOrder, int speed, QWidget* parent = nullptr);

	void init() override;

private:
	void fixedUpdate() override;

	int m_posOrder;
	int m_moveSpeed;
};

class Flame : public Prefab
{
public:
	Flame(QWidget* parent = nullptr);

	void init() override;

private:
	void fixedUpdate() override;

	int m_frameVal;
	int m_frameCount;
	int m_pixmapOrder;
};
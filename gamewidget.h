#pragma once

#include "scenewidget.h"
#include "strwidget.h"
#include "scenery.h"
#include <QLabel>
#include <QShowEvent>
#include <QHideEvent>
#include <QKeyEvent>
#include <QRandomGenerator>
#include <QMediaPlayer>

class GameWidget : public SceneWidget
{
	Q_OBJECT

public:
	enum class Status { move, settle, erase, lose };

	GameWidget(QWidget* parent = nullptr);

protected:
	void showEvent(QShowEvent* event) override;
	void hideEvent(QHideEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;

private:
	void fixedUpdate();

	void createJewelryGroup(bool bonus);
	void repaintJewelry(int posX, int posY);
	void moveJewelryGroup();
	void changeJewelryGroup();
	void dropJewelryGroup();
	void repaintBonusJewelryGroup(int posX, int posY, int idx);
	int checkBoard(bool bonus);
	int calculateScore();
	void eraseJewelry();

	Status m_status;

	int m_score;
	int m_jewelryCount;
	int m_level;

	QLabel* m_backgroundLabel;
	Moon* m_moon;
	Star* m_starArray[8];
	Flame* m_flame;
	
	QLabel* m_nextJewelryGroup[3];
	int m_nextJewelryIdxGroup[3];

	QWidget* m_boardWidget;
	QLabel* m_boardJewelryLabel[16][6];
	int m_boardJewelryIdx[16][6];
	bool m_boardJewelryNeedErase[16][6];

	StrWidget* m_scoreTextWidget;
	StrWidget* m_scoreNumberWidget;
	StrWidget* m_jewelryCountTextWidget;
	StrWidget* m_jewelryCountNumberWidget;
	StrWidget* m_levelTextWidget;
	StrWidget* m_levelNumberWidget;
	StrWidget* m_roundScoreNumberWidget;
	StrWidget* m_gameOverTextWidget;
	int m_roundScoreMoveSpeed;

	QRandomGenerator m_generator;
	QTimer* m_timer;

	QMediaPlayer* m_bgmPlayer;
	QMediaPlayer* m_audioPlayer;

	bool m_leftMovePressed;
	bool m_rightMovePressed;
	bool m_changePressed;
	bool m_acceleratePressed;
	bool m_isPause;
	int m_moveDir;
	bool m_isChange;
	int m_moveFrameCount;
	int m_moveFrameVal;
	int m_changeFrameCount;
	int m_changeFrameVal;
	int m_dropFrameCount;
	int m_dropFrameVal;
	int m_dropFrameNormalVal;
	int m_dropFrameNormalValMax;
	int m_dropFrameNormalValMin;
	int m_dropFrameAccelerateVal;
	int m_blingCount;
	int m_blingVal;
	int m_blingFrameCount;
	int m_blingFrameVal;
	int m_loseFrameCount;
	int m_loseFrameVal;

	int m_boardJewelryGroupIndexX;
	int m_boardJewelryGroupIndexY;

	bool m_preErase;
	int m_eraseNum;
	int m_eraseRounds;
	int m_factor;
	int m_factorMin;
	int m_factorMax;
	int m_roundScore;

	bool m_isBonus;
	bool m_eraseBonus;

	int m_loseIndexX;
	int m_loseIndexY;
};
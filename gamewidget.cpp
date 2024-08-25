#include "gamewidget.h"
#include "managerwidget.h"
#include "common.h"
#include <QPixmap>
#include <QTime>
#include <QAudioOutput>
#include <QUrl>

GameWidget::GameWidget(QWidget* parent)
	: SceneWidget(Qt::StrongFocus, parent)
	, m_roundScoreMoveSpeed(9)
	, m_generator(QRandomGenerator(QTime::currentTime().msecsSinceStartOfDay()))
	, m_timer(new QTimer(this))
	, m_bgmPlayer(new QMediaPlayer(this))
	, m_audioPlayer(new QMediaPlayer(this))
	, m_moveFrameVal(5)
	, m_changeFrameVal(5)
	, m_dropFrameNormalValMax(50)
	, m_dropFrameNormalValMin(18)
	, m_dropFrameAccelerateVal(2)
	, m_blingVal(12)
	, m_blingFrameVal(5)
	, m_loseFrameVal(2)
	, m_factorMin(10)
	, m_factorMax(20)
{
	m_moon = new Moon(this);

	for (int idx = 0; idx < 8; ++idx)
	{
		int pixmapOrder = m_generator.bounded(Common::starPixmapVec.size());
		int speed = m_generator.bounded(8) + 1;
		m_starArray[idx] = new Star(idx, pixmapOrder, speed, this);
	}

	m_backgroundLabel = new QLabel(this);
	m_backgroundLabel->resize(parent->width(), parent->height());
	m_backgroundLabel->setAttribute(Qt::WA_TranslucentBackground, true);

	m_flame = new Flame(this);

	for (int idx = 0; idx < 3; ++idx)
	{
		m_nextJewelryGroup[idx] = new QLabel(this);
		m_nextJewelryGroup[idx]->setGeometry(
			Common::gameNextJewelryPos.x(),
			Common::gameNextJewelryPos.y() + idx * Common::jewelryHeight,
			Common::jewelryWidth,
			Common::jewelryHeight);
	}

	m_boardWidget = new QWidget(this);
	m_boardWidget->setGeometry(Common::gameBoardRect);
	m_boardWidget->setAttribute(Qt::WA_TranslucentBackground, true);

	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			QLabel* label = new QLabel(m_boardWidget);
			label->setGeometry(Common::jewelryWidth * j, Common::jewelryHeight * i, Common::jewelryWidth, Common::jewelryHeight);
			label->setAttribute(Qt::WA_TranslucentBackground, true);
			m_boardJewelryLabel[i][j] = label;
		}
	}

	m_scoreTextWidget = new StrWidget("SCORE", QColor::fromRgb(100, 176, 255), this);
	m_scoreTextWidget->move(Common::gameScoreTextPos);

	m_scoreNumberWidget = new StrWidget("0000000", QColor::fromRgb(192, 223, 255), this);
	m_scoreNumberWidget->move(Common::gameScoreNumberPos);

	m_jewelryCountTextWidget = new StrWidget("JEWELRY", QColor::fromRgb(183, 30, 123), this);
	m_jewelryCountTextWidget->move(Common::gameJewelryCountTextPos);

	m_jewelryCountNumberWidget = new StrWidget("00000", QColor::fromRgb(255, 129, 112), this);
	m_jewelryCountNumberWidget->move(Common::gameJewelryCountNumberPos);

	m_levelTextWidget = new StrWidget("LEVEL", QColor::fromRgb(100, 176, 255), this);
	m_levelTextWidget->move(Common::gameLevelTextPos);

	m_levelNumberWidget = new StrWidget("000", QColor::fromRgb(136, 216, 0), this);
	m_levelNumberWidget->move(Common::gameLevelNumberPos);

	m_roundScoreNumberWidget = new StrWidget("00000", QColor::fromRgb(192, 223, 255), this);

	m_gameOverTextWidget = new StrWidget("GAME  OVER", Common::gameOverTextColor[0], this, false);
	m_gameOverTextWidget->move(Common::gameOverTextPos);

	connect(m_timer, &QTimer::timeout, this, &GameWidget::fixedUpdate);

	m_bgmPlayer->setLoops(-1);
}


void GameWidget::showEvent(QShowEvent* event)
{
	m_timer->start(Common::mspf);

	m_bgmPlayer->setSource(QUrl(Common::bgmTemplatePath));
	QAudioOutput* bgmOutput = new QAudioOutput(this);
	bgmOutput->setVolume(100);
	m_bgmPlayer->setAudioOutput(bgmOutput);
	m_bgmPlayer->play();

	QAudioOutput* audioOutput = new QAudioOutput(this);
	audioOutput->setVolume(100);
	m_audioPlayer->setAudioOutput(audioOutput);

	m_status = Status::move;

	m_score = 0;
	m_jewelryCount = 0;
	m_level = 0;
	m_scoreNumberWidget->changeStr("0000000");
	m_jewelryCountNumberWidget->changeStr("00000");
	m_levelNumberWidget->changeStr("000");

	m_backgroundLabel->setPixmap(QPixmap(Common::backgroundImgTemplatePath));
	m_moon->active();
	for (auto& star : m_starArray)
		star->active();
	m_flame->active();

	for (int idx = 0; idx < 3; ++idx)
		m_nextJewelryIdxGroup[idx] = m_generator.bounded(Common::jewelryPixmapVec.size() - 1);

	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 6; ++j)
			m_boardJewelryLabel[i][j]->clear();
	}
	std::fill(&m_boardJewelryIdx[0][0], &m_boardJewelryIdx[0][0] + 96, -1);
	std::fill(&m_boardJewelryNeedErase[0][0], &m_boardJewelryNeedErase[0][0] + 96, false);

	createJewelryGroup(false);

	m_leftMovePressed = false;
	m_rightMovePressed = false;
	m_changePressed = false;
	m_acceleratePressed = false;
	m_isPause = false;
	m_moveDir = 0;
	m_isChange = false;
	m_moveFrameCount = 0;
	m_changeFrameCount = 0;
	m_dropFrameCount = 0;
	m_dropFrameNormalVal = m_dropFrameNormalValMax;
	m_dropFrameVal = m_dropFrameNormalVal;
	m_blingCount = 0;
	m_blingFrameCount = 0;
	m_loseFrameCount = 0;

	m_boardJewelryGroupIndexX = 0;
	m_boardJewelryGroupIndexY = 3;

	m_preErase = false;
	m_eraseNum = 0;
	m_eraseRounds = 0;
	m_factor = m_factorMin;
	m_roundScore = 0;

	m_roundScoreNumberWidget->move(Common::gameRoundScoreNumberPos);
	m_roundScoreNumberWidget->hide();

	m_gameOverTextWidget->hide();

	m_isBonus = false;
	m_eraseBonus = false;

	m_loseIndexX = 15;
	m_loseIndexY = 5;

	QWidget::showEvent(event);
}

void GameWidget::hideEvent(QHideEvent* event)
{
	if (m_score > Common::hiScore)
		Common::hiScore = m_score;

	m_timer->stop();

	m_bgmPlayer->stop();
	m_audioPlayer->stop();

	m_moon->deactive();
	for (auto& star : m_starArray)
		star->deactive();
	m_flame->deactive();

	QWidget::hideEvent(event);
}

void GameWidget::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_A)
		m_leftMovePressed = true;
	if (event->key() == Qt::Key_D)
		m_rightMovePressed = true;
	if (event->key() == Qt::Key_W)
		m_changePressed = true;
	if (event->key() == Qt::Key_S)
		m_acceleratePressed = true;

	if (m_status != Status::lose)
	{
		if (event->key() == Qt::Key_Space)
		{
			m_isPause = !m_isPause;
			if (m_isPause)
			{
				m_timer->stop();
				m_bgmPlayer->stop();
			}
			else
			{
				m_timer->start();
				m_bgmPlayer->play();
			}
		}
	}
	else
	{
		if (event->key() == Qt::Key_Return)
		{
			ManagerWidget* parent = qobject_cast<ManagerWidget*>(parentWidget());
			parent->setCurrentIndex(0);
		}
	}
}

void GameWidget::keyReleaseEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_A)
		m_leftMovePressed = false;
	if (event->key() == Qt::Key_D)
		m_rightMovePressed = false;
	if (event->key() == Qt::Key_W)
		m_changePressed = false;
	if (event->key() == Qt::Key_S)
		m_acceleratePressed = false;
}

void GameWidget::fixedUpdate()
{
	switch (m_status)
	{
	case Status::move:
	{
		if (m_leftMovePressed == m_rightMovePressed)
			m_moveDir = 0;
		else
			m_moveDir = m_leftMovePressed ? -1 : 1;
		m_isChange = m_changePressed;
		m_dropFrameVal = m_acceleratePressed ? m_dropFrameAccelerateVal : m_dropFrameNormalVal;

		if (m_isBonus)
		{
			if (m_blingFrameCount >= m_blingFrameVal)
			{
				if (m_boardJewelryLabel[m_boardJewelryGroupIndexX][m_boardJewelryGroupIndexY]->isVisible())
				{
					for (int i = 0; i < 3; ++i)
						m_boardJewelryLabel[m_boardJewelryGroupIndexX + i][m_boardJewelryGroupIndexY]->hide();
				}
				else
				{
					for (int i = 0; i < 3; ++i)
						m_boardJewelryLabel[m_boardJewelryGroupIndexX + i][m_boardJewelryGroupIndexY]->show();
				}
				m_blingFrameCount = 0;
			}

			else
				++m_blingFrameCount;
		}

		if (m_moveFrameCount >= m_moveFrameVal)
		{
			if (m_moveDir)
				moveJewelryGroup();
			m_moveDir = 0;
			m_moveFrameCount = 0;
		}
		else
			++m_moveFrameCount;

		if (m_changeFrameCount >= m_changeFrameVal)
		{
			if (m_isChange)
			{
				changeJewelryGroup();
				m_audioPlayer->stop();
				m_audioPlayer->setSource(QUrl(Common::wrongAudioPath));
				m_audioPlayer->play();
			}
			m_isChange = false;
			m_changeFrameCount = 0;
		}
		else
			++m_changeFrameCount;

		if (m_dropFrameCount >= m_dropFrameVal)
		{
			if (m_boardJewelryGroupIndexX < 13
				&& m_boardJewelryIdx[m_boardJewelryGroupIndexX + 3][m_boardJewelryGroupIndexY] == -1)
				dropJewelryGroup();
			else
			{
				m_audioPlayer->stop();
				m_audioPlayer->setSource(QUrl(Common::dropAudioPath));
				m_audioPlayer->play();

				if (m_isBonus)
				{
					int idx = m_boardJewelryGroupIndexX == 13
						? m_generator.bounded(6)
						: m_boardJewelryIdx[m_boardJewelryGroupIndexX + 3][m_boardJewelryGroupIndexY];

					for (int i = 0; i < 3; ++i)
						repaintBonusJewelryGroup(m_boardJewelryGroupIndexX, m_boardJewelryGroupIndexY, idx);

					for (int i = 0; i < 16; ++i)
					{
						for (int j = 0; j < 6; ++j)
							m_boardJewelryLabel[i][j]->show();
					}
					m_blingFrameCount = 0;
				}

				m_status = Status::settle;
			}
			m_dropFrameCount = 0;
		}
		else
			++m_dropFrameCount;

		break;
	}

	case Status::settle:
	{
		m_eraseNum = checkBoard(m_eraseBonus);
		m_eraseBonus = false;

		if (m_eraseNum)
		{
			m_audioPlayer->stop();
			m_audioPlayer->setSource(QUrl(Common::scoreAudioPath));
			m_audioPlayer->play();

			m_roundScore = calculateScore();

			std::string roundScoreStr = std::to_string(m_roundScore);
			m_roundScoreNumberWidget->changeStr((std::string(5 - roundScoreStr.length(), '0') + roundScoreStr).c_str());
			m_roundScoreNumberWidget->move(Common::gameRoundScoreNumberPos);
			m_roundScoreNumberWidget->show();

			++m_eraseRounds;
			m_status = Status::erase;
		}

		else
		{
			int minX = 15;
			for (int j = 0; j < 6; ++j)
			{
				int i = 0;
				for (; i < 16 && m_boardJewelryIdx[i][j] == -1; ++i);
				if (minX > i)
					minX = i;
			}

			if (minX < 3)
				m_status = Status::lose;

			else
			{
				if (m_eraseRounds)
				{
					m_preErase = true;
					++m_factor;
					if (m_factor > m_factorMax)
						m_factor = m_factorMax;
				}
				else
				{
					if (m_preErase)
						--m_factor;
					m_preErase = false;
					--m_factor;
					if (m_factor < m_factorMin)
						m_factor = m_factorMin;
				}

				m_eraseRounds = 0;

				if (!m_isBonus && m_jewelryCount >= 70 * (m_level + 1))
				{
					createJewelryGroup(true);
					m_isBonus = true;
					m_eraseBonus = true;
				}

				else
				{
					if (m_isBonus)
					{
						++m_level;
						m_levelNumberWidget->changeStr(std::to_string(m_level).c_str());

						if (m_dropFrameNormalVal > m_dropFrameNormalValMin)
							m_dropFrameNormalVal -= 4;

						int len = strlen(Common::backgroundImgTemplatePath);
						char* backgroundImgPath = new char[len + 1];
						strcpy_s(backgroundImgPath, len + 1, Common::backgroundImgTemplatePath);
						backgroundImgPath[len - 5] += m_level % 8;
						m_backgroundLabel->setPixmap(QPixmap(backgroundImgPath));
						delete[] backgroundImgPath;

						len = strlen(Common::bgmTemplatePath);
						char* bgmPath = new char[len + 1];
						strcpy_s(bgmPath, len + 1, Common::bgmTemplatePath);
						bgmPath[len - 5] += m_level % 8;
						m_bgmPlayer->stop();
						m_bgmPlayer->setSource(QUrl(bgmPath));
						m_bgmPlayer->play();
						delete[] bgmPath;

						m_isBonus = false;
					}
					createJewelryGroup(false);
				}

				m_status = Status::move;
			}
		}

		break;
	}

	case Status::erase:
	{
		if (m_roundScoreNumberWidget->pos().x() <= Common::width)
			m_roundScoreNumberWidget->move(m_roundScoreNumberWidget->pos().x() + m_roundScoreMoveSpeed, m_roundScoreNumberWidget->pos().y());

		else if (m_roundScoreNumberWidget->isVisible())
		{
			m_roundScoreNumberWidget->hide();

			m_score += m_roundScore;
			m_jewelryCount += m_eraseNum;
			m_scoreNumberWidget->changeStr(std::to_string(m_score).c_str());
			m_jewelryCountNumberWidget->changeStr(std::to_string(m_jewelryCount).c_str());
		}

		else;
			

		if (m_blingFrameCount >= m_blingFrameVal)
		{
			if (m_blingCount >= m_blingVal)
			{
				eraseJewelry();
				m_blingCount = 0;
				m_blingFrameCount = 0;
				m_status = Status::settle;
			}

			else
			{
				if (m_blingCount >= m_blingVal / 2)
				{
					for (int i = 0; i < 16; ++i)
					{
						for (int j = 0; j < 6; ++j)
						{
							if (m_boardJewelryNeedErase[i][j])
								m_boardJewelryLabel[i][j]->setPixmap(Common::jewelryPixmapVec[m_blingCount - m_blingVal / 2]);
						}
					}
				}

				else
				{
					for (int i = 0; i < 16; ++i)
					{
						for (int j = 0; j < 6; ++j)
						{
							if (m_boardJewelryNeedErase[i][j])
							{
								if (m_boardJewelryLabel[i][j]->isVisible())
									m_boardJewelryLabel[i][j]->hide();
								else
									m_boardJewelryLabel[i][j]->show();
							}
						}
					}
				}
				
				++m_blingCount;
				m_blingFrameCount = 0;
			}
		}
		else
			++m_blingFrameCount;

		break;
	}

	case Status::lose:
	{
		if (m_loseIndexX < 3)
		{
			m_gameOverTextWidget->changeColor(Common::gameOverTextColor[m_level % 8]);
			m_gameOverTextWidget->show();
		}

		else
		{
			if (m_loseFrameCount >= m_loseFrameVal)
			{
				m_boardJewelryLabel[m_loseIndexX][m_loseIndexY]->setPixmap(Common::jewelryPixmapVec[6]);
				if (m_loseIndexY == 0)
				{
					m_audioPlayer->stop();
					m_audioPlayer->setSource(QUrl(Common::wrongAudioPath));
					m_audioPlayer->play();
					--m_loseIndexX;
					m_loseIndexY = 5;
				}
				else
					--m_loseIndexY;

				m_loseFrameCount = 0;
			}

			else
				++m_loseFrameCount;
		}

		break;
	}

	default: break;
	}
}

void GameWidget::createJewelryGroup(bool bonus)
{
	if (bonus)
	{
		for (int idx = 0; idx < 3; ++idx)
		{
			m_boardJewelryIdx[idx][3] = 6;
			m_boardJewelryLabel[idx][3]->setPixmap(Common::jewelryPixmapVec[6]);
		}
	}

	else
	{
		for (int idx = 0; idx < 3; ++idx)
		{
			m_boardJewelryIdx[idx][3] = m_nextJewelryIdxGroup[idx];
			m_boardJewelryLabel[idx][3]->setPixmap(Common::jewelryPixmapVec[m_nextJewelryIdxGroup[idx]]);
		}

		for (int idx = 0; idx < 3; ++idx)
		{
			m_nextJewelryIdxGroup[idx] = m_generator.bounded(Common::jewelryPixmapVec.size() - 1);
			m_nextJewelryGroup[idx]->setPixmap(Common::jewelryPixmapVec[m_nextJewelryIdxGroup[idx]]);
		}
	}
	
	m_boardJewelryGroupIndexX = 0;
	m_boardJewelryGroupIndexY = 3;
}

void GameWidget::repaintJewelry(int posX, int posY)
{
	if (m_boardJewelryIdx[posX][posY] == -1)
		m_boardJewelryLabel[posX][posY]->clear();
	else
		m_boardJewelryLabel[posX][posY]->setPixmap(Common::jewelryPixmapVec[m_boardJewelryIdx[posX][posY]]);
}

void GameWidget::moveJewelryGroup()
{
	if (m_boardJewelryGroupIndexY + m_moveDir >= 0 && m_boardJewelryGroupIndexY + m_moveDir < 6
		&& m_boardJewelryIdx[m_boardJewelryGroupIndexX][m_boardJewelryGroupIndexY + m_moveDir] == -1
		&& m_boardJewelryIdx[m_boardJewelryGroupIndexX + 1][m_boardJewelryGroupIndexY + m_moveDir] == -1
		&& m_boardJewelryIdx[m_boardJewelryGroupIndexX + 2][m_boardJewelryGroupIndexY + m_moveDir] == -1)
	{
		m_boardJewelryIdx[m_boardJewelryGroupIndexX][m_boardJewelryGroupIndexY + m_moveDir] =
			m_boardJewelryIdx[m_boardJewelryGroupIndexX][m_boardJewelryGroupIndexY];
		m_boardJewelryIdx[m_boardJewelryGroupIndexX + 1][m_boardJewelryGroupIndexY + m_moveDir] =
			m_boardJewelryIdx[m_boardJewelryGroupIndexX + 1][m_boardJewelryGroupIndexY];
		m_boardJewelryIdx[m_boardJewelryGroupIndexX + 2][m_boardJewelryGroupIndexY + m_moveDir] =
			m_boardJewelryIdx[m_boardJewelryGroupIndexX + 2][m_boardJewelryGroupIndexY];

		m_boardJewelryIdx[m_boardJewelryGroupIndexX][m_boardJewelryGroupIndexY] = -1;
		m_boardJewelryIdx[m_boardJewelryGroupIndexX + 1][m_boardJewelryGroupIndexY] = -1;
		m_boardJewelryIdx[m_boardJewelryGroupIndexX + 2][m_boardJewelryGroupIndexY] = -1;

		for (int i = 0; i < 3; ++i)
		{
			repaintJewelry(m_boardJewelryGroupIndexX + i, m_boardJewelryGroupIndexY);
			repaintJewelry(m_boardJewelryGroupIndexX + i, m_boardJewelryGroupIndexY + m_moveDir);
		}

		m_boardJewelryGroupIndexY += m_moveDir;
	}
}

void GameWidget::changeJewelryGroup()
{
	int temp = m_boardJewelryIdx[m_boardJewelryGroupIndexX + 2][m_boardJewelryGroupIndexY];
	m_boardJewelryIdx[m_boardJewelryGroupIndexX + 2][m_boardJewelryGroupIndexY] =
		m_boardJewelryIdx[m_boardJewelryGroupIndexX + 1][m_boardJewelryGroupIndexY];
	m_boardJewelryIdx[m_boardJewelryGroupIndexX + 1][m_boardJewelryGroupIndexY] =
		m_boardJewelryIdx[m_boardJewelryGroupIndexX][m_boardJewelryGroupIndexY];
	m_boardJewelryIdx[m_boardJewelryGroupIndexX][m_boardJewelryGroupIndexY] = temp;

	for (int i = 0; i < 3; ++i)
		repaintJewelry(m_boardJewelryGroupIndexX + i, m_boardJewelryGroupIndexY);
}

void GameWidget::dropJewelryGroup()
{
	m_boardJewelryIdx[m_boardJewelryGroupIndexX + 3][m_boardJewelryGroupIndexY] =
		m_boardJewelryIdx[m_boardJewelryGroupIndexX + 2][m_boardJewelryGroupIndexY];
	m_boardJewelryIdx[m_boardJewelryGroupIndexX + 2][m_boardJewelryGroupIndexY] =
		m_boardJewelryIdx[m_boardJewelryGroupIndexX + 1][m_boardJewelryGroupIndexY];
	m_boardJewelryIdx[m_boardJewelryGroupIndexX + 1][m_boardJewelryGroupIndexY] =
		m_boardJewelryIdx[m_boardJewelryGroupIndexX][m_boardJewelryGroupIndexY];
	m_boardJewelryIdx[m_boardJewelryGroupIndexX][m_boardJewelryGroupIndexY] = -1;

	for (int i = 0; i <= 3; ++i)
		repaintJewelry(m_boardJewelryGroupIndexX + i, m_boardJewelryGroupIndexY);

	++m_boardJewelryGroupIndexX;
}

void GameWidget::repaintBonusJewelryGroup(int posX, int posY, int idx)
{
	for (int i = 0; i < 3; ++i)
	{
		m_boardJewelryIdx[m_boardJewelryGroupIndexX + i][m_boardJewelryGroupIndexY] = idx;
		repaintJewelry(m_boardJewelryGroupIndexX + i, m_boardJewelryGroupIndexY);
	}
}

int GameWidget::checkBoard(bool bonus)
{
	if (bonus)
	{
		int idx = m_boardJewelryIdx[m_boardJewelryGroupIndexX][m_boardJewelryGroupIndexY];
		for (int i = 0; i < 16; ++i)
		{
			for (int j = 0; j < 6; ++j)
			{
				if (m_boardJewelryIdx[i][j] == idx)
					m_boardJewelryNeedErase[i][j] = true;
			}
		}
	}

	else
	{
		for (int i = 0; i < 16; ++i)
		{
			for (int j = 0; j < 6; ++j)
			{
				if (m_boardJewelryIdx[i][j] == -1)
					continue;

				if (i < 14
					&& m_boardJewelryIdx[i][j] == m_boardJewelryIdx[i + 1][j]
					&& m_boardJewelryIdx[i][j] == m_boardJewelryIdx[i + 2][j])
				{
					m_boardJewelryNeedErase[i][j] = true;
					m_boardJewelryNeedErase[i + 1][j] = true;
					m_boardJewelryNeedErase[i + 2][j] = true;
				}

				if (j < 4
					&& m_boardJewelryIdx[i][j] == m_boardJewelryIdx[i][j + 1]
					&& m_boardJewelryIdx[i][j] == m_boardJewelryIdx[i][j + 2])
				{
					m_boardJewelryNeedErase[i][j] = true;
					m_boardJewelryNeedErase[i][j + 1] = true;
					m_boardJewelryNeedErase[i][j + 2] = true;
				}

				if (i < 14 && j < 4
					&& m_boardJewelryIdx[i][j] == m_boardJewelryIdx[i + 1][j + 1]
					&& m_boardJewelryIdx[i][j] == m_boardJewelryIdx[i + 2][j + 2])
				{
					m_boardJewelryNeedErase[i][j] = true;
					m_boardJewelryNeedErase[i + 1][j + 1] = true;
					m_boardJewelryNeedErase[i + 2][j + 2] = true;
				}

				if (i > 1 && j < 4
					&& m_boardJewelryIdx[i][j] == m_boardJewelryIdx[i - 1][j + 1]
					&& m_boardJewelryIdx[i][j] == m_boardJewelryIdx[i - 2][j + 2])
				{
					m_boardJewelryNeedErase[i][j] = true;
					m_boardJewelryNeedErase[i - 1][j + 1] = true;
					m_boardJewelryNeedErase[i - 2][j + 2] = true;
				}
			}
		}
	}

	int eraseNum = 0;
	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			if (m_boardJewelryNeedErase[i][j])
				++eraseNum;
		}
	}

	return eraseNum;
}

int GameWidget::calculateScore()
{
	return 10 * m_eraseNum * m_factor * (1 << m_eraseRounds) + m_generator.bounded(10);
}

void GameWidget::eraseJewelry()
{
	for (int j = 0; j < 6; ++j)
	{
		int slow = 15;
		for (int fast = 15; fast >= 0; --fast)
		{
			if (!m_boardJewelryNeedErase[fast][j])
				m_boardJewelryIdx[slow--][j] = m_boardJewelryIdx[fast][j];
		}
		for (; slow >= 0; --slow)
			m_boardJewelryIdx[slow][j] = -1;
	}

	for (int i = 0; i < 16; ++i)
	{
		for (int j = 0; j < 6; ++j)
			repaintJewelry(i, j);
	}

	std::fill(&m_boardJewelryNeedErase[0][0], &m_boardJewelryNeedErase[0][0] + 96, false);
}
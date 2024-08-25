#pragma once

#include <QRect>
#include <QVector>
#include <QPixmap>
#include <QWidget>
#include <QColor>

namespace Common
{
	extern const char* jewelryImgPath;

	extern const char* digitImgPath;
	extern const char* letterImgPath;

	extern const char* backgroundImgTemplatePath;

	extern const char* flameImgPath[2];
	extern const char* moonImgPath;
	extern const char* starImgPath;

	extern const char* bgmTemplatePath;
	extern const char* dropAudioPath;
	extern const char* wrongAudioPath;
	extern const char* scoreAudioPath;

	extern QVector<QPixmap> jewelryPixmapVec;

	extern QVector<QPixmap> digitPixmapVec;
	extern QVector<QPixmap> letterPixmapVec;
	
	extern QVector<QPixmap> starPixmapVec;


	extern const int width;
	extern const int height;
	extern const int mspf;

	extern const int jewelryWidth;
	extern const int jewelryHeight;
	extern const int fontSize;
	extern const int fontMargin;

	extern const QRect titleLetterRect;
	extern const bool titleLetterPixel[7][7][7];
	extern const QPoint titleHiScoreTextPos;
	extern const QPoint titleHiScoreNumberPos;

	extern const QPoint gameNextJewelryPos;
	extern const QPoint gameScoreTextPos;
	extern const QPoint gameScoreNumberPos;
	extern const QPoint gameJewelryCountTextPos;
	extern const QPoint gameJewelryCountNumberPos;
	extern const QPoint gameLevelTextPos;
	extern const QPoint gameLevelNumberPos;
	extern const QPoint gameRoundScoreNumberPos;
	extern const QPoint gameOverTextPos;
	extern const QColor gameOverTextColor[8];
	extern const int gameMoonPosX;
	extern const int gameStarPosX[8];
	extern const QPoint gameFlamePos;
	extern const QRect gameBoardRect;


	extern int hiScore;


	void splitImg(const char* imgPath, QVector<QPixmap>& pixmapVec, int rowNum, int colNum);

	void applyColorFilter(QPixmap& pixmap, const QColor& color);
}
#include "common.h"
#include <QPoint>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <cstring>

const char* Common::jewelryImgPath = ":/image/jewelry.png";

const char* Common::digitImgPath = ":/image/font/digit.png";
const char* Common::letterImgPath = ":/image/font/letter.png";

const char* Common::backgroundImgTemplatePath = ":/image/background/background1.png";
const char* Common::flameImgPath[2]{ ":/image/background/flame1.png", ":/image/background/flame2.png" };
const char* Common::moonImgPath = ":/image/background/moon.png";
const char* Common::starImgPath = ":/image/background/star.png";

const char* Common::bgmTemplatePath = "qrc:/audio/bgm/bgm1.mp3";
const char* Common::dropAudioPath = "qrc:/audio/drop.mp3";
const char* Common::wrongAudioPath = "qrc:/audio/wrong.mp3";
const char* Common::scoreAudioPath = "qrc:/audio/score.mp3";

QVector<QPixmap> Common::jewelryPixmapVec;

QVector<QPixmap> Common::digitPixmapVec;
QVector<QPixmap> Common::letterPixmapVec;

QVector<QPixmap> Common::starPixmapVec;


const int Common::width = 1024;
const int Common::height = 896;
const int Common::mspf = 20;

const int Common::jewelryWidth = 64;
const int Common::jewelryHeight = 64;
const int Common::fontSize = 28;
const int Common::fontMargin = 4;

const QRect Common::titleLetterRect = QRect(268, 96, jewelryWidth * 7, jewelryHeight * 7);
const bool Common::titleLetterPixel[7][7][7] =
  // J
{ { { false, false, false, true, true, true, true },
	{ false, false, false, false, false, true, true },
	{ false, false, false, false, false, true, true },
	{ false, false, false, false, false, true, true },
	{ true, true, false, false, false, true, true },
	{ true, true, false, false, false, true, true },
	{ false, true, true, true, true, true, false } },
  // E
  { { true, true, true, true, true, true, true },
	{ true, true, false, false, false, false, false },
	{ true, true, false, false, false, false, false },
	{ true, true, true, true, true, true, false },
	{ true, true, false, false, false, false, false },
	{ true, true, false, false, false, false, false },
	{ true, true, true, true, true, true, true } },
  // W
  { { true, true, false, false, false, true, true },
	{ true, true, false, false, false, true, true },
	{ true, true, false, true, false, true, true },
	{ true, true, true, true, true, true, true },
	{ true, true, true, true, true, true, true },
	{ true, true, true, false, true, true, true },
	{ true, true, false, false, false, true, true } },
  // E
  { { true, true, true, true, true, true, true },
	{ true, true, false, false, false, false, false },
	{ true, true, false, false, false, false, false },
	{ true, true, true, true, true, true, false },
	{ true, true, false, false, false, false, false },
	{ true, true, false, false, false, false, false },
	{ true, true, true, true, true, true, true } },
  // L
  { { false, true, true, false, false, false, false },
	{ false, true, true, false, false, false, false },
	{ false, true, true, false, false, false, false },
	{ false, true, true, false, false, false, false },
	{ false, true, true, false, false, false, false },
	{ false, true, true, false, false, false, false },
	{ false, true, true, true, true, true, true } },
  // R
  { { true, true, true, true, true, true, false },
	{ true, true, false, false, false, true, true },
	{ true, true, false, false, false, true, true },
	{ true, true, false, false, true, true, true },
	{ true, true, true, true, true, false, false },
	{ true, true, false, true, true, true, false },
	{ true, true, false, false, true, true, true } },
  // Y
  { { false, true, true, false, false, true, true },
	{ false, true, true, false, false, true, true },
	{ false, true, true, false, false, true, true },
	{ false, false, true, true, true, true, false },
	{ false, false, false, true, true, false, false },
	{ false, false, false, true, true, false, false },
	{ false, false, false, true, true, false, false } } };
const QPoint Common::titleHiScoreTextPos = QPoint(236, 768);
const QPoint Common::titleHiScoreNumberPos = QPoint(524, 768);

const QPoint Common::gameNextJewelryPos = QPoint(84, 32);
const QPoint Common::gameScoreTextPos = QPoint(756, 32);
const QPoint Common::gameScoreNumberPos = QPoint(724, 72);
const QPoint Common::gameJewelryCountTextPos = QPoint(724, 128);
const QPoint Common::gameJewelryCountNumberPos = QPoint(756, 168);
const QPoint Common::gameLevelTextPos = QPoint(756, 224);
const QPoint Common::gameLevelNumberPos = QPoint(788, 264);
const QPoint Common::gameRoundScoreNumberPos = QPoint(340, 104);
const QPoint Common::gameOverTextPos = QPoint(244, 192);
const QColor Common::gameOverTextColor[8] =
{	QColor::fromRgb(153, 78, 0),
	QColor::fromRgb(102, 102, 102),
	QColor::fromRgb(160, 26, 204),
	QColor::fromRgb(0, 143, 50),
	QColor::fromRgb(183, 30, 123),
	QColor::fromRgb(56, 135, 0),
	QColor::fromRgb(181, 49, 32),
	QColor::fromRgb(21, 95, 217) };
const int Common::gameMoonPosX = 852;
const int Common::gameStarPosX[8] = { 672, 716, 760, 804, 848, 892, 936, 980 };
const QPoint Common::gameFlamePos = QPoint(636, 192);
const QRect Common::gameBoardRect = QRect(212, -160, jewelryWidth * 6, jewelryHeight * 16);


int Common::hiScore = 0;

void Common::splitImg(const char* imgPath, QVector<QPixmap>& pixmapVec, int rowNum, int colNum)
{
	QImage image(imgPath);
	
	int imgTileWidth = image.width() / colNum;
	int imgTileHeight = image.height() / rowNum;
	for (int i = 0; i < rowNum; ++i)
	{
		for (int j = 0; j < colNum; ++j)
		{
			QRect rect(imgTileWidth * j, imgTileHeight * i, imgTileWidth, imgTileHeight);
			QPixmap pixmap = QPixmap::fromImage(image.copy(rect));
			pixmapVec.emplace_back(pixmap);
		}
	}
}


void Common::applyColorFilter(QPixmap& pixmap, const QColor& color) {
	QImage image = pixmap.toImage();

	for (int y = 0; y < image.height(); ++y)
	{
		for (int x = 0; x < image.width(); ++x)
		{
			if (qAlpha(image.pixel(x, y)) == 0)
				continue;
			image.setPixelColor(x, y, color);
		}
	}

	pixmap.convertFromImage(image);
}
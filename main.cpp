#include "magicjewelry.h"
#include "common.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Common::splitImg(Common::jewelryImgPath, Common::jewelryPixmapVec, 1, 7);
    Common::splitImg(Common::digitImgPath, Common::digitPixmapVec, 1, 10);
    Common::splitImg(Common::letterImgPath, Common::letterPixmapVec, 1, 26);
    Common::splitImg(Common::starImgPath, Common::starPixmapVec, 1, 4);

    MagicJewelry w;
    w.show();
    return a.exec();
}

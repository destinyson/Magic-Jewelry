#include "magicjewelry.h"
#include "common.h"

MagicJewelry::MagicJewelry(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->resize(Common::width, Common::height);
    ui.centralWidget->resize(size());

    m_mgrWidget = new ManagerWidget(ui.centralWidget);
}

MagicJewelry::~MagicJewelry()
{}

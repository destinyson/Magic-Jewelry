#pragma once

#include "managerwidget.h"
#include "ui_magicjewelry.h"
#include <QtWidgets/QMainWindow>

class MagicJewelry : public QMainWindow
{
    Q_OBJECT

public:
    MagicJewelry(QWidget *parent = nullptr);
    ~MagicJewelry();

private:
    Ui::MagicJewelryClass ui;

    ManagerWidget* m_mgrWidget;
};

#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Eshop.h"

class Eshop : public QMainWindow
{
    Q_OBJECT

public:
    Eshop(QWidget *parent = nullptr);
    ~Eshop();

private:
    Ui::EshopClass ui;
};

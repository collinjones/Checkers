#include "playerNames.h"
#include "ui_playerNames.h"

PlayerNamesWindow::PlayerNamesWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayerNamesWindow)
{
    ui->setupUi(this);
}

PlayerNamesWindow::~PlayerNamesWindow()
{
    delete ui;
}

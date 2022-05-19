#include "PlayerNameForm.h"
#include "ui_PlayerNameForm.h"

PlayerNameForm::PlayerNameForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlayerNameForm)
{
    ui->setupUi(this);
}

PlayerNameForm::~PlayerNameForm()
{
    delete ui;
}

void PlayerNameForm::on_submitButton_clicked()
{
    player1Name = ui->player1Name->toPlainText();
    player2Name = ui->player2Name->toPlainText();
    close();
}


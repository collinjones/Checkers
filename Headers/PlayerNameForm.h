#ifndef PLAYERNAMEFORM_H
#define PLAYERNAMEFORM_H

#include <QDialog>

namespace Ui {
class PlayerNameForm;
}

class PlayerNameForm : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerNameForm(QWidget *parent = nullptr);
    ~PlayerNameForm();

    QString get_player1(){return player1Name;}
    QString get_player2(){return player2Name;}

private slots:
    void on_submitButton_clicked();

private:
    Ui::PlayerNameForm *ui;
    QString player1Name;
    QString player2Name;
};

#endif // PLAYERNAMEFORM_H

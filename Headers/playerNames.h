#ifndef PLAYERNAMES_H
#define PLAYERNAMES_H

#include <QDialog>

namespace Ui {
class PlayerNamesWindow;
}

class PlayerNamesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PlayerNamesWindow(QWidget *parent = nullptr);
    ~PlayerNamesWindow();

private:
    Ui::PlayerNamesWindow *ui;
};

#endif // PLAYERNAMES_H

#include "player.h"
#include "piece.h"
#include "mainwindow.h"

Player::Player(QString name, QColor color)
{
    score_ = 0;
    pieces_remaining_ = 12;
    name_ = name;
    color_ = color;
}

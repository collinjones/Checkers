#ifndef PLAYER_H
#define PLAYER_H

#include "qcolor.h"
#include <QString>

class Piece;

class Player
{
public:
    Player(){;}
    Player(QString name, QColor color);

    /* GETTERS */
    QString get_name(){return name_;}
    QColor get_color(){return color_;}
    int get_score(){return score_;}
    int get_pieces_remaining(){return pieces_remaining_;}
    std::vector<Piece*> get_pieces(){return pieces_;}
    int get_id(){return id_;}

    /* SETTERS */
    void set_id(int id){id_ = id;}
    void add_piece(Piece* p){pieces_.push_back(p);}
    void add_score(){score_++;}


private:
    QString name_;
    QColor color_;
    int score_;
    int pieces_remaining_;
    std::vector<Piece*> pieces_;
    int id_;

};

#endif // PLAYER_H

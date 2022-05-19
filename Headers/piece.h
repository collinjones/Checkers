#ifndef PIECE_H
#define PIECE_H

#include <QtWidgets>
#include "player.h"

class Tile;

class Piece: public QObject, public QGraphicsItem, public Player
{

    Q_OBJECT

public:
    Piece(){;}
    Piece(QColor color, int id, Player* owner, Tile* tile);

    void set_color(QColor color){color_ = color;}
    void set_pos(int x, int y){setPos(x,y);}
    void set_tile(Tile* tile){tile_ = tile;}
    void set_row(int r){row_ = r;}
    void set_col(int c){col_ = c;}
    QLabel* set_king();
    void set_crown_pos(int x, int y);

    QPoint get_crown_pos(){return img_label_->pos();}
    QLabel* get_img_label(){return img_label_;}
    int get_id(){return id_;}
    int get_x(){return x_;}
    int get_y(){return y_;}
    int get_row(){return row_;}
    int get_col(){return col_;}
    Player* get_owner(){return owner_;}
    Tile* get_tile(){return tile_;}
    QColor get_color(){return color_;}
    bool is_king(){return king_;}

    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
private:
    int id_;
    int x_;
    int y_;
    int row_;
    int col_;
    QColor color_;
    Player* owner_;
    Tile* tile_;
    bool king_;
    QPixmap crown_img_;
    QLabel* img_label_;

signals:
    void PieceSelected(Piece* p);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};



#endif // PIECE_H

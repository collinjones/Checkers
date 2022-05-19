#ifndef TILE_H
#define TILE_H

#include <QGraphicsItem>

class Piece;

class Tile : public QObject, public QGraphicsItem {

    Q_OBJECT

public:
    Tile(QColor color, int x, int y, int tile_id);

    /* GETTERS */
    QColor get_color(){return color_;}
    int get_id(){return id_;}
    int get_x(){return x_;}
    int get_y(){return y_;}
    Piece* get_piece(){return piece_;}
    int get_row(){return row_;}
    int get_col(){return col_;}

    /* SETTERS */
    void set_color(QColor color){color_ = color; update();}
    void set_id(int id){id_ = id;}
    void set_piece(Piece* p){piece_ = p;}
    void set_row(int r){row_ = r;}
    void set_col(int c){col_ = c;}
    void highlight();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

private:
    QColor color_;
    int id_;
    static const int width_ = 50;
    static const int height_ = 50;
    int row_;
    int col_;
    int x_;
    int y_;
    Piece* piece_;

signals:
    void TileSelected(Tile* t);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // TILE_H

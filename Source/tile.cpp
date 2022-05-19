#include <QtWidgets>

#include "piece.h"
#include "tile.h"

#define BRIGHTNESS_FACTOR 400

Tile::Tile(QColor color, int x, int y, int tile_id) {
    color_ = color;
    x_ = x;
    y_ = y;
    id_ = tile_id;
    piece_ = nullptr;
}

QRectF Tile::boundingRect() const {
    return QRectF(x_, y_, width_, height_);
}

QPainterPath Tile::shape() const {
    QPainterPath path;
    path.addRect(x_, y_, width_, height_);
    return path;
}

void Tile::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget){
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_));
    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->height_));
    painter->setBrush(b);
}

void Tile::mousePressEvent(QGraphicsSceneMouseEvent *event){
    Q_UNUSED(event);
    emit TileSelected(this);
}

void Tile::highlight(){
    /* Highlights tiles by setting color to be a brighter color than the tile's current color */
    set_color(color_.lighter(BRIGHTNESS_FACTOR));
}

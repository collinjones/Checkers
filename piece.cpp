#include <QtWidgets>
#include <QPixmap>
#include <QLabel>

#include "piece.h"
#include "player.h"

Piece::Piece(QColor color, int id, Player* owner, Tile* tile)
{
    x_ = 0;
    y_ = 0;
    color_ = color;
    id_ = id;
    owner_ = owner;
    tile_ = tile;
    king_ = false;
    img_label_ = nullptr;
}

QPainterPath Piece::shape() const {
    QPainterPath path;
    path.addEllipse(x_, y_, 40, 40);
    return path;
}

QRectF Piece::boundingRect() const {
    return QRectF(x_, y_, 40, 40);
}

void Piece::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget){
    Q_UNUSED(widget);

    QBrush b = painter->brush();
    QPen p;

    /* DETERMINES COLOR OF PIECES BASED ON OWNER */
    if(owner_->get_id() == 1){
        p = QPen(Qt::white, 1);
    }
    else{
        p = QPen(Qt::black, 1);
    }

    painter->setPen(p);
    painter->setBrush(QBrush(color_));
    painter->drawEllipse(this->x_, this->y_, 40, 40);
    painter->setBrush(b);
}

void Piece::mousePressEvent(QGraphicsSceneMouseEvent *event){
    Q_UNUSED(event);
    emit PieceSelected(this);
    update();
}

void Piece::set_crown_pos(int x, int y){
    const QRect pos(x+8,y+7, 25,25);
    img_label_->setGeometry(pos);
}

QLabel* Piece::set_king(){
    const QRect pos(x()+8,y()+7, 25,25);
    QPixmap pm("F:/Users/Collin/Documents/Image_Test/crown.png");
    img_label_ = new QLabel();
    img_label_->setGeometry(pos);

    img_label_->setPixmap(pm);
    img_label_->setScaledContents(true);

    king_ = true;

    return img_label_;
}

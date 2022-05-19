#ifndef UTILITIES_H
#define UTILITIES_H

#include "qcolor.h"
#include "qdebug.h"

QColor CHOCOLATE = QColor(117, 66, 32);
QColor BEIGE = QColor(196, 139, 100);
QColor BABY_BLUE = QColor(109, 194, 191);
QColor SOFT_PINK = QColor(204, 94, 162);
// QColor VERY_LIGHT_RED = QColor(252,204,204);

QColor get_tile_on_color(){
    /* Returns the color that pieces are supposed to be on */
    return CHOCOLATE;
}

QColor get_tile_off_color(){
    /* Returns the color that pieces are not supposed to be on */
    return BEIGE;
}

QColor get_piece_color1(){
    /* Returns the color of player 1 pieces */
    return BABY_BLUE;
}

QColor get_piece_color2(){
    /* Returns the color of player 2 pieces */
    return SOFT_PINK;
}

bool is_even(int num){
    return (num % 2);
}


QColor get_checkerboard_color(int x, int y){
    /* USED INSIDE OF NESTED FOR LOOP */
    /* Returns color based on x and y input. */
    if(is_even(y)){
        if(is_even(x)){
            return get_tile_on_color();
        }
        else{
            return get_tile_off_color();
        }
    }
    else{
        if(is_even(x)){
            return get_tile_off_color();
        }
        else{
            return get_tile_on_color();
        }
    }
}



#endif // UTILITIES_H

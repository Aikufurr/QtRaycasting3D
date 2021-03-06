#include "wall.h"

Wall::Wall(int x, int y, int xz, int xy) {
    pos.x = x;
    pos.y = y;
    dir.x = xz;
    dir.y = xy;
}

void Wall::paint(QPainter *painter) {
    // Set the pen to white
    painter->setPen(QPen(Qt::white));
    // Draw the line from x,y,w,h
    painter->drawLine(pos.x, pos.y, dir.x, dir.y);
}

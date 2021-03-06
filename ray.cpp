#include "ray.h"
#include <QtMath>

Ray::Ray(QVector2D *pos, double angle) {
    this->pos = pos;
    this->dir.setX(qCos(angle));
    this->dir.setY(qSin(angle));
}

QPoint Ray::cast(Wall *wall) {
    // https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection#Given_two_points_on_each_line_segment
    const double x1 = wall->pos.x;
    const double y1 = wall->pos.y;
    const double x2 = wall->dir.x;
    const double y2 = wall->dir.y;


    const double x3 = this->pos->x();
    const double y3 = this->pos->y();
    const double x4 = this->pos->x() + this->dir.x();
    const double y4 = this->pos->y() + this->dir.y();

    const double den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (den == 0) {
        return QPoint(0,0);
    }

    const double t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
    const double u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;

    if (0 < t && t < 1 && 0 < u) {
        // The point of intersect
        QPoint point;
        point.setX(x1 + t*(x2 - x1));
        point.setY(y1 + t*(y2 - y1));
        return point;
    } else {
        return QPoint(0,0);
    }
}

#ifndef RAY_H
#define RAY_H

#include "wall.h"
#include <QDebug>
#include <QFont>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include <QVector2D>
#include <QWidget>
#include <QtMath>

class Ray : public QObject {
    Q_OBJECT

public:
    Ray();
    Ray(QVector2D *pos, double angle);
    QPoint cast(Wall *wall);

    QVector2D *pos;
    QVector2D dir;

};

#endif // RAY_H

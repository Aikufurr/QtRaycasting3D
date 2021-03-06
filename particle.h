#ifndef PARTICLE_H
#define PARTICLE_H

#include "ray.h"
#include "wall.h"
#include <QList>
#include <QObject>
#include <QPainter>
#include <QPoint>
#include <QVector2D>
#include <QtMath>
#include <limits>

class Particle : public QObject {
    Q_OBJECT

public:
    Particle(int width, int height);
    void paint(QPainter *painter);
    QList<double> intersect(QPainter *painter, QList<Wall *> walls);
    void rotate(double angle);
    void move(int amount, QList<Wall *> walls);
    QVector2D pos;
    QList<Ray *> rays;
    double heading = 0;
    int fov = 40;

private:
    int width;
    int height;
    double resolution = 0.5; // lower is more quality

    double closestRay;
};

#endif // PARTICLE_H

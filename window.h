#ifndef WINDOW_H
#define WINDOW_H

#include "particle.h"
#include "wall.h"
#include <QDebug>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QList>
#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QPoint>
#include <QTimer>
#include <QTimer>
#include <QWidget>

//! [0]
class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    virtual void paintEvent(QPaintEvent *event);

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    Particle *particle;
    QList<Wall *> walls;
    int width   = 400;
    int height  = 400;
    double moveSpeed = 1.5;
    double turnSpeed = 0.025;

    QString keys = "----";

    QTimer *Key_Up      = new QTimer(this);
    QTimer *Key_Down    = new QTimer(this);
    QTimer *Key_Left    = new QTimer(this);
    QTimer *Key_Right   = new QTimer(this);
    double map(double x, double in_min, double in_max, double out_min, double out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    double clamp(double n, double lower, double upper) {
        return std::max(lower, std::min(n, upper));
    }
};
//! [0]

#endif

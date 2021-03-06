#ifndef WALL_H
#define WALL_H

#include <QFont>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QWidget>

class Wall: public QObject {
    Q_OBJECT

public:
    Wall();
    Wall(int x, int y, int xz, int xy);
    void paint(QPainter *painter);

    struct pos_t {
        int x = 0;
        int y = 0;
    } pos;
    struct dir_t {
        int x = 0;
        int y = 0;
    } dir;
};

#endif // WALL_H

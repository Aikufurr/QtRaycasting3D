#include "window.h"

Window::Window() {
    this->setWindowTitle(tr(QString("Pseudo 3D Raycaster %1").arg(keys).toUtf8()));
    this->setFixedSize(width*2, height);

    // Create the particle
    particle = new Particle(width, height);

    // Create the walls
    // Edges of the scene
    walls << (new Wall(0, 0, width, 0))                     // Top
          << (new Wall(width, 0, width, height))            // Right
          << (new Wall(width, height, 0, height))           // Bottom
          << (new Wall(0, height, 0, 0));                   // Left
    // Corners of the scene
    walls << (new Wall(0, 10, 10, 0))                       // Top Left
          << (new Wall(width-10, 0, width, 10))             // Top Right
          << (new Wall(0, height-10, 10, height))           // Bottom Left
          << (new Wall(width-10, height, width, height-10));// Bottom Right
    // Walls in the middle of the scene
    walls << (new Wall(300, 100, 300, 300))                 // Wall on the middle right
          << (new Wall(25, 200, 150, 300))                  // Wall on the lower left
          << (new Wall(150, 75, 200, 75));                  // Wall on the middle top

    // This allows for continuous movement when a key is held down
    QObject::connect(this->Key_Up, &QTimer::timeout, this, [=](){
        particle->move(this->moveSpeed, walls);
        this->update();
    });
    QObject::connect(this->Key_Down, &QTimer::timeout, this, [=](){
        particle->move(-this->moveSpeed, walls);
        this->update();
    });
    QObject::connect(this->Key_Left, &QTimer::timeout, this, [=](){
        particle->rotate(-this->turnSpeed);
        this->update();
    });
    QObject::connect(this->Key_Right, &QTimer::timeout, this, [=](){
        particle->rotate(this->turnSpeed);
        this->update();
    });
}

void Window::paintEvent(QPaintEvent *event) {
    // Create the painter
    QPainter painter(this);
    // Fill the background with black
    painter.fillRect(event->rect(), QBrush(QColor(0, 0, 0)));
    // Enable antialiasing
    painter.setRenderHint(QPainter::Antialiasing);
    // Set the pen to white
    painter.setPen(QPen(QColor(Qt::white)));
    // Draw the divider line
    painter.drawLine(width, 0, width, height);
    // Paint the particle
    particle->paint(&painter);

    // For each wall
    foreach(Wall *wall, walls) {
        // Pain the wall
        wall->paint(&painter);
    }

    // Get the list of the minimum distances
    // [function] Draw the ray line from the particle pos to the walls that intersect
    QList<double> scene = particle->intersect(&painter, walls);

    // Save the state of the painter's translation
    painter.save();
    // Translate the painter to the right side
    painter.translate(width, 0);

    // Draw the sky and floor
    painter.fillRect(0, 0, width, height / 2, QBrush(QColor(0, 200, 200)));         // Sky
    painter.fillRect(0, height/2, width, height / 2, QBrush(QColor(50, 50, 50)));   // Floor

    const double w = width / scene.size();
    // For each ray
    for (int i = 0; i < scene.size(); i++) {
        // Get the distance of the ray and store it
        const double rayDist = scene.at(i);
        // Conver the distance of the ray to greyscale
        const double c = clamp(map(rayDist * rayDist, 0, width*width, 255, 0), 0, 255);
        // Convert the distance to the hight of the "wall"
        const double h = (width / rayDist) * ((width/2) / qTan(particle->fov/2));
        const double xPos = i * w;
        const double yPos = (height/2) - (h/2);
        // Draw the rectange and fill it with the grayscale colour
        painter.fillRect(xPos, yPos, w+1, h, QBrush(QColor(c, c, c)));
    }
    // Restore the painter back to it's original translation
    painter.restore();
    painter.end();
}

void Window::keyPressEvent(QKeyEvent *ev) {
    // On arrowkey down, replace the keys string with the correct key
    // ---- = ←↑↓→
    // Start the responding timer to repeat the keypress for continuous movement/turning
    if (ev->key() == Qt::Key_Up) {
        keys.replace(1, 1, QString("↑"));
        this->Key_Up->start(10);
    }
    if (ev->key() == Qt::Key_Down) {
        keys.replace(2, 1, QString("↓"));
        this->Key_Down->start(10);
    }
    if (ev->key() == Qt::Key_Left) {
        keys.replace(0, 1, QString("←"));
        this->Key_Left->start(10);
    }
    if (ev->key() == Qt::Key_Right) {
        keys.replace(3, 1, QString("→"));
        this->Key_Right->start(10);
    }
    // Update the title to have the keypress
    this->setWindowTitle(tr(QString("Pseudo 3D Raycaster %1").arg(keys).toUtf8()));
}

void Window::keyReleaseEvent(QKeyEvent *ev) {
    // On arrowkey up, replace the keys string with the placeholder
    // Stop the responding timer to stop the movement/turning
    if (ev->key() == Qt::Key_Up) {
        keys.replace(1, 1, QString("-"));
        this->Key_Up->stop();
    }
    if (ev->key() == Qt::Key_Down) {
        keys.replace(2, 1, QString("-"));
        this->Key_Down->stop();
    }
    if (ev->key() == Qt::Key_Left) {
        keys.replace(0, 1, QString("-"));
        this->Key_Left->stop();
    }
    if (ev->key() == Qt::Key_Right) {
        keys.replace(3, 1, QString("-"));
        this->Key_Right->stop();
    }
    // Update the title to have the keypress
    this->setWindowTitle(tr(QString("Pseudo 3D Raycaster %1").arg(keys).toUtf8()));
}

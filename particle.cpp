#include "particle.h"

double map(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
double clamp(double n, double lower, double upper) {
    return std::max(lower, std::min(n, upper));
}

Particle::Particle(int width, int height) {
    this->width = width;
    this->height = height;
    this->pos.setX(width/2);
    this->pos.setY(height/2);

    // Create the rays within the given fov, 0 being the centre
    int index = 0;
    for (double angle = -this->fov/2; angle < this->fov/2; angle += this->resolution) {
        this->rays.insert(index++, new Ray(&this->pos, qDegreesToRadians((double) angle)));
    }
}

void Particle::rotate(double angle) {
    heading += angle;
    // Replace the rays with new ones at a new angle
    int index = 0;
    for (double angle = -this->fov/2; angle < this->fov/2; angle += this->resolution) {
        this->rays.replace(index++, new Ray(&this->pos, qDegreesToRadians((double) angle) + heading));
    }
}

void Particle::move(int amount, QList<Wall *> walls) {
    // Create a new velocity vector
    QVector2D velocity(qCos(this->heading), qSin(this->heading));
    const double currentMagnitude = qSqrt((velocity.x() * velocity.x()) + (velocity.y() * velocity.y()));
    velocity.setX(velocity.x() * amount / currentMagnitude);
    velocity.setY(velocity.y() * amount / currentMagnitude);

    // If going backwards
    if (amount < 0) {
        // Create a ray behind the particle
        Ray *ray = new Ray(&this->pos, qDegreesToRadians((double) 180) + heading);
        // Record for lowest distance
        float record = std::numeric_limits<float>::max();
        // For each wall in the scene
        foreach(Wall *wall, walls) {
            // Cast the ray to the wall
            const QPoint point = ray->cast(wall);
            // If it intersects
            if (point != QPoint(0,0)) {
                // Get the distance from the particle pos to the interect
                float distance = this->pos.distanceToPoint(QVector2D(point));
                // Get the angle from the heading to the ray
                double a = this->heading - qAtan2(ray->dir.y(), ray->dir.x());
                if (a < 0) {
                    a += 2*M_PI;
                }
                if (a > 2*M_PI) {
                    a -= 2*M_PI;
                }

                // Multiply the cosine of the new angle to decrease the fish-eye effect
                distance *= qCos(a);
                // If the distance is less than the recordes lowest distance
                if (distance < record) {
                    // Set the record to the minimum of the ray distance
                    record = -distance;
                }
            }
        }
        // If the distance is greater than 10 (diameter of the particle) allow movement
        // If the statement is false movement is blocked
        if (record > 10) {
            this->pos = this->pos + velocity;
        }
        // Delete the ray for memory
        delete ray;
    } else if (closestRay > 10) {
        // If the distance forwards is greater than 10 (diameter of the particle) allow movement
        // If the statement is false movement is blocked
        this->pos = this->pos + velocity;
    }
}

void Particle::paint(QPainter *painter) {
    // Set the pen to white
    painter->setPen(QPen(Qt::white));
    // Draw the ellipse representing the particle
    // -8 to offset to draw in the centre
    painter->drawEllipse(this->pos.x()-8, this->pos.y()-8, 16, 16);
}

QList<double> Particle::intersect(QPainter *painter, QList<Wall *> walls) {
    // List of the ray distances
    QList<double> scene;
    // For each ray
    for (int i = 0; i < this->rays.size(); i++) {
        // Get the ray at index
        Ray *ray = this->rays.at(i);
        // Placehold point(x,y) of intersect
        QPoint intersect_point;
        // Record for the lowest distance
        float record = std::numeric_limits<float>::max();
        // For each wall
        // For each wall in the scene
        foreach(Wall *wall, walls) {
            // Cast the ray to the wall
            const QPoint point = ray->cast(wall);
            // If it intersects
            if (point != QPoint(0,0)) {
                // Get the distance from the particle pos to the intersect
                float distance = this->pos.distanceToPoint(QVector2D(point));
                // Get the angle from the heading to the ray
                double a = this->heading - qAtan2(ray->dir.y(), ray->dir.x());
                if (a < 0) {
                    a += 2*M_PI;
                }
                if (a > 2*M_PI) {
                    a -= 2*M_PI;
                }

                // Multiply the cosine of the new angle to decrease the fish-eye effect
                distance *= qCos(a);
                // If the distance is less than the recordes lowest distance
                if (distance < record) {
                    // Set the record to the minimum of the ray distance
                    record = distance;
                    // Set the closest to the point of intersect
                    intersect_point = point;
                }
            }
        }
        // If the point of intersect is not 0,0
        if (intersect_point != QPoint(0,0)) {
            // Draw the ray line from the particle pos to the point of intersect
            painter->drawLine(this->pos.x(), this->pos.y(), intersect_point.x(), intersect_point.y());
        }
        // Insert lowest distance to the scene
        scene.insert(i, (double) record);
        // Keep track of the closest ray for movement
        closestRay = *std::min_element(scene.begin(), scene.end());
    }
    // Return the list of the ray distances
    return scene;
}

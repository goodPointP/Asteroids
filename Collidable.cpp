#include "Collidable.hpp"

Collidable::~Collidable() {
}

float Collidable::getRadius() const {
    return radiuss;
}

void Collidable::setRadius(float radiuss) {
    Collidable::radiuss = radiuss;
}

// satellite.h
#ifndef SATELLITE_H
#define SATELLITE_H

#include "celestialbody.h"

class Satellite : public CelestialBody
{
public:
    Satellite(float radius, float distanceFromPlanet, const QColor& color, float rotationSpeed);
    void draw() override; // Переопределяем функцию draw()
    float getDistanceFromPlanet() const;
    float getRotationSpeed() const;
private:
    float distanceFromPlanet;
    float rotationSpeed;
};

#endif // SATELLITE_H

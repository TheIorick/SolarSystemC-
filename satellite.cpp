// satellite.cpp
#include "satellite.h"

Satellite::Satellite(float radius, float distanceFromPlanet, const QColor& color, float rotationSpeed)
    : CelestialBody(radius, distanceFromPlanet, color), distanceFromPlanet(distanceFromPlanet), rotationSpeed(rotationSpeed)
{
}

void Satellite::draw()
{
    // Рисование будет в GLWidget.
}

float Satellite::getDistanceFromPlanet() const
{
    return distanceFromPlanet;
}

float Satellite::getRotationSpeed() const
{
    return rotationSpeed;
}

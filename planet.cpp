// planet.cpp
#include "planet.h"

Planet::Planet(float radius, float distanceFromSun, const QColor& color)
    : CelestialBody(radius, distanceFromSun, color)
{
}

void Planet::draw()
{
    // Рисование будет в GLWidget.
}

void Planet::addSatellite(Satellite *satellite)
{
    satellites.append(satellite);
}

const QVector<Satellite*>& Planet::getSatellites() const
{
    return satellites;
}

// planet.h
#ifndef PLANET_H
#define PLANET_H

#include "celestialbody.h"
#include <QVector>
#include "satellite.h"

class Planet : public CelestialBody
{
public:
    Planet(float radius, float distanceFromSun, const QColor& color);
    void draw() override; // Переопределяем функцию draw()
    void addSatellite(Satellite *satellite);
    const QVector<Satellite*>& getSatellites() const;


private:
    QVector<Satellite*> satellites;
};

#endif // PLANET_H

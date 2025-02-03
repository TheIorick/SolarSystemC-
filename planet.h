// planet.h
#ifndef PLANET_H
#define PLANET_H

#include "celestialbody.h"
#include <QVector>
#include "satellite.h"
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

class Planet : public CelestialBody
{
public:
    Planet(float radius, float distanceFromSun, const QColor& color);
    void draw(QOpenGLShaderProgram *shaderProgram, const QMatrix4x4& modelViewMatrix) override;
    void addSatellite(Satellite *satellite);
    const QVector<Satellite*>& getSatellites() const;
    float getRotationSpeed() const;
private:
    QVector<Satellite*> satellites;
    float rotationSpeed;
    static constexpr float daysInYear = 365.25f;
    static constexpr float secondsInDay = 86400.0f;
    static constexpr float planetCoefSpeed = 300000.0f;
};

#endif // PLANET_H

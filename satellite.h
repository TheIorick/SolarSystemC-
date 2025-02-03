// satellite.h
#ifndef SATELLITE_H
#define SATELLITE_H

#include "celestialbody.h"
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

class Satellite : public CelestialBody
{
public:
    Satellite(float radius, float distanceFromPlanet, const QColor& color, float rotationSpeed);
    void draw(QOpenGLShaderProgram *shaderProgram, const QMatrix4x4& modelViewMatrix) override;
    float getDistanceFromPlanet() const;
    float getRotationSpeed() const;
private:
    float distanceFromPlanet;
    float rotationSpeed;
};

#endif // SATELLITE_H

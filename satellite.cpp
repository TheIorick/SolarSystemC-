// satellite.cpp
#include "satellite.h"
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

Satellite::Satellite(float radius, float distanceFromPlanet, const QColor& color, float rotationSpeed)
    : CelestialBody(radius, distanceFromPlanet, color), distanceFromPlanet(distanceFromPlanet), rotationSpeed(rotationSpeed)
{
}

void Satellite::draw(QOpenGLShaderProgram *shaderProgram, const QMatrix4x4& modelViewMatrix)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    QColor color = getColor();
    QVector3D satColor(color.redF(), color.greenF(), color.blueF());
    shaderProgram->setUniformValue("color", satColor);

    QMatrix4x4 satelliteModelViewMatrix = modelViewMatrix;
    satelliteModelViewMatrix.rotate(0, QVector3D(0.0f, 1.0f, 0.0f)); //Вращение спутника вместе с планетой
    satelliteModelViewMatrix.rotate(0, QVector3D(0.0f, 1.0f, 0.0f)); //Вращение спутника вокруг планеты
    satelliteModelViewMatrix.translate(getDistanceFromPlanet(), 0.0f, 0.0f);
    satelliteModelViewMatrix.scale(getRadius());
    shaderProgram->setUniformValue("modelViewMatrix", satelliteModelViewMatrix);
    f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 37 * 37 * 2);
}

float Satellite::getDistanceFromPlanet() const
{
    return distanceFromPlanet;
}

float Satellite::getRotationSpeed() const
{
    return rotationSpeed;
}

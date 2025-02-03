// planet.cpp
#include "planet.h"
#include <cmath>
#include <QOpenGLShaderProgram>
#include "satellite.h"
#include <QOpenGLFunctions>
#include <QMatrix4x4>

Planet::Planet(float radius, float distanceFromSun, const QColor& color)
    : CelestialBody(radius, distanceFromSun, color)
{
    // Расчет скорости вращения планеты вокруг солнца,
    // этот код зависит от того, какую планету создали
    float orbitalPeriod;

    if (radius == 0.4f)
    {
        orbitalPeriod = 0.24f; // Меркурий
    } else if (radius == 0.6f)
    {
        orbitalPeriod = 0.62f; // Венера
    } else if(radius == 0.7f)
    {
        orbitalPeriod = 1.00f; // Земля
    } else if (radius == 0.5f)
    {
        orbitalPeriod = 1.88f; // Марс
    } else if (radius == 1.2f)
    {
        orbitalPeriod = 11.86f; // Юпитер
    } else if (radius == 1.0f)
    {
        orbitalPeriod = 29.46f; // Сатурн
    } else if (radius == 0.9f)
    {
        orbitalPeriod = 84.01f; // Уран
    } else
    {
        orbitalPeriod = 164.8f; // Нептун
    }
    rotationSpeed = 360.0f / (orbitalPeriod * daysInYear * secondsInDay) * planetCoefSpeed;
}

void Planet::draw(QOpenGLShaderProgram *shaderProgram, const QMatrix4x4& modelViewMatrix)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    QColor color = getColor();
    QVector3D planetColor(color.redF(), color.greenF(), color.blueF());
    shaderProgram->setUniformValue("color", planetColor);


    QMatrix4x4 planetModelViewMatrix = modelViewMatrix;
    // Вращаем планету вокруг Солнца
    planetModelViewMatrix.rotate(0, QVector3D(0.0f, 1.0f, 0.0f));
    planetModelViewMatrix.translate(getDistanceFromSun(), 0.0f, 0.0f);
    planetModelViewMatrix.scale(getRadius());
    shaderProgram->setUniformValue("modelViewMatrix", planetModelViewMatrix);
    f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 37 * 37 * 2);

    // Отрисовка спутников
    const QVector<Satellite*> &satellites = getSatellites();
    for (int j = 0; j < satellites.size(); ++j) {
        Satellite *satellite = satellites[j];
        satellite->draw(shaderProgram, planetModelViewMatrix);
    }
}

void Planet::addSatellite(Satellite *satellite)
{
    satellites.append(satellite);
}

const QVector<Satellite*>& Planet::getSatellites() const
{
    return satellites;
}

float Planet::getRotationSpeed() const
{
    return rotationSpeed;
}

#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <QVector3D>
#include <QColor>

class CelestialBody
{
public:
    CelestialBody(float radius, float distanceFromSun, const QColor& color);
    virtual ~CelestialBody() = default;

    virtual void draw() = 0; // Чисто виртуальная функция, должна быть реализована в дочерних классах

    float getRadius() const { return radius; }
    float getDistanceFromSun() const { return distanceFromSun; }
    QColor getColor() const { return color; }

protected:
    float radius;            // Радиус тела
    float distanceFromSun;   // Расстояние от Солнца
    QColor color;             // Цвет тела
};

#endif // CELESTIALBODY_H

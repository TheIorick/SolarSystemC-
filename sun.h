#ifndef SUN_H
#define SUN_H

#include "celestialbody.h"

class Sun : public CelestialBody
{
public:
    Sun(float radius, float distanceFromSun, const QColor& color);
    void draw() override; // Переопределяем функцию draw()

private:
         // Здесь могут быть дополнительные свойства, специфичные для солнца
};

#endif // SUN_H

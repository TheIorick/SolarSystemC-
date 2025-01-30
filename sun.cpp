#include "sun.h"
#include <QOpenGLFunctions>

Sun::Sun(float radius, float distanceFromSun, const QColor& color)
    : CelestialBody(radius, distanceFromSun, color)
{
}

void Sun::draw()
{

}

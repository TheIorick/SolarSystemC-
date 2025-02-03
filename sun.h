#ifndef SUN_H
#define SUN_H

#include "celestialbody.h"
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
class Sun : public CelestialBody
{
public:
    Sun(float radius, float distanceFromSun, const QColor& color);
    void draw(QOpenGLShaderProgram *shaderProgram, const QMatrix4x4& modelViewMatrix) override;

private:
         // Здесь могут быть дополнительные свойства, специфичные для солнца
};

#endif // SUN_H

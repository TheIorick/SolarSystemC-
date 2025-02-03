#include "sun.h"
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

Sun::Sun(float radius, float distanceFromSun, const QColor& color)
    : CelestialBody(radius, distanceFromSun, color)
{
}

void Sun::draw(QOpenGLShaderProgram *shaderProgram, const QMatrix4x4& modelViewMatrix)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    QColor color = getColor();
    QVector3D planetColor(color.redF(), color.greenF(), color.blueF());
    shaderProgram->setUniformValue("color", planetColor);

    QMatrix4x4 planetModelViewMatrix = modelViewMatrix;
    planetModelViewMatrix.translate(getDistanceFromSun(), 0.0f, 0.0f);
    planetModelViewMatrix.scale(getRadius());
    shaderProgram->setUniformValue("modelViewMatrix", planetModelViewMatrix);
    f->glDrawArrays(GL_TRIANGLE_STRIP, 0, 37 * 37 * 2);
}

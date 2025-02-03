// glwidget.h
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <QVector>
#include <QColor>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>

#include "celestialbody.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT;

public:
    void setRotationSpeed(int speed);
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    void createSphere(float radius, int segments);
    void setupVertexAttribs();
    void updatePlanetPositions();
    void drawRings(float innerRadius, float outerRadius, float y_position);
    void calculateShadows();

    QVector<QVector3D> vertices;
    QVector<QVector3D> normals; // Нормали
    float xRot = 0;
    float yRot = 0;
    float zoom = -15.0f; // Начальное значение масштабирования
    QPoint lastMousePos;

    QVector3D cameraPos = QVector3D(0.0f, 0.0f, 0.0f); // Позиция камеры
    QVector3D cameraMoveSpeed = QVector3D(0.1f, 0.1f, 0.1f);
    bool isCameraMoving = false; // Флаг, если камера двигается

    QVector<CelestialBody*> celestialBodies;

    QOpenGLShaderProgram *shaderProgram;
    GLuint VAO, VBO, NBO; // NBO для нормалей
    QMatrix4x4 projectionMatrix;

    QVector<float> planetAngles; // Углы для вращения планет
    float rotationSpeed = 1.0f; // Скорость вращения
    QVector<float> planetRotationSpeeds; // Скорости вращения планет
    QVector<QVector<float>> satelliteAngles; // Углы для вращения спутников
    QTimer *timer;
    QVector<bool> shadows; // Вектор для хранения информации о тенях
};

#endif // GLWIDGET_H

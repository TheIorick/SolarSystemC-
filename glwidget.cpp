// glwidget.cpp
#include "glwidget.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtMath>
#include "planet.h"
#include "sun.h"
#include "satellite.h"

// Конструктор GLWidget
GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent), shaderProgram(nullptr), VAO(0), VBO(0), NBO(0)
{
    // Создаем таймер для анимации
    timer = new QTimer(this);
    // Подключаем сигнал timeout таймера к слоту updatePlanetPositions для обновления позиций планет
    connect(timer, &QTimer::timeout, this, &GLWidget::updatePlanetPositions);
    timer->start(20); // Запускаем таймер с интервалом 20 мс (50 FPS)
}

// Деструктор GLWidget
GLWidget::~GLWidget()
{
    makeCurrent(); // Делаем контекст OpenGL текущим для этого виджета
    // Освобождаем ресурсы OpenGL
    glDeleteVertexArrays(1, &VAO); // Удаляем VAO
    glDeleteBuffers(1, &VBO);     // Удаляем VBO
    glDeleteBuffers(1, &NBO);     // Удаляем NBO
    delete shaderProgram;         // Удаляем шейдерную программу
    doneCurrent(); // Освобождаем контекст OpenGL

    // Удаляем все объекты небесных тел
    for (CelestialBody *body : celestialBodies) {
        delete body;
    }
    celestialBodies.clear(); // Очищаем вектор
}

// Инициализация OpenGL
void GLWidget::initializeGL()
{
    initializeOpenGLFunctions(); // Инициализация функций OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Устанавливаем цвет фона (черный)
    glEnable(GL_DEPTH_TEST); // Включаем тест глубины для правильной отрисовки объектов
    glLineWidth(2.0f); // Устанавливаем ширину линий

    createSphere(1.0f, 36); // Создаем геометрию сферы (радиус 1, 36 сегментов)

    // Создание и настройка шейдерной программы
    shaderProgram = new QOpenGLShaderProgram(this);

    // Загрузка и компиляция вершинного шейдера
    QFile vertexShaderFile(":/vertexshader.vsh");
    if (!vertexShaderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл вершинного шейдера";
        close();
        return;
    }
    QTextStream vertexShaderStream(&vertexShaderFile);
    QString vertexShaderSource = vertexShaderStream.readAll();
    QOpenGLShader *vertexShader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    if (!vertexShader->compileSourceCode(vertexShaderSource)) {
        qDebug() << "Ошибка компиляции вершинного шейдера:" << vertexShader->log();
        close();
        delete vertexShader;
        return;
    }
    shaderProgram->addShader(vertexShader);
    // Загрузка и компиляция фрагментного шейдера
    QFile fragmentShaderFile(":/fragmentshader.fsh");
    if (!fragmentShaderFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Не удалось открыть файл фрагментного шейдера";
        close();
        return;
    }
    QTextStream fragmentShaderStream(&fragmentShaderFile);
    QString fragmentShaderSource = fragmentShaderStream.readAll();
    QOpenGLShader *fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    if (!fragmentShader->compileSourceCode(fragmentShaderSource)) {
        qDebug() << "Ошибка компиляции фрагментного шейдера:" << fragmentShader->log();
        close();
        delete fragmentShader;
        return;
    }
    shaderProgram->addShader(fragmentShader);

    // Линковка шейдерной программы
    if (!shaderProgram->link()) {
        qDebug() << "Ошибка линковки шейдерной программы:" << shaderProgram->log();
        close();
        return;
    }
    delete vertexShader;
    delete fragmentShader;

    setupVertexAttribs(); // Настройка атрибутов вершин

    // Создание планет и солнца
    float scaleFactor = 1.5f; // Коэффициент масштабирования расстояний
    Sun *sun = new Sun(1.5f, 0.0f * scaleFactor, QColor(255, 255, 0));
    Planet *mercury = new Planet(0.4f, 2.2f + 0.39f * scaleFactor, QColor(128, 128, 128));
    Planet *venus = new Planet(0.6f, 3.2f + 0.72f * scaleFactor, QColor(255, 165, 0));
    Planet *earth = new Planet(0.7f, 4.6f +  1.00f * scaleFactor, QColor(100, 149, 237));
    Planet *mars = new Planet(0.5f, 6.2f +  1.52f * scaleFactor, QColor(255, 0, 0));
    Planet *jupiter = new Planet(1.2f,6.2f +   5.20f * scaleFactor, QColor(255, 140, 0));
    Planet *saturn = new Planet(1.0f,6.2f +   9.54f * scaleFactor, QColor(255, 228, 196));
    Planet *uranus = new Planet(0.9f,6.2f +   19.20f * scaleFactor, QColor(0, 255, 255));
    Planet *neptune = new Planet(0.8f,6.2f +  30.06f * scaleFactor, QColor(0, 0, 128));
    // Создание спутников
    Satellite* moon = new Satellite(0.2f, 1.3f, QColor(200,200,200), 1000.0f);
    Satellite* deimos = new Satellite(0.1f, 1.4f, QColor(180,180,180), 1200.0f);
    Satellite* phobos = new Satellite(0.15f, 1.6f, QColor(150,150,150), 1500.0f);
    earth->addSatellite(moon);
    mars->addSatellite(deimos);
    mars->addSatellite(phobos);
    celestialBodies.append(sun);
    celestialBodies.append(mercury);
    celestialBodies.append(venus);
    celestialBodies.append(earth);
    celestialBodies.append(mars);
    celestialBodies.append(jupiter);
    celestialBodies.append(saturn);
    celestialBodies.append(uranus);
    celestialBodies.append(neptune);

    // Инициализируем углы вращения для каждой планеты
    planetAngles.resize(celestialBodies.size());
    for (int i = 0; i < planetAngles.size(); ++i) {
        planetAngles[i] = 0.0f;
    }
    // Инициализируем скорости вращения для каждой планеты
    planetRotationSpeeds.resize(celestialBodies.size());
    for (int i = 0; i < celestialBodies.size(); i++) {
        if(dynamic_cast<Planet*>(celestialBodies[i]))
        {
            planetRotationSpeeds[i] = dynamic_cast<Planet*>(celestialBodies[i])->getRotationSpeed();
        } else
        {
            planetRotationSpeeds[i] = 0.0f;
        }

    }

    // Инициализируем углы для спутников
    satelliteAngles.resize(celestialBodies.size());
    for(int i = 0; i < celestialBodies.size(); ++i)
    {
        if(dynamic_cast<Planet*>(celestialBodies[i]))
        {
            Planet* planet = dynamic_cast<Planet*>(celestialBodies[i]);
            const QVector<Satellite*> &satellites = planet->getSatellites();
            satelliteAngles[i].resize(satellites.size());
            for (int j = 0; j < satellites.size(); j++)
            {
                satelliteAngles[i][j] = 0.0f;
            }
        }
    }

    shadows.resize(celestialBodies.size(), false);
}

// Изменение размеров окна
void GLWidget::resizeGL(int w, int h)
{
    // Настройка матрицы проекции
    QMatrix4x4 projectionMatrix;
    projectionMatrix.setToIdentity(); // Устанавливаем в единичную матрицу
    float aspect = float(w) / float(h ? h : 1); // Вычисляем соотношение сторон
    projectionMatrix.perspective(45.0f, aspect, 0.1f, 100.0f); // Настраиваем перспективную проекцию
    this->projectionMatrix = projectionMatrix; // Сохраняем матрицу проекции
}

// Отрисовка сцены
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderProgram->bind();
    QMatrix4x4 modelViewMatrix;
    modelViewMatrix.setToIdentity();
    modelViewMatrix.translate(cameraPos.x(), cameraPos.y(), zoom);
    modelViewMatrix.rotate(xRot, QVector3D(1.0f, 0.0f, 0.0f));
    modelViewMatrix.rotate(yRot, QVector3D(0.0f, 1.0f, 0.0f));
    shaderProgram->setUniformValue("projectionMatrix", projectionMatrix);
    shaderProgram->setUniformValue("modelViewMatrix", modelViewMatrix);

    glBindVertexArray(VAO);
        // Получаем позицию солнца в мировых координатах
    QVector3D sunPosition = QVector3D(0.0f, 0.0f, 0.0f);
    QMatrix4x4 sunModelMatrix;
    sunModelMatrix.setToIdentity(); // Матрица модели солнца
    sunPosition = sunModelMatrix.map(sunPosition);
    shaderProgram->setUniformValue("lightPosition", sunPosition);
    shaderProgram->setUniformValue("viewPosition", cameraPos);

    // Отрисовка небесных тел
    for (int i = 0; i < celestialBodies.size(); ++i) {
        CelestialBody *body = celestialBodies[i];

        QMatrix4x4 planetModelViewMatrix = modelViewMatrix;

        // Вращаем планету вокруг Солнца
        if (i > 0) { // Не вращаем Солнце
            planetModelViewMatrix.rotate(planetAngles[i], QVector3D(0.0f, 1.0f, 0.0f));
        }

        // Рисуем кольца Сатурна
        if (i == 6) { // Сатурн имеет индекс 6
            QMatrix4x4 ringModelViewMatrix = modelViewMatrix;
            if (i > 0) {
                ringModelViewMatrix.rotate(planetAngles[i], QVector3D(0.0f, 1.0f, 0.0f));
            }
            ringModelViewMatrix.rotate(-90.0f, QVector3D(1.0f, 0.0f, 0.0f));
            ringModelViewMatrix.translate(body->getDistanceFromSun(),0.0f,0.0f);
            shaderProgram->setUniformValue("modelViewMatrix", ringModelViewMatrix);
            shaderProgram->setUniformValue("color", QVector3D(0.9f, 0.6f, 0.5f));
            drawRings(1.2f, 1.75f, 0.0f);
            drawRings(1.8f, 2.35f, 0.0f);
            drawRings(2.4f, 3.0f, 0.0f);
        }

        // Настраиваем источник света и тени
        QVector3D lightDir;
        float ambientStrength;
        bool isShadowed;

        QVector3D planetPosition = QVector3D(body->getDistanceFromSun(), 0.0f, 0.0f);
        planetPosition = planetModelViewMatrix.map(planetPosition);
        if (i == 0) // Для солнца
        {
            lightDir = QVector3D(0, 0, 0) - planetPosition;
            ambientStrength = 1.0f;
            isShadowed = false;

        } else
        {
            lightDir = (sunPosition - planetPosition).normalized();
            ambientStrength = 0.2f;
            isShadowed = shadows[i];
        }
        // передаем направление света в мировых координатах
        QMatrix4x4 normalMatrix = modelViewMatrix.inverted().transposed();
        QVector3D transformedLightDir = (normalMatrix * QVector4D(lightDir, 0.0f)).toVector3D();


        shaderProgram->setUniformValue("lightDirection", transformedLightDir);
        shaderProgram->setUniformValue("ambientStrength", ambientStrength);
        shaderProgram->setUniformValue("shadowed", isShadowed);

        body->draw(shaderProgram, planetModelViewMatrix);
    }
    glBindVertexArray(0);
    shaderProgram->release();
}

// Обработка нажатия кнопки мыши
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePos = event->position().toPoint(); // Запоминаем позицию курсора
    if (event->button() == Qt::MiddleButton)
    {
        isCameraMoving = true;
    }
}

// Обработка перемещения мыши
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    float dx = event->position().x() - lastMousePos.x();
    float dy = event->position().y() - lastMousePos.y();

    if (isCameraMoving){
        cameraPos.setX(cameraPos.x() + dx * cameraMoveSpeed.x());
        cameraPos.setY(cameraPos.y() - dy * cameraMoveSpeed.y()); // Инвертируем dy, для перемещения камеры
    } else {
        xRot += dy;
        yRot += dx;
    }

    lastMousePos = event->position().toPoint(); // Запоминаем позицию курсора
    update(); // Запрашиваем перерисовку виджета
}

// Обработка вращения колеса мыши
void GLWidget::wheelEvent(QWheelEvent *event)
{
    zoom += event->angleDelta().y() / 120.0f; // Изменяем zoom в зависимости от прокрутки
    zoom = qBound(-100.0f, zoom, -2.0f); // Ограничиваем значения zoom
    update(); // Запрашиваем перерисовку виджета
}

// Создание геометрии сферы
void GLWidget::createSphere(float radius, int segments)
{
    vertices.clear(); // Очищаем вектор вершин
    normals.clear(); // Очищаем вектор нормалей
    // Создаем вершины сферы
    for (int i = 0; i <= segments; ++i) {
        float lat0 = M_PI * (-0.5f + (float)i / segments);
        float z0  = radius * sinf(lat0);
        float zr0 = radius * cosf(lat0);

        float lat1 = M_PI * (-0.5f + (float)(i+1) / segments);
        float z1 = radius * sinf(lat1);
        float zr1 = radius * cosf(lat1);
        for (int j = 0; j <= segments; ++j) {
            float lng = 2 * M_PI * (float)j / segments;
            float x0 = cosf(lng) * zr0;
            float y0 = sinf(lng) * zr0;
            float x1 = cosf(lng) * zr1;
            float y1 = sinf(lng) * zr1;

            QVector3D vertex0(x0, y0, z0);
            QVector3D vertex1(x1, y1, z1);
            vertices.append(vertex0);
            vertices.append(vertex1);

            // Нормаль для вершины сферы - это просто нормализованный вектор от центра к вершине
            normals.append(vertex0.normalized());
            normals.append(vertex1.normalized());
        }
    }
}

// Настройка атрибутов вершин
void GLWidget::setupVertexAttribs()
{
    glGenVertexArrays(1, &VAO); // Создаем VAO
    glGenBuffers(1, &VBO);      // Создаем VBO
    glGenBuffers(1, &NBO);      // Создаем NBO

    glBindVertexArray(VAO); // Привязываем VAO

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Привязываем VBO
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(QVector3D), vertices.constData(), GL_STATIC_DRAW); // Загружаем данные вершин в VBO

    // Настраиваем атрибут вершин (позиция)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), nullptr);
    glEnableVertexAttribArray(0); // Включаем атрибут

    glBindBuffer(GL_ARRAY_BUFFER, NBO); // Привязываем NBO
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(QVector3D), normals.constData(), GL_STATIC_DRAW); // Загружаем данные нормалей в NBO

    // Настраиваем атрибут нормалей
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(QVector3D), nullptr);
    glEnableVertexAttribArray(1); // Включаем атрибут

    glBindBuffer(GL_ARRAY_BUFFER, 0);  // Отвязываем VBO
    glBindVertexArray(0);           // Отвязываем VAO
}

// Обработка отпускания кнопки мыши
void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        isCameraMoving = false;
    }
}

// Обновление позиций планет
void GLWidget::updatePlanetPositions()
{
    // Обновляем углы вращения для каждой планеты
    for (int i = 1; i < celestialBodies.size(); ++i) { // Начинаем с 1, чтобы не вращать Солнце
        planetAngles[i] += planetRotationSpeeds[i] * rotationSpeed;
        if (planetAngles[i] > 360.0f) {
            planetAngles[i] -= 360.0f;
        }

        // Обновление вращения спутников
        if(dynamic_cast<Planet*>(celestialBodies[i]))
        {
            Planet* planet = dynamic_cast<Planet*>(celestialBodies[i]);
            const QVector<Satellite*> &satellites = planet->getSatellites();
            for (int j = 0; j < satellites.size(); j++)
            {
                satelliteAngles[i][j] += satellites[j]->getRotationSpeed() * rotationSpeed / 1000.0f;
                if (satelliteAngles[i][j] > 360.0f) {
                    satelliteAngles[i][j] -= 360.0f;
                }
            }
        }
    }
    calculateShadows();
    update(); // Запрашиваем перерисовку виджета
}

// Установка скорости вращения
void GLWidget::setRotationSpeed(int speed)
{
    rotationSpeed = static_cast<float>(speed) / 20.0f;
}

void GLWidget::drawRings(float innerRadius, float outerRadius, float y_position)
{
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 360; ++i) {
        float angle = i * M_PI / 180.0f;
        float innerX = innerRadius * cosf(angle);
        float innerY = innerRadius * sinf(angle);
        float outerX = outerRadius * cosf(angle);
        float outerY = outerRadius * sinf(angle);
        glVertex3f(innerX, y_position + innerY, 0.0f);
        glVertex3f(outerX, y_position + outerY, 0.0f);
    }
    glEnd();
}

void GLWidget::calculateShadows()
{
    QVector3D sunPosition = QVector3D(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < celestialBodies.size(); ++i)
    {
        shadows[i] = false;
        if (i == 0) continue; // Не затеняем Солнце
        CelestialBody* body = celestialBodies[i];
        QMatrix4x4 modelMatrix;
        modelMatrix.setToIdentity();
        if (i > 0) {
            modelMatrix.rotate(planetAngles[i], QVector3D(0.0f, 1.0f, 0.0f));
        }

        QVector3D planetPosition = QVector3D(body->getDistanceFromSun(), 0.0f, 0.0f);
        planetPosition = modelMatrix.map(planetPosition);


        QVector3D sunToPlanet = planetPosition - sunPosition;
        for (int j = 0; j < celestialBodies.size(); j++)
        {
            if (i == j) continue; // Не проверяем само на себя
            if (j == 0) continue; // Солнце не может блокировать свет
            CelestialBody* blockerBody = celestialBodies[j];
            QMatrix4x4 blockerModelMatrix;
            blockerModelMatrix.setToIdentity();
            if (j > 0) {
                blockerModelMatrix.rotate(planetAngles[j], QVector3D(0.0f, 1.0f, 0.0f));
            }
            QVector3D blockerPosition = QVector3D(blockerBody->getDistanceFromSun(), 0.0f, 0.0f);
            blockerPosition = blockerModelMatrix.map(blockerPosition);

            QVector3D sunToBlocker = blockerPosition - sunPosition;
            float sunToPlanetDist = sunToPlanet.length();
            float sunToBlockerDist = sunToBlocker.length();
            float blockerRadius = blockerBody->getRadius();

            // Проверяем, находится ли планета между солнцем и текущим телом
            if (sunToBlockerDist < sunToPlanetDist)
            {
                // Проверяем, что вектор от солнца до планеты "затеняемого объекта" и от солнца до "затеняющего" объекта имеют одинаковое направление
                float dotProduct = QVector3D::dotProduct(sunToPlanet.normalized(), sunToBlocker.normalized());
                if (dotProduct > 0.99f) // Почти коллинеарны
                {
                    // Теперь грубая проверка, находится ли тело внутри проекции сферы другого тела на отрезок от солнца до планеты
                    QVector3D diff = planetPosition - blockerPosition;
                    float distanceToLine = (diff - QVector3D::dotProduct(diff, sunToBlocker.normalized()) * sunToBlocker.normalized()).length();
                    if(distanceToLine < blockerRadius)
                    {
                        shadows[i] = true;
                        break;
                    }
                }
            }
        }
    }
}

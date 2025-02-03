#include "mainwindow.h"
#include "glwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Создаем наш OpenGL виджет
    glWidget = new GLWidget(this);

    // Создаем слайдер
    speedSlider = new QSlider(Qt::Horizontal, this);
    speedSlider->setMinimum(0);
    speedSlider->setMaximum(200);
    speedSlider->setValue(20); // Начальное значение

    QLabel* speedLabel = new QLabel("Animation Speed:");

    // Создаем layout и добавляем виджет в него
    QHBoxLayout *sliderLayout = new QHBoxLayout;
    sliderLayout->addWidget(speedLabel);
    sliderLayout->addWidget(speedSlider);


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(sliderLayout);
    layout->addWidget(glWidget);


    // Создаем центральный виджет и устанавливаем layout
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Устанавливаем размер окна
    resize(800, 600);

    connect(speedSlider, &QSlider::valueChanged, glWidget, &GLWidget::setRotationSpeed);
}

MainWindow::~MainWindow()
{
    delete glWidget; // Удаляем виджет в деструкторе
}

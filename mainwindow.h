#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>

class GLWidget; // Forward declaration
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    GLWidget *glWidget; // Указатель на наш OpenGL виджет
    QSlider *speedSlider;
};
#endif // MAINWINDOW_H

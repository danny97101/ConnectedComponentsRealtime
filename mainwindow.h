#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include <math.h>


#include <QLabel>
#include <QMainWindow>
#include <QtCore>
using namespace cv;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void displayFrame();

private:
    Ui::MainWindow *ui;
    cv::VideoCapture cam;
    Ptr<BackgroundSubtractor> fgbg;
    Mat orig;
};

#endif // MAINWINDOW_H

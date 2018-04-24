#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <map>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    srand(time(NULL));
    ui->setupUi(this);
    cam=cv::VideoCapture(0);

    //QTimer *qTimer=new QTimer(this);
    //connect(qTimer,SIGNAL(timeout()),this,SLOT(displayFrame()));
    //qTimer->start(10);
    orig = Mat(1000, 1000, CV_8U);
    for (int row = 0; row < orig.rows; row++) {
        unsigned char* pixel = orig.ptr<unsigned char>(row);
        for (int col = 0; col < orig.cols; col++) {
            if (row > 10 && row < 100 && col > 100 && col < 200)
                *pixel = 255;
            else if (row > 300 && row < 600 && col > 300 && col < 600)
                *pixel =255;
            else if (row > 400 && row < 500 && col > 250 && col < 301)
                *pixel = 255;
            else if (row > 800 && row < 990 && col > 10 && col < 900)
                *pixel = 255;
            else if (row > 50 && row < 990 && col > 800 && col < 900)
                *pixel = 255;
            else *pixel = 0;

            //*pixel = rand()%10<1?0:255;

            pixel++;
        }
    }
    imshow("original", orig);



    // provisional labeling
    int labels[orig.rows][orig.cols];
    std::map<int,int> equivalence;
    int nextLabel = 0;

    for (int i = 0; i < orig.rows; i++) {
        for (int j = 0; j< orig.cols; j++) {
            labels[i][j] = -1;
        }
    }


    Mat firstPass = Mat(orig.rows, orig.cols, CV_8UC3);
    for (int row = 0; row < orig.rows; row++) {
        unsigned char* pixel = orig.ptr<unsigned char>(row);
        Vec3b* newPixel = firstPass.ptr<Vec3b>(row);
        for (int col = 0; col < orig.cols; col++) {
            if (*pixel == 255) {
                int label;
                if (labels[row][col] == -1) {
                    label = nextLabel++;
                    equivalence[label] = label;
                } else {
                    label = labels[row][col];
                }
                labels[row][col] = label;
                if (orig.at<unsigned char>(row, col +1) && col < orig.cols - 1) labels[row][col + 1] = label;
                if (orig.at<unsigned char>(row+1, col +1) &&col < orig.cols - 1 && row < orig.rows - 1) labels[row + 1][col + 1] = label;
                if (orig.at<unsigned char>(row+1, col) &&row < orig.rows - 1) labels[row + 1][col] = label;
                if (orig.at<unsigned char>(row+1, col -1) &&row < orig.rows -1 && col > 0) labels[row + 1][col - 1] = label;


                //equivalence recording
                //only checking the other 4 neighbors
                if (col > 0 && labels[row][col-1] != -1 && labels[row][col-1] != label) {
                    equivalence[std::max(labels[row][col],labels[row][col-1])] = std::min(labels[row][col],labels[row][col-1]);
                }
                if (col > 0 && row > 0 && labels[row-1][col-1] != -1 && labels[row-1][col-1] != label) {
                    equivalence[std::max(labels[row][col],labels[row-1][col-1])] = std::min(labels[row][col],labels[row-1][col-1]);
                }
                if (row > 0 && labels[row-1][col] != -1 && labels[row-1][col] != label) {
                    equivalence[std::max(labels[row][col],labels[row-1][col])] = std::min(labels[row][col],labels[row-1][col]);
                }
                if (row > 0 && col < orig.cols - 1 && labels[row-1][col+1] != -1 && labels[row-1][col+1] != label) {
                    equivalence[std::max(labels[row][col],labels[row-1][col+1])] = std::min(labels[row][col],labels[row-1][col+1]);
                }


            }
            pixel++;
            newPixel++;
        }
    }

    int scale = 255 / nextLabel;

    for (int row = 0; row < orig.rows; row++) {
        Vec3b* newPixel = firstPass.ptr<Vec3b>(row);
        for (int col = 0; col < orig.cols; col++) {
            if (labels[row][col] == -1) {
                (*newPixel)[2] = 0;
                (*newPixel)[0] = 0;
            } else {
                (*newPixel)[2] = (labels[row][col]+1) * scale;
                (*newPixel)[0] = 255 - (labels[row][col]+1) * scale;
            }


            (*newPixel)[1] = 0;

            newPixel++;
        }
    }

    imshow("first pass", firstPass);

    //equivalence replacing

    for (int row = 0; row < orig.rows; row++) {
        Vec3b* newPixel = firstPass.ptr<Vec3b>(row);
        for (int col = 0; col < orig.cols; col++) {
            if (labels[row][col] == -1) {
                (*newPixel)[2] = 0;
                (*newPixel)[0] = 0;
            } else {
                (*newPixel)[2] = (equivalence[labels[row][col]]+1) * scale;
                (*newPixel)[0] = 255 - (equivalence[labels[row][col]]+1) * scale;
            }


            (*newPixel)[1] = 0;

            newPixel++;
        }
    }
    imshow("replaced", firstPass);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayFrame() {

}

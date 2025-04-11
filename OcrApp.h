

#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QFileDialog>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

class OCRApp : public QWidget {
    Q_OBJECT

public:
    OCRApp(QWidget *parent = nullptr);

private slots:
    void loadImage();
    void runOCR();

private:
    QLabel *imageLabel;
    QTextEdit *textEdit;
    QPushButton *loadButton;
    QPushButton *ocrButton;
    cv::Mat image;
    tesseract::TessBaseAPI *tess;
};


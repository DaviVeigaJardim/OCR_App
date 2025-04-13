#ifndef OCRAPP_H
#define OCRAPP_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QCheckBox>
#include <tesseract/baseapi.h>
#include <opencv2/core.hpp>

class OCRApp : public QWidget {
    Q_OBJECT

public:
    OCRApp(QWidget *parent = nullptr);
    ~OCRApp();

private slots:
    void loadImage();
    void runOCR();
    void updateProcessedImage();

private:
    QLabel *imageLabel;
    QLabel *processedImageLabel;
    QTextEdit *textEdit;
    QPushButton *loadButton;
    QPushButton *ocrButton;
    QCheckBox *binarizationCheckBox;
    QCheckBox *grayScaleCheckBox;
    tesseract::TessBaseAPI *tess;
    cv::Mat image;

    cv::Mat preprocessImage(const cv::Mat& inputImage);
};

#endif // OCRAPP_H
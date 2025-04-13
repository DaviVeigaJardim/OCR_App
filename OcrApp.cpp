#include "OcrApp.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QScrollArea>
#include <QFileDialog>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QGroupBox>
#include <QFormLayout>

OCRApp::OCRApp(QWidget *parent) : QWidget(parent) {
    imageLabel = new QLabel(this);
    processedImageLabel = new QLabel(this);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(imageLabel);
    scrollArea->setWidgetResizable(true);
    imageLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    QScrollArea *processedScrollArea = new QScrollArea(this);
    processedScrollArea->setWidget(processedImageLabel);
    processedScrollArea->setWidgetResizable(true);
    processedImageLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    textEdit = new QTextEdit(this);
    loadButton = new QPushButton("Carregar Imagem", this);
    ocrButton = new QPushButton("Executar OCR", this);

    binarizationCheckBox = new QCheckBox("Binarização", this);
    grayScaleCheckBox = new QCheckBox("Escala de Cinza", this);

    QGroupBox *preprocessGroupBox = new QGroupBox("Pré-processamento", this);
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(grayScaleCheckBox);
    formLayout->addRow(binarizationCheckBox);
    preprocessGroupBox->setLayout(formLayout);

    connect(loadButton, &QPushButton::clicked, this, &OCRApp::loadImage);
    connect(ocrButton, &QPushButton::clicked, this, &OCRApp::runOCR);
    connect(binarizationCheckBox, &QCheckBox::stateChanged, this, &OCRApp::updateProcessedImage);
    connect(grayScaleCheckBox, &QCheckBox::stateChanged, this, &OCRApp::updateProcessedImage);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(scrollArea);
    layout->addWidget(processedScrollArea);
    layout->addWidget(textEdit);
    layout->addWidget(preprocessGroupBox);
    layout->addWidget(loadButton);
    layout->addWidget(ocrButton);
    setLayout(layout);

    tess = new tesseract::TessBaseAPI();
    if (tess->Init(NULL, "por")) {
        qDebug() << "Não foi possível inicializar o Tesseract.";
    }
}

void OCRApp::loadImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Carregar Imagem", "", "Imagens (*.png *.jpg *.jpeg)");
    if (!fileName.isEmpty()) {
        image = cv::imread(fileName.toStdString());
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        imageLabel->setPixmap(QPixmap::fromImage(QImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888)));
        imageLabel->adjustSize();
        updateProcessedImage();
    }
}

void OCRApp::runOCR() {
    if (image.empty()) return;

    cv::Mat processedImage = preprocessImage(image);
    tess->SetImage(processedImage.data, processedImage.cols, processedImage.rows, 3, processedImage.step);
    char *outText = tess->GetUTF8Text();
    textEdit->setText(QString(outText));
    delete[] outText;
}

cv::Mat OCRApp::preprocessImage(const cv::Mat& inputImage) {
    cv::Mat processedImage = inputImage.clone();

    if (grayScaleCheckBox->isChecked()) {
        cv::cvtColor(processedImage, processedImage, cv::COLOR_BGR2GRAY);
        if (binarizationCheckBox->isChecked()) {
            cv::threshold(processedImage, processedImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
            cv::cvtColor(processedImage, processedImage, cv::COLOR_GRAY2RGB);
        }
    } else if (binarizationCheckBox->isChecked()) {
        cv::Mat gray;
        cv::cvtColor(processedImage, gray, cv::COLOR_BGR2GRAY);
        cv::threshold(gray, processedImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        cv::cvtColor(processedImage, processedImage, cv::COLOR_GRAY2RGB);
    }

    return processedImage;
}

void OCRApp::updateProcessedImage() {
    if (image.empty()) return;

    cv::Mat processedImage = preprocessImage(image);
    QImage qImage(processedImage.data, processedImage.cols, processedImage.rows, processedImage.step, QImage::Format_RGB888);
    processedImageLabel->setPixmap(QPixmap::fromImage(qImage));
    processedImageLabel->adjustSize();
}

OCRApp::~OCRApp() {
    tess->End();
}
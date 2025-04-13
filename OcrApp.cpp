#include "OcrApp.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QScrollArea>
#include <QFileDialog>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QGroupBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QScrollBar>

OCRApp::OCRApp(QWidget *parent) : QWidget(parent) {
    imageLabel = new QLabel(this);
    processedImageLabel = new QLabel(this);

    originalImageScrollArea = new QScrollArea(this);
    originalImageScrollArea->setWidget(imageLabel);
    originalImageScrollArea->setWidgetResizable(true);
    imageLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    processedImageScrollArea = new QScrollArea(this);
    processedImageScrollArea->setWidget(processedImageLabel);
    processedImageScrollArea->setWidgetResizable(true);
    processedImageLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    textEdit = new QTextEdit(this);
    loadButton = new QPushButton("Carregar Imagem", this);
    ocrButton = new QPushButton("Executar OCR", this);

    plateModeCheckBox = new QCheckBox("Modo Pré-processamento", this);

    QGroupBox *preprocessGroupBox = new QGroupBox("Opções", this);
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(plateModeCheckBox);
    preprocessGroupBox->setLayout(formLayout);

    connect(loadButton, &QPushButton::clicked, this, &OCRApp::loadImage);
    connect(ocrButton, &QPushButton::clicked, this, &OCRApp::runOCR);
    connect(plateModeCheckBox, &QCheckBox::stateChanged, this, &OCRApp::updateProcessedImage);

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->addWidget(originalImageScrollArea, 0, 0);
    gridLayout->addWidget(processedImageScrollArea, 0, 1);
    gridLayout->addWidget(textEdit, 1, 0, 1, 2);
    gridLayout->addWidget(preprocessGroupBox, 2, 0, 1, 2);
    gridLayout->addWidget(loadButton, 3, 0);
    gridLayout->addWidget(ocrButton, 3, 1);
    setLayout(gridLayout);

    resize(1200, 800);

    tess = new tesseract::TessBaseAPI();
    if (tess->Init(NULL, "por")) {
        qDebug() << "Não foi possível inicializar o Tesseract.";
    }
}

void OCRApp::loadImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Carregar Imagem", "", "Imagens (*.png *.jpg *.jpeg)");
    if (!fileName.isEmpty()) {
        cv::Mat image = cv::imread(fileName.toStdString());
        if (image.empty()) return;

        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        this->image = image;
        this->currentFileName = fileName;

        QSize scrollAreaSize = originalImageScrollArea->viewport()->size();
        QImage qImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
        QPixmap pixmap = QPixmap::fromImage(qImage);

        imageLabel->setPixmap(pixmap.scaled(scrollAreaSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        imageLabel->adjustSize();

        updateProcessedImage();
    }
}

void OCRApp::runOCR() {
    if (image.empty()) return;

    cv::Mat processedImage = preprocessImage(image);

    tess->SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    tess->SetImage(processedImage.data, processedImage.cols, processedImage.rows, 3, processedImage.step);

    char *outText = tess->GetUTF8Text();
    textEdit->setText(QString(outText));
    delete[] outText;
}

cv::Mat OCRApp::preprocessImage(const cv::Mat& inputImage) {
    cv::Mat processedImage = inputImage.clone();

    if (plateModeCheckBox->isChecked()) {
        cv::Mat filtered, gray, claheOut;

        // Leve filtro bilateral para preservar bordas e remover ruído
        cv::bilateralFilter(processedImage, filtered, 9, 75, 75);

        // Converter para escala de cinza
        cv::cvtColor(filtered, gray, cv::COLOR_BGR2GRAY);

        // CLAHE para melhorar contraste local
        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
        clahe->apply(gray, claheOut);

        // Converter de volta para RGB (Tesseract espera imagem colorida)
        cv::cvtColor(claheOut, processedImage, cv::COLOR_GRAY2RGB);
    }

    return processedImage;
}

void OCRApp::updateProcessedImage() {
    if (image.empty()) return;

    cv::Mat processedImage = preprocessImage(image);
    QImage qImage(processedImage.data, processedImage.cols, processedImage.rows, processedImage.step, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qImage);

    QSize scrollAreaSize = processedImageScrollArea->viewport()->size();
    processedImageLabel->setPixmap(pixmap.scaled(scrollAreaSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    processedImageLabel->adjustSize();
}

OCRApp::~OCRApp() {
    tess->End();
}
#include "OcrApp.h"
#include <QVBoxLayout> 
#include <QDebug> 

OCRApp::OCRApp(QWidget *parent) : QWidget(parent)
{
    imageLabel = new QLabel(this);
    textEdit = new QTextEdit(this);
    loadButton = new QPushButton("Carregar Imagem", this);
    ocrButton = new QPushButton("Executar OCR", this);

    connect(loadButton, &QPushButton::clicked, this, &OCRApp::loadImage);
    connect(ocrButton, &QPushButton::clicked, this, &OCRApp::runOCR);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(imageLabel);
    layout->addWidget(textEdit);
    layout->addWidget(loadButton);
    layout->addWidget(ocrButton);
    setLayout(layout); // Usa setLayout para definir o layout do widget

    tess = new tesseract::TessBaseAPI();
    if (tess->Init(NULL, "por")) 
    { // Configura para português
        qDebug() << "Não foi possível inicializar o Tesseract.";
    }
}

void OCRApp::loadImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Carregar Imagem", "", "Imagens (*.png *.jpg *.jpeg)");
    if (!fileName.isEmpty())
    {
        image = cv::imread(fileName.toStdString());
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        QImage qImage(image.data, image.cols, image.rows, image.step, QImage::Format_RGB888);
        imageLabel->setPixmap(QPixmap::fromImage(qImage));
    }
}

void OCRApp::runOCR() 
{
    if (image.empty()) return;

    tess->SetImage(image.data, image.cols, image.rows, 3, image.step);
    char *outText = tess->GetUTF8Text();
    textEdit->setText(QString(outText));
    delete[] outText;
}

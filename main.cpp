#include "OcrApp.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    OCRApp widget;
    widget.show();
    return app.exec();
}
#include "imagetopdf.h"

#include <QApplication>
#include <QIcon>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImageToPDF w;
    const QIcon logoprogram(":/itp/convert.png");
    w.setWindowIcon(logoprogram);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    w.show();
    return a.exec();
}

#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    Controller c = new Controller();
    c.setup_signals();

    w.show();

    return a.exec();
}

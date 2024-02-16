#include "chatapplicationserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatApplicationServer w;
    w.show();
    return a.exec();
}

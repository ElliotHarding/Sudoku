#include "dlg_home.h"

#include <QApplication>

uint main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DLG_Home w;
    w.show();
    return a.exec();
}

#include "dlg_home.h"
#include "ui_dlg_home.h"

DLG_Home::DLG_Home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DLG_Home)
{
    ui->setupUi(this);
}

DLG_Home::~DLG_Home()
{
    delete ui;
}


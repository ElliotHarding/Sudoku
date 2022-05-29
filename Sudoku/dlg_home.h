#ifndef DLG_HOME_H
#define DLG_HOME_H

#include <QMainWindow>
#include "tile.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DLG_Home; }
QT_END_NAMESPACE

class DLG_Home : public QMainWindow
{
    Q_OBJECT

public:
    DLG_Home(QWidget *parent = nullptr);
    ~DLG_Home();

private:
    Ui::DLG_Home *ui;

    QVector<QVector<Tile*>> m_board;
};
#endif // DLG_HOME_H

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

protected:
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::DLG_Home *ui;

    QVector<QVector<Tile*>> m_board;
    Tile* m_pSelectedTile;

    void generateBoard();
};
#endif // DLG_HOME_H

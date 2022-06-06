#ifndef DLG_HOME_H
#define DLG_HOME_H

#include "tile.h"

#include <atomic>

#include <QMainWindow>
#include <QMutex>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class DLG_Home; }
QT_END_NAMESPACE

class AiThread;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DLG_Home::DLG_Home
///
///
class DLG_Home : public QMainWindow
{
    Q_OBJECT

public:
    DLG_Home(QWidget *parent = nullptr);
    ~DLG_Home();

protected:
    ///User input events
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    ///UI Button
    void on_btn_reset_clicked();
    void on_btn_ai_clicked();

    ///Ai
    void updateCell(const int& x, const int& y, const int& value);

private:
    Ui::DLG_Home *ui;

    QVector<QVector<Tile*>> m_board;
    Tile* m_pSelectedTile;

    ///AI
    AiThread* m_pAiThread;

    void generateBoard();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AiThread
///
///
class AiThread : public QThread
{
        Q_OBJECT
public:
    AiThread();

    void setBoard(const QVector<QVector<int>>& board);
    void setStop();
    bool isWorking();

    void run();

signals:
    void updateCell(const int& x, const int& y, const int& value);

private:
    bool findSolution(QVector<QVector<int>>& board, const int& x, const int& y);

    QVector<QVector<int>> m_board;
    std::atomic<bool> m_bStop;
    std::atomic<bool> m_bWorking;
};

#endif // DLG_HOME_H

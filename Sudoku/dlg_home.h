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

    void setSelectedTile(Tile* pTile);

protected:
    ///User input events
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    ///UI Button
    void on_btn_reset_clicked();
    void on_btn_ai_clicked();

    ///Ai
    void updateCell(const uint& x, const uint& y, const uint& value);
    void updateBoard(const std::vector<std::vector<uint>>& board);

private:
    Ui::DLG_Home *ui;

    std::vector<std::vector<Tile*>> m_board;
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

    void setBoard(const std::vector<std::vector<uint>>& board);
    void setStop();
    bool isWorking();

    void run();

signals:
    void updateCell(const uint& x, const uint& y, const uint& value);
    void updateBoard(const std::vector<std::vector<uint>>& board);

private:
    bool findSolution(std::vector<std::vector<uint>>& board, const uint x, const uint y);

    ///Atomic flags
    std::atomic<bool> m_bStop;
    std::atomic<bool> m_bWorking;

    ///Memory
    std::vector<std::vector<uint>> m_board;
    uint m_nextX;
    uint m_nextY;
};

#endif // DLG_HOME_H

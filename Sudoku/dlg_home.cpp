#include "dlg_home.h"
#include "ui_dlg_home.h"
#include "settings.h"
#include "math.h"

#include <random>
#include <QDebug>
#include <QMouseEvent>
#include <QRandomGenerator>

DLG_Home::DLG_Home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DLG_Home)
    , m_pSelectedTile(nullptr)
{
    ui->setupUi(this);

    m_pAiThread = new AiThread();
    connect(m_pAiThread, SIGNAL(updateCell(const int&, const int&, const int&)), this, SLOT(updateCell(const int&, const int&, const int&)));
    m_pAiThread->start();

    for(int x = 0; x < Settings::BoardCountX; x++)
    {
        m_board.push_back(QVector<Tile*>());
        for(int y = 0; y < Settings::BoardCountY; y++)
        {
            m_board[x].push_back(new Tile(this, Settings::BoardRect.left() + x * Settings::TileSize, Settings::BoardRect.top() + y * Settings::TileSize, Settings::TileSize, Settings::TileSize));
        }
    }

    generateBoard();
}

DLG_Home::~DLG_Home()
{
    for(int x = 0; x < Settings::BoardCountX; x++)
    {
        for(int y = 0; y < Settings::BoardCountY; y++)
        {
            delete m_board[x][y];
        }
    }

    //Stop ai thread
    m_pAiThread->setStop();
    while(!m_pAiThread->isSetStop())
    {
        QThread::sleep(1);
    }
    m_pAiThread->terminate();
    delete m_pAiThread;

    delete ui;
}

void DLG_Home::mousePressEvent(QMouseEvent *mouseEvent)
{
    if(m_pSelectedTile)
    {
        m_pSelectedTile->setSelected(false);
    }

    if(Settings::BoardRect.contains(mouseEvent->pos()))
    {
        const int xOffset = mouseEvent->pos().x() - Settings::BoardRect.x();
        const int yOffset = mouseEvent->pos().y() - Settings::BoardRect.y();
        const int x = floor(xOffset/Settings::TileSize);
        const int y = floor(yOffset/Settings::TileSize);

        m_pSelectedTile = m_board[x][y];
        m_pSelectedTile->setSelected(true);
    }
}

void DLG_Home::keyPressEvent(QKeyEvent *event)
{
    if(m_pSelectedTile && !m_pSelectedTile->isPermanent())
    {
        if(event->key() >= Qt::Key::Key_0 && event->key() <= Qt::Key::Key_9)
        {
            m_pSelectedTile->setValue(event->key() - 48);
        }
        else if(event->key() == Qt::Key::Key_Backspace || event->key() == Qt::Key::Key_Delete)
        {
            m_pSelectedTile->setValue(0);
        }
    }
}

void randomlyFillSubGrid(QVector<QVector<Tile*>>& board, const int& startX, const int& startY)
{
    int x = startX;
    int y = startY;

    std::mt19937 eng(clock());
    std::vector<int> randomSubGrid = Settings::SubGridOptions;
    std::shuffle(randomSubGrid.begin(), randomSubGrid.end(), eng);

    for(size_t i = 0; i < randomSubGrid.size(); i++)
    {
        board[x][y]->setValue(randomSubGrid[i]);

        if((i+1) % Settings::SubGridCountX == 0)
        {
            y++;
            x=startX;
        }
        else
        {
            x++;
        }
    }
}

bool validPosition(QVector<QVector<Tile*>>& board, const int& xCheck, const int& yCheck, const int& numToCheck)
{
    //Check row
    for(int col = 0; col < board.size(); col++)
    {
        if(board[col][yCheck]->value() == numToCheck)
        {
            return false;
        }
    }

    //Check col
    for(int row = 0; row < board[0].size(); row++)
    {
        if(board[xCheck][row]->value() == numToCheck)
        {
            return false;
        }
    }

    //Check sub grid
    const int subStartX = xCheck - (xCheck % 3);
    const int subStartY = yCheck - (yCheck % 3);
    for(int x = subStartX; x < subStartX + 3; x++)
    {
        for(int y = subStartY; y < subStartY + 3; y++)
        {
            if(board[x][y]->value() == numToCheck)
            {
                return false;
            }
        }
    }

    return true;
}

bool validPosition(QVector<QVector<int>>& board, const int& xCheck, const int& yCheck, const int& numToCheck)
{
    //Check row
    for(int col = 0; col < board.size(); col++)
    {
        if(board[col][yCheck] == numToCheck)
        {
            return false;
        }
    }

    //Check col
    for(int row = 0; row < board[0].size(); row++)
    {
        if(board[xCheck][row] == numToCheck)
        {
            return false;
        }
    }

    //Check sub grid
    const int subStartX = xCheck - (xCheck % Settings::SubGridCountX);
    const int subStartY = yCheck - (yCheck % Settings::SubGridCountY);
    for(int x = subStartX; x < subStartX + Settings::SubGridCountX; x++)
    {
        for(int y = subStartY; y < subStartY + Settings::SubGridCountY; y++)
        {
            if(board[x][y] == numToCheck)
            {
                return false;
            }
        }
    }

    return true;
}

bool fillBoardPossible(QVector<QVector<Tile*>>& board, const int& x, const int& y)
{
    for(int newNum : Settings::SubGridOptions)
    {
        if(validPosition(board, x, y, newNum))
        {
            //Set board[x][y] to testing newNum
            board[x][y]->setValue(newNum);

            //Find next free board pos
            int nextX = x;
            int nextY = y;
            while(board[nextX][nextY]->value() != 0)
            {
                if(nextX > board.size()-2)
                {
                    nextX = 0;
                    nextY++;

                    //If reached the end of the board
                    if(nextY == board[0].size())
                    {
                        return true;
                    }
                }
                else
                {
                    nextX++;
                }
            }

            //Try fill in rest of board with newNum in board pos x,y
            if(fillBoardPossible(board, nextX, nextY))
            {
                return true;
            }

            board[x][y]->setValue(0);
        }
    }

    return false;
}

void DLG_Home::generateBoard()
{
    //Reset board
    for(int x = 0; x < m_board.size(); x++)
    {
        for(int y = 0; y < m_board[x].size(); y++)
        {
            m_board[x][y]->setValue(0);
        }
    }

    //Diagonally fill centeral subgrids
    randomlyFillSubGrid(m_board, 0, 0);
    randomlyFillSubGrid(m_board, 3, 3);
    randomlyFillSubGrid(m_board, 6, 6);

    //Try fill rest of board
    if(fillBoardPossible(m_board, 3, 0))
    {
        qDebug() << "DLG_Home::generateBoard - Filled successfully";
    }
    else
    {
        qDebug() << "DLG_Home::generateBoard - Fill failed";
    }

    //Prep to clear random cells
    // - Get a list of cells to keep
    QList<QPoint> keptCells;
    for(int i = 0; i < Settings::NumberOfCellsToKeepAfterGen;)
    {
        const int x = QRandomGenerator::global()->generateDouble() * m_board.size();
        const int y = QRandomGenerator::global()->generateDouble() * m_board.size();
        const QPoint cellToKeep = QPoint(x,y);
        if(!keptCells.contains(cellToKeep))
        {
            keptCells.push_back(cellToKeep);
            i++;
        }
    }

    //Remove all cells except ones in keptCells
    for(int x = 0; x < m_board.size(); x++)
    {
        for(int y = 0; y < m_board[x].size(); y++)
        {
            const QPoint boardLocation = QPoint(x,y);
            if(!keptCells.contains(boardLocation))
            {
                m_board[x][y]->setValue(0);
                m_board[x][y]->setPermanent(false);
            }
            else
            {
                keptCells.removeOne(boardLocation);
                m_board[x][y]->setPermanent(true);
            }
        }
    }
}

void DLG_Home::on_btn_reset_clicked()
{
    generateBoard();
}

void DLG_Home::on_btn_ai_clicked()
{
    QVector<QVector<int>> board = QVector<QVector<int>>(m_board.size(), QVector<int>(m_board[0].size(), 0));
    for(int x = 0; x < m_board.size(); x++)
    {
        for(int y = 0; y < m_board[x].size(); y++)
        {
            board[x][y] = m_board[x][y]->value();
        }
    }
    m_pAiThread->setBoard(board);
}

void DLG_Home::updateCell(const int &x, const int &y, const int &value)
{
    m_board[x][y]->setValue(value);
}

AiThread::AiThread() : QThread(),
    m_bStop(false),
    m_bWorkOnBoard(false)
{
}

void AiThread::setBoard(const QVector<QVector<int>>& board)
{
    m_mutex.lock();
    m_board = board;
    m_bWorkOnBoard = true;
    m_mutex.unlock();
}

void AiThread::setStop()
{
    m_mutex.lock();
    m_bStop = true;
    m_mutex.unlock();
}

bool AiThread::isSetStop()
{
    m_mutex.lock();
    const bool isStop = m_bStop;
    m_mutex.unlock();
    return isStop;
}

bool findFirstEmptySpot(const QVector<QVector<int>>& board, int& x, int& y)
{
    while(board[x][y] != 0)
    {
        if(x > board.size()-2)
        {
            x = 0;
            y++;

            if(y == board[0].size())
            {
                return false;
            }
        }
        else
        {
            x++;
        }
    }

    return true;
}

void AiThread::run()
{
    while(true)
    {
        m_mutex.lock();
        if(m_bStop)
        {
            m_mutex.unlock();
            return;
        }

        if(m_bWorkOnBoard)
        {
#ifdef AI_DEBUG
    clock_t start = clock();
#endif
            m_bWorkOnBoard = false;

            QVector<QVector<int>> board = m_board;

            m_mutex.unlock();

            int x = 0;
            int y = 0;
            if(findFirstEmptySpot(board, x, y))
            {
                if(findSolution(board, x, y))
                {
                    qDebug() << "AiThread::run - Ai found solution";
                }
                else
                {
                    qDebug() << "AiThread::run - Ai failed";
                }
            }
            else
            {
                qDebug() << "AiThread::run - set board is full";
            }


#ifdef AI_DEBUG
    clock_t end = clock();
    qDebug() << "AiThread::run: Think time: " << end - start;
#endif
        }
        else
        {
            m_mutex.unlock();
        }
    }
}

bool AiThread::findSolution(QVector<QVector<int>>& board, const int &x, const int &y)
{
    for(int newNum : Settings::SubGridOptions)
    {
        if(validPosition(board, x, y, newNum))
        {
            //Set board[x][y] to testing newNum
            board[x][y] = newNum;
            emit updateCell(x, y, newNum);

            //Find next free board pos
            int nextX = x;
            int nextY = y;
            if(!findFirstEmptySpot(board, nextX, nextY))
            {
                return true;
            }

            //Try fill in rest of board with newNum in board pos x,y
            if(findSolution(board, nextX, nextY))
            {
                return true;
            }

            board[x][y] = 0;
            emit updateCell(x, y, 0);
        }
    }

    return false;
}

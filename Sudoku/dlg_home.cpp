#include "dlg_home.h"
#include "ui_dlg_home.h"
#include "settings.h"
#include "math.h"

#include <random>
#include <QDebug>
#include <QMouseEvent>
#include <QRandomGenerator>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// DLG_Home::DLG_Home
///
/// Main window that displays the game
DLG_Home::DLG_Home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DLG_Home)
    , m_pSelectedTile(nullptr)
{
    ui->setupUi(this);

    m_pAiThread = new AiThread();
    connect(m_pAiThread, SIGNAL(updateCell(const uint&, const uint&, const uint&)), this, SLOT(updateCell(const uint&, const uint&, const uint&)));
    qRegisterMetaType<std::vector<std::vector<uint>>>("std::vector<std::vector<uint>>");
    connect(m_pAiThread, SIGNAL(updateBoard(const std::vector<std::vector<uint>>&)), this, SLOT(updateBoard(const std::vector<std::vector<uint>>&)));
    m_pAiThread->start();

    for(uint x = 0; x < Settings::BoardCountX; x++)
    {
        m_board.push_back(std::vector<Tile*>());
        for(uint y = 0; y < Settings::BoardCountY; y++)
        {
            m_board[x].push_back(new Tile(this, Settings::BoardRect.left() + x * Settings::TileSize, Settings::BoardRect.top() + y * Settings::TileSize, Settings::TileSize, Settings::TileSize));
        }
    }

#ifndef AI_DEBUG
    generateBoard();
#endif
}

DLG_Home::~DLG_Home()
{
    for(uint x = 0; x < Settings::BoardCountX; x++)
    {
        for(uint y = 0; y < Settings::BoardCountY; y++)
        {
            delete m_board[x][y];
        }
    }

    //Stop ai thread
    m_pAiThread->setStop();
    while(m_pAiThread->isWorking())
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
        const uint xOffset = mouseEvent->pos().x() - Settings::BoardRect.x();
        const uint yOffset = mouseEvent->pos().y() - Settings::BoardRect.y();
        const uint x = floor(xOffset/Settings::TileSize);
        const uint y = floor(yOffset/Settings::TileSize);

        m_pSelectedTile = m_board[x][y];
        m_pSelectedTile->setSelected(true);
    }
}

void DLG_Home::keyPressEvent(QKeyEvent *event)
{
    if(m_pSelectedTile && !m_pSelectedTile->isPermanent() && !m_pAiThread->isWorking())
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

void randomlyFillSubGrid(std::vector<std::vector<uint>>& board, const uint& startX, const uint& startY)
{
    uint x = startX;
    uint y = startY;

    std::mt19937 eng(clock());
    std::vector<uint> randomSubGrid = Settings::SubGridOptions;
    std::shuffle(randomSubGrid.begin(), randomSubGrid.end(), eng);

    for(size_t i = 0; i < randomSubGrid.size(); i++)
    {
        board[x][y] = randomSubGrid[i];

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

bool validPosition(std::vector<std::vector<uint>>& board, const uint& xCheck, const uint& yCheck, const uint& numToCheck)
{
    //Check row
    for(uint col = 0; col < board.size(); col++)
    {
        if(board[col][yCheck] == numToCheck)
        {
            return false;
        }
    }

    //Check col
    for(uint row = 0; row < board[0].size(); row++)
    {
        if(board[xCheck][row] == numToCheck)
        {
            return false;
        }
    }

    //Check sub grid
    const uint subStartX = xCheck - (xCheck % Settings::SubGridCountX);
    const uint subStartY = yCheck - (yCheck % Settings::SubGridCountY);
    for(uint x = subStartX; x < subStartX + Settings::SubGridCountX; x++)
    {
        for(uint y = subStartY; y < subStartY + Settings::SubGridCountY; y++)
        {
            if(board[x][y] == numToCheck)
            {
                return false;
            }
        }
    }

    return true;
}

bool findNextEmptySpot(const std::vector<std::vector<uint>>& board, uint& x, uint& y)
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

bool fillBoardPossible(std::vector<std::vector<uint>>& board, const uint& x, const uint& y)
{
    for(uint newNum : Settings::SubGridOptions)
    {
        if(validPosition(board, x, y, newNum))
        {
            //Set board[x][y] to testing newNum
            board[x][y] = newNum;

            //Find next free board pos
            uint nextX = x;
            uint nextY = y;
            if(!findNextEmptySpot(board, nextX, nextY))
            {
                return true;
            }

            //Try fill in rest of board with newNum in board pos x,y
            if(fillBoardPossible(board, nextX, nextY))
            {
                return true;
            }

            board[x][y] = 0;
        }
    }

    return false;
}

void DLG_Home::generateBoard()
{
    // - Fill a complete board
    // - Remove all but except Settings::NumberOfCellsToKeepAfterGen in random locations
    // - Set m_board to remaning values

    std::vector<std::vector<uint>> boardToFill = std::vector<std::vector<uint>>(m_board.size(), std::vector<uint>(m_board[0].size(), 0));

    //Diagonally fill centeral subgrids
    randomlyFillSubGrid(boardToFill, 0, 0);
    randomlyFillSubGrid(boardToFill, 3, 3);
    randomlyFillSubGrid(boardToFill, 6, 6);

    //Try fill rest of board
    if(fillBoardPossible(boardToFill, 3, 0))
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
    for(uint i = 0; i < Settings::NumberOfCellsToKeepAfterGen;)
    {
        const uint x = QRandomGenerator::global()->generateDouble() * m_board.size();
        const uint y = QRandomGenerator::global()->generateDouble() * m_board.size();
        const QPoint cellToKeep = QPoint(x,y);
        if(!keptCells.contains(cellToKeep))
        {
            keptCells.push_back(cellToKeep);
            i++;
        }
    }

    //Remove all cells except ones in keptCells
    for(uint x = 0; x < m_board.size(); x++)
    {
        for(uint y = 0; y < m_board[x].size(); y++)
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
                m_board[x][y]->setValue(boardToFill[x][y]);
                m_board[x][y]->setPermanent(true);
            }
        }
    }
}

void DLG_Home::on_btn_reset_clicked()
{
    if(!m_pAiThread->isWorking())
    {
        generateBoard();
    }
}

void DLG_Home::on_btn_ai_clicked()
{
    if(!m_pAiThread->isWorking())
    {
        #ifdef AI_DEBUG

            std::vector<std::vector<uint>> board = {{0,0,0, 0,0,0, 0,8,0},
                                           {4,0,0, 0,0,0, 0,0,7},
                                           {0,8,0, 7,0,0, 0,0,0},

                                           {1,5,0, 6,0,0, 0,0,0},
                                           {0,0,0, 0,9,0, 0,0,0},
                                           {0,0,4, 0,8,0, 0,9,3},

                                           {2,0,0, 0,0,0, 0,0,1},
                                           {9,0,0, 0,0,0, 0,0,0},
                                           {0,6,0, 0,0,1, 9,5,0}};

            for(uint x = 0; x < board.size(); x++)
            {
                for(uint y = 0; y < board[x].size(); y++)
                {
                    m_board[x][y]->setValue(board[x][y]);
                    m_board[x][y]->setPermanent(board[x][y] != 0);
                }
            }

        #else

            std::vector<std::vector<uint>> board = std::vector<std::vector<uint>>(m_board.size(), std::vector<uint>(m_board[0].size(), 0));
            for(uint x = 0; x < m_board.size(); x++)
            {
                for(uint y = 0; y < m_board[x].size(); y++)
                {
                    board[x][y] = m_board[x][y]->value();
                }
            }

        #endif

        m_pAiThread->setBoard(board);
    }
}

void DLG_Home::updateCell(const uint &x, const uint &y, const uint &value)
{
    m_board[x][y]->setValue(value);
}

void DLG_Home::updateBoard(const std::vector<std::vector<uint>>& board)
{
    for(uint x = 0; x < board.size(); x++)
    {
        for(uint y = 0; y < board[x].size(); y++)
        {
            m_board[x][y]->setValue(board[x][y]);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// AiThread::AiThread
///
///
AiThread::AiThread() : QThread(),
    m_bStop(false),
    m_bWorking(false)
{
}

void AiThread::setBoard(const std::vector<std::vector<uint>>& board)
{
    if(!m_bWorking)
    {
        m_board = board;
        m_bWorking = true;
    }
}

void AiThread::setStop()
{
    m_bStop = true;
}

bool AiThread::isWorking()
{
    return m_bWorking;
}

void AiThread::run()
{
    while(true)
    {
        if(m_bStop)
        {
            return;
        }

        if(m_bWorking)
        {
#ifdef AI_DEBUG
    clock_t start = clock();
#endif

            uint x = 0;
            uint y = 0;
            if(findNextEmptySpot(m_board, x, y))
            {
                if(findSolution(m_board, x, y))
                {
                    #ifndef AI_SHOW_PROGRESS
                        emit updateBoard(m_board);
                    #endif
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

            m_bWorking = false;
        }
    }
}

bool AiThread::findSolution(std::vector<std::vector<uint>>& board, const uint x, const uint y)
{
    //Try all possible options
    for(uint newNum : Settings::SubGridOptions)
    {
        if(validPosition(board, x, y, newNum))
        {
            //Set board[x][y] to testing newNum
            board[x][y] = newNum;
            #ifdef AI_SHOW_PROGRESS
                emit updateCell(x, y, newNum);
            #endif

            //Find next free board pos
            m_nextX = x;
            m_nextY = y;
            if(!findNextEmptySpot(board, m_nextX, m_nextY))
            {
                return true;
            }

            //Try fill in rest of board with newNum in board pos x,y
            if(findSolution(board, m_nextX, m_nextY))
            {
                return true;
            }

            board[x][y] = 0;
            #ifdef AI_SHOW_PROGRESS
                emit updateCell(x, y, 0);
            #endif
        }
    }

    return false;
}

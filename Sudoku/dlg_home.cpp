#include "dlg_home.h"
#include "ui_dlg_home.h"
#include "settings.h"
#include "math.h"

#include <QDebug>
#include <QMouseEvent>

DLG_Home::DLG_Home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DLG_Home)
    , m_pSelectedTile(nullptr)
{
    ui->setupUi(this);

    for(int x = 0; x < Settings::BoardCountX; x++)
    {
        m_board.push_back(QVector<Tile*>());
        for(int y = 0; y < Settings::BoardCountY; y++)
        {
            m_board[x].push_back(new Tile(this, Settings::BoardRect.left() + x * Settings::TileSize, Settings::BoardRect.top() + y * Settings::TileSize, Settings::TileSize, Settings::TileSize));
        }
    }

    m_board[0][1]->setValue(5);
    m_board[0][1]->setPermanent(true);

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

void fillSubGrid(const std::vector<int> subGridOptions, QVector<QVector<Tile*>>& board, const int& startX, const int& startY)
{
    int x = startX;
    int y = startY;

    std::vector<int> randomSubGrid = subGridOptions;
    std::random_shuffle(randomSubGrid.begin(), randomSubGrid.end());

    for(size_t i = 0; i < randomSubGrid.size(); i++)
    {
        board[x][y]->setValue(randomSubGrid[i]);

        if((i+1) % 3 == 0)
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

void DLG_Home::generateBoard()
{
    const std::vector<int> subGridOptions = {1,2,3,4,5,6,7,8,9};

    fillSubGrid(subGridOptions, m_board, 0, 0);
    fillSubGrid(subGridOptions, m_board, 3, 3);
    fillSubGrid(subGridOptions, m_board, 6, 6);
}


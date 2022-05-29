#include "dlg_home.h"
#include "ui_dlg_home.h"
#include "settings.h"
#include "math.h"

#include <QDebug>
#include <QMouseEvent>

DLG_Home::DLG_Home(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DLG_Home)
    , m_selectedTile(Settings::UnselectedTile)
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

    m_board[0][1]->setValue(5, true);
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
    if(m_selectedTile != Settings::UnselectedTile)
    {
        m_board[m_selectedTile.x()][m_selectedTile.y()]->setSelected(false);
    }

    if(Settings::BoardRect.contains(mouseEvent->pos()))
    {
        const int xOffset = mouseEvent->pos().x() - Settings::BoardRect.x();
        const int yOffset = mouseEvent->pos().y() - Settings::BoardRect.y();
        const int x = floor(xOffset/Settings::TileSize);
        const int y = floor(yOffset/Settings::TileSize);

        m_selectedTile.setX(x);
        m_selectedTile.setY(y);

        m_board[x][y]->setSelected(true);
    }
}

void DLG_Home::keyPressEvent(QKeyEvent *event)
{
    if(m_selectedTile != Settings::UnselectedTile)
    {
        if(event->key() >= Qt::Key::Key_0 && event->key() <= Qt::Key::Key_9)
        {
            setValue(m_selectedTile, event->key() - 48);
        }
        else if(event->key() == Qt::Key::Key_Backspace || event->key() == Qt::Key::Key_Delete)
        {
            setValue(m_selectedTile, 0);
        }
    }
}

void DLG_Home::setValue(const QPoint& location, const int& value)
{
    m_board[location.x()][location.y()]->setValue(value);
}


#include "tile.h"
#include "settings.h"

#include <QPainter>

Tile::Tile(QWidget* parent, const int& x, const int& y, const int& w, const int& h) : QWidget(parent),
    m_value(0),
    m_bPermanent(false),
    m_bSelected(false)
{
    setGeometry(x, y, w, h);
}

void Tile::setValue(int value, bool permanent)
{
    m_value = value;
    m_bPermanent = permanent;
    update();
}

int Tile::value()
{
    return m_value;
}

void Tile::setSelected(bool selected)
{
    m_bSelected = selected;
    update();
}

void Tile::reset()
{
    m_value = 0;
    update();
}

void Tile::paintEvent(QPaintEvent*)
{
    if(m_value != 0 || m_bSelected)
    {
        QPainter painter(this);
        painter.setPen(m_bPermanent ? Settings::TileTextColorPermanent : Settings::TileTextColorEditable);

        if(m_value != 0)
        {
            //Prep value text drawing
            painter.setFont(Settings::TileTextFont);
            const float textWidth = Settings::TileTextFontMetrics.horizontalAdvance(m_value);

            //Draw value text
            painter.drawText(QPoint(Settings::TileSize/2 - textWidth/2, Settings::TileSize/2 + Settings::TileTextFontMetrics.height()/4), QString::number(m_value));
        }

        if(m_bSelected)
        {
            painter.drawRect(QRect(0, 0, Settings::TileSize-1, Settings::TileSize-1));
        }
    }
}

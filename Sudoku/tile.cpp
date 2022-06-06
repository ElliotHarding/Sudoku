#include "tile.h"
#include "settings.h"
#include "dlg_home.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

Tile::Tile(DLG_Home* parent, const uint& x, const uint& y, const uint& w, const uint& h) : QWidget(parent),
    m_value(0),
    m_bPermanent(false),
    m_bSelected(false),
    m_pHome(parent)
{
    setGeometry(x, y, w, h);
}

void Tile::setValue(uint value)
{
    m_value = value;
    update();
}

uint Tile::value()
{
    return m_value;
}

void Tile::setPermanent(bool permanent)
{
    m_bPermanent = permanent;
    update();
}

bool Tile::isPermanent()
{
    return m_bPermanent;
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

void Tile::mousePressEvent(QMouseEvent* mouseEvent)
{
    m_bSelected = true;
    m_pHome->setSelectedTile(this);
    update();
}

void Tile::paintEvent(QPaintEvent*)
{
    if(m_value != 0 || m_bSelected)
    {
        QPainter pauinter(this);
        pauinter.setPen(m_bPermanent ? Settings::TileTextColorPermanent : Settings::TileTextColorEditable);

        if(m_value != 0)
        {
            //Prep value text drawing
            pauinter.setFont(Settings::TileTextFont);
            const float textWidth = Settings::TileTextFontMetrics.horizontalAdvance(QString::number(m_value));

            //Draw value text
            pauinter.drawText(QPointF(Settings::TileSize/2 - textWidth/2, Settings::TileSize/2 + Settings::TileTextFontMetrics.height()/4), QString::number(m_value));
        }

        if(m_bSelected)
        {
            pauinter.drawRect(QRect(1, 1, Settings::TileSize-2, Settings::TileSize-2));
        }
    }
}

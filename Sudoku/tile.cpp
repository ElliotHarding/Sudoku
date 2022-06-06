#include "tile.h"
#include "settings.h"
#include "dlg_home.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Tile::Tile
///
Tile::Tile(DLG_Home* parent, const uint& x, const uint& y, const uint& w, const uint& h) : BaseTile(parent, parent, x, y, w, h),
    m_pPotentialTile(new PotentialTile(parent, this, Settings::TileSize - Settings::PotentialTileSize - Settings::PotentialTilePadding, Settings::PotentialTilePadding, Settings::PotentialTileSize, Settings::PotentialTileSize))
{
}

void Tile::reset()
{
    m_value = 0;
    m_pPotentialTile->reset();
    update();
}

void Tile::resetPotentialTile()
{
    m_pPotentialTile->reset();
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
            const float textWidth = Settings::TileTextFontMetrics.horizontalAdvance(QString::number(m_value));

            //Draw value text
            painter.drawText(QPointF(geometry().width()/2 - textWidth/2, geometry().height()/2 + Settings::TileTextFontMetrics.height()/4), QString::number(m_value));
        }

        if(m_bSelected)
        {
            painter.drawRect(QRect(1, 1, geometry().width()-2, geometry().height()-2));
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BaseTile::BaseTile
///
BaseTile::BaseTile(DLG_Home* pHome, QWidget* pParent, const uint &x, const uint &y, const uint &w, const uint &h) : QWidget(pParent),
    m_value(0),
    m_bSelected(false),
    m_bPermanent(false),
    m_pHome(pHome)
{
    setGeometry(x, y, w, h);
}

void BaseTile::setValue(uint value)
{
    m_value = value;
    update();
}

uint BaseTile::value()
{
    return m_value;
}

void BaseTile::setPermanent(bool permanent)
{
    m_bPermanent = permanent;
    update();
}

bool BaseTile::isPermanent()
{
    return m_bPermanent;
}

void BaseTile::setSelected(bool selected)
{
    m_bSelected = selected;
    update();
}

void BaseTile::reset()
{
    m_value = 0;
    update();
}

void BaseTile::mousePressEvent(QMouseEvent*)
{
    m_bSelected = true;
    m_pHome->setSelectedTile(this);
    update();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PotentialTile::PotentialTile
///
PotentialTile::PotentialTile(DLG_Home *pHome, Tile *pParent, const uint &x, const uint &y, const uint &w, const uint &h) : BaseTile(pHome, pParent, x, y, w, h)
{
}

void PotentialTile::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.fillRect(QRect(1, 1, geometry().width()-2, geometry().height()-2), Settings::PotentialTileColor);

    if(m_value != 0 || m_bSelected)
    {
        painter.setPen(Settings::TileTextColorEditable);
        if(m_value != 0)
        {
            //Prep value text drawing
            painter.setFont(Settings::PotentialTileTextFont);
            const float textWidth = Settings::PotentialTileTextFontMetrics.horizontalAdvance(QString::number(m_value));

            //Draw value text
            painter.drawText(QPointF(geometry().width()/2 - textWidth/2, geometry().height()/2 + Settings::PotentialTileTextFontMetrics.height()/3), QString::number(m_value));
        }

        if(m_bSelected)
        {
            painter.drawRect(QRect(1, 1, geometry().width()-2, geometry().height()-2));
        }
    }
}

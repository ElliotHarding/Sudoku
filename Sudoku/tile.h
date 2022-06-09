#ifndef TILE_H
#define TILE_H

#include <QWidget>

class DLG_Home;
class Tile;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// BaseTile
///
class BaseTile : public QWidget
{
    Q_OBJECT
public:
    BaseTile(DLG_Home* pHome, QWidget* pParent, const uint& x, const uint& y, const uint& w, const uint& h);

    void setValue(uint value);
    uint value();

    void setPermanent(bool permanent);
    bool isPermanent();

    void setSelected(bool selected);

    void reset();

protected:
    void mousePressEvent(QMouseEvent* mouseEvent) override;

    uint m_value;
    bool m_bSelected;
    bool m_bPermanent;

private:
    DLG_Home* m_pHome;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PotentialTile
///
class PotentialTile : public BaseTile
{
    Q_OBJECT
public:
    PotentialTile(DLG_Home* pHome, Tile* pParent, const uint& x, const uint& y, const uint& w, const uint& h);

protected:
    void paintEvent(QPaintEvent* paintEvent) override;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Tile
///
class Tile : public BaseTile
{
    Q_OBJECT
public:
    Tile(DLG_Home* parent, const uint& x, const uint& y, const uint& w, const uint& h);

    void reset();
    void resetPotentialTile();

protected:
    void paintEvent(QPaintEvent* paintEvent) override;

private:
    PotentialTile* m_pPotentialTile;
};

#endif // TILE_H

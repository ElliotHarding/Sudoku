#ifndef TILE_H
#define TILE_H

#include <QWidget>

class DLG_Home;

class Tile : QWidget
{
    Q_OBJECT
public:
    Tile(DLG_Home* parent, const uint& x, const uint& y, const uint& w, const uint& h);

    void setValue(uint value);
    uint value();

    void setPermanent(bool permanent);
    bool isPermanent();

    void setSelected(bool selected);

    void reset();

protected:
    void mousePressEvent(QMouseEvent* mouseEvent) override;
    void paintEvent(QPaintEvent* paintEvent) override;

private:
    uint m_value;
    bool m_bPermanent;
    bool m_bSelected;

    DLG_Home* m_pHome;
};

#endif // TILE_H

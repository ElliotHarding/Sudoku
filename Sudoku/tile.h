#ifndef TILE_H
#define TILE_H

#include <QWidget>

class Tile : QWidget
{
    Q_OBJECT
public:
    Tile(QWidget* parent, const uint& x, const uint& y, const uint& w, const uint& h);

    void setValue(uint value);
    uint value();

    void setPermanent(bool permanent);
    bool isPermanent();

    void setSelected(bool selected);

    void reset();

protected:
    void paintEvent(QPaintEvent* paintEvent) override;

private:
    uint m_value;
    bool m_bPermanent;
    bool m_bSelected;
};

#endif // TILE_H

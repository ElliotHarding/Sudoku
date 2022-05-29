#ifndef TILE_H
#define TILE_H

#include <QWidget>

class Tile : QWidget
{
    Q_OBJECT
public:
    Tile(QWidget* parent, const int& x, const int& y, const int& w, const int& h);

    void setValue(int value);
    int value();

    void setPermanent(bool permanent);
    bool isPermanent();

    void setSelected(bool selected);

    void reset();

protected:
    void paintEvent(QPaintEvent* paintEvent) override;

private:
    int m_value;
    bool m_bPermanent;
    bool m_bSelected;
};

#endif // TILE_H

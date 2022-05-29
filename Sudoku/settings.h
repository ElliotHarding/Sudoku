#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColor>
#include <QFontMetrics>

namespace Settings
{
///Tile size
const int TileSize = 50;

///Tile text
const QColor TileTextColorPermanent = Qt::black;
const QColor TileTextColorEditable = Qt::blue;
const QFont TileTextFont = QFont("Helvetica [Cronyx]", 10, QFont::Normal);
const QFontMetrics TileTextFontMetrics(TileTextFont);

///Board
const int BoardCountX = 9;
const int BoardCountY = 9;
const QRect BoardRect(0, 0, TileSize * BoardCountX, TileSize * BoardCountY);
}

#endif // SETTINGS_H

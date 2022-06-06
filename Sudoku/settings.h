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
const int SubGridCountX = 3;
const int SubGridCountY = 3;
const QRect BoardRect(0, 0, TileSize * BoardCountX, TileSize * BoardCountY);

///Board gen
const std::vector<int> SubGridOptions = {1,2,3,4,5,6,7,8,9};
const int NumberOfCellsToKeepAfterGen = 20;

}

#define AI_DEBUG

#define AI_SHOW_PROGRESS

#endif // SETTINGS_H

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColor>
#include <QFontMetrics>

namespace Settings
{
///Tile size
const uint TileSize = 50;

///Tile color
const QColor PotentialTileColor = Qt::gray;

///Tile text
const QColor TileTextColorPermanent = Qt::black;
const QColor TileTextColorEditable = Qt::blue;
const QFont TileTextFont = QFont("Helvetica [Cronyx]", 15, QFont::Normal);
const QFont PotentialTileTextFont = QFont("Helvetica [Cronyx]", 6, QFont::Normal);
const QFontMetrics TileTextFontMetrics(TileTextFont);
const QFontMetrics PotentialTileTextFontMetrics(PotentialTileTextFont);

///Board
const uint BoardCountX = 9;
const uint BoardCountY = 9;
const uint SubGridCountX = 3;
const uint SubGridCountY = 3;
const QRect BoardRect(0, 0, TileSize * BoardCountX, TileSize * BoardCountY);

///Board gen
const std::vector<uint> SubGridOptions = {1,2,3,4,5,6,7,8,9};
const uint NumberOfCellsToKeepAfterGen = 20;

}

//#define AI_DEBUG

//#define AI_SHOW_PROGRESS

#endif // SETTINGS_H

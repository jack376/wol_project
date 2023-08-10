#include "stdafx.h"
#include "TileInfoTable.h"
#include "rapidcsv.h"

bool TileInfoTable::Load()
{
    rapidcsv::Document doc("tables/TileInfoTable.csv");

    std::vector<std::string> tileName          = doc.GetColumn<std::string>(0);
    std::vector<int>         tileIndexX        = doc.GetColumn<int>(1);
    std::vector<int>         tileIndexY        = doc.GetColumn<int>(2);
    std::vector<int>         tileType          = doc.GetColumn<int>(3);
    std::vector<int>         tileSize          = doc.GetColumn<int>(4);
    std::vector<int>         tileScaleFactor   = doc.GetColumn<int>(5);
    std::vector<int>         tileLayer         = doc.GetColumn<int>(6);

    std::vector<std::string> textureId         = doc.GetColumn<std::string>(7);
    std::vector<int>         textureRectLeft   = doc.GetColumn<int>(8);
    std::vector<int>         textureRectTop    = doc.GetColumn<int>(9);
    std::vector<int>         textureRectWidth  = doc.GetColumn<int>(10);
    std::vector<int>         textureRectHeight = doc.GetColumn<int>(11);

    for (int i = 0; i < tileName.size(); ++i)
    {
        TileInfo tileInfo;

        tileInfo.tileName          = tileName[i];
        tileInfo.tileIndexX        = tileIndexX[i];
        tileInfo.tileIndexY        = tileIndexY[i];
        tileInfo.tileType          = tileType[i];
        tileInfo.tileSize          = tileSize[i];
        tileInfo.tileScaleFactor   = tileScaleFactor[i];
        tileInfo.tileLayer         = tileLayer[i];

        tileInfo.textureId         = textureId[i];
        tileInfo.textureRectLeft   = textureRectLeft[i];
        tileInfo.textureRectTop    = textureRectTop[i];
        tileInfo.textureRectWidth  = textureRectWidth[i];
        tileInfo.textureRectHeight = textureRectHeight[i];

        table[tileName[i]] = tileInfo;
    }
    return true;
}

void TileInfoTable::Release()
{
    table.clear();
}

bool TileInfoTable::Save(const std::string& path)
{
    rapidcsv::Document doc;

    std::vector<std::string> tileName;
    std::vector<int> tileIndexX;
    std::vector<int> tileIndexY;
    std::vector<int> tileType;
    std::vector<int> tileSize;
    std::vector<int> tileScaleFactor;
    std::vector<int> tileLayer;

    std::vector<std::string> textureId;
    std::vector<int> textureRectLeft;
    std::vector<int> textureRectTop;
    std::vector<int> textureRectWidth;
    std::vector<int> textureRectHeight;

    for (const auto& pair : table)
    {
        const TileInfo& tileInfo = pair.second;

        tileName.push_back(tileInfo.tileName);
        tileIndexX.push_back(tileInfo.tileIndexX);
        tileIndexY.push_back(tileInfo.tileIndexY);
        tileType.push_back(tileInfo.tileType);
        tileSize.push_back(tileInfo.tileSize);
        tileScaleFactor.push_back(tileInfo.tileScaleFactor);
        tileLayer.push_back(tileInfo.tileLayer);

        textureId.push_back(tileInfo.textureId);
        textureRectLeft.push_back(tileInfo.textureRectLeft);
        textureRectTop.push_back(tileInfo.textureRectTop);
        textureRectWidth.push_back(tileInfo.textureRectWidth);
        textureRectHeight.push_back(tileInfo.textureRectHeight);
    }

    doc.SetColumn(0,  tileName);
    doc.SetColumn(1,  tileIndexX);
    doc.SetColumn(2,  tileIndexY);
    doc.SetColumn(3,  tileType);
    doc.SetColumn(4,  tileSize);
    doc.SetColumn(5,  tileScaleFactor);
    doc.SetColumn(6,  tileLayer);

    doc.SetColumn(7,  textureId);
    doc.SetColumn(8,  textureRectLeft);
    doc.SetColumn(9,  textureRectTop);
    doc.SetColumn(10, textureRectWidth);
    doc.SetColumn(11, textureRectHeight);

    try
    {
        doc.Save(path);
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "ERROR : Not Save CSV: " << e.what() << std::endl;
        return false;
    }
}

const TileInfo& TileInfoTable::Get(const std::string& tileName)
{
    auto find = table.find(tileName);
    if (find == table.end())
    {
        throw std::runtime_error("ERROR : Undefined Tile Index");
    }
    return find->second;
}

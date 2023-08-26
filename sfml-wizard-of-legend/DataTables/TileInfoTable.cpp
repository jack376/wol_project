#include "stdafx.h"
#include "TileInfoTable.h"
#include "rapidcsv.h"

bool TileInfoTable::Load()
{
    rapidcsv::Document doc("tables/BossRoom_0822_184837_fix.csv");

    std::vector<std::string> tileName = doc.GetColumn<std::string>(0);

    std::vector<int> tileIndexX = doc.GetColumn<int>(1);
    std::vector<int> tileIndexY = doc.GetColumn<int>(2);
    std::vector<int> tileType   = doc.GetColumn<int>(3);
    std::vector<int> tileSize   = doc.GetColumn<int>(4);
    std::vector<int> tileScale  = doc.GetColumn<int>(5);
    std::vector<int> tileLayer  = doc.GetColumn<int>(6);

    std::vector<std::string> textureId = doc.GetColumn<std::string>(7);

    std::vector<int> topTextureRectL    = doc.GetColumn<int>(8);
    std::vector<int> topTextureRectT    = doc.GetColumn<int>(9);
    std::vector<int> bottomTextureRectL = doc.GetColumn<int>(10);
    std::vector<int> bottomTextureRectT = doc.GetColumn<int>(11);

    std::vector<int> spawnLocation = doc.GetColumn<int>(12);

    for (int i = 0; i < tileName.size(); ++i)
    {
        TileInfo tileInfo;

        tileInfo.tileName = tileName[i];

        tileInfo.tileIndexX = tileIndexX[i];
        tileInfo.tileIndexY = tileIndexY[i];
        tileInfo.tileType   = tileType[i];
        tileInfo.tileSize   = tileSize[i];
        tileInfo.tileScale  = tileScale[i];
        tileInfo.tileLayer  = tileLayer[i];

        tileInfo.textureId = textureId[i];

        tileInfo.topTextureRectL    = topTextureRectL[i];
        tileInfo.topTextureRectT    = topTextureRectT[i];
        tileInfo.bottomTextureRectL = bottomTextureRectL[i];
        tileInfo.bottomTextureRectT = bottomTextureRectT[i];
        
        tileInfo.spawnLocation = spawnLocation[i];

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
    std::vector<int> tileScale;
    std::vector<int> tileLayer;

    std::vector<std::string> textureId;

    std::vector<int> topTextureRectL;
    std::vector<int> topTextureRectT;
    std::vector<int> bottomTextureRectL;
    std::vector<int> bottomTextureRectT;

    std::vector<int> spawnLocation;

    for (const auto& pair : table)
    {
        const TileInfo& tileInfo = pair.second;

        tileName.push_back(tileInfo.tileName);

        tileIndexX.push_back(tileInfo.tileIndexX);
        tileIndexY.push_back(tileInfo.tileIndexY);
        tileType.push_back(tileInfo.tileType);
        tileSize.push_back(tileInfo.tileSize);
        tileScale.push_back(tileInfo.tileScale);
        tileLayer.push_back(tileInfo.tileLayer);

        textureId.push_back(tileInfo.textureId);

        topTextureRectL.push_back(tileInfo.topTextureRectL);
        topTextureRectT.push_back(tileInfo.topTextureRectT);
        bottomTextureRectL.push_back(tileInfo.bottomTextureRectL);
        bottomTextureRectT.push_back(tileInfo.bottomTextureRectT);

        spawnLocation.push_back(tileInfo.spawnLocation);
    }

    doc.SetColumn(0,  tileName);

    doc.SetColumn(1,  tileIndexX);
    doc.SetColumn(2,  tileIndexY);
    doc.SetColumn(3,  tileType);
    doc.SetColumn(4,  tileSize);
    doc.SetColumn(5,  tileScale);
    doc.SetColumn(6,  tileLayer);

    doc.SetColumn(7,  textureId);

    doc.SetColumn(8,  topTextureRectL);
    doc.SetColumn(9,  topTextureRectT);
    doc.SetColumn(10, bottomTextureRectL);
    doc.SetColumn(11, bottomTextureRectT);

    doc.SetColumn(12, spawnLocation);

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

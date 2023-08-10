#pragma once
#include "DataTable.h"

struct TileInfo
{
    std::string tileName;
    int tileIndexX;
    int tileIndexY;
    int tileType;
    int tileSize;
    int tileScaleFactor;
    int tileLayer;

    std::string textureId;
    int textureRectLeft;
    int textureRectTop;
    int textureRectWidth;
    int textureRectHeight;
};

class TileInfoTable : public DataTable
{
protected:
    std::unordered_map<std::string, TileInfo> table;

public:
    TileInfoTable() : DataTable(DataTable::Ids::Tile) {};
    virtual ~TileInfoTable() override = default;

    virtual bool Load() override;
    virtual void Release() override;

    bool Save(const std::string& path);

    const TileInfo& Get(const std::string& tileName);
};
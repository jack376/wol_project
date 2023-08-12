#pragma once
#include "DataTable.h"

struct TileInfo
{
    std::string tileName;

    int tileIndexX;
    int tileIndexY;
    int tileType;
    int tileSize;
    int tileScale;
    int tileLayer;

    std::string textureId;

    int topTextureRectL;
    int topTextureRectT;
    int bottomTextureRectL;
    int bottomTextureRectT;
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
#pragma once
#include "Command.h"
#include "Tile.h"

class Tile;
class TileCommand : public Command
{
public:
    struct TileState
    {
        Tile::TileType type;
        sf::Vector2i index;
        std::string textureId;
        sf::IntRect textureRect;
        int size;
        int layer;
    };

protected:
    TileState beforeState;
    TileState afterState;
    Tile* targetTile;

public:
    TileCommand(Tile* tile, const TileState& before, const TileState& after)
        : targetTile(tile), beforeState(before), afterState(after) {}

    void execute() override;
    void undo() override;

    void applyState(Tile* tile, const TileState& state);
};


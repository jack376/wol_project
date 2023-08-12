#pragma once
#include "Command.h"
#include "Tile.h"

class Tile;
class TileCommand : public Command
{
public:
    struct TileState
    {
        std::string textureId;
        Tile::TileType type;
        sf::Vector2i index;
        sf::IntRect topRect;
        sf::IntRect bottomRect;
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


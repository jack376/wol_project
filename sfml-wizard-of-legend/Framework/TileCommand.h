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
        TileType type;
        sf::Vector2i index;
        sf::IntRect topRect;
        sf::IntRect bottomRect;
        int size;
        int layer;
    };

protected:
    Tile* targetTile;

    TileState beforeState;
    TileState afterState;

public:
    TileCommand(Tile* tile, const TileState& before, const TileState& after)
        : targetTile(tile), beforeState(before), afterState(after) {}

    void Execute() override;
    void Undo() override;

    void ApplyState(Tile* tile, const TileState& state);
};


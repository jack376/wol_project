#include "stdafx.h"
#include "TileCommand.h"

void TileCommand::execute()
{
    applyState(targetTile, afterState);
}

void TileCommand::undo()
{
    applyState(targetTile, beforeState);
}

void TileCommand::applyState(Tile* tile, const TileState& state)
{
    tile->SetTexture(state.textureId);
    tile->SetType(state.type);
    tile->SetIndex(state.index.x, state.index.y);
    tile->SetTextureRect(state.textureRect, state.textureId);
    tile->SetTileSize(state.size);
    tile->SetLayer(state.layer);
}
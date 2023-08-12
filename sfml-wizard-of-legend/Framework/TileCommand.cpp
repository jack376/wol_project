#include "stdafx.h"
#include "TileCommand.h"

void TileCommand::Execute()
{
    ApplyState(targetTile, afterState);
}

void TileCommand::Undo()
{
    ApplyState(targetTile, beforeState);
}

void TileCommand::ApplyState(Tile* tile, const TileState& state)
{
    tile->SetTexture(state.textureId);
    tile->SetType(state.type);
    tile->SetIndex(state.index.x, state.index.y);
    tile->SetTextureRectTop(state.topRect, state.textureId);
    tile->SetTextureRectBottom(state.bottomRect, state.textureId);
    tile->SetTileSize(state.size);
    tile->SetLayer(state.layer);
}
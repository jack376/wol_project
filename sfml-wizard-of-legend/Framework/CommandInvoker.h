#pragma once
#include "Command.h"

class CommandInvoker
{
protected:
    std::map<int, std::vector<std::unique_ptr<Command>>> commandStorage;

    std::stack<int> undoStack;
    std::stack<int> redoStack;

public:
    void Execute(std::vector<std::unique_ptr<Command>> commands, int commandId);
    void Undo();
    void Redo();
};
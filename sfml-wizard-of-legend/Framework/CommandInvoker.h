#pragma once
#include "Command.h"

class CommandInvoker
{
protected:
    std::stack<std::unique_ptr<Command>> undoStack;
    std::stack<std::unique_ptr<Command>> redoStack;

public:
    void execute(std::unique_ptr<Command> command);
    void undo();
    void redo();
};
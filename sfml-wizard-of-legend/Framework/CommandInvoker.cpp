#include "stdafx.h"
#include "CommandInvoker.h"

void CommandInvoker::execute(std::unique_ptr<Command> command)
{
    command->execute();
    undoStack.push(std::move(command));
    while (!redoStack.empty())
    {
        redoStack.pop();
    }
}

void CommandInvoker::undo()
{
    if (!undoStack.empty())
    {
        undoStack.top()->undo();
        redoStack.push(std::move(undoStack.top()));
        undoStack.pop();
    }
}

void CommandInvoker::redo()
{
    if (!redoStack.empty())
    {
        redoStack.top()->execute();
        undoStack.push(std::move(redoStack.top()));
        redoStack.pop();
    }
}

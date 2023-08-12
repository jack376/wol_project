#include "stdafx.h"
#include "CommandInvoker.h"

void CommandInvoker::Execute(std::vector<std::unique_ptr<Command>> commands, int commandId)
{
    for (auto& command : commands)
    {
        command->Execute();
        commandStorage[commandId].push_back(std::move(command));
    }
    undoStack.push(commandId);

    while (!redoStack.empty())
    {
        redoStack.pop();
    }
}

void CommandInvoker::Undo()
{
    if (!undoStack.empty())
    {
        int commandId = undoStack.top();
        for (auto& command : commandStorage[commandId]) 
        {
            command->Undo();
        }
        //std::cout << "undo ID : " << commandId << std::endl;
        //std::cout << "undoStack ID : " << undoStack.top() << std::endl;

        redoStack.push(commandId);
        undoStack.pop();
    }
}

void CommandInvoker::Redo()
{
    if (!redoStack.empty())
    {
        int commandId = redoStack.top();
        for (auto& command : commandStorage[commandId]) 
        {
            command->Execute();
        }
        //std::cout << "redo ID : " << commandId << std::endl;
        //std::cout << "redoStack ID : " << redoStack.top() << std::endl;

        undoStack.push(commandId);
        redoStack.pop();
    }
}

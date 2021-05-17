#include "KeyEvtHandler.h"

using namespace std;

void KeyHandler :: refresh()
{
    document->updatePage();
    text = document->getPage();
    showPage();
}

void QuitHandler::Handle(int key)
{
    if(key == CTRL_Q)
    {
        document->saveFile();
    }
}

void EnterHandler::Handle(int key)
{
    int row = view->GetCursorY();
    int pos = view->GetCursorX();
    if(key == ENTER)
    {
        if(document->mode == Edit)
        {
            document->keyAccept(row, pos, key);
            document->move(row + 1, 0, key);
            view->SetCursorX(document->getCursorX());
            view->SetCursorY(document->getCursorY());
        }
        else if(document->mode == Search)
        {
            if(document->searchString.size() != 0)
            {
                document->search(document->searchString);
                view->ClearColor();
                for(int i = 0; i < document->getSpos().size(); i++)
                {
                    view->SetColor(document->getSpos()[i][0], document->getSpos()[i][1], document->getSpos()[i][1] + document->searchString.size() - 1, TEXT_COLOR_RED);
                }
            }
        }
        else if(document->mode == Replace)
        {
            if(document->searchString.size() != 0)
            {
                if(document->replaceString.size() != 0)
                {
                    view->ClearColor();
                    document->replace(document->searchString, document->replaceString);
                    document->move(view->GetCursorY(), view->GetCursorX(), CTRL_Z);
                    view->SetCursorX(document->getCursorX());
                    view->SetCursorY(document->getCursorY());
                    for(int i = 0; i < document->getSpos().size(); i++)
                    {
                        view->SetColor(document->getSpos()[i][0], document->getSpos()[i][1], document->getSpos()[i][1] + document->replaceString.size() - 1, TEXT_COLOR_CYAN);
                    }
                    document->searchString = document->replaceString;
                }
                document->mode = Search;
                document->replaceString = "";
                view->ClearStatusRows();
                view->AddStatusRow("Find: " + document->searchString, "SEARCH MDOE", true);
            }
        }
        refresh();
    }
    else
    {
        (*this).KeyHandler::Handle(key);
    }
}

void UndoHandler :: Handle(int key)
{
    int row = view->GetCursorY();
    int pos = view->GetCursorX();
    if(key == CTRL_Z)
    {
        if(document->mode == Edit)
        {
            document->keyAccept(row, pos, key);
            document->move(row, pos, CTRL_Z);
            view->SetCursorX(document->getCursorX());
            view->SetCursorY(document->getCursorY());
        }
        refresh();
    }
    else
    {
        (*this).KeyHandler::Handle(key);
    }
}

void RedoHandler :: Handle(int key)
{
    int row = view->GetCursorY();
    int pos = view->GetCursorX();
    if(key == CTRL_Y)
    {
        if(document->mode == Edit)
        {
            document->keyAccept(row, pos, key);
            document->move(row, pos, CTRL_Y);
            view->SetCursorX(document->getCursorX());
            view->SetCursorY(document->getCursorY());                
        }
        refresh();
    }
    else
    {
        (*this).KeyHandler::Handle(key);
    }
}

void DeleteHandler :: Handle(int key)
{
    int row = view->GetCursorY();
    int pos = view->GetCursorX();
    if (key == BACKSPACE)
    {
        if(document->mode == Edit)
        {
            document->keyAccept(row, pos, key);
            document->move(document->getCursorY(), document->getCursorX(), key);
            view->SetCursorX(document->getCursorX());
            view->SetCursorY(document->getCursorY());
        }
        else if(document->mode == Search)
        {
            if(document->searchString.length() != 0)
            {
                document->searchString.pop_back();
            }
            view->ClearStatusRows();
            view->AddStatusRow("Find: " + document->searchString, "SEARCH MDOE", true);
        }
        else if(document->mode == Replace)
        {
            if(document->replaceString.length() != 0)
            {
                document->replaceString.pop_back();
            }
            view->ClearStatusRows();
            view->AddStatusRow(document->replaceString, "REPLACE MDOE", true);
        }
        refresh();
    }
    else
    {
        (*this).KeyHandler::Handle(key);
    }
}

void MoveHandler :: Handle(int key)
{
    int row = view->GetCursorY();
    int pos = view->GetCursorX();
    if(key == ARROW_RIGHT)
    {
        if(document->mode == Edit)
        {
            document->move(row, pos + 1, key);
            view->SetCursorX(document->getCursorX());
            view->SetCursorY(document->getCursorY());
        }
        refresh();
    }
    else if(key == ARROW_LEFT)
    {
        if(document->mode == Edit)
        {
            document->move(row, pos - 1, key);
            view->SetCursorX(document->getCursorX());
            view->SetCursorY(document->getCursorY());
        }
        refresh();
    }
    else if(key == ARROW_UP)
    {
        if(document->mode == Edit)
        {
            document->move(row - 1, pos, key);
            view->SetCursorX(document->getCursorX());
            view->SetCursorY(document->getCursorY());
        }
        refresh();
    }
    else if(key == ARROW_DOWN)
    {
        if(document->mode == Edit)
        {
            document->move(row + 1, pos, key);
            view->SetCursorX(document->getCursorX());
            view->SetCursorY(document->getCursorY());
        }
        refresh();
    }
    else
    {
        (*this).KeyHandler::Handle(key);
    }
}

void InsertHandler :: Handle(int key)
{
    int row = view->GetCursorY();
    int pos = view->GetCursorX();
    if(key > 31 && key < 127 && key != 47)
    {
        if(document->mode == Edit)
        {
            document->keyAccept(row, pos, key);
            document->move(row, pos + 1, key);
            view->SetCursorX(document->getCursorX());
            view->SetCursorY(document->getCursorY());
        }
        else if(document->mode == Search)
        {
            document->searchString.append(1, key);
            view->ClearStatusRows();
            view->AddStatusRow("Find: " + document->searchString, "SEARCH MDOE", true);
        }
        else if(document->mode == Replace)
        {
            document->replaceString.append(1, key);
            view->ClearStatusRows();
            view->AddStatusRow("Replace: " + document->searchString + ": "+ document->replaceString, "REPLACE MDOE", true);
        }
        refresh();
    }
    else
    {
        (*this).KeyHandler::Handle(key);
    }
}

void FAPHandler ::Handle(int key)
{
    if(key == CTRL_F)
    {
        if(document->mode == Edit)
        {
            document->mode = Search;
            document->searchString = "";
            view->ClearStatusRows();
            view->AddStatusRow("Find: " + document->searchString, "SEARCH MDOE", true);
        }
        refresh();
    }
    else if(key == ESC)
    {
        if(document->mode == Search)
        {
            document->mode = Edit;
            document->searchString = "";
            view->ClearColor();
            view->ClearStatusRows();
            view->AddStatusRow("", "EDIT MDOE", true);
        }
        else if(document->mode == Replace)
        {
            document->mode = Search;
            document->replaceString = "";
            view->ClearColor();
            view->ClearStatusRows();
            view->AddStatusRow("Find: " + document->searchString, "SEARCH MODE", true);
        }
        refresh();
    }
    else if(key == 47)
    {
        if(document->mode == Search)
        {
            document->mode = Replace;
            view->ClearStatusRows();
            view->AddStatusRow("Replace: " + document->searchString + ": "+ document->replaceString, "REPLACE MODE", true);
        }
        refresh();
    }
    else
    {
        (*this).KeyHandler::Handle(key);
    }
}

void KeyHandler :: showPage()
{
    view->InitRows();
    int rowNum = document->getRowNum();
    for(int i = 0; i < rowNum; i++)
    {
        view->AddRow((*text)[i]);
    }
}
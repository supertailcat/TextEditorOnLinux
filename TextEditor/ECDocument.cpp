#include "ECDocument.h"
#include <iostream>
#include <fstream>

using namespace std;

ECDocument::ECDocument()
{
    text = new vector<string>();
    curPage = new vector<string>();
    cursor = -1;
};

void ECDocument::readFile(char* name)
{
    filename = name;
    ifstream infile(filename);
    if(!infile)
    {
        (*text).emplace((*text).begin(), string());
    }
    string str;
    while(getline(infile, str))
    {
        (*text).push_back(str);
    }
    infile.close();
    makeHistory();
}

void ECDocument::saveFile()
{
    if(filename == nullptr)
    {
        time_t timep;
        time(&timep);
        filename = asctime(gmtime(&timep));
    }
    ofstream outfile(filename);
    for(int i = 0; i < (*text).size(); i++)
    {
        outfile << (*text)[i] << endl;
    }
}

void ECDocument::makeHistory()
{
    vector<string>* oldtext = new vector<string>(*text);
    history.push_back(oldtext);
    cursor++;
}

void ECDocument::clearHistory()
{
    int size = history.size();
    for(int i = cursor + 1; i < size; i++)
    {
        delete history[i];
    }
    history.resize(cursor + 1);
}

void ECDocument::undo(int row, int pos)
{
    cx = pos;
    cy = row;
    if(cursor == 0)
        return;
    delete text;
    cursor--;
    text = new vector<string>(*history[cursor]);
}

void ECDocument::redo(int row, int pos)
{
    cx = pos;
    cy = row;
    if(cursor == history.size() - 1)
        return;
    delete text;
    cursor++;
    text = new vector<string>(*history[cursor]);
}

void ECDocument::keyAccept(int row, int pos, int key)
{
    if(key == ENTER)
    {
        clearHistory();
        enter(row, pos);
        makeHistory();
    }
    else if (key == BACKSPACE)
    {
        clearHistory();
        del(row, pos);
        makeHistory();
    }
    else if (key > 999 && key < 1004)
    {
        move(row, pos, key);
    }
    else if (key == CTRL_Z)
    {
        undo(row, pos);
    }
    else if (key == CTRL_Y)
    {
        redo(row, pos);
    }
    else
    {
        clearHistory();
        insert(row, pos, key);
        makeHistory();
    }
}

void ECDocument::setRowNumInView(int r)
{
    rowNumInView = r;
}

int ECDocument::getRowNum()
{
    return (page + 1) * rowNumInView <= (*text).size() ? rowNumInView : (*text).size() % rowNumInView;
}

int ECDocument::getCurPage()
{
    return page;
}

vector<string>* ECDocument::getPage()
{
    return curPage;
}

void ECDocument::updatePage()
{
    curPage->clear();
    for(int i = page * rowNumInView; i < page * rowNumInView + getRowNum(); i++)
    {
        curPage->push_back((*text)[i]);
    }
}

int ECDocument::getCursorX()
{
    return cx;
}

int ECDocument::getCursorY()
{
    return cy;
}

void ECDocument::move(int row, int pos, int key)
{
    //legal
    if(row > -1 && pos > -1 && row < getRowNum() && pos <= (*text)[page * rowNumInView + row].length())
    {
        cx = pos;
        cy = row;
    }
    //illegal
    else
    {
        switch (key)
        {
        case ARROW_LEFT:
            if(row == 0)
            {
                if(page == 0)
                {
                    cx = 0;
                    cy = 0;
                }
                else
                {
                    cx = (*text)[page * rowNumInView + row - 1].length();
                    cy = rowNumInView - 1;
                    prePage();
                }
            }
            else//pre line
            {
                cx = (*text)[page * rowNumInView + row - 1].length();
                cy = row - 1;
            }
            break;
        case ARROW_RIGHT:
            if(row == rowNumInView - 1)
            {
                if(page == (int)((*text).size() - 1) / rowNumInView)//is last page
                {
                    cx = pos - 1;
                    cy = row;
                }
                else
                {
                    cx = 0;
                    cy = 0;
                    nextPage();
                }
            }
            else//next line?
            {
                if(page * rowNumInView + row < (*text).size() - 1)//next line
                {
                    cx = 0;
                    cy = row + 1;
                }
                else
                {
                    cx = pos - 1;
                    cy = row;
                }
            }
            break;
        case ARROW_UP:
            if(row == -1)
            {
                if(page == 0)
                {
                    cx = pos;
                    cy = 0;
                }
                else
                {
                    cx = pos > (*text)[page * rowNumInView + row].length() ? (*text)[page * rowNumInView + row].length() : pos;
                    cy = rowNumInView - 1;
                    prePage();
                }
            }
            else
            {
                cx = pos > (*text)[page * rowNumInView + row].length() ? (*text)[page * rowNumInView + row].length() : pos;
                cy = row;
            }
            break;
        case ENTER:
        case ARROW_DOWN:
            if(row == rowNumInView)
            {
                if(page == (int)((*text).size() - 1) / rowNumInView)
                {
                    cx = pos;
                    cy = rowNumInView - 1;
                }
                else
                {
                    cx = pos > (*text)[page * rowNumInView + row].length() ? (*text)[page * rowNumInView + row].length() : pos;
                    cy = 0;
                    nextPage();
                }
            }
            else
            {
                if(page * rowNumInView + row < (*text).size())
                {
                    cx = pos > (*text)[page * rowNumInView + row].length() ? (*text)[page * rowNumInView + row].length() : pos;
                    cy = row;
                }
                else
                {
                    cx = pos;
                    cy = row - 1;
                }
            }
            break;
        default://CTRL_Z CTRL_Y CTRL_F...
            if(page * rowNumInView + row < (*text).size())
            {
                cx = (*text)[page * rowNumInView + row].length();
                cy = row;
            }
            else
            {
                if(row == 0)
                {
                    cx = pos > (*text)[page * rowNumInView + row].length() ? (*text)[page * rowNumInView + row].length() : pos;
                    cy = rowNumInView - 1;
                    prePage();
                }
                else
                {
                    cx = pos > (*text)[page * rowNumInView + row].length() ? (*text)[page * rowNumInView + row].length() : pos;
                    cy = row - 1;
                }
            }
            break;
        }
    }
}

void ECDocument::insert(int row, int pos, int key)
{
    row = page * rowNumInView + row;
    (*text)[row].insert(pos, 1, key);
}

void ECDocument::enter(int row, int pos)
{
    row = page * rowNumInView + row;
    if((*text).empty())
    {
        (*text).push_back(string());
    }
    string str1((*text)[row].substr(0, pos));
    string str2((*text)[row].substr(pos));
    (*text)[row].erase();
    (*text)[row].append(str2);
    (*text).insert((*text).begin()+=row, str1);
}

void  ECDocument::del(int row, int pos)
{
    int realRow = page * rowNumInView + row;
    if(pos == 0)
    {
        if(row != 0)
        {
            cx = (*text)[realRow - 1].length();
            cy = row - 1;
            (*text)[realRow - 1].append((*text)[realRow]);
            (*text).erase((*text).begin()+=realRow);
        }
        else
        {
            if(page != 0)
            {
                cx = (*text)[realRow - 1].length();
                cy = rowNumInView - 1;
                (*text)[realRow - 1].append((*text)[realRow]);
                (*text).erase((*text).begin()+=realRow);
                prePage();
            }
        }
    }
    else
    {
        cx = pos - 1;
        cy = row;
        string str1 = (*text)[realRow].substr(0, pos - 1).append((*text)[realRow].substr(pos));
        (*text)[realRow].erase();
        (*text)[realRow].append(str1);
    }
}

void ECDocument::prePage()
{
    page--;
}

void ECDocument::nextPage()
{
    page++;
}

void ECDocument::search(string searchString)
{
    spos.clear();
    for(int i = page * rowNumInView; i < page * rowNumInView + getRowNum(); i++)
    {
        int index = 0;
        while (true)
        {
            index = (*text)[i].find(searchString, index);
            if(index == -1)
                break;
            int* temp = new int[2]();
            temp[0] = i - page * rowNumInView;
            temp[1] = index;
            index += searchString.length();
            spos.push_back(temp);
        }
    }
}

void ECDocument::replace(string searchString, string replaceString)
{
    search(searchString);
    if(spos.size() != 0)
    {
        clearHistory();
        int row = -1;
        int count = 0;
        for(int i = 0; i < spos.size(); i++)
        {
            if(row == spos[i][0])
            {
                count++;
                spos[i][1] += ((int)replaceString.size() - (int)searchString.size()) * count;
            }
            else
            {
                row = spos[i][0];
                count = 0;
            }
            (*text)[spos[i][0] + page * rowNumInView].replace(spos[i][1], searchString.size(), replaceString);
        }
        makeHistory();
    }
}

vector<int*>& ECDocument::getSpos()
{
    return spos;
}
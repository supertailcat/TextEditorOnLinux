#ifndef ECDocument_h
#define ECDocument_h

#include "ECView.h"
#include <string>
#include <vector>
#include <iostream>
#define MAXPOS 50

using namespace std;

enum Mode{Edit, Search, Replace};

class ECDocument
{
private:
    char* filename;
    int page = 0;
    int rowNumInView;
    int cx = 0;
    int cy = 0;
    int cursor;
    vector<int*> spos;
    vector<string>* text;
    vector<vector<string>*> history;
    vector<string>* curPage;
    void nextPage();
    void prePage();
    
public:
    Mode mode = Edit;
    string searchString;
    string replaceString;
    ECDocument();
    ~ECDocument(){};
    void readFile(char* filename);
    void saveFile();
    void makeHistory();
    void clearHistory();
    void keyAccept(int row, int pos, int key);
    int getCursorX();
    int getCursorY();
    void setRowNumInView(int r);
    int getRowNum();
    void updatePage();
    int getCurPage();
    vector<string>* getPage();
    void move(int row, int pos, int key);
    void insert(int row, int pos, int key);
    void enter(int row, int pos);
    void del(int row, int pos);
    void undo(int row, int pos);
    void redo(int row, int pos);
    void search(string searchString);
    void replace(string searchString, string replaceString);
    vector<int*>& getSpos();
};

#endif

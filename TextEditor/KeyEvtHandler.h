#ifndef KeyEvtHandler_h
#define KeyEvtHandler_h

#include "ECView.h"
#include "ECDocument.h"

#define MAXPOS 50


class KeyHandler
{
public:
    KeyHandler(){};
    ~KeyHandler(){};
    void SetView(ECView * v){view = v;}
    void SetDocument(ECDocument * d){document = d;}
    virtual void Handle(int key)
    {
        document->setRowNumInView(view->GetRowNumInView());
        if(nxt_Handler != nullptr)
        {
            (*nxt_Handler).Handle(key);
        }
        else
        {
            document->updatePage();
            text = document->getPage();
            showPage();
        }
    }
    void refresh();
    void SetNext(KeyHandler*  nxt)
    {
        nxt_Handler = nxt;
        (*nxt_Handler).SetView(view);
        (*nxt_Handler).SetDocument(document);
    }
protected:
    ECView * view;
    vector<string>* text;
    ECDocument* document;
    KeyHandler * nxt_Handler = nullptr;
    void showPage();
    
};

class QuitHandler : public KeyHandler
{
    public: void Handle(int key);
};

class EnterHandler : public KeyHandler
{
    public: void Handle(int key);
};

class UndoHandler : public KeyHandler
{
    public: void Handle(int key);
};

class RedoHandler : public KeyHandler
{
    public: void Handle(int key);
};

class DeleteHandler : public KeyHandler
{
    public: void Handle(int key);
};

class MoveHandler : public KeyHandler
{
    public: void Handle(int key);
};

class InsertHandler : public KeyHandler
{
    public: void Handle(int key);
};

class FAPHandler : public KeyHandler
{
    public: void Handle(int key);
};

#endif
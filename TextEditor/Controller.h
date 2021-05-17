#ifndef EditorController_h
#define EditorController_h

#include "ECView.h"
#include "ECDocument.h"
#include "KeyEvtHandler.h"

class EditorController : public ECObserver
{
public:
    EditorController(char* filename);
    ~EditorController(){document->saveFile();};
    void Update()
    {
        handler->Handle(view->GetPressedKey());
    };

private:
    ECView* view;
    KeyHandler* handler;
    ECDocument* document;
};

#endif
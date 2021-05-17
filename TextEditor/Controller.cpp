#include "ECView.h"
#include "Controller.h"
#include "ECObserver.h"

#include <iostream>

EditorController :: EditorController(char* filename)
{
    view = new ECView();
    view->SetCursorX(0);
    view->SetCursorY(0);
    view->AddStatusRow("", "EDIT MODE", true);
    
    document = new ECDocument();
    document->setRowNumInView(view->GetRowNumInView());
    document->readFile(filename);

    handler = new KeyHandler();
    InsertHandler* h1 = new InsertHandler();
    DeleteHandler* h2 = new DeleteHandler();
    EnterHandler* h3 = new EnterHandler();
    MoveHandler* h4 = new MoveHandler();
    RedoHandler* h5 = new RedoHandler();
    UndoHandler* h6 = new UndoHandler();
    FAPHandler* h7 = new FAPHandler();
    QuitHandler* h8 = new QuitHandler();
    
    handler->SetView(view);
    handler->SetDocument(document);
    handler->SetNext(h1);
    h1->SetNext(h2);
    h2->SetNext(h3);
    h3->SetNext(h4);
    h4->SetNext(h5);
    h5->SetNext(h6);
    h6->SetNext(h7);
    h7->SetNext(h8);
    view->Attach(this);
    handler->refresh();
    view->Show();
}
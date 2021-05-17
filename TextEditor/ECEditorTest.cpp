// Test code for editor
#include "ECView.h"
#include "Controller.h"

#include <iostream>

using namespace  std;

int myCounter = 0;

int main(int argc, char *argv[])
{
    EditorController* myObserver = new EditorController(argv[1]);
    delete myObserver;
    return 0;
}

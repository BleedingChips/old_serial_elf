//#include <fstream>
#include <iostream>
using namespace std;
#include "textpicture.h"
#include "poserial.h"
int main(int K,char** P)
{
    PO::Insert("TestSerial",&TestSerialInitFunction);
    //PO::Insert("World",&World_Init_Function);
    //PO::Insert("Test",&TextFunction);
    //PO::Regedit("Test");
    PO::Regedit("TestSerial");
    //HumanResource::InsertFunction("Original",&Debug_Original_Personage);
    PO::Init();
    PO::MainLoop();
}

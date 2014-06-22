#ifndef PO_H
#define PO_H
#include "pologic.h"
#include "porender.h"
#include <string>
#include <map>
//#include "scriptionanalyze.h"
class PO
{
    //using namespace std;
    bool Runing;
    static PO* POPoint;
    static bool Exist;
    static void Creat();

    Handle<PORender> PM;
    Handle<POLogic> PA;
    std::list<std::string> NewList;

    int WindowW;
    int WindowH;

    std::string FontAdress;

    std::string Title;

public:
    static int Thread_Function(void*);
    static std::list<void(*)(void)> ExitList;
    static void SetDefultFont(const std::string& K)
    {
        Creat();
        POPoint->FontAdress=K;
    }

    void Exit();
    static void MainLoop();

    static void Insert(const std::string& N,InitFunction P)
    {
        Creat();
        POPoint->PM->PPS->InsertFunction(N,P);
    }
    static void Insert(const std::string& N ,InitFunctionPublic P)
    {
        Creat();
        POPoint->PM->PPS->InsertFunction(N,P);
    }

    static void SetWindow(const int& W,const int& H) {}
    static void SetTitle(const std::string& L) {}

    bool RunAccessorial()
    {
        return PA->MainLogic();
    }

    static void Regedit(const std::string& N)
    {
        Creat();
        POPoint->NewList.push_back(N);
    }

    static void Init();

    PO():WindowW(640),WindowH(480),Title(Translate("拉拉拉拉德玛西亚")),FontAdress("font/fksnjt.ttc") {}
};

PO* PO::POPoint;
bool PO::Exist=false;

void PO::MainLoop()
{
    bool Run=true;
    POPoint->PM->Run=true;
    for(std::list<std::string>::iterator Po=POPoint->NewList.begin(); Po!=POPoint->NewList.end(); ++Po)
        POPoint->PM->CreatPlugin(*Po);
    POPoint->PM->CreatThread(Thread_Function,POPoint);
    while(Run)
    {
        POPoint->PM->SynchroIn();
        Run=POPoint->PM->MainRender();
        POPoint->PM->SynchroOut();
    }
    POPoint->PM->FinalExit();
    POPoint->Exit();
}

void PO::Exit()
{
    PM.Unused();
    PA.Unused();
}

void PO::Creat()
{
    if(!Exist)
    {
        POPoint=new PO();
        Exist=true;
        POPublic* PPS=new POPublic();
        POPoint->PM=new PORender(POPoint->Title,POPoint->WindowW,POPoint->WindowH);
        POPoint->PA=new POLogic();
        POPoint->PM->Connect(PPS);
        POPoint->PA->Connect(PPS);
    }
}


void PO::Init()
{
    Creat();
    if(POPoint->FontAdress!="")
        POPoint->PM->SetFontAdress(POPoint->FontAdress);
    POPoint->PM->Init();
}

int PO::Thread_Function(void* D)
{
    bool Run=true;
    while(Run)
    {
        POPoint->PA->SynchroIn();
        Run=POPoint->RunAccessorial();
        POPoint->PA->SynchroOut();
    }
    POPoint->PA->FinalExit();
}

#endif // PO_H

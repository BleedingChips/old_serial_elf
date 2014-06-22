#ifndef  POLOGIC_H
#define POLOGIC_H
#include "popublic.h"
#include "pogenvironment.h"
#include "informationpool.h"
#include "plugin.h"
class POLogic:public ThreadUnit::Work,public InformationPool
{
    friend class PO;
    unsigned long RunTime;
    unsigned long IntervalTime;

    static const  long DeflutInterTime;

    bool Run;
    POPublic *PPS;

    ThreadMessage<int>::Sent TMI;
    ExtraBuffer<INFL>::Receive EBHM;
    ExtraBuffer<PAL>::Receive EBHPA;
    ExtraBuffer<PML>::Sent EBHPM;
    PAL AP;
    PostBox PB;
    void ThreadSynchroIn();
    void ThreadSynchroOut()
    {
        if(!PPS->Run)Run=false;
        else if(!Run)PPS->Run=false;
    }
    void ThreadConnect(ThreadUnit *);
    void FinalExit()
    {
        AP.clear();
    }
public:

    float MouseX;
    float MouseY;
    float MouseZ;
    bool MainLogic();
    const unsigned long& Time()
    {
        return IntervalTime;
    }
    void AccessorialStart();
    void AccessorialFinish();

    void GetFloatMouse(float& XO,float& YO)
    {
        XO=MouseX;
        YO=MouseY;
    }
    void GetFloatMouse(float& XO,float& YO,float& ZO)
    {
        XO=MouseX;
        YO=MouseY;
        ZO=MouseZ;
    }
    void SetAccessorialStart()
    {
        RunTime=GetTicks();
        IntervalTime=DeflutInterTime;
    }

    void Regedit(Handle<Plugin::Logic> K);
    void Init() {}

    void Exit()
    {
        Run=false;
    }
    void AddLogic(PAL* Tem);
    void SentAndRun(INFL* Tem,PAL::iterator Po);
    void CleanInf(INFL *Tem)
    {
        Tem->CleanAll();
    }
    void Regedit(const std::string& K);
    void AddRender(PML* Tem,const Handle<Plugin::Render>& U)
    {
        Tem->push_back(U);
    }
    void LockMouse()
    {
        TMI.InsertThreadMessage(POPublic::MouseLock);
    }
    void ReleaseMouse()
    {
        TMI.InsertThreadMessage(POPublic::MouseRelease);
    }
    void CreatPlugin(const std::string& K);
    ~POLogic() {}
};
const  long POLogic::DeflutInterTime=6;

void POLogic::AddLogic(PAL *Tem)
{
    if(!Tem->empty())
    {
        for(PAL::iterator Po=Tem->begin(); Po!=Tem->end(); ++Po)
            if(*Po)
                Regedit(*Po);
        Tem->clear();
    }
}

void POLogic::SentAndRun(INFL *Tem,PAL::iterator Po)
{
    const std::set<int> &Select=(*Po)->Select();
    if(!Select.empty())
        for(std::set<int>::iterator Tm=Select.begin(); Tm!=Select.end(); ++Tm)
        {
            if(!Tem->Pool[*Tm].empty())
                for(std::deque<Handle<Information> >::iterator Tm2=Tem->Pool[*Tm].begin(); Tm2!=Tem->Pool[*Tm].end(); ++Tm2)
                    if(Po->Exist())(*Po)->Respond(*Tm2,this);
        }
}

void POLogic::Regedit(const std::string &K)
{
    Handle<Plugin::Render> PM;
    Handle<Plugin::Logic> PA;
    Plugin* PS;
    if(PPS->CreatPlugin(K,PM,PA))
    {
        if(PA.Exist())
        {
            PA->SetName(K);
            Regedit(PA);
        }
        if(PM.Exist())
            EBHPM.Write(&POLogic::AddRender,this,PM);
    }
    else if(PPS->CreatPlugin(K,PM,PA,PS))
    {
        if(PA.Exist())
        {
            PA->SetName(K);
            PA->Connect(PS);
            Regedit(PA);
        }
        if(PM.Exist())
        {
            PM->Connect(PS);
            EBHPM.Write(&POLogic::AddRender,this,PM);
        }
    }
}


void POLogic::Regedit(Handle<Plugin::Logic> K)
{
    K->Set(&PB);
    K->Init(this);
    AP.push_back(K);
    K->SynchroIn();
}

void POLogic::ThreadConnect(ThreadUnit *SP)
{
    PPS=dynamic_cast<POPublic*>(SP);
    Entrust(EBHPA.GetPoint(&PPS->EBHPA));
    Entrust(EBHM.GetPoint(&PPS->EBHM));
    Entrust(TMI.GetPoint(&PPS->TMI));
    Entrust(EBHPM.GetPoint(&PPS->EBHPM));
}

void POLogic::ThreadSynchroIn()
{
    Run=PPS->Run;
    MouseX=PPS->MouseX;
    MouseY=PPS->MouseY;
    MouseZ=PPS->MouseZ;
}

bool POLogic::MainLogic()
{
    if(Run)
    {
        AccessorialStart();
        EBHPA.Read(&POLogic::AddLogic,this);
        for(PAL::iterator Po=AP.begin(); Po!=AP.end(); ++Po)
        {
            if(Po->Exist())
            {
                (*Po)->SynchroIn();
                EBHM.Read(&POLogic::SentAndRun,this,Po);
                (*Po)->SearchInformation(this);
                (*Po)->Work(this);
                (*Po)->SynchroOut();
            }
            else AP.erase(Po--);
        }
        EBHM.Read(&POLogic::CleanInf,this);
        PB.Swap();
        AccessorialFinish();
        return true;
    }
    else return false;
}


void POLogic::AccessorialStart()
{
    RunTime=GetTicks();
}

void POLogic::AccessorialFinish()
{
    IntervalTime=GetTicks()-RunTime;
    if(IntervalTime<DeflutInterTime)
    {
        FrameworkDelay(DeflutInterTime-IntervalTime);
        IntervalTime=DeflutInterTime;
    }
}
#endif // POACCESSORIAL_H

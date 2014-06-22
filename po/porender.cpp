/*#include "porender.h"
void PORender::ReadRender(PML *Tem)
{
    if(!Tem->empty())
    {
        for(PML::iterator Po=Tem->begin(); Po!=Tem->end(); ++Po)
            if(Po->Exist())
            {
                (*Po)->Init(this);
                APL.push_back(*Po);
            }
        Tem->clear();
    }
}

bool PORender::CreatPlugin(const std::string &P)
{
    Handle<Plugin::Render> HPH;
    Handle<Plugin::Logic> HPA;
    Plugin* PS;
    if(PPS->CreatPlugin(P,HPH,HPA))
    {
        if(HPH.Exist())
        {
            HPH->Init(this);
            APL.push_back(HPH);
            HPH->SynchroIn();
        }
        if(HPA.Exist())
        {
            HPA->SetName(P);
            EBHPA.Write(&PORender::AddLogic,this,HPA);
        }
        return true;
    }
    else if(PPS->CreatPlugin(P,HPH,HPA,PS))
    {
        if(HPH.Exist())
        {
            HPH->Connect(PS);
            HPH->Init(this);
            APL.push_back(HPH);
            HPH->SynchroIn();
        }
        if(HPA.Exist())
        {
            HPA->SetName(P);
            HPA->Connect(PS);
            EBHPA.Write(&PORender::AddLogic,this,HPA);
        }
        return true;
    }
    return false;
}

void PORender::FinalExit()
{
    APL.clear();
    POGEnvironment::Exit();
}


void PORender::ThreadSynchroIn()
{
    if(!PPS->Run)
        Run=false;
}

void PORender::ThreadSynchroOut()
{
    if(!PPS->Run)
        Run=false;
    else if(!Run)
        PPS->Run=false;
    PPS->MouseX=MouseX;
    PPS->MouseY=MouseY;
    PPS->MouseZ=MouseZ;
}


void PORender::ThreadConnect(ThreadUnit *SP)
{
    PPS=dynamic_cast<POPublic*>(SP);
    Entrust(EBHPA.GetPoint(&PPS->EBHPA));
    Entrust(EBHM.GetPoint(&PPS->EBHM));
    Entrust(TMI.GetPoint(&PPS->TMI));
    Entrust(EBHPM.GetPoint(&PPS->EBHPM));
}

void PORender::Init()
{
    POGEnvironment::Init();
    RunTime=GetTicks();
    srand(GetTicks());
}

bool PORender::MainRender()
{
    if(Run)
    {
        MainStart();
        Handle<Information> Tem;
        while(GetEvent(Tem))
        {
            if(Tem.Exist())
            {
                if(Tem->IsExit())
                {
                    Run=false;
                    return false;
                }
                else
                {
                    if(Tem->IsMouse())
                    {
                        int K,Y;
                        Tem->GetMouse(K,Y);
                        MouseX=K*2.0/WindowW-1.0;
                        MouseY=1.0-Y*2.0/WindowH;
                        MouseZ=0.0;
                    }
                    if(Tem->IsKeyUp())
                        if(Tem->IsSpecialKeyESC())
                        {
                            Run=false;
                            return false;
                        }
                    EBHM.Write(&PORender::AddInf,this,Tem);
                }
            }
        }
        int Temp;
        while(TMI.GetThreadMessage(Temp))
        {
            switch(Temp)
            {
            case POPublic::MouseLock:
                LockMouse();
                break;
            case POPublic::MouseRelease:
                ReleaseMouse();
                break;
            }
        }
        EBHPM.Read(&PORender::ReadRender,this);
        if(!MouseFree())
            LockMouseDetail(WindowW/2,WindowH/2);
        Tem.Unused();
        for(PML::iterator Po=APL.begin(); Po!=APL.end(); ++Po)
            if(Po->Exist()) (*Po)->SynchroIn();
            else APL.erase(Po--);
        Set3D();
        for(PML::iterator Po=APL.begin(); Po!=APL.end(); ++Po)
            if(Po->Exist()) (*Po)->Layer3D(this);
            else APL.erase(Po--);
        for(PML::iterator Po=APL.begin(); Po!=APL.end(); ++Po)
            if(Po->Exist()) (*Po)->LayerBland(this);
            else APL.erase(Po--);
        for(PML::iterator Po=APL.begin(); Po!=APL.end(); ++Po)
            if(Po->Exist()) (*Po)->LayerSpecial(this);
            else APL.erase(Po--);
        Set2D();
        for(PML::iterator Po=APL.begin(); Po!=APL.end(); ++Po)
            if(Po->Exist()) (*Po)->Layer2D(this);
            else APL.erase(Po--);
        for(PML::iterator Po=APL.begin(); Po!=APL.end(); ++Po)
            if(Po->Exist())
            {
                (*Po)->SynchroOut();
            }
            else APL.erase(Po--);
        //TrackResource();
        MainFinish();
        return true;
    }
    else return false;
}

void PORender::MainStart()
{
    RunTime=GetTicks();
    CleanScreen();
}

void PORender::MainFinish()
{
    IntervalTime=GetTicks()-RunTime;
    if(IntervalTime<DeflutInterTime)
    {
        FrameworkDelay(DeflutInterTime-IntervalTime);
        IntervalTime=DeflutInterTime;
    }
}*/

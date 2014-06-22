#ifndef DIALOGMT_H
#define DIALOGMT_H
#include "handle.h"
#include "pomain.h"
#include "pottf.h"
class DialogData
{
public:
    bool Clean;
    std::list<std::string> TemBuffer;
    bool Change;
    bool AUsing;
    DialogData():Clean(false),Change(false),AUsing(false) {}
};

class DialogP
{
public:
    DialogData DD[2];
    int Maxchar;
    int AUsing;
    int MUsing;
    DialogP():AUsing(0),MUsing(0) {}
    void AAdd()
    {
        AUsing++;
        AUsing%=2;
    }
    void MAdd()
    {
        MUsing++;
        MUsing%=2;
    }
    bool Same()
    {
        return AUsing==MUsing;
    }
    DialogData& MCall()
    {
        return DD[MUsing];
    }
    DialogData& ACall()
    {
        return DD[AUsing];
    }
};

class DialogM:public HandleSeed
{
    DialogP* TemL;
    std::list<POTTF> AD;
    int MaxCha;
    bool Clean;
    bool Change;
    bool Ready;
    int LineNumber;
public:
    void GetSpace(DialogP* Tem)
    {
        TemL=Tem;
    }
    void SynchroIn();
    void SynchroOut();
    operator bool()
    {
        return Clean||Change||(!AD.empty());
    }
    void Clear()
    {
        AD.clear();
    }
    void Add(std::string);
    void AddLine(std::string);
    void Display(XYF S,XYF E,float R=1.0,float B=1.0,float G=1.0,float A=1.0);
    DialogM():MaxCha(40),LineNumber(0)
    {
        Clear();
    }
    static std::string Cut(std::string::iterator&,std::string::iterator,int S);
    int MaxChar()
    {
        return MaxCha;
    }
    int Line()
    {
        return LineNumber;
    }
    bool NeedChange()
    {
        return Ready;
    }
    void Init(POMain *P);
};

class DialogA:public HandleSeed
{
    DialogP* TemL;
    int MaxCha;
    bool Clean;
    bool Change;
    bool MUsing;
    int NumberOfLine;
    std::string Cut(std::string::iterator& B, std::string::iterator E, int S);
public:
    void GetSpace(DialogP* Tem)
    {
        TemL=Tem;
    }
    void SynchroIn();
    void SynchroOut();
    int Line()
    {
        return NumberOfLine;
    }
    void MaxChar(int K)
    {
        MaxCha=K;
    }
    int MaxChar()
    {
        return MaxCha;
    }
    void AddLine(std::string Kc);
    void Add(std::string K);
    void Clear()
    {
        TemL->ACall().TemBuffer.clear();
        NumberOfLine=0;
        Clean=true;
    }
    DialogA():MaxCha(40),NumberOfLine(0) {}
};

void DialogA::SynchroIn()
{
    TemL->ACall().AUsing=true;
    Change=TemL->ACall().Change;
    Clean=TemL->ACall().Clean;
}

void DialogA::SynchroOut()
{
    TemL->ACall().AUsing=false;
    TemL->ACall().Clean=Clean;
    TemL->ACall().Change=Change;

    if(Change||Clean)
        if(TemL->Same())
            TemL->AAdd();
    Clean=false;
    Change=false;
    TemL->Maxchar=MaxCha;
}

void DialogM::SynchroIn()
{
    if(!TemL->MCall().AUsing)
    {
        if(TemL->MCall().Change||TemL->MCall().Clean)
        {
            Clean=TemL->MCall().Clean;
            Change=TemL->MCall().Change;
            TemL->MCall().Clean=false;
            TemL->MCall().Change=false;
            Ready=true;
        }
    }
    MaxCha=TemL->Maxchar;
}

void DialogM::SynchroOut()
{
    if(Ready)
        TemL->MAdd();
    Ready=false;
}

void DialogA::AddLine(std::string Kc)
{
    Kc=Translate(Kc);
    std::string::iterator L=Kc.begin();
    while(L!=Kc.end())
    {
        std::string K=DialogA::Cut(L,Kc.end(),MaxCha);
        NumberOfLine+=1;
        TemL->ACall().TemBuffer.push_back(K);
    }
    Change=true;
}

void DialogA::Add(std::string K)
{
    K=Translate(K);
    if(!TemL->ACall().TemBuffer.empty())
    {
        std::string C=*TemL->ACall().TemBuffer.rbegin();
        TemL->ACall().TemBuffer.pop_back();
        K=C+K;
    }
    std::string::iterator L=K.begin();
    while(L!=K.end())
    {
        std::string TemK=Dialog::Cut(L,K.end(),MaxCha);
        NumberOfLine+=1;
        TemL->ACall().TemBuffer.push_back(TemK);
    }
    Change=true;
}
std::string DialogA::Cut(std::string::iterator& B, std::string::iterator E, int S)
{
    std::string::iterator Po=B;
    int CharSpa=0;
    int LastChar=0;
    int LineChar=0;
    for(; Po!=E; ++Po)
    {
        if(*Po<0)
        {
            if(*Po>(int)0xfffffffc)
                CharSpa=6;
            else if(*Po>(int)0xfffffff8)
                CharSpa=5;
            else if(*Po>(int)0xfffffff0)
                CharSpa=4;
            else if(*Po>(int)0xffffffe0)
                CharSpa=3;
            else if(*Po>(int)0xffffffc0)
                CharSpa=2;
            LastChar=2;
        }
        else
        {
            if(*Po=='\n')
            {
                std::string::iterator Pc=B;
                B=Po+1;
                return std::string(Pc,Po+1);
            }
            CharSpa=1;
            LastChar=1;
        }
        if(LineChar+LastChar>S)
        {
            std::string::iterator Pc=B;
            B=Po;
            return std::string(Pc,Po);
        }
        else
        {
            LineChar+=LastChar;
        }
        Po+=CharSpa-1;
    }
    if(B!=E)
    {
        Po=B;
        B=E;
        return std::string(Po,E);
    }
}

void DialogM::Init(POMain *P)
{
    if(Ready)
    {
        if(Clean)
        {
            AD.clear();
            Clean=false;
            LineNumber=0;
        }
        if(Change)
        {
            POTTF POT;
            for(std::list<std::string>::iterator Po=TemL->MCall().TemBuffer.begin(); Po!=TemL->MCall().TemBuffer.end(); ++Po)
            {
                POT=P->CreatTTFUTF8(*Po);
                AD.push_back(POT);
                ++LineNumber;
            }
            TemL->MCall().TemBuffer.clear();
        }
    }
}

void DialogM::Display(XYF S, XYF E, float R, float B, float G, float A)
{
    if(!AD.empty())
    {
        if(S.X<E.X&&S.Y>E.Y)
        {
            float Tc=(E.X-S.X)/(MaxCha)/0.4;
            float Sp=S.Y;
            for(std::list<POTTF>::iterator Po=AD.begin(); Po!=AD.end(); ++Po)
            {
                if(Sp-Tc>=E.Y)
                {
                    Po->Display(XYF(S.X,Sp),XYF(1.0,0.0),Tc,R,B,G);
                    Sp-=Tc;
                }
                else
                {
                    break;
                }
            }
        }
    }
}
#endif // DIALOGMT_H

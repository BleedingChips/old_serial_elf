#ifndef EDITFRAME_H
#define EDITFRAME_H
#include <list>
#include "threadpart.h"
#include "extrabuffer.h"
#include "porender.h"
#include "pottf.h"
#include "information.h"
typedef std::list<int> EditCL;
typedef std::deque<int> EditTranCD;
class EditFrameData
{
public:
    unsigned int MaxLine;
    unsigned int MaxOfLine;
    bool Active;
    EditFrameData& operator=(const EditFrameData& Te)
    {
        MaxLine=Te.MaxLine;
        MaxOfLine=Te.MaxOfLine;
        Active=Te.Active;
        return *this;
    }
    EditFrameData():MaxLine(40),MaxOfLine(10),Active(false) {}
};

class EditFrameFactor
{
public:
    EditCL AllData;
    EditCL::iterator Cursor;
    void AddChar(int K)
    {
        if(K>=' '&&K<=126)
        {
            AllData.insert(Cursor,K);
        }
        else if(K==8)
            AllData.erase((--Cursor)++);
        else if(K==-1)
            Clear();
        else if(K==-2)
            Cursor--;
        else if(K==-3)
            Cursor++;
    }
    void Clear()
    {
        AllData.clear();
        Cursor=AllData.end();
    }
    EditFrameFactor()
    {
        Cursor=AllData.end();
    }
};

class EditFrame:public ThreadPart
{
    ExtraBuffer<EditTranCD> Data;
    EditFrameData EFD;
public:
    class Render;
    class Logic;
};

class EditFrame::Render:public ThreadPart::Work<EditFrame>
{
    ExtraBuffer<EditTranCD>::Receive EBETCD;
    EditFrameData EFD;
public:

    static std::map<int,POTTF> Data;
    unsigned int LastLine;
    EditFrameFactor EFF;
    operator bool()
    {
        return EFD.Active;
    }
    bool NeedChange()
    {
        return EBETCD;
    }
    bool Change()
    {
        EBETCD.Read(&Render::LoadChar,this);
    }
    void EntrustGet(EditFrame *Te)
    {
        Entrust(EBETCD.GetPoint(&Te->Data));
    }
    void WorkSynchroIn()
    {
        EFD=this->Call()->EFD;
    }
    void WorkSynchroOut() {}
    void LoadChar(EditTranCD*);
    void Display(PORender*,const XYF& S,const XYF& E,float R=1.0,float G=1.0,float B=1.0,float A=1.0);
    Render():LastLine(1) {}
};
std::map<int,POTTF> EditFrame::Render::Data;
class EditFrame::Logic:public ThreadPart::Work<EditFrame>
{
    ExtraBuffer<EditTranCD>::Sent EBETCD;
    EditFrameData EFD;
public:
    EditFrameFactor EFF;
    std::string Comment;
    void Active()
    {
        EFD.Active=true;
    }
    void DisActive()
    {
        EFD.Active=false;
    }
    operator bool()
    {
        return EFD.Active;
    }
    bool CommentFinish;
    bool BigInPut;
    void EntrustGet(EditFrame *Te)
    {
        Entrust(EBETCD.GetPoint(&Te->Data));
    }
    bool IsComment()const
    {
        return CommentFinish;
    }
    std::string GetComment()
    {
        CommentFinish=false;
        return Comment;
    }
    void WorkSynchroIn() {}
    void WorkSynchroOut()
    {
        this->Call()->EFD=EFD;
    }
    void AddDetail(EditTranCD* D,const int& P)
    {
        D->push_back(P);
    }
    void Add(int L)
    {
        EBETCD.Write(&EditFrame::Logic::AddDetail,this,L);
        EFF.AddChar(L);
    }
    void Clear()
    {
        EFF.Clear();
        EBETCD.Write(&EditFrame::Logic::AddDetail,this,-1);
    }
    void KeyScanf(const Information* I);
    //Logic(){}
};

void EditFrame::Render::LoadChar(EditTranCD * K)
{
    for(EditTranCD::iterator Po=K->begin(); Po!=K->end(); ++Po)
        EFF.AddChar(*Po);
    K->clear();
}

void EditFrame::Render::Display(PORender * P, const XYF &S, const XYF &E, float R, float G, float B, float A)
{
    if(EFD.Active)
    {
        float Size((E.X-S.X)/EFD.MaxLine/0.4);
        int T(0);
        int L(0);
        bool HaveCursor=false;
        std::map<int,POTTF>::iterator Potte=Data.find('>');
        if(Potte!=Data.end())
            Potte->second.Display(XYF(T*Size*0.4+S.X,S.Y-L*Size),XYF(1.0,0.0),Size,R,G,B,A);
        else
        {
            char Tr[2]=">";
            POTTF Tem(P->CreatTTFUTF8(Tr));
            std::pair<std::map<int,POTTF>::iterator ,bool> Tc=Data.insert(std::make_pair('>',Tem));
            if(Tc.second)
                (Tc.first)->second.Display(XYF(T*Size*0.4+S.X,S.Y-L*Size),XYF(1.0,0.0),Size,R,G,B,A);
        }
        if(++T>EFD.MaxLine)
        {
            T=0;
            ++L;
        }
        if(L<EFD.MaxOfLine)
        {
            for(EditCL::iterator Po=EFF.AllData.begin(); Po!=EFF.AllData.end(); ++Po)
            {
                std::map<int,POTTF>::iterator Po2=Data.find(*Po);
                if(Po2!=Data.end())
                    Po2->second.Display(XYF(T*Size*0.4+S.X,S.Y-L*Size),XYF(1.0,0.0),Size,R,G,B,A);
                else
                {
                    char Tr[3]="AB";
                    char *Temp=Tr;
                    *((int*)(Temp))=*Po;
                    POTTF Tem(P->CreatTTFUTF8(Tr));
                    std::pair<std::map<int,POTTF>::iterator ,bool> Tc=Data.insert(std::make_pair(*Po,Tem));
                    if(Tc.second)
                        (Tc.first)->second.Display(XYF(T*Size*0.4+S.X,S.Y-L*Size),XYF(1.0,0.0),Size,R,G,B,A);
                }
                if(++T>=EFD.MaxLine)
                {
                    T=0;
                    ++L;
                }
                if(Po==EFF.Cursor)
                {
                    glColor3f(R,G,B);
                    glBegin(GL_QUADS);
                    glVertex2f((T-1)*Size*0.4+S.X,S.Y-(L+1)*Size+0.01);
                    glVertex2f((T-1)*Size*0.4+S.X,S.Y-(L+1)*Size);
                    glVertex2f((T)*Size*0.4+S.X,S.Y-(L+1)*Size);
                    glVertex2f((T)*Size*0.4+S.X,S.Y-(L+1)*Size+0.01);
                    glEnd();
                    HaveCursor=true;
                }
            }
            if(!HaveCursor)
            {
                glColor3f(R,G,B);
                glBegin(GL_QUADS);
                glVertex2f(T*Size*0.4+S.X,S.Y-(L+1)*Size+0.01);
                glVertex2f(T*Size*0.4+S.X,S.Y-(L+1)*Size);
                glVertex2f((T+1)*Size*0.4+S.X,S.Y-(L+1)*Size);
                glVertex2f((T+1)*Size*0.4+S.X,S.Y-(L+1)*Size+0.01);
                glEnd();
            }
        }
    }
}

void EditFrame::Logic::KeyScanf(const Information *I)
{
    if(EFD.Active)
    {
        if(I->IsKeyUp())
        {
            if(I->IsSpecialKeyLeft())
                Add(-2);
            else if(I->IsSpecialKeyRight())
                Add(-3);
            else if(I->IsSpecialKeyEnter())
            {
                Comment.clear();
                for(EditCL::iterator Po=EFF.AllData.begin(); Po!=EFF.AllData.end(); ++Po)
                    Comment+=*Po;
                CommentFinish=true;
                Clear();
                DisActive();
            }
            else if(I->IsSpecialKeyESC())
            {
                Clear();
                DisActive();
            }
            else
                Add(I->GetKey());
        }
    }
}

#endif // EDITFRAME_H

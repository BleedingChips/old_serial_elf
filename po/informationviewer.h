#ifndef INFORMATIONVIEWER_H
#define INFORMATIONVIEWER_H
#include "information.h"
#include <list>
#include <string>
#include <deque>
#include <map>
#include <set>
#include "informationpool.h"
typedef std::deque<Handle<Information> > SinglePostBox;
typedef std::map<std::string,SinglePostBox> MultyPostBox;
typedef std::map<std::string,int> PostBoxCount;

class PostBox
{
public:
    MultyPostBox Box[2];
    PostBoxCount Count;
    int Read,Write;
    PostBox():Read(0),Write(1) {}
    void Add(const std::string& Tem);
    void Delete(const std::string& Tem);
    bool Insert(const Handle<Information>& Tem,const std::string&);
    void Swap()
    {
        CleanBox();
        Read=(Read+1)%2;
        Write=(Write+1)%2;
    }
    void CleanBox();
    void CleanAll()
    {
        Box[0].clear();
        Box[1].clear();
        Count.clear();
    }
};

class POLogic;
class InformationViewer
{
    bool UsingPoint;
    PostBox* PBP;
    std::set<int> Mode;
    std::string Name;
public:
    void OpenKey()
    {
        Mode.insert(InformationPool::Key);
    }
    void CloseKey()
    {
        Mode.erase(InformationPool::Key);
    }
    void OpenMouse()
    {
        Mode.insert(InformationPool::Mouse);
    }
    void CloseMouse()
    {
        Mode.erase(InformationPool::Mouse);
    }
    void OpenWindow()
    {
        Mode.insert(InformationPool::Window);
    }
    void CloseWindow()
    {
        Mode.erase(InformationPool::Window);
    }
    void OpenAll()
    {
        OpenKey();
        OpenMouse();
        OpenWindow();
    }
    void CloseAll()
    {
        CloseKey();
        CloseMouse();
        CloseWindow();
    }
    void SetName(const std::string& T)
    {
        Name=T;
    }
    const std::set<int>& Select()
    {
        return Mode;
    }
    void SearchInformation(POLogic*);
    virtual void Respond(const Information*,POLogic *) {}
    void Set(PostBox* C)
    {
        if(UsingPoint) PBP->Delete(Name);
        PBP=C;
        UsingPoint=true;
        PBP->Add(Name);
    }
    void Insert(const Handle<Information>& Tem,const std::string& P)
    {
        if(UsingPoint)PBP->Insert(Tem,P);
    }
    InformationViewer():UsingPoint(false) {}
    ~InformationViewer();
};

void InformationViewer::SearchInformation(POLogic* P)
{
    if(UsingPoint)
    {
        MultyPostBox& Box=PBP->Box[PBP->Read];
        MultyPostBox::iterator Po=Box.find(Name);
        if(Po!=Box.end())
            if(!Po->second.empty())
                for(SinglePostBox::iterator Go=Po->second.begin(); Go!=Po->second.end(); ++Go)
                    Respond(*Go,P);
    }
}


void PostBox::Add(const std::string& Tem)
{
    PostBoxCount::iterator Po=Count.find(Tem);
    if(Po!=Count.end())
        Po->second+=1;
    else
    {
        Box[0][Tem];
        Box[1][Tem];
        Count[Tem]=0;
    }
}

void PostBox::Delete(const std::string& Tem)
{
    PostBoxCount::iterator Po=Count.find(Tem);
    if(Po!=Count.end())
    {
        Po->second-=1;
        if(Po->second<=0)
        {
            for(int i=0; i<2; ++i)
            {
                MultyPostBox::iterator Po=Box[i].find(Tem);
                if(Po!=Box[i].end())
                    Box[i].erase(Po);
            }
        }
    }
}

bool PostBox::Insert(const Handle<Information>& Tem,const std::string& Name)
{
    MultyPostBox::iterator Po=Box[Write].find(Name);
    if(Po!=Box[Write].end())
    {
        Po->second.push_back(Tem);
        return true;
    }
    return false;
}

void PostBox::CleanBox()
{
    for(MultyPostBox::iterator Po=Box[Read].begin(); Po!=Box[Read].end(); ++Po)
        Po->second.clear();
}

InformationViewer::~InformationViewer()
{
    if(UsingPoint)
    {
        PBP->Delete(Name);
    }
}

#endif // INFORMATIONVIEWER_H

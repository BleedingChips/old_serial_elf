#ifndef THREADUNIT_H
#define THREADUNIT_H
#include "pomutex.h"
#include <list>
#include "information.h"
#include "threadmessage.h"
#include "threadpart.h"
typedef std::deque<ThreadPart::WorkBase*> TPWP;

class ThreadUnit
{
    POMutex PM;
    int UsedCount;
public:
    ThreadUnit():UsedCount(0) {}
    void Lock()
    {
        PM.Lock();
    }
    void UnLock()
    {
        PM.UnLock();
    }
    virtual ~ThreadUnit() {}
    class Work:virtual public HandleSeed
    {
        ThreadUnit* PS;
        bool UsingPSP;
        TPWP WorkList;
    protected:
        virtual void ThreadSynchroIn() {}
        virtual void ThreadSynchroOut() {}
        virtual void ThreadConnect(ThreadUnit*) {}
        void Entrust(ThreadPart::WorkBase* TPW)
        {
            WorkList.push_back(TPW);
        }
    public:
        Work():UsingPSP(false) {}
        void SynchroIn();
        void SynchroOut();
        void Connect(ThreadUnit*);
        virtual ~Work();
    };
};
void ThreadUnit::Work::Connect(ThreadUnit* PT)
{
    PS=PT;
    PT->ThreadUnit::Lock();
    ++PT->ThreadUnit::UsedCount;
    UsingPSP=true;
    ThreadConnect(PT);
    PT->ThreadUnit::UnLock();
}
ThreadUnit::Work::~Work()
{
    if(UsingPSP)
    {
        PS->ThreadUnit::Lock();
        --PS->ThreadUnit::UsedCount;
        if(PS->ThreadUnit::UsedCount<=0)
        {
            PS->ThreadUnit::UnLock();
            delete PS;
        }
        else
            PS->ThreadUnit::UnLock();
    }
}
void ThreadUnit::Work::SynchroIn()
{
    if(UsingPSP)
    {
        PS->ThreadUnit::Lock();
        for(TPWP::iterator Po=WorkList.begin(); Po!=WorkList.end(); ++Po)
            (*Po)->SynchroIn();
        ThreadSynchroIn();
        PS->ThreadUnit::UnLock();
    }
}

void ThreadUnit::Work::SynchroOut()
{
    if(UsingPSP)
    {
        PS->ThreadUnit::Lock();
        for(TPWP::iterator Po=WorkList.begin(); Po!=WorkList.end(); ++Po)
            (*Po)->SynchroOut();
        ThreadSynchroOut();
        PS->ThreadUnit::UnLock();
    }
}
#endif // THREADUNIT_H

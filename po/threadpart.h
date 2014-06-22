#ifndef THREADPART_H
#define THREADPART_H
#include <deque>
class ThreadPart
{
public:
    class WorkBase
    {
    protected:
        virtual void WorkSynchroIn()=0;
        virtual void WorkSynchroOut()=0;
    public:
        std::deque<WorkBase*> WorkList;
        void SynchroIn()
        {
            EntrustIn();
            WorkSynchroIn();
        }
        void SynchroOut()
        {
            EntrustOut();
            WorkSynchroOut();
        }
        void Entrust(WorkBase* TPW)
        {
            WorkList.push_back(TPW);
        }
        void EntrustIn();
        void EntrustOut();
    };
    template <typename T>
    class Work:public WorkBase
    {
        T* Data;
    public:
        WorkBase* GetPoint(T* Te)
        {
            Data=Te;
            EntrustGet(Te);
            return this;
        }
        virtual void EntrustGet(T* Te) {}
        T* Call()
        {
            return Data;
        }
    };

    ThreadPart() {}
    virtual ~ThreadPart() {}
};

void ThreadPart::WorkBase::EntrustIn()
{
    for(std::deque<WorkBase*>::iterator Po=WorkList.begin(); Po!=WorkList.end(); ++Po)
        (*Po)->SynchroIn();
}

void ThreadPart::WorkBase::EntrustOut()
{
    for(std::deque<WorkBase*>::iterator Po=WorkList.begin(); Po!=WorkList.end(); ++Po)
        (*Po)->SynchroOut();
}
#endif // THREADPART_H

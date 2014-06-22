#ifndef THREADMESSAGE_H
#define THREADMESSAGE_H
#include "threadpart.h"
#include <deque>
template <typename T>
class ThreadMessage:public ThreadPart
{
    std::deque<T> Data;
public:
    class Receive:public ThreadPart::Work<ThreadMessage>
    {
        std::deque<T> In;
        void WorkSynchroIn()
        {
            if(!this->Call()->Data.empty())
            {
                In.insert(In.end(),this->Call()->Data.begin(),this->Call()->Data.end());
                this->Call()->Data.clear();
            }
        }
        void WorkSynchroOut() {}
    public:
        bool GetThreadMessage(T& Tem)
        {
            if(!In.empty())
            {
                Tem=*In.begin();
                In.pop_front();
                return true;
            }
            return false;
        }
    };
    class Sent:public ThreadPart::Work<ThreadMessage>
    {
        std::deque<T> Out;
        void WorkSynchroIn() {}
        void WorkSynchroOut()
        {
            if(!Out.empty())
            {
                this->Call()->Data.insert(this->Call()->Data.end(),Out.begin(),Out.end());
                Out.clear();
            }
        }
    public:
        void InsertThreadMessage(const T& K)
        {
            Out.push_back(K);
        }
    };
};
#endif // THREADMESSAGE_H

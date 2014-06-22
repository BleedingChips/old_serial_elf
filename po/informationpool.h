#ifndef INFORMATIONPOOL_H
#define INFORMATIONPOOL_H
#include <set>
#include <deque>
#include "information.h"
#define INFORMATIONPOOLMAX 3
class InformationPool
{
    int Order;
public:
    static const int Mouse;
    static const int Key;
    static const int Window;
    std::deque<Handle<Information> > Pool[INFORMATIONPOOLMAX];
    void AddInfromation(Handle<Information> K);
    InformationPool():Order(0) {}
    void CleanAll()
    {
        for(int i=0; i<INFORMATIONPOOLMAX; ++i)Pool[i].clear();
        Order=0;
    }
};

const int InformationPool::Mouse=0;
const int InformationPool::Key=1;
const int InformationPool::Window=2;

void InformationPool::AddInfromation(Handle<Information> K)
{
    if(K)
    {
        K->Order=Order;
        ++Order;
        if(K->IsMouse())
            Pool[Mouse].push_back(K);
        else if(K->IsKey())
            Pool[Key].push_back(K);
        else if(K->IsWindow())
            Pool[Window].push_back(K);
    }
}

#endif // INFORMATIONPOOL_H

#ifndef POPUBLIC_H
#define POPUBLIC_H
#include "plugin.h"
#include "extrabuffer.h"
#include "informationpool.h"
#include "threadmessage.h"

typedef void(*InitFunction)(Handle<Plugin::Render>&,Handle<Plugin::Logic>&);
typedef void(*InitFunctionPublic)(Handle<Plugin::Render>&,Handle<Plugin::Logic>&,Plugin*&);



typedef std::deque<Handle<Plugin::Render> > PML;
typedef std::deque<Handle<Plugin::Logic> > PAL;
typedef InformationPool  INFL;
typedef std::deque<std::string > STRL;
class POPluginInitFunction
{
    std::map<std::string,InitFunction> InitFunc;
    std::map<std::string,InitFunctionPublic>  InitFuncPublic;
    POMutex POM;
public:
    void InsertFunction(const std::string& N ,const InitFunction& P)
    {
        POM.Lock();
        InitFunc.insert(std::make_pair(N,P));
        POM.UnLock();
    }
    void InsertFunction(const std::string& N ,const InitFunctionPublic& P)
    {
        POM.Lock();
        InitFuncPublic.insert(std::make_pair(N,P));
        POM.UnLock();
    }
    bool CreatPlugin(const std::string& K,Handle<Plugin::Render>&,Handle<Plugin::Logic>&);
    bool CreatPlugin(const std::string& K,Handle<Plugin::Render>&,Handle<Plugin::Logic>&,Plugin*&);
};

bool POPluginInitFunction::CreatPlugin(const std::string &K, Handle<Plugin::Render> & PM, Handle<Plugin::Logic> & PA)
{
    POM.Lock();
    std::map<std::string,InitFunction>::iterator Po2=InitFunc.find(K);
    if(Po2!=InitFunc.end())
    {
        (*Po2->second)(PM,PA);
        POM.UnLock();
        return true;
    }
    POM.UnLock();
    return false;
}

bool POPluginInitFunction::CreatPlugin(const std::string &K, Handle<Plugin::Render> & PM, Handle<Plugin::Logic> & PA,Plugin*& PS)
{
    POM.Lock();
    std::map<std::string,InitFunctionPublic>::iterator Po3=InitFuncPublic.find(K);
    if(Po3!=InitFuncPublic.end())
    {
        (*Po3->second)(PM,PA,PS);
        POM.UnLock();
        return true;
    }
    POM.UnLock();
    return false;
}
class POPublic:public ThreadUnit
{
public:
    static const int MouseLock;
    static const int MouseRelease;
    bool Run;
    float MouseX;
    float MouseY;
    float MouseZ;
    POPluginInitFunction PIF;
    ThreadMessage<int> TMI;
    ExtraBuffer<STRL > EBS;
    ExtraBuffer<INFL > EBHM;
    ExtraBuffer<PAL > EBHPA;
    ExtraBuffer<PML> EBHPM;
    bool CreatPlugin(const std::string& K,Handle<Plugin::Render> & PM, Handle<Plugin::Logic> & PA)
    {
        return PIF.CreatPlugin(K,PM,PA);
    }
    bool CreatPlugin(const std::string& K,Handle<Plugin::Render> & PM, Handle<Plugin::Logic> & PA,Plugin*& P)
    {
        {
            return PIF.CreatPlugin(K,PM,PA,P);
        }
    }
    POPublic():Run(true) {}
    void InsertFunction(const std::string& N ,const InitFunction& P)
    {
        PIF.InsertFunction(N,P);
    }
    void InsertFunction(const std::string& N ,const InitFunctionPublic& P)
    {
        PIF.InsertFunction(N,P);
    }
};
const int POPublic::MouseLock=0;
const int POPublic::MouseRelease=1;
#endif // POPUBLIC_H

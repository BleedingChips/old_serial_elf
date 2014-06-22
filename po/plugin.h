#ifndef PLUGIN_H
#define PLUGIN_H
#include "handle.h"
#include "threadunit.h"
#include "informationviewer.h"
class PORender;
class POLogic;
class Plugin:public ThreadUnit
{
    bool Runing;
public:
    Plugin():Runing(true) {}
    class Logic:public ThreadUnit::Work,public InformationViewer
    {
        Plugin* Point;
        bool Runing;
        void ThreadSynchroIn();
        void ThreadSynchroOut();
        void ThreadConnect(ThreadUnit *P)
        {
            Point=dynamic_cast<Plugin*>(P);
            PluginConnect(Point);
        }
    protected:
        virtual void PluginConnect(Plugin*) {}
        virtual void PluginSynchroIn() {}
        virtual void PluginSynchroOut() {}
    public:
        virtual void Init(POLogic *) {}
        virtual void Work(POLogic *) {}
        void DestoryPlugin()
        {
            Runing=false;
        }
        Logic():Runing(true) {}
    };
    class Render:public ThreadUnit::Work
    {
        Plugin* Point;
        bool Runing;
        void ThreadSynchroIn();
        void ThreadSynchroOut();
        void ThreadConnect(ThreadUnit *P)
        {
            Point=dynamic_cast<Plugin*>(P);
            PluginConnect(Point);
        }
    protected :
        virtual void PluginConnect(Plugin*) {}
        virtual void PluginSynchroIn() {}
        virtual void PluginSynchroOut() {}
    public:
        virtual void Init(PORender*) {}
        virtual void Layer3D(PORender*) {}
        virtual void Layer2D(PORender*) {}
        virtual void LayerBland(PORender*) {}
        virtual void LayerSpecial(PORender*) {}
        void DestoryPlugin()
        {
            Runing=false;
        }
        Render():Runing(true) {}
    };
};

void Plugin::Render::ThreadSynchroIn()
{
    Runing=Point->Runing;
    PluginSynchroIn();
}

void Plugin::Render::ThreadSynchroOut()
{
    Point->Runing=Runing;
    PluginSynchroOut();
    if(!Runing)
        Destory();
}

void Plugin::Logic::ThreadSynchroIn()
{
    Runing=Point->Runing;
    PluginSynchroIn();
}

void Plugin::Logic::ThreadSynchroOut()
{
    Point->Runing=Runing;
    PluginSynchroOut();
    if(!Runing)
        Destory();
}
#endif // PLUGIN_H

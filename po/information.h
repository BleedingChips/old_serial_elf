#ifndef  DIY_INFORMATION_H
#define  DIY_INFORMATION_H
#include <list>
#include <string>
#include <vector>
#include <map>
#include <SDL2/SDL_events.h>
#include "handle.h"

union DataForInformation
{
    unsigned int ui;
    bool b;
    unsigned char uc;
    char c;
    int i;
    float f;
    void* v;//析构时不会调用DELETE
    operator int()
    {
        return i;
    }
    operator bool()
    {
        return b;
    }
    operator char()
    {
        return c;
    }
    operator float()
    {
        return f;
    }
    operator void*()
    {
        return v;
    }
    operator unsigned int()
    {
        return ui;
    }
    operator unsigned char()
    {
        return uc;
    }
    void operator= (const unsigned char& Tem)
    {
        uc=Tem;
    }
    void operator= (const char& Tem)
    {
        c=Tem;
    }
    void operator= (const unsigned int& Tem)
    {
        ui=Tem;
    }
    void operator= (const int& Tem)
    {
        i=Tem;
    }
    void operator= (const float& Tem)
    {
        f=Tem;
    }
    void operator= (const bool& Tem)
    {
        b=Tem;
    }
    void operator= ( void* Tem)
    {
        v=Tem;
    }
    //void operator= (const DataForInformation& Tc){this->d=Tc.d;}
};
class Information:public HandleSeed
{
    //friend Handle<Information> Translate(SDL_Event&);
public:
    static std::map<unsigned int,unsigned int> LetterDefine;
    static bool MapInit;
    unsigned int Order;
    SDL_Event Event;
    Handle<HandleSeed> DataE;
    ~Information() {}
    Information():Order(0) {}

    void operator= (const Information& Tec)
    {
        Order=Tec.Order;
        Event=Tec.Event;
        DataE=Tec.DataE;
    }
    void operator= (const SDL_Event& P)
    {
        Event=P;
    }
    Information(const Information& P):Order(P.Order),DataE(P.DataE)
    {
        Event=P.Event;
    }

    bool IsWindow()const
    {
        return Event.type==SDL_WINDOWEVENT;
    }

    bool IsCustom()const
    {
        return Order==0;
    }

    bool IsExit()const
    {
        return Event.type==SDL_QUIT;
    }

    bool IsKey()const
    {
        return IsKeyDown()||IsKeyUp();
    }
    bool IsKeyDown()const
    {
        return Event.type==SDL_KEYDOWN;
    }
    bool IsKeyUp()const
    {
        return Event.type==SDL_KEYUP;
    }
    unsigned int GetRealKey()const
    {
        return Event.key.keysym.sym;
    }
    unsigned int GetKey()const;
    bool IsKeySeem()const;
    bool IsSpecialKeyESC()const
    {
        return Event.key.keysym.sym==SDLK_ESCAPE;
    }
    bool IsSpecialKeyUp()const
    {
        return Event.key.keysym.sym==SDLK_UP;
    }
    bool IsSpecialKeyDown()const
    {
        return Event.key.keysym.sym==SDLK_DOWN;
    }
    bool IsSpecialKeyRight()const
    {
        return Event.key.keysym.sym==SDLK_RIGHT;
    }
    bool IsSpecialKeyLeft()const
    {
        return Event.key.keysym.sym==SDLK_LEFT;
    }
    bool IsSpecialKeyDelete()const
    {
        return Event.key.keysym.sym==SDLK_BACKSPACE ;
    }
    bool IsSpecialKeyEnter()const
    {
        return Event.key.keysym.sym==SDLK_RETURN;
    }
    bool IsSpecialKeySpace()const
    {
        return Event.key.keysym.sym==SDLK_SPACE;
    }
    bool IsMouse()const
    {
        return IsMouseMotion()||IsMouseButtonDown()||IsMouseButtonUp();
    }
    bool IsMouseMotion()const
    {
        return Event.type==SDL_MOUSEMOTION;
    }
    bool IsMouseButtonDown()const
    {
        return Event.type==SDL_MOUSEBUTTONDOWN;
    }
    bool IsMouseButtonUp()const
    {
        return Event.type==SDL_MOUSEBUTTONUP;
    }
    bool IsMouseWheel()const
    {
        return Event.type==SDL_MOUSEWHEEL;
    }
    int MouseLocationX()const
    {
        if(IsMouseMotion()) return Event.motion.x;
        return Event.button.x;
    }
    int MouseLocationY()const
    {
        if(IsMouseMotion()) return Event.motion.y;
        return Event.button.y;
    }
    int MouseMotionX()const
    {
        return Event.motion.xrel;
    }
    int MouseMotionY()const
    {
        return Event.motion.yrel;
    }
    void GetMouse(int &K,int &P)const
    {
        K=MouseLocationX();
        P=MouseLocationY();
    }
    void GetMouseMotion(int &K,int P)const
    {
        K=MouseMotionX();
        P=MouseMotionY();
    }


    bool IsMouseButtonLeft()const
    {
        return Event.button.button==SDL_BUTTON_LEFT;
    }
    bool IsMouseButtonMiddle()const
    {
        return Event.button.button==SDL_BUTTON_MIDDLE;
    }
    bool IsMouseButtonRight()const
    {
        return Event.button.button==SDL_BUTTON_RIGHT;
    }

    //bool IsWindow(){return IsWindowActive()||IsWindowDisActive();}
};
std::map<unsigned int,unsigned int> Information::LetterDefine;
bool Information::MapInit=false;

bool Information::IsKeySeem()const
{
    unsigned int Key(Event.key.keysym.sym);
    if((Key>='!'&&Key<='~')||(Key>=SDLK_KP_1&&Key<=SDLK_KP_0))
        return true;
    return false;
}

unsigned int Information::GetKey() const
{
    if(Event.key.keysym.sym>='a'&&Event.key.keysym.sym<='z')
    {
        bool Up=false;
        if(Event.key.keysym.mod&KMOD_CAPS)
            Up=!Up;
        if(Event.key.keysym.mod&KMOD_SHIFT)
            Up=!Up;
        if(Up)
            return Event.key.keysym.sym-'a'+'A';
    }
    else if(Event.key.keysym.sym>='!'&&Event.key.keysym.sym<='~')
    {
        if(Event.key.keysym.mod&KMOD_SHIFT)
        {
            if(!MapInit)
            {
                LetterDefine['1']='!';
                LetterDefine['2']='@';
                LetterDefine['3']='#';
                LetterDefine['4']='$';
                LetterDefine['5']='%';
                LetterDefine['6']='^';
                LetterDefine['7']='&';
                LetterDefine['8']='*';
                LetterDefine['9']='(';
                LetterDefine['0']=')';
                LetterDefine['-']='_';
                LetterDefine['=']='+';
                LetterDefine['\\']='|';
                LetterDefine['[']='{';
                LetterDefine[']']='}';
                LetterDefine[';']=':';
                LetterDefine['\'']='"';
                LetterDefine[',']='<';
                LetterDefine['.']='>';
                LetterDefine['/']='?';
                MapInit=true;
            }
            return LetterDefine[Event.key.keysym.sym];
        }
    }
    return Event.key.keysym.sym;
}

#endif

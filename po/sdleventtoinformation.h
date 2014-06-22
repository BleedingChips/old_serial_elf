#ifndef SDLEVENTTOINFORMATION_H
#define SDLEVENTTOINFORMATION_H
#include <SDL2/SDL.h>
#include "information.h"
#include "handle.h"
Handle<Information> Translate(SDL_Event& Event)
{
    Handle<Information> Temporary=new Information;
    switch(Event.type)
    {
    case SDL_MOUSEMOTION:
        Temporary->Category=Information::MOUSEMOTION;
        Temporary->Data1.i=Event.button.x;
        Temporary->Data2.i=Event.button.y;
        Temporary->Data3.i=Event.motion.xrel;
        Temporary->Data4.i=Event.motion.yrel;
        break;
    case SDL_MOUSEBUTTONDOWN:
        Temporary->Category=Information::MOUSEBUTTONDOWN;
        Temporary->Data1.i=Event.button.x;
        Temporary->Data2.i=Event.button.y;
        switch(Event.button.button)
        {
        case SDL_BUTTON_LEFT:
            Temporary->Data3.i=Information::LEFT;
            break;
        case SDL_BUTTON_MIDDLE:
            Temporary->Data3.i=Information::MIDDLE;
            break;
        case SDL_BUTTON_RIGHT:
            Temporary->Data3.i=Information::RIGHT;
        }

        Temporary->Data3.i=Event.button.button;
        break;
    case SDL_MOUSEBUTTONUP:
        Temporary->Category=Information::MOUSEBUTTONUP;
        Temporary->Data1.i=Event.button.x;
        Temporary->Data2.i=Event.button.y;
        switch(Event.button.button)
        {
        case SDL_BUTTON_LEFT:
            Temporary->Data3.i=Information::LEFT;
            break;
        case SDL_BUTTON_MIDDLE:
            Temporary->Data3.i=Information::MIDDLE;
            break;
        case SDL_BUTTON_RIGHT:
            Temporary->Data3.i=Information::RIGHT;
        }
        break;
    case SDL_KEYDOWN:
        Temporary->Category=Information::INFKEYDOWN;
        Temporary->Data1.i=Event.key.keysym.sym;
        Temporary->Data2.i=Event.key.keysym.mod;
        break;
    case SDL_KEYUP:
        Temporary->Category=Information::INFKEYUP;
        Temporary->Data1.i=Event.key.keysym.sym;
        Temporary->Data2.i=Event.key.keysym.mod;
        break;
    case SDL_WINDOWEVENT:
        switch(Event.window.event)
        {
        case SDL_WINDOWEVENT_ENTER:
            Temporary->Category=Information::INFWINDOWACTIVE;
            break;
        case SDL_WINDOWEVENT_LEAVE:
            Temporary->Category=Information::INFWINDOWDISACTIVE;
            break;
        }
        break;
    case SDL_QUIT:
        Temporary->Category=Information::INFEXIT;
        break;
    /*case SDL_ACTIVEEVENT:
        if(Event.active.state==2)
            Temporary->Category=INFWINDOWDISACTIVE;
        else if(Event.active.state==6)
            Temporary->Category=INFWINDOWACTIVE;*/
    default :
        break;
    }
    return Temporary;
}

#endif // SDLEVENTTOINFORMATION_H

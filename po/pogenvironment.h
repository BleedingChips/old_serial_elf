#ifndef POGENVIRONMENT_H
#define POGENVIRONMENT_H

#include <SDL2/SDL.h>

#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "poerror.h"
#include "language.h"
#include "picturemanager.h"
#include "fontmanager.h"
inline unsigned long GetTicks(){return SDL_GetTicks();}
inline void FrameworkDelay(const unsigned long& T){SDL_Delay(T);}

class POGEnvironment
{
    SDL_Window* Window(){return MainSurface;}
    SDL_Renderer* Renderer(){return MainRenderer;}
protected:
    int WindowW,WindowH;
    bool MouseLocked,WindowActive;
    SDL_Event Event;
    SDL_Thread* Thread1;
    SDL_Window* MainSurface;
    SDL_Renderer *MainRenderer;
    SDL_GLContext GLContext;
    bool FontInit;
    bool FontAvalible;
    std::string FontAdress;
    std::string Title;
    std::list<void(*)(void)> ExitList;

    bool GetEvent(Handle<Information>&);
    void ExitFunction(void (*K)(void)){ExitList.push_back(K);}
    void CleanScreen(){SDL_GL_SwapWindow(MainSurface);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);}
    void CreatWindow();
    void CreatThread(int (*)(void*),void*);
    void Set3D();
    void Set2D();
    void Exit();
    void LockMouseDetail(unsigned int K,unsigned int P){SDL_WarpMouseInWindow(MainSurface,K,P);}
public:

    /*Handle<GLTexture> CreatTTF(std::string K)
    {
        return CreatTTF(K,SDL_TTF_FONT);
    }
    Handle<GLTexture> CreatTTF(std::string K,TTF_Font* P)
    {
        return CreatTTFUTF8(Translate(K),P);
    }
    Handle<GLTexture> CreatTTFUTF8(std::string S,TTF_Font* P);
    Handle<GLTexture> CreatTTFUTF8(std::string P)
    {
        return CreatTTFUTF8(P,SDL_TTF_FONT);
    }*/
    static bool CreatTexture(SDL_Surface*,unsigned int& P,unsigned int& W,unsigned int& H);
    void SetFontAdress(const std::string& P){FontAdress=P;FontAvalible=true;}
    void Init();
    POGEnvironment(const std::string& Ti,const int& W,const int& H):Title(Ti),WindowW(W),WindowH(H),FontInit(false),FontAvalible(false),MouseLocked(false),WindowActive(true) {}
    POGEnvironment(const std::string& Ti,const int& W,const int& H,const std::string& FA):Title(Ti),WindowW(W),WindowH(H),FontInit(false),FontAvalible(true),FontAdress(FA),MouseLocked(false),WindowActive(true) {}
    void LockMouse(){MouseLocked=true;}
    void ReleaseMouse(){MouseLocked=false;}
    bool MouseFree()const{return !MouseLocked;}
    bool Active(){return WindowActive;}
};




/*Handle<GLTexture> POGEnvironment::CreatPicture(std::string K)
{
    std::map<std::string,Handle<GLTexture> >::iterator Po=GLTexture::TextureMap.find(K);

    if(Po!=GLTexture::TextureMap.end())
        return Po->second;
    else
    {
        SDL_Surface* Picture = IMG_Load(K.c_str());
        Handle<GLTexture> Tem=CreatTexture(Picture,K);
        SDL_FreeSurface(Picture);
        return Tem;
    }
}

Handle<GLTexture> POGEnvironment::CreatTTFUTF8(std::string S, TTF_Font *P)
{
    std::map<std::string,Handle<GLTexture> >::iterator Po=GLTexture::TextureMap.find(S);
    if(Po!=GLTexture::TextureMap.end())
        return Po->second;
    else
    {
        SDL_Surface* Picture = TTF_RenderUTF8_Blended(P,S.c_str(),(SDL_Color)
        {
            255,255,255
        } );
        Handle<GLTexture> Tem=CreatTexture(Picture,S);
        SDL_FreeSurface(Picture);
        return Tem;
    }
}


void POGEnvironment::TrackResource()
{
    if(GLTexture::Change)
    {
        for(std::list<std::string> ::iterator Po=GLTexture::DeleteList.begin(); Po!=GLTexture::DeleteList.end(); ++Po)
        {
            std::map<std::string,Handle<GLTexture> >::iterator Po2=GLTexture::TextureMap.find(*Po);
            if(Po2!=GLTexture::TextureMap.end())
            {
                if(Po2->second)
                {
                    if(Po2->second->C<=0)
                        GLTexture::TextureMap.erase(Po2);
                }
                else
                    GLTexture::TextureMap.erase(Po2);
            }
        }
        GLTexture::DeleteList.clear();
        GLTexture::Change=false;
    }
}*/


void POGEnvironment::Set3D()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70,static_cast<float>(WindowW)/WindowH,0.05,1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void POGEnvironment::Set2D()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0,1.0,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void POGEnvironment::Exit()
{
    int Resoult;
    SDL_WaitThread(Thread1,&Resoult);
    //GLTexture::CleanAll();
    /*if(FontAvalible)
        TTF_CloseFont(SDL_TTF_FONT);*/
    for(std::list<void(*)(void)>::reverse_iterator Po=ExitList.rbegin(); Po!=ExitList.rend(); ++Po)
        (*Po)();
    SDL_DestroyRenderer(MainRenderer);
    SDL_DestroyWindow(MainSurface);
    ExitList.clear();
}

bool POGEnvironment::GetEvent(Handle<Information>& I)
{
    if(SDL_PollEvent(&Event))
    {
        I.Unused();
        I=new Information;
        (*I)=Event;
        return true;
    }
    return false;
}


void POGEnvironment::CreatThread(int (*F)(void*),void* K)
{
    Thread1=SDL_CreateThread(F,"POMultyThread",K);
}

void POGEnvironment::Init()
{
    SDL_ClearError();
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) //SDL³õÊ¼»¯Ê§°Ü
        POError::GetError("There is Something Wrong!! The SDL Can Not Init As Your Wish.Please Check your Developmental Environmrnt./n Or May Be You Can Get Something From Here <"+std::string(SDL_GetError()),-1);
    if(IMG_Init(~0)<0)
        POError::GetError(std::string("The SDL_image can not Init as your wish, Or may be you can get something from here:/n")+SDL_GetError(),-1);
    if(TTF_Init()<0)
        POError::GetError(std::string("The SDL_TTF can not Init as your wish, Or may be you can get something from here:/n")+SDL_GetError(),-1);
    ExitFunction(SDL_Quit);
    ExitFunction(IMG_Quit);
    ExitFunction(TTF_Quit );
    ExitFunction(FontManager::FontManagerExit);
    ExitFunction(PictureManager::TextureMangerExit);
    CreatWindow();
}


void POGEnvironment::CreatWindow()
{
    if(!FontInit)
    {
        if(FontAvalible)
        {
            FontManager::CreatDefaultFont(FontAdress);
            FontInit=true;
        }
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    MainSurface= SDL_CreateWindow(Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,WindowW,WindowH,SDL_WINDOW_OPENGL );
    MainRenderer=SDL_CreateRenderer(MainSurface, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    GLContext = SDL_GL_CreateContext(MainSurface);
    SDL_GL_SetSwapInterval(1);
}

#endif // POGENVIRONMENT_H

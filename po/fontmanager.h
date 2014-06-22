#ifndef FONTMANAGER_H
#define FONTMANAGER_H
#include <map>
#include <vector>
#include <string>
#include "gltexture.h"
#include "utf8string.h"
#include <SDL2/SDL_ttf.h>
namespace FontManager
{
    typedef std::vector<float> FloatArray;
    std::map<std::string,TTF_Font*> AllFont;
    TTF_Font* Default;
    bool UseDefaultFont=false;

    bool CreatFont(const std::string& Adress,const std::string& Define,unsigned int Size=60);
    bool CreatDefaultFont(const std::string& Adress,unsigned int Size=60);
    bool HaveFont(const std::string& U)       {return AllFont.find(U)!=AllFont.end();}
    bool HaveDefault()            {return UseDefaultFont;}
    TTF_Font* DefaultFont()    {return Default;}
    void FontManagerExit();

    class StaticTextData:public GLTexture
    {
        unsigned int CountChar;
        unsigned int CountWidth;
    public:
        unsigned int GetChar()   const   {return CountChar;}
        unsigned int GetWidth()  const   {return CountWidth;}
        void Set(unsigned int P,unsigned int W,unsigned int H,unsigned int C,unsigned int Wi)   {GLTexture::Set(P,W,H);CountChar=C;CountWidth=Wi;}
        StaticTextData(unsigned int P,unsigned int W,unsigned int H,unsigned int C,unsigned int Wi):GLTexture(P,W,H),CountChar(C),CountWidth(Wi){}
        StaticTextData(const StaticTextData& P):GLTexture(P),CountChar(P.CountChar),CountWidth(P.CountWidth){}
        void FreeText(){GLTexture::FreeTexture();}
    };

    bool CreatText(const std::string& P,StaticTextData& T);
    bool CreatText(const std::string& P,TTF_Font* Font,StaticTextData& T);
    bool CreatText(const std::string& P,const std::string& FontName,StaticTextData& T);


    class DynamicTextData:public GLTexture
    {
        unsigned int CountWidth;
    public:
        unsigned int GetWidth()  const   {return CountWidth;}
        DynamicTextData(unsigned int P,unsigned int W,unsigned int H,unsigned int Wi):GLTexture(P,W,H),CountWidth(Wi){}
        DynamicTextData(const DynamicTextData& K):GLTexture(K),CountWidth(K.CountWidth){}
        DynamicTextData():GLTexture(0,0,0),CountWidth(0){}
        void FreeText(){GLTexture::FreeTexture();}
        void Set(unsigned int P,unsigned int W,unsigned int H,unsigned int Wi){GLTexture::Set(P,W,H);CountWidth=Wi;}
    };
    std::map<unsigned int,DynamicTextData> IntDynamicMap;
    std::map<std::string,DynamicTextData> StringDynamicMap;
    bool CreatDynamicText(const std::string&,FloatArray&);
    bool CreatDynamicText(const std::string& U);
    bool GetDynamicData(unsigned int P,unsigned int& I,unsigned int& T);
    bool GetDynamicData(const std::string& P,unsigned int& I,unsigned int& T);
};

bool FontManager::CreatDefaultFont(const std::string& Adress,unsigned int Size)
{
    if(UseDefaultFont)
    {
        TTF_CloseFont(Default);
        UseDefaultFont=false;
    }
    Default=TTF_OpenFont(Adress.c_str(),Size);
    if(Default)
        UseDefaultFont=true;
    return UseDefaultFont;
}

bool FontManager::CreatFont(const std::string& Adress,const std::string& Define, unsigned int Size)
{
    using namespace std;
    map<string,TTF_Font*>::iterator Po=AllFont.find(Define);
    if(Po==AllFont.end())
    {
        TTF_Font* Go=TTF_OpenFont(Adress.c_str(),Size);
        AllFont.insert(make_pair(Define,Go));
        return true;
    }
    return false;
}

void FontManager::FontManagerExit()
{
    using namespace std;
    for(map<string,TTF_Font*>::iterator Po=AllFont.begin();Po!=AllFont.end();++Po)
        TTF_CloseFont(Po->second);
    AllFont.clear();
    if(UseDefaultFont)
    {
        TTF_CloseFont(Default);
        UseDefaultFont=false;
    }
    for(map<unsigned int,DynamicTextData>::iterator Po=IntDynamicMap.begin();Po!=IntDynamicMap.end();++Po)
        Po->second.FreeText();
    IntDynamicMap.clear();
    StringDynamicMap.clear();
}

bool FontManager::CreatText(const std::string& P, const std::string& FontName, StaticTextData& T)
{
    using namespace std;
    T.FreeText();
    map<string,TTF_Font*>::iterator Po=AllFont.find(FontName);
    if(Po!=AllFont.end())
        return CreatText(P,Po->second,T);
    return false;
}

bool FontManager::CreatText(const std::string& P,TTF_Font* Font, StaticTextData& T)
{
    T.FreeText();
    SDL_Surface* Picture = TTF_RenderUTF8_Blended(Font,P.c_str(),(SDL_Color){255,255,255} );
    if(Picture)
    {
        T.Set(0,0,0,UTF8String::CountChar(P),UTF8String::CountWidth(P));
        bool Ready=GLTexture::CreatTexture(Picture,T);
        SDL_FreeSurface(Picture);
        return Ready;
    }
    return false;
}

bool FontManager::CreatText(const std::string& P, StaticTextData& T)
{
    T.FreeText();
    if(UseDefaultFont)
        return CreatText(P,Default,T);
    return false;
}

bool FontManager::CreatDynamicText(const std::string& K,FloatArray& PV)
{
    using namespace UTF8String;
    using namespace std;
    CharDeque CD;
    Separate(K,CD);
    for(CharDeque::iterator Po=CD.begin();Po!=CD.end();++Po)
    {
        map<string,DynamicTextData>::iterator Po2=StringDynamicMap.find(*Po);
        if(Po2!=StringDynamicMap.end())
        {
            PV.push_back(Po2->second.GetPoint());
        }else{
            if(UseDefaultFont)
            {
                unsigned int P=0,W,H;
                SDL_Surface* Picture = TTF_RenderUTF8_Blended(Default,Po->c_str(),(SDL_Color){255,255,255} );
                if(Picture)
                {
                    bool Ready=GLTexture::CreatTexture(Picture,P,W,H);
                    SDL_FreeSurface(Picture);
                    if(!Ready)
                        return false;
                    else{
                        DynamicTextData DTD(P,W,H,CountWidth(*Po));
                        StringDynamicMap.insert(make_pair(*Po,DTD));
                        map<unsigned int,DynamicTextData>::iterator Po3=IntDynamicMap.find(P);
                        if(Po3==IntDynamicMap.end())
                            IntDynamicMap.insert(make_pair(P,DTD));
                        else
                            IntDynamicMap[P].Set(DTD.GetPoint(),DTD.GetTextureW(),DTD.GetTextureH(),DTD.GetWidth());
                        PV.push_back(P);
                    }
                }else return false;
            }else return false;
        }
    }
    return true;
}

bool FontManager::GetDynamicData(unsigned int P,unsigned int& I,unsigned int& T)
{
    std::map<unsigned int,DynamicTextData>::iterator Po=IntDynamicMap.find(P);
    if(Po==IntDynamicMap.end())
        return false;
    I=Po->second.GetPoint();
    T=Po->second.GetWidth();
    return true;
}

bool FontManager::GetDynamicData(const std::string& P,unsigned int& I,unsigned int& T)
{
    std::map<std::string,DynamicTextData>::iterator Po=StringDynamicMap.find(P);
    if(Po==StringDynamicMap.end())
        return false;
    I=Po->second.GetPoint();
    T=Po->second.GetWidth();
    return true;
}

bool FontManager::CreatDynamicText(const std::string& K)
{
    using namespace UTF8String;
    using namespace std;
    CharDeque CD;
    Separate(K,CD);
    for(CharDeque::iterator Po=CD.begin();Po!=CD.end();++Po)
    {
        map<string,DynamicTextData>::iterator Po2=StringDynamicMap.find(*Po);
        if(Po2==StringDynamicMap.end())
        {
            if(UseDefaultFont)
            {
                unsigned int P=0,W,H;
                SDL_Surface* Picture = TTF_RenderUTF8_Blended(Default,Po->c_str(),(SDL_Color){255,255,255} );
                if(Picture)
                {
                    bool Ready=GLTexture::CreatTexture(Picture,P,W,H);
                    SDL_FreeSurface(Picture);
                    if(!Ready)
                        return false;
                    else{
                        DynamicTextData DTD(P,W,H,CountWidth(*Po));
                        StringDynamicMap.insert(make_pair(*Po,DTD));
                        map<unsigned int,DynamicTextData>::iterator Po3=IntDynamicMap.find(P);
                        if(Po3==IntDynamicMap.end())
                            IntDynamicMap.insert(make_pair(P,DTD));
                        else
                            IntDynamicMap[P].Set(DTD.GetPoint(),DTD.GetTextureW(),DTD.GetTextureH(),DTD.GetWidth());
                    }
                }else return false;
            }else return false;
        }
    }
    return true;
}
#endif // FONTMANAGER_H_INCLUDED

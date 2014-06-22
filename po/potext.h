#ifndef POTEXT_H
#define POTEXT_H
#include "gltexture.h"
#include "baseclass.h"
#include "fontmanager.h"
#include "handle.h"
#include <sstream>
class POTextStatic:public FontManager::StaticTextData,public HandleSeed
{
public:
    operator bool(){return GLTexture::IsAvalible();}
    POTextStatic():StaticTextData(0,0,0,0,0){}
    POTextStatic(POTextStatic& I):StaticTextData(I){I.Set(0,0,0,0,0);}
    bool CreatText(const std::string& P,TTF_Font* F){return FontManager::CreatText(P,F,*this);}
    bool CreatText(const std::string& P){return FontManager::CreatText(P,*this);}
    void Display(const XYF& P,const XYF& T,float S=0.1, float R=1.0, float G=1.0, float B=1.0,float A=1.0);
    ~POTextStatic(){FontManager::StaticTextData::FreeText();}
    static void SetColor(float R=1.0, float G=1.0, float B=1.0,float A=1.0);
};

class POTextDynamic
{
protected:
    FontManager::FloatArray PA;
public:
    void Clear(){PA.clear();}
    bool CreatText(const std::string& P){Clear();return FontManager::CreatDynamicText(P,PA);}
    bool InsertText(const std::string& P){return FontManager::CreatDynamicText(P,PA);}
    operator bool(){return !PA.empty();}
    void Display(const XYF& P,const XYF& T,float S=0.1, float R=1.0, float G=1.0, float B=1.0,float A=1.0);
    unsigned int Width();
    unsigned int PopFront(unsigned int S=1);
    unsigned int PopBack(unsigned int S=1);
};

unsigned int POTextDynamic::PopFront(unsigned int S)
{
    unsigned int Size=PA.size();
    if(Size<=S)
    {
        PA.clear();
        return Size;
    }else{
        unsigned int Block=Size-S;
        for(int i=0;i<Block;++i)
            PA[i]=PA[S+i];
        PA.resize(Block);
        return S;
    }
};

unsigned int POTextDynamic::PopBack(unsigned int S)
{
    unsigned int Size=PA.size();
    if(Size<=S)
    {
        PA.clear();
        return Size;
    }else{
        PA.resize(Size-S);
        return Size-S;
    }
};
class PONumber:public POTextDynamic
{
    static std::stringstream StaticStream;
public:
    template<typename Type>
    void Display(const Type& P,const XYF&,const XYF&,float S=0.1, float R=1.0, float G=1.0, float B=1.0,float A=1.0);
};
std::stringstream PONumber::StaticStream;

unsigned int POTextDynamic::Width()
{
    unsigned int CW=0;
    unsigned int Point,W;
    for(FontManager::FloatArray::iterator Po=PA.begin();Po!=PA.end();++Po)
        if(FontManager::GetDynamicData(*Po,Point,W))
            CW+=W;
    return CW;
}





template<typename Type>
void PONumber::Display(const Type& P,const XYF& PD,const XYF& TD,float S, float R, float G, float B,float A)
{
    StaticStream.clear();
    std::string Tem;
    StaticStream<<P;
    StaticStream>>Tem;
    CreatText(Tem);
    POTextDynamic::Display(PD,TD,S,R,G,B,A);
}

void POTextStatic::Display(const XYF& P,const  XYF& T,float S, float R, float G, float B,float A)
{
    bool Shendu=glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    XYF Tem=T.Uint(S*0.4*GetWidth());
    XYF Tem2(Tem.Y,-Tem.X);
    Tem2=Tem2.Uint(S);
    P.OutFloat(GLTexture::Location);
    (P+Tem2).OutFloat(GLTexture::Location+2);
    (P+Tem+Tem2).OutFloat(GLTexture::Location+4);
    (P+Tem).OutFloat(GLTexture::Location+6);
    GLTexture::SetColor(R,G,B,A);
    GLTexture::Display(GLTexture::Texture,2,GLTexture::Location,2,GLTexture::Color,4,GLTexture::Order,6);
    if(!Shendu)
        glDisable(GL_BLEND);
}

void POTextDynamic::Display(const XYF& PD,const XYF& TD,float S, float R, float G, float B,float A)
{
    glVertexPointer(2,GL_FLOAT,0,GLTexture::Location);
    glTexCoordPointer(2,GL_FLOAT,0,GLTexture::Texture);
    glColorPointer(4,GL_FLOAT,0,GLTexture::Color);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    GLTexture::SetColor(R,G,B,A);
    bool Shendu=glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    XYF P=PD;
    XYF T=TD;
    XYF D(T.Y,-T.X);
    D=D.Uint(S);
    T=T.Uint(S*0.4);

    for(FontManager::FloatArray::iterator Po=PA.begin();Po!=PA.end();++Po)
    {
        unsigned int Point,W;
        if(FontManager::GetDynamicData(*Po,Point,W))
        {
            P.OutFloat(GLTexture::Location);
            (P+D).OutFloat(GLTexture::Location+2);
            (P+D+W*T).OutFloat(GLTexture::Location+4);
            (P+W*T).OutFloat(GLTexture::Location+6);
            P=P+W*T;
            glBindTexture(GL_TEXTURE_2D,Point);
            glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT,GLTexture::Order);
        }else{
            P=P+T;
        }
    }
    //std::cout<<std::endl;
    glDisable(GL_TEXTURE_2D);
    if(!Shendu)
        glDisable(GL_BLEND);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    //glDisableClientState(GL_COLOR_ARRAY);
}
/*void POTTF::Display(const XYZF& P, const XYZF& T,const XYZF& D,float S, float R, float G, float B,float A)
{
    bool Shendu=glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    XYZF Tem=T.Uint(S*0.4*Data->Size);
    XYZF Tem2=D.Uint(S);
    SetPoint(&Lx[0],P);
    SetPoint(&Lx[3],P+Tem2);
    SetPoint(&Lx[6],P+Tem+Tem2);
    SetPoint(&Lx[9],P+Tem);
    glEnableClientState(GL_COLOR_ARRAY);
    SetColor(R,G,B,A);
    glColorPointer(4,GL_FLOAT,0,DefineColor);
    Data->Display(Tx,2,Lx,3,K,6);
    glDisableClientState(GL_COLOR_ARRAY);
    if(!Shendu)
        glDisable(GL_BLEND);
}
/*void POTTF::DisplayMiddle(const XYF& P,const  XYF& T, float S, float R, float G, float B,float A)
{
    XYF Tem=P-T.Uint(Data->Size/2.0*S*0.4);
    Display(Tem,T,S,R,G,B,A);
}
void POTTF::DisplayMiddle(const XYZF &P, const XYZF &T, const XYZF &D, float S, float R, float G, float B, float A)
{
    XYZF Tem=P-T.Uint(Data->Size/2.0*S*0.4);
    Display(Tem,T,D,S,R,G,B,A);
}*/
#endif // POTTF_H

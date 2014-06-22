#ifndef DIALOG_H
#define DIALOG_H
#include "potext.h"
#include "language.h"
#include "utf8string.h"
class StaticDialog
{
    unsigned int LimitedWidth;
    std::deque<Handle<POTextStatic> > AllData;
public:
    void Insert(const std::string& J);
    unsigned int Line(){return AllData.size();}
    StaticDialog(unsigned int W):LimitedWidth(W){}
    void Display(float SX,float EX,float SY,float R=1.0, float B=1.0, float G=1.0, float A=1.0);
    void Clear(){AllData.clear();}
    unsigned int CountLine(){return AllData.size();}
    void PopFront(){if(!AllData.empty()) AllData.pop_front();}
    void PopBack(){if(!AllData.empty()) AllData.pop_back();}
};
class DynamicDialog:public POTextDynamic
{
public:
    void Display(float SX,float EX,float SY,unsigned int LW,float R=1.0, float B=1.0, float G=1.0, float A=1.0);
    bool Insert(const std::string& D) {return POTextDynamic::InsertText(D);}
    unsigned int CountLine(unsigned int P);
};
class EditDialog
{
    std::list<std::string> Data;
    std::list<std::string>::iterator Cursor;
    unsigned long Time;
public:
    EditDialog():Time(0){Cursor=Data.end();}
    void Insert(const std::string& P){UTF8String::CharDeque CD;UTF8String::Separate(P,CD);for(UTF8String::CharDeque::iterator Po=CD.begin();Po!=CD.end();++Po){Data.insert(Cursor,*Po);}}
    void CursorBack();
    void CursorFront();
    void Delete();
    void Replace(const std::string& I);
    std::string GetString();
    void Display(float SX,float EX,float SY,unsigned int W,unsigned long Interval,float R,float G,float B,float A);
    void Clear(){Data.clear();Cursor=Data.end();}
};

void EditDialog::CursorBack()
{
    if(Cursor!=Data.end())
        ++Cursor;
}
void EditDialog::CursorFront()
{
    if(Cursor!=Data.begin())
        --Cursor;
}
void EditDialog::Delete()
{
    std::list<std::string>::iterator Po=Cursor;
    if(--Po!=Data.end())
        Data.erase(Po);
}



void EditDialog::Replace(const std::string& U)
{
    if(Cursor==Data.end())
        Insert(U);
    else{
        *Cursor=U;
        ++Cursor;
    }
}

std::string EditDialog::GetString()
{
    std::string Tem;
    for(std::list<std::string>::iterator Po=Data.begin();Po!=Data.end();++Po)
        Tem+=*Po;
    return Tem;
}
unsigned int DynamicDialog::CountLine(unsigned int P)
{
    unsigned int CL=0,CW=0,Point,W;
    if(PA.empty())
        return 0;
    else{
        CL=1;
        for(FontManager::FloatArray::iterator Po=PA.begin(); Po!=PA.end(); ++Po)
            if(FontManager::GetDynamicData(*Po,Point,W))
            {
                CW+=W;
                if(CW>P)
                {
                    CL+=1;
                    CW=W;
                }
            }
    }
    return CL;
}

void StaticDialog::Insert(const std::string& BufferT)
{
    std::string Buffer=BufferT;
    unsigned int CW,CC;
    std::string Tem;
    while(1)
    {
        Tem=UTF8String::GetTextWidthCut(Buffer,LimitedWidth,CC,CW);
        if(CW==0)
            break;
        else
        {
            Handle<POTextStatic> Han=new POTextStatic;
            Han->CreatText(Tem);
            AllData.push_back(Han);
        }
    }
}
void StaticDialog::Display(float SX,float EX,float SY, float R, float B, float G, float A)
{
    float P=(EX-SX)/(LimitedWidth);
    float L=P/0.4;
    for(std::deque<Handle<POTextStatic> >::iterator Po=AllData.begin();Po!=AllData.end();++Po)
    {
        (*Po)->Display(XYF(SX,SY),XYF(1.0,0.0),L,R,B,G,A);
        SY-=L;
    }
}

void DynamicDialog::Display(float SX,float EX,float SY,unsigned int LW,float R, float B, float G, float A)
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

    float PE=(EX-SX)/LW;
    float L=PE/0.4;
    unsigned int WC=0;
    XYF P(SX,SY);
    XYF T(PE,0.0);
    XYF D(0.0,-L);
    for(FontManager::FloatArray::iterator Po=PA.begin();Po!=PA.end();++Po)
    {
        unsigned int Point,W;
        if(FontManager::GetDynamicData(*Po,Point,W))
        {
            WC+=W;
            if(WC>LW)
            {
                WC=W;
                P.X=SX;
                P.Y-=L;
            }
            P.OutFloat(GLTexture::Location);
            (P+D).OutFloat(GLTexture::Location+2);
            (P+D+W*T).OutFloat(GLTexture::Location+4);
            (P+W*T).OutFloat(GLTexture::Location+6);
            P.X+=W*PE;
            glBindTexture(GL_TEXTURE_2D,Point);
            glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT,GLTexture::Order);
        }else{
            P.X+=PE;
            WC+=1;
            if(WC>LW)
            {
                WC=1;
                P.X=SX;
                P.Y-=L;
            }
        }
    }
    glDisable(GL_TEXTURE_2D);
    if(!Shendu)
        glDisable(GL_BLEND);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}

void EditDialog::Display(float SX,float EX,float SY,unsigned int LW,unsigned long Interval,float R,float G,float B,float A)
{
    GLTexture::StartTextureWithColor(2,2,4);
    GLTexture::SetColor(R,G,B,A);
    bool Shendu=glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    Time+=Interval;
    float PE=(EX-SX)/LW;
    float L=PE/0.4;
    unsigned int WC=0;
    XYF P(SX,SY);
    XYF T(PE,0.0);
    XYF D(0.0,-L);
    XYF TemCur(P);
    for(std::list<std::string>::iterator Po=Data.begin();Po!=Data.end();++Po)
    {
        unsigned int Point,W;
        if(FontManager::GetDynamicData(*Po,Point,W)||(FontManager::CreatDynamicText(*Po)&&FontManager::GetDynamicData(*Po,Point,W)))
        {
            if(Po==Cursor)
                TemCur=P;
            WC+=W;
            if(WC>LW)
            {
                WC=W;
                P.X=SX;
                P.Y-=L;
            }
            P.OutFloat(GLTexture::Location);
            (P+D).OutFloat(GLTexture::Location+2);
            (P+D+W*T).OutFloat(GLTexture::Location+4);
            (P+W*T).OutFloat(GLTexture::Location+6);
            P.X+=W*PE;
            glBindTexture(GL_TEXTURE_2D,Point);
            glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT,GLTexture::Order);
        }else{
            if(Po==Cursor)
                TemCur=P;
            P.X+=PE;
            WC+=1;
            if(WC>LW)
            {
                WC=1;
                P.X=SX;
                P.Y-=L;
            }
        }
    }
    glDisable(GL_TEXTURE_2D);
    if(Time<600)
    {
        if(Cursor==Data.end())
            TemCur=P;
        TemCur.OutFloat(GLTexture::Location);
        (TemCur+D).OutFloat(GLTexture::Location+2);
        (TemCur+D+0.2*T).OutFloat(GLTexture::Location+4);
        (TemCur+0.2*T).OutFloat(GLTexture::Location+6);
        glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT,GLTexture::Order);
    }else if(Time>1200)
        Time=0;
    if(!Shendu)
        glDisable(GL_BLEND);
    GLTexture::CloseTextureWithColor();
}
#endif // DIALOG_H

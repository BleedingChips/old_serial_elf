#ifndef STATICDIALOG_H
#define STATICDIALOG_H
#include "pottf.h"
#include "pomain.h"
#include "language.h"
class StaticDialog
{
    std::list<std::string> AS;
    std::list<POTTF> AD;
    int NumberOfLine;
    int MaxCha;
    bool Change;
public:
    operator bool()
    {
        return !AS.empty();
    }
    void Clear()
    {
        AS.clear();
        AD.clear();
        NumberOfLine=0;
    }
    void Init(POMain* P);
    void Add(std::string);
    void AddLine(std::string);
    void MaxChar(int K)
    {
        MaxCha=K;
    }
    int MaxChar()
    {
        return MaxCha;
    }
    int Line()
    {
        return NumberOfLine;
    }
    void Display(XYF S,XYF E,float R=1.0,float B=1.0,float G=1.0,float A=1.0);
    Dialog():MaxCha(34)
    {
        Clear();
    }
    static std::string Cut(std::string::iterator&,std::string::iterator,int S);
};

void Dialog::AddLine(std::string Kc)
{
    Kc=Translate(Kc);
    std::string::iterator L=Kc.begin();
    while(L!=Kc.end())
    {
        std::string K=Dialog::Cut(L,Kc.end(),MaxCha);
        NumberOfLine+=1;
        AS.push_back(K);
    }
    Change=true;
}

void Dialog::Add(std::string K)
{
    K=Translate(K);
    if(!AS.empty())
    {
        std::string C=*AS.rbegin();
        AS.pop_back();
        AD.pop_back();
        K=C+K;
    }
    std::string::iterator L=K.begin();
    while(L!=K.end())
    {
        std::string TemK=Dialog::Cut(L,K.end(),MaxCha);
        NumberOfLine+=1;
        AS.push_back(TemK);
    }
    Change=true;
}
std::string Dialog::Cut(std::string::iterator& B, std::string::iterator E, int S)
{
    std::string::iterator Po=B;
    int CharSpa=0;
    int LastChar=0;
    int LineChar=0;
    for(; Po!=E; ++Po)
    {
        if(*Po<0)
        {
            if(*Po>(int)0xfffffffc)
                CharSpa=6;
            else if(*Po>(int)0xfffffff8)
                CharSpa=5;
            else if(*Po>(int)0xfffffff0)
                CharSpa=4;
            else if(*Po>(int)0xffffffe0)
                CharSpa=3;
            else if(*Po>(int)0xffffffc0)
                CharSpa=2;
            LastChar=2;
        }
        else
        {
            if(*Po=='\n')
            {
                std::string::iterator Pc=B;
                B=Po+1;
                return std::string(Pc,Po+1);
            }
            CharSpa=1;
            LastChar=1;
        }
        if(LineChar+LastChar>S)
        {
            std::string::iterator Pc=B;
            B=Po;
            return std::string(Pc,Po);
        }
        else
        {
            LineChar+=LastChar;
        }
        Po+=CharSpa-1;
    }
    if(B!=E)
    {
        Po=B;
        B=E;
        return std::string(Po,E);
    }
}

void Dialog::Init(POMain *P)
{
    if(Change)
    {
        AD.clear();
        for(std::list<std::string>::iterator Po=AS.begin(); Po!=AS.end(); ++Po)
        {
            AD.push_back(P->CreatTTFUTF8(*Po));
        }
    }
    Change=false;
}

void Dialog::Display(XYF S, XYF E, float R, float B, float G, float A)
{
    if(S.X<E.X&&S.Y>E.Y)
    {
        float Tc=(E.X-S.X)/(MaxCha)/0.4;
        float Sp=S.Y;
        for(std::list<POTTF>::iterator Po=AD.begin(); Po!=AD.end(); ++Po)
        {
            if(Sp-Tc>=E.Y)
            {
                Po->Display(XYF(S.X,Sp),XYF(1.0,0.0),Tc,R,B,G);
                Sp-=Tc;
            }
            else
            {
                break;
            }
        }
    }
}


#endif // STATICDIALOG_H

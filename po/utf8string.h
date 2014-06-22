#ifndef UTF8STRING_H
#define UTF8STRING_H
#include <string>
#include <deque>
#include <list>
namespace UTF8String
{
    unsigned int CountWidth(const std::string&);
    unsigned int CountChar(const std::string&);
    std::string GetTextChar(const std::string&,unsigned int,unsigned int& C,unsigned int& W);
    std::string GetTextWidth(const std::string&,unsigned int, unsigned int& C,unsigned int& W);
    std::string GetTextCharCut(std::string& ,unsigned int ,unsigned int&,unsigned int& W);
    std::string GetTextWidthCut(std::string& ,unsigned int ,unsigned int&,unsigned int& W);
    inline std::string GetTextChar(const std::string& Data,unsigned int P){unsigned int K,Pc;return GetTextChar(Data,P,K,Pc);}
    inline std::string GetTextCharCut(std::string& Data,unsigned int P){unsigned int K,Pc;return GetTextCharCut(Data,P,K,Pc);}
    unsigned int CutTextChar(std::string &,unsigned int );
    unsigned int CutTextWidth(std::string &,unsigned int );
    typedef std::deque<std::string> CharDeque;
    typedef std::list<std::string> CharList;
    unsigned int Separate(const std::string&,CharDeque&);
    unsigned int Separate(const std::string&,CharList&);
};

unsigned int UTF8String::CountChar(const std::string& Data)
{
    unsigned int CC=0;
    for(std::string::const_iterator Po=Data.begin(); Po<Data.end(); ++Po)
    {
        unsigned int P=static_cast<unsigned char>(*Po);
        if(P>=0x000000F0)
            Po+=3;
        else if(P>=0x000000e0)
            Po+=2;
        else if(P>=0x000000C0)
            Po+=1;
        CC+=1;
    }
    return CC;
}

unsigned int UTF8String::CountWidth(const std::string& Data)
{
    unsigned int CW=0;
    for(std::string::const_iterator Po=Data.begin(); Po<Data.end(); ++Po)
    {
        unsigned int P=static_cast<unsigned char>(*Po);
        if(P>=0x000000F0)
        {
            Po+=3;
            CW+=2;
        }else if(P>=0x000000e0)
        {
            Po+=2;
            CW+=2;
        }else if(P>=0x000000C0)
        {
            Po+=1;
            CW+=2;
        }else
        CW+=1;
    }
    return CW;
}

std::string UTF8String::GetTextChar(const std::string& Data,unsigned int TC,unsigned int& OC,unsigned int& OW)
{
    OC=0;
    OW=0;
    unsigned int CC=0,CW=0;
    for(std::string::const_iterator Po=Data.begin(),Po2;Po<Data.end();)
    {
        unsigned int P=static_cast<unsigned char>(*Po);
        Po2=Po;
        if(P>=0x000000F0)
        {
            Po+=4;
            CW+=2;
        }else if(P>=0x000000e0)
        {
            Po+=3;CW+=2;
        }else if(P>=0x000000C0)
        {
            Po+=2;CW+=2;
        }else{
            Po+=1;
            CW+=1;
        }
        CC+=1;
        if(CC>TC)
        {
            std::string Tem(Data.begin(),Po2);
            return Tem;
        }else if(CC==TC)
        {
            OC=CC;
            OW=CW;
            std::string Tem(Data.begin(),Po);
            return Tem;
        }
        OC=CC;
        OW=CW;
    }
    return Data;
}
std::string UTF8String::GetTextWidth(const std::string& Data,unsigned int TC,unsigned int& OC,unsigned int& OW)
{
    OC=0;
    OW=0;
    unsigned int CC=0,CW=0;
    for(std::string::const_iterator Po=Data.begin(),Po2;Po<Data.end();)
    {
        unsigned int P=static_cast<unsigned char>(*Po);
        Po2=Po;
        if(P>=0x000000F0)
        {
            Po+=4;
            CW+=2;
        }else if(P>=0x000000e0)
        {
            Po+=3;CW+=2;
        }else if(P>=0x000000C0)
        {
            Po+=2;CW+=2;
        }else{
            Po+=1;
            CW+=1;
        }
        CC+=1;
        if(CW>TC)
        {
            std::string Tem(Data.begin(),Po2);
            return Tem;
        }else if(CW==TC)
        {
            OC=CC;
            OW=CW;
            std::string Tem(Data.begin(),Po);
            return Tem;
        }
        OC=CC;
        OW=CW;
    }
    return Data;
}
std::string UTF8String::GetTextCharCut(std::string& Data,unsigned int TC,unsigned int& OC,unsigned int& OW)
{
    OC=0;
    OW=0;
    unsigned int CC=0;
    unsigned int CW=0;
    for(std::string::iterator Po=Data.begin(),Po2;Po<Data.end();)
    {
        unsigned int P=static_cast<unsigned char>(*Po);
        Po2=Po;
        if(P>=0x000000F0)
        {
            Po+=4;
            CW+=2;
        }else if(P>=0x000000e0)
        {
            Po+=3;CW+=2;
        }else if(P>=0x000000C0)
        {
            Po+=2;CW+=2;
        }else{
            Po+=1;
            CW+=1;
        }
        CC+=1;
        if(CC>TC)
        {
            std::string Tem(Data.begin(),Po2);
            std::string Tem2(Po2,Data.end());
            Data=Tem2;
            return Tem;
        }else if(CC==TC)
        {
            OC=CC;
            OW=CW;
            std::string Tem(Data.begin(),Po);
            std::string Tem2(Po,Data.end());
            Data=Tem2;
            return Tem;
        }
        OC=CC;
        OW=CW;
    }
    std::string Tem=Data;
    Data.clear();
    return Tem;
}


std::string UTF8String::GetTextWidthCut(std::string& Data,unsigned int TC,unsigned int& OC,unsigned int& OW)
{
    OC=0;
    OW=0;
    unsigned int CC=0;
    unsigned int CW=0;
    for(std::string::iterator Po=Data.begin(),Po2;Po<Data.end();)
    {
        unsigned int P=static_cast<unsigned char>(*Po);
        Po2=Po;
        if(P>=0x000000F0)
        {
            Po+=4;
            CW+=2;
        }else if(P>=0x000000e0)
        {
            Po+=3;CW+=2;
        }else if(P>=0x000000C0)
        {
            Po+=2;CW+=2;
        }else{
            Po+=1;
            CW+=1;
        }
        CC+=1;
        if(CW>TC)
        {
            std::string Tem(Data.begin(),Po2);
            std::string Tem2(Po2,Data.end());
            Data=Tem2;
            return Tem;
        }else if(CW==TC)
        {
            OC=CC;
            OW=CW;
            std::string Tem(Data.begin(),Po);
            std::string Tem2(Po,Data.end());
            Data=Tem2;
            return Tem;
        }
        OC=CC;
        OW=CW;
    }
    std::string Tem=Data;
    Data.clear();
    return Tem;
}


unsigned int UTF8String::CutTextChar(std::string& Data,unsigned int TC)
{
    unsigned int OC=0;
    unsigned int CC=0;
    for(std::string::iterator Po=Data.begin(),Po2;Po<Data.end();)
    {
        unsigned int P=static_cast<unsigned char>(*Po);
        Po2=Po;
        if(P>=0x000000F0)
            Po+=4;
        else if(P>=0x000000e0)
            Po+=3;
        else if(P>=0x000000C0)
            Po+=2;
        else
            Po+=1;
        CC+=1;
        if(CC>TC)
        {
            std::string Tem2(Po2,Data.end());
            Data=Tem2;
            return OC;
        }else if(CC==TC)
        {
            OC=CC;
            std::string Tem2(Po,Data.end());
            Data=Tem2;
            return OC;
        }
        OC=CC;
    }
    Data.clear();
    return OC;
}

unsigned int UTF8String::CutTextWidth(std::string& Data,unsigned int TC)
{
    unsigned int OC=0;
    unsigned int CC=0;
    for(std::string::iterator Po=Data.begin(),Po2;Po<Data.end();)
    {
        unsigned int P=static_cast<unsigned char>(*Po);
        Po2=Po;
        if(P>=0x000000F0)
        {
            Po+=4;CC+=2;
        }else if(P>=0x000000e0)
        {
            Po+=3;CC+=2;
        }else if(P>=0x000000C0)
        {
            Po+=2;CC+=2;
        }else{
            Po+=1;
            CC+=1;
        }
        if(CC>TC)
        {
            std::string Tem2(Po2,Data.end());
            Data=Tem2;
            return OC;
        }else if(CC==TC)
        {
            OC=CC;
            std::string Tem2(Po,Data.end());
            Data=Tem2;
            return OC;
        }
        OC=CC;
    }
    Data.clear();
    return OC;
}


unsigned int UTF8String::Separate(const std::string& Data,CharDeque& CL)
{
    unsigned int CC=0;
    for(std::string::const_iterator Po=Data.begin(),Po2;Po<Data.end();)
    {
        unsigned int P=static_cast<unsigned char>(*Po);
        Po2=Po;
        if(P>=0x000000F0)
            Po+=4;
        else if(P>=0x000000e0)
            Po+=3;
        else if(P>=0x000000C0)
            Po+=2;
        else
            Po+=1;
        CC+=1;
        std::string Tem(Po2,Po);
        CL.push_back(Tem);
    }
    return CC;
}

unsigned int UTF8String::Separate(const std::string& Data,CharList& CL)
{
    unsigned int CC=0;
    for(std::string::const_iterator Po=Data.begin(),Po2;Po<Data.end();)
    {
        unsigned int P=static_cast<unsigned char>(*Po);
        Po2=Po;
        if(P>=0x000000F0)
            Po+=4;
        else if(P>=0x000000e0)
            Po+=3;
        else if(P>=0x000000C0)
            Po+=2;
        else
            Po+=1;
        CC+=1;
        std::string Tem(Po2,Po);
        CL.push_back(Tem);
    }
    return CC;
}
#endif // UTF8STRING_H

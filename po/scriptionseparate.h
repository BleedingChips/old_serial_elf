#ifndef POSCRIPTION_H
#define POSCRIPTION_H
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <fstream>
#define SPE_NUL 0
#define SPE_SYM 1
#define SPE_NUM 2
#define SPE_CHA 3
#define SPE_SPE 4
#define SPE_CL 5
#define SPE_CON 6
#define SPE_BBL 7
#define SPE_BBR 8
#define SPE_SBR 9
#define SPE_SBL 10
#define SPE_COM 11
#define SPE_SEM 12
#define SPE_POI 13
#include <vector>
#include "scriptionanalyze.h"
#define GetError(A,B) ScriptionGetError(A,B)
class ScriptionError
{
public:
    int Line;
    std::string Name;
    ScriptionError(const ScriptionError& T):Line(T.Line),Name(T.Name) {}
    ScriptionError(ScriptionError& T):Line(T.Line),Name(T.Name) {}
    ScriptionError(std::string N,int L):Name(N),Line(L) {}
};

std::list<ScriptionError> AllErrorList;

void ScriptionGetError(std::string N,int L)
{
    ScriptionError Tem(N,L);
    AllErrorList.push_back(Tem);
}

bool HaveScriptionError()
{
    return !AllErrorList.empty();
}
void DisplayError(std::fstream& Tem,std::string Pre="")
{
    for(std::list<ScriptionError>::iterator Po=AllErrorList.begin(); Po!=AllErrorList.end(); ++Po)
    {
        Tem<<Pre<<Po->Name;
        if(Po->Line>0)
            Tem<<"In Line <"<<Po->Line<<">";
        Tem<<"."<<std::endl;
    }
}



class ScriptionFactor
{
public:
    int mLine;
    std::string Data;
    int Mode;
    int Line()
    {
        return mLine;
    }
    int Size()
    {
        return Data.size();
    }
    std::string Detail()
    {
        return Data;
    }
    const char* c_str()
    {
        return Data.c_str();
    }
    operator std::string()
    {
        return Data;
    }
    ScriptionFactor operator=(ScriptionFactor& Tem)
    {
        mLine=Tem.mLine;
        Data=Tem.Data;
        return *this;
    }
    bool operator==(std::string T)
    {
        return Data==T;
    }
    bool operator!=(std::string K)
    {
        return Data!=K;
    }
    bool Is(std::string K)
    {
        return K==Data;
    }
    bool IsNumber()
    {
        return Mode==SPE_NUM;
    }
    bool IsSymple()
    {
        return Mode==SPE_SYM;
    }
    ScriptionFactor operator=(std::string K)
    {
        Data=K;
        return *this;
    }
    ScriptionFactor(std::string D,int L=0,int M=0):Data(D),mLine(L),Mode(M) {}
    ScriptionFactor(ScriptionFactor& Tem):mLine(Tem.mLine),Data(Tem.Data),Mode(Tem.Mode) {}
    ScriptionFactor(const ScriptionFactor& Tem):mLine(Tem.mLine),Data(Tem.Data),Mode(Tem.Mode) {}
};
typedef std::list<ScriptionFactor>::iterator FileFactorPoint;
typedef std::list<ScriptionFactor>::reverse_iterator FileFactorReversePoint;

class FileFactor
{
    std::string FileName;
    FileFactor operator=(FileFactor&) {}
public:
    std::list<ScriptionFactor> FileDetail;
    std::string Adress()
    {
        return FileName;
    }
    void SetAdress(std::string K)
    {
        FileName=K;
    }
    FileFactor() {}
    FileFactor(FileFactor& Tem):FileName(Tem.FileName),FileDetail(Tem.FileDetail) {}
    //bool IsEnd(){return Point==FileDetail.end();}
    void Clear()
    {
        FileDetail.clear();
    }
    void Push(ScriptionFactor& Tem)
    {
        FileDetail.push_back(Tem);
    }
    FileFactorPoint Begin()
    {
        return FileDetail.begin();
    }
    FileFactorPoint End()
    {
        return FileDetail.end();
    }
    FileFactorReversePoint RBegin()
    {
        return FileDetail.rbegin();
    }
    FileFactorReversePoint REnd()
    {
        return FileDetail.rend();
    }
    void Push(std::string D,int L,int M)
    {
        ScriptionFactor Tem(D,L,M);
        Push(Tem);
    }
    void Separate(std::string FileAdress);
};

class ScriptionChar
{
    char TemC;
public:
    int Style();
    char operator= (char K)
    {
        TemC=K;
        return K;
    }
    operator char()
    {
        return TemC;
    }
    ScriptionChar(char Tem=0):TemC(Tem) {}
    bool IsAvalible()
    {
        int M=Style();
        if(M!=SPE_NUL&&M!=SPE_SPE&&M!=SPE_CL)return true;
        return false;
    }
    bool IsSpace()
    {
        if(Style()==SPE_SPE)return true;
        return false;
    }
    bool IsChangeLine()
    {
        if(Style()==SPE_CL)return true;
        return false;
    }
    bool IsUnUse()
    {
        if(Style()==SPE_NUL)return true;
        return false;
    }
    bool operator==(char K)
    {
        if(TemC==K)return true;
        return false;
    }
};

void FileFactor::Separate(std::string FileAdress)
{
    Clear();
    std::ifstream File(FileAdress.c_str());
    int Line=1;
    if(File)
    {
        std::string TemS;
        int Mode=SPE_NUL;
        ScriptionChar TemC;
        bool UsingLastChar=false;
        TemC=File.get();
        while(!File.eof())
        {
            if(Mode==SPE_CON)
            {
                if(TemC!=']')
                {
                    if(TemC.IsChangeLine())
                        Line+=1;
                    TemS+=TemC;
                }
                else
                {
                    Mode=SPE_NUL;
                    Push(TemS,Line,Mode);
                    TemS.clear();
                }
            }
            else if(Mode==SPE_NUL)
            {
                if(TemC.IsChangeLine())
                    Line+=1;
                else if(!TemC.IsSpace())
                {
                    if(TemC=='[')
                    {
                        Mode=SPE_CON;
                    }
                    else
                    {
                        Mode=TemC.Style();
                        TemS+=TemC;
                    }
                }
            }
            else
            {
                if(!TemC.IsAvalible())
                {

#ifndef _WIN32_
                    if(TemC.IsChangeLine())
                    {
                        Line+=1;
                    }
#endif

                    Push(TemS,Line,Mode);
                    Mode=SPE_NUL;
                    TemS.clear();
                }
                else
                {
                    if(TemC.Style()!=Mode)
                    {
                        if(Mode==SPE_NUM&&TemC=='.')
                        {
                            TemS+=TemC;
                        }
                        else if(TemC.Style()==SPE_NUM&&Mode==SPE_CHA)
                        {
                            TemS+=TemC;
                        }
                        else
                        {
                            UsingLastChar=true;
                            Push(TemS,Line,Mode);
                            Mode=SPE_NUL;
                            TemS.clear();
                        }
                    }
                    else
                    {
                        TemS+=TemC;
                    }
                }
            }
            if(UsingLastChar)
            {
                UsingLastChar=false;
            }
            else
                TemC=File.get();
        }
        if(Mode!=SPE_NUL)
        {
            Push(TemS,Line,Mode);
        }
    }
    else
    {
        GetError("Can not open  file <"+FileAdress+">, please check",-1);
    }
}

std::string Unite(FileFactorPoint S,FileFactorPoint E)
{
    std::string Tem;
    for(; S!=E;)
    {
        Tem+=*S;
        ++S;
        if(S!=E)
            Tem+=" ";
    }
    return Tem;
}

/*std::string Unite(FileFactorReversePoint S,FileFactorReversePoint E)
{
    std::string Tem;
    for(;S!=E;)
    {
        Tem=(*S)+Tem;
        ++S;
        if(S!=E)
            Tem="_"+Tem;
    }
    return Tem;
}*/

int ScriptionChar::Style()
{
    if(TemC==0)
        return SPE_NUL;
    else if(TemC==' '||TemC=='\t'||TemC=='\r')
        return SPE_SPE;
    else if(TemC=='\n')
        return SPE_CL;
    else if((TemC>='a'&&TemC<='z')||(TemC>='A'&&TemC<='Z')||TemC=='_')
        return SPE_CHA;
    else if((TemC>='0'&&TemC<='9'))
        return SPE_NUM;
    else if(TemC=='{')
        return SPE_BBL;
    else if(TemC=='}')
        return SPE_BBR;
    else if(TemC=='(')
        return SPE_SBL;
    else if(TemC==')')
        return SPE_SBR;
    else if(TemC==',')
        return SPE_COM;
    else if(TemC==';')
        return SPE_SEM;
    else if(TemC=='.')
        return SPE_POI;
    else return SPE_SYM;
}
#endif // POSCRIPTION_H

#ifndef SERIALTEMINAL_H
#define SERIALTEMINAL_H
#include "poserial.h"
#include "../po/scriptionanalyze.h"
#include "../po/handle.h"
#include "../po/scriptionblock.h"
#include "teminaldata.h"
#include <sstream>
#include <deque>
#include <string>
class SerialTeminal
{
    typedef std::map<std::string,SerialProtocol > ProtocolMap;
    typedef std::map<std::string,SerialData > DataMap;
    typedef std::map<std::string,std::string> CommentMap;
    static std::stringstream Translate;
    bool Wait;
    POSerial POS;
    std::deque<std::string> CommentReport;
    std::deque<std::string> CommentList;
    ProtocolMap Protocol;
    DataMap Data;
    CommentMap CM;
    bool UseProtocol;
    std::string ProtocolName;
public:
    bool ComReady(){return POS;}
    void PushCommentOut(const std::string& P)
    {
        CommentReport.push_back(P);
        if(CommentReport.size()>=50)CommentReport.pop_front();
    }
    bool GetCommentOut(std::string &U)
    {
        if(!CommentReport.empty())
        {
            U=*CommentReport.begin();
            CommentReport.pop_front();
            return true;
        }
        return false;
    }
    void GetComment(const string& P);
    bool GetData(SerialData&);
    SerialTeminal() {}
    std::string TransToStr(char K)
    {
        Translate.clear();
        Translate<<static_cast<int>(K);
        std::string Tem;
        Translate>>Tem;
        return Tem;
    }
    std::string TransToStr(int K)
    {
        Translate.clear();
        Translate<<K;
        std::string Tem;
        Translate>>Tem;
        return Tem;
    }
    bool HaveProtocol()
    {
        return UseProtocol;
    }
    char TransToCha(const std::string &L)
    {
        Translate.clear();
        Translate<<L;
        int Tem;
        Translate>>Tem;
        return static_cast<char>(Tem);
    }
    bool IsDataString(const std::string& U);
    bool IsDataNumber(const std::string& O);
    bool IsNumber(const std::string& U);
    bool Sentting()
    {
        POS.Lock();
        bool Tem=POS.SentComment.Empty();
        POS.UnLock();
        return Tem;
    }

    SerialData TransToSDFromNumber(const std::string& K);
    SerialData TransToSDFromString(const std::string& U);
    static unsigned int TransToUInt(const std::string& P);
    static long TransToLong(const std::string &P);
    void Init() {}
    bool Sperate(const std::string& U);
};
std::stringstream SerialTeminal::Translate;
void SerialTeminal::GetComment(const string &P)
{
    std::deque<std::string> &SSP=CommentList;
    Sperate(P);
    unsigned int Size=SSP.size();
    if(Size>0)
    {
        if(SSP[0]=="open")
        {
            if(Size==2||(Size==3&&IsNumber(SSP[2])))
            {
                if(POS)
                    PushCommentOut(POS.GetName()+" Still At Wrok, Please Close First.");
                else
                {
                    if(SSP[1]>="com1"&&SSP[1]<="com9")
                    {
                        if(Size==2)
                        {
                            POS.CreatSerial(SSP[1]);
                            PushCommentOut("Open "+SSP[1]+" with 9600");
                        }
                        else
                        {
                            unsigned int Bit;
                            Translate.clear();
                            Translate<<SSP[2];
                            Translate>>Bit;
                            POS.CreatSerial(SSP[1],Bit);
                            PushCommentOut("Open "+SSP[1]+" with "+SSP[2]);
                        }
                    }
                    else
                        PushCommentOut("Open Wrong Serial <"+SSP[1]+">.");
                }
            }
            else
                PushCommentOut("open Need 1 Parameter.");
        }
        else if(SSP[0]=="close")
        {
            if(Size==2)
            {
                if(SSP[1]=="serial")
                {
                    if(POS)
                        POS.CloseSerial();
                }
                else if(SSP[1]=="channel")
                {
                    POS.Lock();
                    POS.CleanComment();
                    POS.UnLock();
                }
                else
                    PushCommentOut("Unknow Parameter For close.");
            }
            else
                PushCommentOut("cloas Need 1 Parameter.");
        }
        else if(SSP[0]=="write")
        {
            POS.Lock();
            if(!POS.SentCommentEmpty())
                PushCommentOut("The Channel Isn't Empty.");
            else if(!POS)
                PushCommentOut("The Serial Isn't Opened.");
            else
            {
                ProtocolMap::iterator PoM;
                if(UseProtocol)
                {
                    PoM=Protocol.find(ProtocolName);
                    if(PoM==Protocol.end())
                    {
                        PushCommentOut("Protocol of <"+ProtocolName+"> Not Find.");
                        UseProtocol=false;
                    }
                }
                POS.SentComment.Clear();
                bool Wrong=false;
                bool UsingComment=false;
                for(int i=1; i<Size; ++i)
                {
                    if(SSP[i]=="-wait"&&(!UsingComment)||SSP[i]=="-w")
                    {
                        int j=i+1;
                        if(j<Size&&IsNumber(SSP[j]))
                        {
                            long T=TransToLong(SSP[j]);
                            if(T>=0)
                            {
                                POS.Wait(T);
                                i=j;
                            }
                        }
                        else
                        {
                            PushCommentOut("-wait Need a paratemer for time.");
                            Wrong=true;
                        }
                        UsingComment=false;
                    }
                    else if((SSP[i]=="-deadloop"||SSP[i]=="-dl")&&(!UsingComment))
                    {
                        POS.DeadLoop();
                        UsingComment=false;
                    }
                    else if((SSP[i]=="-loopfor"||SSP[i]=="-lf")&&(!UsingComment))
                    {
                        int j=i+1;
                        if(j<Size&&IsNumber(SSP[j]))
                        {
                            long T=TransToLong(SSP[j]);
                            if(T>=0)
                            {
                                POS.LoopFor(T);
                                i=j;
                            }
                        }
                        else
                        {
                            PushCommentOut("-loopfor Need a paratemer for time.");
                            Wrong=true;
                        }
                        UsingComment=false;
                    }
                    else if(SSP[i]=="\\"&&(!UsingComment))
                        UsingComment=false;
                    else
                    {
                        if(IsDataNumber(SSP[i]))
                        {
                            if(UseProtocol)
                                POS.SentData(PoM->second.MakeUp(TransToSDFromNumber(SSP[i])));
                            else
                                POS.SentData(TransToSDFromNumber(SSP[i]));
                        }
                        else if(IsDataString(SSP[i]))
                        {
                            if(UseProtocol)
                                POS.SentData(PoM->second.MakeUp(TransToSDFromString(SSP[i])));
                            else
                                POS.SentData(TransToSDFromString(SSP[i]));
                        }
                        else
                        {
                            std::map<std::string,SerialData>::iterator Po=Data.find(SSP[i]);
                            if(Po!=Data.end())
                            {
                                if(UseProtocol)
                                    POS.SentData(PoM->second.MakeUp(Po->second));
                                else
                                    POS.SentData(Po->second);
                            }
                            else
                            {
                                PushCommentOut("Can not find Data of <"+SSP[i]+">.");
                                Wrong=false;
                            }
                        }
                        UsingComment=false;
                    }
                }
                if(Wrong)
                    POS.CleanComment();
            }
            POS.UnLock();
        }
        else if(SSP[0]=="load")
        {
            if(Size<2)
                PushCommentOut("Comment load need One Parameter of File Adress.");
            else
            {
                UseProtocol=false;
                FileFactor FF;
                for(int SSPCount=1; SSPCount<Size; ++SSPCount)
                {
                    FF.Clear();
                    FF.Separate(SSP[SSPCount]);
                    ScriptionBlock SB(FF.Begin(),FF.End());
                    for(CommentPoint CP=SB.Begin(); CP!=SB.End(); ++CP)
                    {
                        if(CP->Is("Data"))
                        {
                            std::string Name;
                            int Size=0;
                            for(OperatorPoint OP=CP->Begin(); OP!=CP->End(); ++OP,++Size)
                                Name=OP->Operator->Detail();
                            if(Size!=1)
                                PushCommentOut("Multy or None Name of One Data..."+Name);
                            if(Size>=1)
                            {
                                DataMap::iterator Po=Data.find(Name);
                                if(Po!=Data.end())
                                {
                                    PushCommentOut("Data of <"+Name+"> Is not empty, now Up Data.");
                                    Po->second.Clear();
                                }
                                else
                                {
                                    Data[Name];
                                    Po=Data.find(Name);
                                }
                                std::string TemS;
                                for(FileFactorPoint FFP=CP->ScriptionBegin; FFP!=CP->ScriptionEnd; ++FFP)
                                {
                                    if(*FFP!=",")
                                    {
                                        if(FFP->IsNumber())
                                        {
                                            char Tem=TransToCha(FFP->Detail());
                                            TemS+=Tem;
                                        }
                                    }
                                }
                                Po->second.Insert(TemS);
                                TemS=Po->second.ValueString();
                                PushCommentOut("Data Of "+Name+" Is :"+TemS);
                            }
                        }
                        else if(CP->Is("Protocol"))
                        {
                            std::string Name;
                            int Size=0;
                            for(OperatorPoint OP=CP->Begin(); OP!=CP->End(); ++OP,++Size)
                                Name=OP->Operator->Detail();
                            if(Size!=1)
                                PushCommentOut("Multy or None Name of One Protocol..."+Name);
                            if(Size>=1)
                            {
                                ProtocolMap::iterator Po=Protocol.find(Name);
                                if(Po!=Protocol.end())
                                {
                                    PushCommentOut("Protocol of <"+Name+"> Is not empty, now Up Data.");
                                    Po->second.Clear();
                                }
                                else
                                {
                                    Protocol[Name];
                                    Po=Protocol.find(Name);
                                }
                                for(FileFactorPoint FFP=CP->ScriptionBegin; FFP!=CP->ScriptionEnd; ++FFP)
                                {
                                    if(*FFP!=",")
                                    {
                                        if(FFP->IsNumber())
                                        {
                                            Po->second.Insert(SerialProtocolComment(SPC_CERTAIN,TransToLong(FFP->Detail())));
                                        }
                                        else if(FFP->Is("*"))
                                        {
                                            ++FFP;
                                            if(FFP!=CP->ScriptionEnd)
                                            {
                                                if(FFP->Is("~"))
                                                    Po->second.Insert(SerialProtocolComment(SPC_ANY_ANY,0));
                                                else if(FFP->IsNumber())
                                                    Po->second.Insert(SerialProtocolComment(SPC_ANY_CERTAIN,TransToLong(FFP->Detail())));
                                                else if(FFP->Is(","))
                                                    Po->second.Insert(SerialProtocolComment(SPC_ANY,0));
                                                else
                                                    PushCommentOut("Unknow Parameter of :"+FFP->Detail());
                                            }
                                            else
                                            {
                                                --FFP;
                                                Po->second.Insert(SerialProtocolComment(SPC_ANY,0));
                                            }

                                        }
                                        else if(FFP->Is("SIZE"))
                                        {
                                            Po->second.Insert(SerialProtocolComment(SPC_SIZE,0));
                                        }
                                        else
                                            PushCommentOut("Unkonw Parameter of "+FFP->Detail());
                                    }
                                }
                                std::string Tem=Po->second.OutPut(Translate);
                                PushCommentOut("Protocol Of "+Name+" Is :"+Tem);
                            }
                        }
                        else if(CP->Is("Comment"))
                        {
                            std::string Name;
                            int Size=0;
                            for(OperatorPoint OP=CP->Begin(); OP!=CP->End(); ++OP,++Size)
                                Name=OP->Operator->Detail();
                            if(Size>=1)
                            {
                                CommentMap::iterator Po=CM.find(Name);
                                if(Po!=CM.end())
                                {
                                    PushCommentOut("Comment of <"+Name+"> Is not empty, now Up Data.");
                                    Po->second.clear();
                                }
                                else
                                {
                                    CM[Name];
                                    Po=CM.find(Name);
                                }
                                Po->second=Unite(CP->ScriptionBegin,CP->ScriptionEnd);
                                PushCommentOut("Comment of <"+Name+"> Is "+Po->second);
                            }
                            else
                                PushCommentOut("Comment Need A Name");
                        }
                    }
                }
            }
        }
        else if(SSP[0]=="run")
        {
            if(Size<=1)
                PushCommentOut("run Need One or More Parameter.");
            else
            {
                for(int i=1; i<Size; ++i)
                {
                    CommentMap::iterator Po=CM.find(SSP[i]);
                    if(Po==CM.end())
                    {
                        PushCommentOut("Comment of <"+SSP[i]+"> Not Find.");
                        break;
                    }
                    else
                    {
                        GetComment(Po->second);
                    }
                }
            }
        }
        else if(SSP[0]=="setprotocol")
        {
            if(Size!=2)
                PushCommentOut("Protocol Only Need One Parameter");
            else
            {
                ProtocolName=SSP[1];
                if(Protocol.find(ProtocolName)!=Protocol.end())
                    UseProtocol=true;
                else
                    PushCommentOut("Protocol of <"+SSP[1]+"> Not Find.");
            }
        }
        else if(SSP[0]=="cleanprotocol")
        {
            UseProtocol=false;
        }
        else if(SSP[0]=="showprotocol")
        {
            if(UseProtocol)
                PushCommentOut("Now Protocol Is <"+ProtocolName+">.");
            else
                PushCommentOut("No Protocol Is Selected.");
        }
        else if(SSP[0]=="protocol")
        {
            for(ProtocolMap::iterator Po=Protocol.begin(); Po!=Protocol.end(); ++Po)
            {
                std::string Tem=Po->second.OutPut(Translate);
                PushCommentOut("Protol "+Po->first+" :"+Tem);
            }
        }
        else if(SSP[0]=="data")
        {
            for(DataMap::iterator Po=Data.begin(); Po!=Data.end(); ++Po)
            {
                std::string Tem=Po->second.ValueString();
                PushCommentOut("Data "+Po->first+" :"+Tem);
            }
        }
        else if(SSP[0]=="comment")
        {
            for(CommentMap::iterator Po=CM.begin(); Po!=CM.end(); ++Po)
            {
                std::string Tem=Po->second;
                PushCommentOut("Comment "+Po->first+" :"+Tem);
            }
        }
        else
        {
            /*for(int i=0; i<Size; ++i)
                PushCommentOut(SSP[i]);*/
            PushCommentOut("Unknow Comment.");
        }
    }
}


bool SerialTeminal::GetData(SerialData& I)
{
    POS.Lock();
    bool Tem=POS.GetData(I);
    POS.Clear();
    POS.UnLock();
    return Tem;
}

bool SerialTeminal::IsNumber(const std::string &U)
{
    for(std::string::const_iterator Po=U.begin(); Po!=U.end(); ++Po)
        if(*Po<'0'&&*Po>'9')
            return false;
    return true;
}

unsigned int SerialTeminal::TransToUInt(const std::string &P)
{
    Translate.clear();
    unsigned int T;
    Translate<<P;
    Translate>>T;
    return T;
}
long SerialTeminal::TransToLong(const std::string &P)
{
    Translate.clear();
    long T;
    Translate<<P;
    Translate>>T;
    return T;
}

/*void SerialTeminalRender::Display(PORender *POR)
{
    //Time+=POR->Time();
    if(Out.NeedChange()) {Out.Init(POR);}
    if(Comment.NeedChange()) Comment.Init(POR);
    if(Out.HaveFront())
    {
        if(Time<=500)
        {
            if(Time<=200)
            {
                glColor4f(0.2,0.2,0.2,1.0);
            }else{
                glColor4f(0.2,0.2,0.2,(500.0-Time)/300.0);
            }
            glBegin(GL_QUADS);glVertex(-0.95,0.81);glVertex(-0.95,-0.41);glVertex(0.95,-0.41);glVertex(0.95,0.81);glEnd();
        }
        float Each(1.9/Out.NowCharOfLine()/0.4);
        Out.Display(XYF(-0.95,-0.41+Each*Out.NowLine()),XYF(0.95,-0.41));
    }
    if(Comment.HaveFront())
    {
        float Each(1.9/Comment.NowCharOfLine()/0.4);
        Comment.Display(XYF(-0.95,-0.95+Each*Comment.NowLine()),XYF(0.95,-0.95));
    }
}*/

bool SerialTeminal::Sperate(const std::string &U)
{
    bool Special=false;
    CommentList.clear();
    for(std::string::const_iterator Po=U.begin(); Po!=U.end(); ++Po)
    {
        if(*Po!=' ')
        {
            if(*Po=='%'||*Po=='&')
                Special=true;
            std::string::const_iterator Po2=Po;
            for(; Po2!=U.end(); ++Po2)
            {
                if(Special)
                {
                    if(*Po2==';')
                    {
                        std::string P(Po,Po2);
                        CommentList.push_back(P);
                        Po=Po2;
                        Special=false;
                        break;
                    }
                }
                else
                {
                    if(*Po2==' '&&Po2!=Po)
                    {
                        std::string P(Po,Po2);
                        CommentList.push_back(P);
                        Po=Po2;
                        break;
                    }
                }
            }
            if(Po!=Po2)
            {
                std::string P(Po,Po2);
                CommentList.push_back(P);
                Po=Po2;
                break;
            }
        }
    }
    return true;
}

bool SerialTeminal::IsDataNumber(const std::string &O)
{
    if(!O.empty()&&O[0]=='%')
    {
        unsigned int Size=O.size();
        for(int i=1; i<Size; ++i)
        {
            if(!((O[i]>='0'&&O[i]<='9')||(O[i]>='a'&&O[i]<='f')||(O[i]>='A'&&O[i]<='F')||O[i]==','||O[i]==' '))
                return false;
        }
        return true;
    }
    return false;
}

bool SerialTeminal::IsDataString(const std::string &O)
{
    if(!O.empty()&&O[0]=='&')
    {
        return true;
    }
    return false;
}

SerialData SerialTeminal::TransToSDFromString(const std::string &U)
{
    std::string K(U.begin()+1,U.end());
    SerialData SD(K);
    return SD;
}

SerialData SerialTeminal::TransToSDFromNumber(const std::string &U)
{
    unsigned int Size=U.size();
    char* P=new char[Size];
    unsigned int US=0;
    unsigned int TemN=0;
    for(unsigned int i=1; i<Size; ++i)
    {
        if(U[i]!=','&&U[i]!=' ')
        {
            TemN*=0x0010;
            if(U[i]>='0'&&U[i]<='9')
                TemN|=U[i]-'0';
            else if(U[i]>='a'&&U[i]<='f')
                TemN|=U[i]-'a'+10;
            else if(U[i]>='A'&&U[i]<='F')
                TemN|=U[i]-'A'+10;
        }
        else
        {
            P[US]=static_cast<char>(TemN&0x00ff);
            ++US;
            TemN=0;
        }
    }
    if(TemN!=0)
    {
        P[US]=static_cast<char>(TemN&0x00ff);
        ++US;
    }
    SerialData SD(P,US);
    delete [](P);
    return SD;
}
#endif // SERIALTEMINAL_H

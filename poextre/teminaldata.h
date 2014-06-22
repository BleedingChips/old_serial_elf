#ifndef TEMINALDATA_H
#define TEMINALDATA_H
#include "poserial.h"

enum SerialProtocolCommentType
{
    SPC_CERTAIN,
    SPC_ANY_CERTAIN,
    SPC_ANY,
    SPC_SIZE,
    SPC_ANY_ANY
};

class SerialProtocolComment
{
    SerialProtocolCommentType Mode;
    long Parameter;
public:
    long GetParameter()const
    {
        return Parameter;
    }
    SerialProtocolComment(SerialProtocolCommentType K,long P):Mode(K),Parameter(P) {}
    SerialProtocolComment(const SerialProtocolComment& K):Mode(K.Mode),Parameter(K.Parameter) {}
    bool IsCertain()const
    {
        return Mode==SPC_CERTAIN;
    }
    bool IsAny()const
    {
        return Mode==SPC_ANY;
    }
    bool IsAnyCertain()const
    {
        return Mode==SPC_ANY_CERTAIN;
    }
    bool IsSize()const
    {
        return Mode==SPC_SIZE;
    }
    bool IsAnyAny()const
    {
        return Mode==SPC_ANY_ANY;
    }
};

class SerialProtocol
{
    std::deque<SerialProtocolComment> List;
    bool CertainLong;
    unsigned int Long;
public:
    void Insert(SerialProtocolCommentType K,long P=0)
    {
        Insert(SerialProtocolComment(K,P));
    }
    void Insert(const SerialProtocolComment& U);
    void Clear()
    {
        List.clear();
        CertainLong=true;
        Long=0;
    }
    SerialData MakeUp(const SerialData&P);
    SerialProtocol():CertainLong(true),Long(0) {}
    std::string OutPut(std::stringstream& K);
};

std::string SerialProtocol::OutPut(std::stringstream& K)
{
    std::string Tem;
    for(std::deque<SerialProtocolComment>::iterator Pot=List.begin(); Pot!=List.end(); ++Pot)
    {
        if(Pot->IsCertain())
        {
            std::string TemS;
            K.clear();
            K<<Pot->GetParameter();
            K>>TemS;
            Tem+="<"+TemS+">";
        }
        else if(Pot->IsAny())
            Tem+="<*>";
        else if(Pot->IsAnyAny())
            Tem+="<*~>";
        else if(Pot->IsAnyCertain())
        {
            std::string TemS;
            K.clear();
            K<<Pot->GetParameter();
            K>>TemS;
            Tem+="<*..."+TemS+">";
        }
        else if(Pot->IsSize())
            Tem+="<SIZE>";
    }
    return Tem;
}


void SerialProtocol::Insert(const SerialProtocolComment& Tem)
{
    if(CertainLong)
    {
        if(Tem.IsAny()||Tem.IsCertain()||Tem.IsSize())
            Long+=1;
        else if(Tem.IsAnyCertain())
        {
            Long+=Tem.GetParameter();
        }
        else if(Tem.IsAnyAny())
        {
            CertainLong=false;
            Long=0;
        }
    }
    List.push_back(Tem);
}

SerialData SerialProtocol::MakeUp(const SerialData &P)
{
    if(CertainLong)
    {
        SerialData TemSD;
        unsigned int ChaCount=0;
        for(std::deque<SerialProtocolComment>::iterator Po=List.begin(); Po!=List.end(); ++Po)
        {
            if(Po->IsAny())
            {
                if(ChaCount<P.Size())
                {
                    TemSD.Insert(P[ChaCount]);
                    ++ChaCount;
                }
                else
                    TemSD.Insert('\0');
            }
            else if(Po->IsCertain())
            {
                TemSD.Insert(static_cast<char>(Po->GetParameter()));
            }
            else if(Po->IsSize())
                TemSD.Insert(static_cast<char>(Long));
            else if(Po->IsAnyCertain())
            {
                unsigned int O=ChaCount+Po->GetParameter();
                if(O>P.Size())
                {
                    if(ChaCount<P.Size())
                    {
                        unsigned int K=O-P.Size();
                        TemSD.Insert(P.Char()+ChaCount,P.Char()+P.Size()+1);
                        char* I=new char[K+1];
                        for(int i=0; i<=K; ++i)
                            I[i]='\0';
                        TemSD.Insert(I,&I[K+1]);
                        delete [](I);
                        ChaCount=P.Size();
                    }
                    else
                    {
                        char* I=new char[Po->GetParameter()+1];
                        for(int i=0; i<=Po->GetParameter(); ++i)
                            I[i]='\0';
                        TemSD.Insert(I,&I[Po->GetParameter()+1]);
                        delete [](I);
                    }
                }
                else
                {
                    TemSD.Insert(P.Char()+ChaCount,P.Char()+O+1);
                    ChaCount=O;
                }
            }
        }
        return TemSD;
    }
    else
    {
        std::deque<SerialData> SDList;
        unsigned int ChaCount=0;
        unsigned int SizeCount=0;
        for(std::deque<SerialProtocolComment>::iterator Po=List.begin(); Po!=List.end(); ++Po)
        {
            if(Po->IsAny())
            {
                if(ChaCount<P.Size())
                {
                    std::string K;
                    K+=P[ChaCount];
                    SDList.push_back(SerialData(K));
                    ++ChaCount;
                }
                else
                    SDList.push_back(std::string('\0'));
                ++SizeCount;
            }
            else if(Po->IsCertain())
            {
                std::string K;
                K+=static_cast<char>(Po->GetParameter());
                SDList.push_back(SerialData(K));
                ++SizeCount;
            }
            else if(Po->IsSize())
            {
                ++SizeCount;
            }
            else if(Po->IsAnyCertain())
            {
                SerialData TemSD;
                unsigned int O=ChaCount+Po->GetParameter();
                if(O>P.Size())
                {
                    if(ChaCount<P.Size())
                    {
                        unsigned int K=O-P.Size();
                        TemSD.Insert(P.Char()+ChaCount,P.Char()+P.Size());
                        char* I=new char[K];
                        for(int i=0; i<K; ++i)
                            I[i]='\0';
                        TemSD.Insert(I,&I[K]);
                        delete [](I);
                        ChaCount=P.Size();
                    }
                    else
                    {
                        char* I=new char[Po->GetParameter()];
                        for(int i=0; i<Po->GetParameter(); ++i)
                            I[i]='\0';
                        TemSD.Insert(I,&I[Po->GetParameter()]);
                        delete [](I);
                    }
                }
                else
                {
                    TemSD.Insert(P.Char()+ChaCount,P.Char()+O);
                    ChaCount=O;
                }
                SizeCount+=Po->GetParameter();
                SDList.push_back(TemSD);
            }
            else if(Po->IsAnyAny())
            {
                SerialData TemSD;
                unsigned int PT=P.Size()-ChaCount-1;
                if(PT>0)
                {
                    TemSD.Insert(P.Char()+ChaCount,P.Char()+P.Size());
                    SizeCount+=PT+1;
                    ChaCount=P.Size();
                }
                SDList.push_back(TemSD);
            }
        }
        SerialData TemSD;
        for(std::deque<SerialProtocolComment>::iterator Po=List.begin(); Po!=List.end(); ++Po)
        {
            if(Po->IsSize())
            {
                std::string P;
                P+=static_cast<char>(SizeCount);
                TemSD.Insert(P);
            }
            else
            {
                TemSD.Insert(*SDList.begin());
                SDList.pop_front();
            }
        }
        return TemSD;
    }
}

#endif // TEMINALDATA_H

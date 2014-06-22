#ifndef SERIALTYPE_H
#define SERIALTYPE_H
#include <sstream>
#include <string>
class SerialData
{
    static std::stringstream Translate;
public:
    char* Data;
    unsigned int SizeOfData;
    unsigned int Size()const
    {
        return SizeOfData;
    }
    std::string String();
    std::string ValueString();
    char* Char()const
    {
        return Data;
    }
    operator char*()const
    {
        return Data;
    }
    char operator[](int i)const
    {
        return Data[i];
    }
    SerialData(const std::string& K);
    SerialData(const SerialData& P);
    SerialData(char* D,unsigned int Size);
    SerialData operator=(const SerialData& K);
    SerialData():SizeOfData(0) {}
    ~SerialData();
    void Insert(const std::string& K);
    void Insert(const SerialData& P);
    void Insert(const char P);
    void Insert(const char* B,const char* E);
    void Clear()
    {
        if(SizeOfData!=0)
        {
            delete[](Data);
            SizeOfData=0;
        }
    }
};
std::stringstream  SerialData::Translate;
enum SerialCommentType
{
    STC_STR,
    STC_WAI,
    STC_LOO,
    STC_DLO,
    STC_REA
};
class SerialComment
{
    SerialCommentType Mode;
    unsigned int DataLocation;
public:
    SerialComment(SerialCommentType M,unsigned int P):Mode(M),DataLocation(P) {}
    SerialComment(const SerialComment& J):Mode(J.Mode),DataLocation(J.DataLocation) {}
    bool IsWait()const
    {
        return Mode==STC_WAI;
    }
    bool IsCertain()const
    {
        return Mode==STC_STR;
    }
    bool IsLoop()const
    {
        return Mode==STC_LOO;
    }
    bool IsDeadLoop()const
    {
        return Mode==STC_DLO;
    }
    bool IsRead()const
    {
        return Mode==STC_REA;
    }
    unsigned int GetData()const
    {
        return DataLocation;
    }
};

class SerialCommentList
{
    std::deque<SerialComment> Comment;
    std::deque<long> ParameterList;
    std::deque<SerialData> DataList;
public:
    void Insert(const SerialData&);
    void Insert(SerialCommentType,long);
    void Insert(SerialCommentType P)
    {
        Comment.push_back(SerialComment(P,0));
    }
    SerialComment& operator[](int P)
    {
        return Comment[P];
    }
    unsigned int Size()const
    {
        return Comment.size();
    }
    long GetParameter(int P)
    {
        return ParameterList[Comment[P].GetData()];
    }
    SerialData& GetSerialData(int P)
    {
        return DataList[Comment[P].GetData()];
    }
    void Clear()
    {
        Comment.clear();
        ParameterList.clear();
        DataList.clear();
    }
    bool Empty()
    {
        return Comment.empty();
    }
};

void SerialCommentList::Insert(const SerialData &P)
{
    Comment.push_back(SerialComment(STC_STR,DataList.size()));
    DataList.push_back(P);
}

void SerialCommentList::Insert(SerialCommentType SCT, long P)
{
    Comment.push_back(SerialComment(SCT,ParameterList.size()));
    ParameterList.push_back(P);
}

void SerialData::Insert(const std::string &K)
{
    unsigned int Size=K.size()+SizeOfData;
    char *TemData=NULL;
    if(Size>0)
    {
        TemData=new char[Size];
        for(int i=0; i<SizeOfData; ++i)
            TemData[i]=Data[i];
        for(int i=0; i<K.size(); ++i)
            TemData[i+SizeOfData]=K[i];
    }
    Clear();
    Data=TemData;
    SizeOfData=Size;
}

void SerialData::Insert(const SerialData& K)
{
    unsigned int Size=K.Size()+SizeOfData;
    char *TemData=NULL;
    if(Size>0)
    {
        TemData=new char[Size];
        for(int i=0; i<SizeOfData; ++i)
            TemData[i]=Data[i];
        for(int i=0; i<K.Size(); ++i)
            TemData[i+SizeOfData]=K[i];
    }
    Clear();
    Data=TemData;
    SizeOfData=Size;
}

void SerialData::Insert(const char P)
{
    unsigned int Size=1+SizeOfData;
    char *TemData=NULL;
    if(Size>0)
    {
        TemData=new char[Size];
        for(int i=0; i<SizeOfData; ++i)
            TemData[i]=Data[i];
        TemData[SizeOfData]=P;
    }
    Clear();
    Data=TemData;
    SizeOfData=Size;
}

void SerialData::Insert(const char *B, const char *E)
{
    unsigned int U=E-B-1;
    unsigned int Size=U+SizeOfData;
    char *TemData=NULL;
    if(Size>0)
    {
        TemData=new char[Size];
        for(int i=0; i<SizeOfData; ++i)
            TemData[i]=Data[i];
        for(int i=0; i<U; ++i)
            TemData[i+SizeOfData]=B[i];
    }
    Clear();
    Data=TemData;
    SizeOfData=Size;
}

std::string SerialData::String()
{
    std::string K;
    for(int i=0; i<SizeOfData; ++i)
        K+=Data[i];
    return K;
}

SerialData::SerialData(char *D, unsigned int Size)
{
    SizeOfData=Size;
    if(Size>0)
    {
        Data=new char[SizeOfData];
        for(int i=0; i<SizeOfData; ++i)
            Data[i]=D[i];
    }
}

SerialData::SerialData(const SerialData &P)
{
    SizeOfData=P.SizeOfData;
    if(SizeOfData>0)
    {
        Data=new char[SizeOfData];
        for(int i=0; i<SizeOfData; ++i)
            Data[i]=P.Data[i];
    }
}

SerialData::SerialData(const std::string &K)
{
    SizeOfData=K.size();
    if(SizeOfData>0)
    {
        Data=new char[SizeOfData];
        char* Tem=Data;
        for(std::string::const_iterator Po=K.begin(); Po!=K.end(); ++Po,++Tem)
            *Tem=*Po;
    }
}

SerialData::~SerialData()
{
    if(SizeOfData>0)
        delete[](Data);
}

SerialData SerialData::operator =(const SerialData& Tem)
{
    unsigned int P=Tem.Size();
    char *TemData=NULL;
    if(P>0)
    {
        TemData=new char[P];
        for(int i=0; i<P; ++i)
            TemData[i]=Tem.Data[i];
    }
    Clear();
    SizeOfData=P;
    Data=TemData;
    return *this;
}

std::string SerialData::ValueString()
{
    std::string Tem;
    for(int i=0; i<SizeOfData; ++i)
    {
        std::string K;
        Translate.clear();
        Translate<<std::hex<<(static_cast<int>(Data[i])&0x00ff);
        Translate>>K;
        Tem+=K+" ";
    }
    return Tem;
}
#endif // SERIALTYPE_H

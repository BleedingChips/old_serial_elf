#ifndef SCRIPTIONANALYZE_H
#define SCRIPTIONANALYZE_H
#include <string>
#include <sstream>
#include <deque>
#include <vector>
class StringSperate
{
    std::vector<std::string> SperatedString;
public:
    void Sperate(const std::string& K);
    size_t Size()
    {
        return SperatedString.size();
    }
    std::string& operator[](const int& i)
    {
        return SperatedString[i];
    }
    void Clear()
    {
        SperatedString.clear();
    }
    StringSperate() {}
    StringSperate(const StringSperate& P):SperatedString(P.SperatedString) {}
    StringSperate(const std::string& K)
    {
        Sperate(K);
    }
};
std::string Cut(std::string::iterator& B,const std::string::iterator& E, int S)
{
    std::string::iterator Po=B;
    int CharSpa(0);
    int LastChar(0);
    int LineChar(0);
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
std::string AdressCount(std::string N,std::string T)
{
    for(std::string::reverse_iterator Po=N.rbegin(); Po!=N.rend(); ++Po)
    {
        if(*Po=='/')
        {
            std::string C(N.begin(),++(Po.base()));
            C+=T;
            return C;
        }
    }
    return T;
}
template<class T> void Translate(const std::string& S,T& D)
{
    static std::stringstream SStream;
    SStream.clear();
    SStream<<S;
    SStream>>D;
}
int CalculateSize(std::string K)
{
    bool Dou=false;
    int Cal=0;
    int CalChar=0;
    for(std::string::iterator Po=K.begin(); Po!=K.end(); ++Po)
    {
        unsigned int KK=*Po;
        if(CalChar==0)
        {
            if(*Po>0)
                CalChar=1;
            else
            {
                if(*Po>(int)0xfffffffc)
                    CalChar=6;
                else if(*Po>(int)0xfffffff8)
                    CalChar=5;
                else if(*Po>(int)0xfffffff0)
                    CalChar=4;
                else if(*Po>(int)0xffffffe0)
                    CalChar=3;
                else if(*Po>(int)0xffffffc0)
                    CalChar=2;
                Dou=true;
            }
        }
        if(CalChar==1)
        {
            if(Dou)
                Cal+=2;
            else Cal+=1;
            Dou=false;
        }
        --CalChar;
    }
    return Cal;
}
void StringSperate::Sperate(const std::string &K)
{
    for(std::string::const_iterator P1=K.begin(); P1!=K.end(); ++P1)
    {
        if(*P1!=' ')
        {
            std::string::const_iterator P2=P1;
            for(; P2!=K.end(); ++P2)
            {
                if(*P2==' ')
                {
                    SperatedString.push_back(std::string(P1,P2));
                    P1=P2;
                    break;
                }
            }
            if(P1!=P2)
            {
                SperatedString.push_back(std::string(P1,P2));
                P1=P2;
                break;
            }
        }
    }
}

#endif // SCRIPTIONANALYZE_H

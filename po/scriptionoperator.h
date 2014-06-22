#ifndef SCRIPTIOBOPERATOR_H
#define SCRIPTIOBOPERATOR_H
#include "scriptionseparate.h"

bool JumpBrace(FileFactorPoint S,FileFactorPoint& NP,FileFactorPoint E)
{
    int Size=0;
    NP=S;
    while(S!=E)
    {
        if(S->Is("{"))
            Size+=1;
        else if(S->Is("}"))
            Size-=1;
        if(Size==0)
        {
            NP=S;
            return true;
        }
        if(Size<0)
        {
            GetError("A \"}\" need a \"{\"",S->Line());
            return false;
        }
        ++S;
    }
    if(Size!=0)
    {
        GetError("A \"{\" can not find a \"}\"",NP->Line());
        return false;
    }
    return true;
}
/*bool JumpBrace(FileFactorReversePoint S,FileFactorReversePoint& NP,FileFactorReversePoint E)
{
    int Size=0;
    NP=S;
    while(S!=E)
    {
        if(S->Is("}"))
            Size+=1;
        else if(S->Is("{"))
            Size-=1;
        if(Size==0)
        {
            NP=S;
            return true;
        }
        if(Size<0)
        {
            GetError("A \"}\" need a \"{\"",S->Line());
            return false;
        }
        ++S;
    }
    if(Size!=0)
    {
        GetError("A \"{\" can not find a \"}\"",NP->Line());
        return false;
    }
   return true;
}*/


class ScriptionOperator
{
public:
    FileFactorPoint Operator;
    FileFactorPoint ScriptionBegin;
    FileFactorPoint ScriptionEnd;
    int Line()
    {
        return Operator->Line();
    }
    std::string Scription()
    {
        return Unite(ScriptionBegin,ScriptionEnd);
    }
    ScriptionOperator(const ScriptionOperator& K):ScriptionBegin(K.ScriptionBegin),Operator(K.Operator),ScriptionEnd(K.ScriptionEnd) {}
    //ScriptionOperator(FileFactorReversePoint S,FileFactorReversePoint E);
    ScriptionOperator(FileFactorPoint S,FileFactorPoint E);
    bool Is(std::string K)
    {
        return (*Operator)==K;
    }
    FileFactorPoint Begin()
    {
        return ScriptionBegin;
    }
    FileFactorPoint End()
    {
        return ScriptionEnd;
    }
    std::string Detail()
    {
        return *Operator;
    }
    void CheckDetail(std::fstream& K,std::string Pre="   -")
    {
        K<<Pre+Operator->Detail()+":"+Unite(ScriptionBegin,ScriptionEnd)<<std::endl;
    }
};

typedef std::deque<ScriptionOperator>::iterator OperatorPoint;
typedef std::deque<ScriptionOperator>::reverse_iterator OperatorReversePoint;

ScriptionOperator::ScriptionOperator(FileFactorPoint S, FileFactorPoint E)
{
    if(S!=E)
    {
        Operator=++S;
        if(S==E)
            GetError("An Operator Need An Style. ",(--S)->Line());
        else
        {
            ScriptionBegin=++S;
            if(S!=E)
            {
                if(S->Is("{"))
                {
                    if(JumpBrace(S,ScriptionEnd,E))
                        ++ScriptionBegin;
                }
                else
                {
                    ScriptionEnd=++S;
                }
                if(ScriptionEnd!=E)
                {
                    if(ScriptionEnd->Is("}"))
                    {
                        FileFactorPoint Tec=ScriptionEnd;
                        if(E!=++Tec)
                            GetError("Operator Do not need so much Scription.",ScriptionEnd->Line());
                    }
                    else
                        GetError("Operator  Do not need so much Scription.",ScriptionEnd->Line());
                }
            }
            else
            {
                ScriptionBegin=E;
                ScriptionEnd=E;
            }
        }
    }
    else
        GetError("An Empty Operator.",-0);
}

/*template<class T>
void SepareteOperator(FileFactorPoint S,FileFactorPoint E, T* R)
{
    int Size=0;
    bool Ready=false;
    FileFactorPoint TS;
    while(S!=E)
    {
        if(S->Is('-'))
        {
            if(Size!=0)
            {
                if(Ready)
                {
                    R->push_back(ScriptionOperator(TS,S));
                }else{
                    Ready=true;
                }
                Ts=S;
            }
        }else if(S->Is('{'))
        {
            Szie+=1;
        }else if(S->Is('}'))
        {
            Size-=1;
        }
        ++S;
    }
}*/

#endif // SCRIPTIOBOPERATOR_H

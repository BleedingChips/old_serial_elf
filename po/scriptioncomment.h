#ifndef SCRIPTIONCOMMENT_H
#define SCRIPTIONCOMMENT_H
#include "scriptionoperator.h"
#include <fstream>
class ScriptionComment;
typedef std::deque<ScriptionComment>::iterator CommentPoint;
typedef std::deque<ScriptionComment>::reverse_iterator CommentReversePoint;

class ScriptionComment
{
public:
    FileFactorPoint Comment;
    FileFactorPoint ScriptionBegin;
    FileFactorPoint ScriptionEnd;
    bool AbleToJumo;
    CommentPoint Next;
    std::deque<ScriptionOperator> OperatorList;
    ScriptionComment(FileFactorPoint S,FileFactorPoint E);
    ScriptionComment(const ScriptionComment& K):ScriptionBegin(K.ScriptionBegin),AbleToJumo(false),Comment(K.Comment),ScriptionEnd(K.ScriptionEnd),OperatorList(K.OperatorList) {}
    ScriptionComment():AbleToJumo(false) {}
    std::string Scription()
    {
        return Unite(ScriptionBegin,ScriptionEnd);
    }
    OperatorPoint Begin()
    {
        return OperatorList.begin();
    }
    OperatorPoint End()
    {
        return OperatorList.end();
    }
    int Line()
    {
        return Comment->Line();
    }
    std::string Detail()
    {
        return Comment->Detail();
    }
    void Set(FileFactorPoint C,FileFactorPoint S)
    {
        CreatOperator(C,S);
    }
    void CreatOperator(FileFactorPoint Tem,FileFactorPoint Tem2);
    bool Is(std::string K)
    {
        return (*Comment)==K;
    }
    void CheckDetail(std::fstream& K,std::string Pre=" *");
    void SetJump(CommentPoint P)
    {
        AbleToJumo=true;
        Next=P;
    }
    bool AbleToJump()
    {
        return AbleToJumo;
    }
    void Jump(CommentPoint &C)
    {
        if(AbleToJump()) C=Next;
    }
    void Clear()
    {
        OperatorList.clear();
    }
};

void ScriptionComment::CheckDetail(std::fstream& K,std::string Pre)
{
    K<<Pre+Comment->Detail()+":"+Unite(ScriptionBegin,ScriptionEnd)<<"In Line"<<Line()<<std::endl;
    for(OperatorPoint OP=Begin(); OP!=End(); ++OP)
    {
        OP->CheckDetail(K);
    }
}

ScriptionComment::ScriptionComment(FileFactorPoint S, FileFactorPoint E):AbleToJumo(false)
{
    if(S!=E)
    {
        FileFactorPoint CS=S;
        FileFactorPoint OS=S;
        //Error<<"Add Comment"<<Unite(S,E)<<endl;
        while(S!=E)
        {
            if(S->Is("*"))
                CS=S;
            else if(S->Is("{"))
            {
                if(!JumpBrace(S,S,E))
                    break;
            }
            else if(S->Is("-"))
            {
                OS=S;
                break;
            }
            ++S;
        }
        if(OS==CS)
            OS=E;
        Comment=++CS;
        if(CS==OS)
            GetError("An Comment Need An Style. ",(--CS)->Line());
        else
        {
            ++CS;
            if(CS!=OS)
            {
                ScriptionBegin=CS;
                if(CS->Is("{"))
                {
                    if(JumpBrace(CS,ScriptionEnd,OS))
                        ScriptionBegin=++CS;
                }
                else
                {
                    ScriptionEnd=++CS;
                }
                if(ScriptionEnd!=OS)
                {
                    //Error<<"XXX"<<Unite(ScriptionBegin,ScriptionEnd)<<endl;
                    if(ScriptionEnd->Is("}"))
                    {
                        FileFactorPoint Tec=ScriptionEnd;
                        if(OS!=++Tec)
                            GetError("Comment Do not need so much Scription.",ScriptionEnd->Line());
                    }
                    else
                        GetError("Comment Do not need so much Scription.",ScriptionEnd->Line());
                }
            }
            else
            {
                ScriptionBegin=OS;
                ScriptionEnd=OS;
            }
        }
        CS=OS;
        while(OS!=E)
        {
            if(OS->Is("-"))
            {
                if(CS!=OS)
                {
                    OperatorList.push_back(ScriptionOperator(CS,OS));
                    CS=OS;
                }
            }
            else if(OS->Is("{"))
            {
                if(!JumpBrace(OS,OS,E))
                    break;
            }
            ++OS;
        }
        if(CS!=OS)
        {
            OperatorList.push_back(ScriptionOperator(CS,OS));
        }
    }
    else
        GetError("An Empty Commet.",-0);
}

void ScriptionComment::CreatOperator(FileFactorPoint Tem,FileFactorPoint Tem2)
{
    OperatorList.clear();
    FileFactorPoint S;
    FileFactorPoint E;
    FileFactorPoint T=Tem;
    bool Find=false;
    bool Continue=false;
    if(Tem!=Tem2)
    {
        while(T!=Tem2)
        {
            if(*T=="-")
            {
                if(!Continue)
                {
                    if(!Find)
                    {
                        S=T;
                        Find=true;
                    }
                    else
                    {
                        E=T;
                        OperatorList.push_back(ScriptionOperator(S,E));
                        Find=false;
                        continue;
                    }
                }
            }
            else if(*T=="{")
            {
                Continue=true;
            }
            else if(*T=="}")
            {
                Continue=false;
            }
            ++T;
        }
        OperatorList.push_back(ScriptionOperator(S,Tem2));
    }
}
typedef std::deque<ScriptionComment>::iterator CommentPoint;
typedef std::deque<ScriptionComment>::reverse_iterator CommentReversePoint;
#endif // SCRIPTIONCOMMENT_H

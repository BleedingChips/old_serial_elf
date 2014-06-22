#ifndef SCRIPTIONBLOCK_H
#define SCRIPTIONBLOCK_H
#include "scriptioncomment.h"
#include "handle.h"
class ScriptionBlock:public HandleSeed
{
public:
    ScriptionBlock(FileFactorPoint C,FileFactorPoint S)
    {
        Set(C,S);
    }
    ScriptionBlock(CommentPoint C,CommentPoint S)
    {
        CreatComment(C,S);
    }
    ScriptionBlock(CommentReversePoint C,CommentReversePoint S)
    {
        CreatComment(C,S);
    }
    void CreatComment(CommentReversePoint Tem,CommentReversePoint Tem2);
    ScriptionBlock() {}
    std::deque<ScriptionComment> CommentList;
    void CreatComment(FileFactorPoint Tem,FileFactorPoint Tem2);
    void CreatComment(CommentPoint Tem,CommentPoint Tem2);
    void Set(FileFactorPoint C,FileFactorPoint S)
    {
        CreatComment(C,S);
    }
    CommentPoint Begin()
    {
        return CommentList.begin();
    }
    CommentPoint End()
    {
        return CommentList.end();
    }
    CommentReversePoint RBegin()
    {
        return CommentList.rbegin();
    }
    CommentReversePoint REnd()
    {
        return CommentList.rend();
    }
    void CheckDetail(std::fstream& K);
    void Clear()
    {
        CommentList.clear();
    }
};

void ScriptionBlock::CheckDetail(std::fstream& K)
{
    for(CommentPoint OP=Begin(); OP!=End(); ++OP)
    {
        OP->CheckDetail(K);
    }
}

void ScriptionBlock::CreatComment(FileFactorPoint Tem,FileFactorPoint Tem2)
{
    CommentList.clear();
    if(Tem!=Tem2)
    {
        FileFactorPoint S=Tem;
        while(Tem!=Tem2)
        {
            if(Tem->Is("*"))
            {
                if(S!=Tem)
                {
                    CommentList.push_back(ScriptionComment(S,Tem));
                    S=Tem;
                }
            }
            else if(Tem->Is("{"))
            {
                if(!JumpBrace(Tem,Tem,Tem2))
                    break;
            }
            else if(Tem->Is("}"))
            {
                GetError("Symple } need a {",Tem->Line());
            }
            ++Tem;
        }
        if(S!=Tem)
        {
            CommentList.push_back(ScriptionComment(S,Tem));
        }
    }
}

void ScriptionBlock::CreatComment(CommentPoint Tem,CommentPoint Tem2)
{
    CommentList.clear();
    for(; Tem!=Tem2; ++Tem)
    {
        CommentList.push_back(*Tem);
    }
}

void ScriptionBlock::CreatComment(CommentReversePoint Tem,CommentReversePoint Tem2)
{
    CommentList.clear();
    for(; Tem!=Tem2; ++Tem)
    {
        CommentList.push_front(*Tem);
    }
}
#endif // SCRIPTIONBLOCK_H

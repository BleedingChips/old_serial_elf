#ifndef DIALOGMT_H
#define DIALOGMT_H
#include "handle.h"
#include "dialog.h"
#include "extrabuffer.h"
#include "threadpart.h"
#include "threadmessage.h"
enum DialogMTCommentStyle
{
    DMT_CLE,
    DMT_ADS,
    DMT_CUF,
    DMT_CUB,
    DMT_CUD
};
class DialogMTComment
{
public:
    DialogMTCommentStyle Comment;
    int Parameter;
    DialogMTComment(DialogMTCommentStyle T,int K=0):Comment(T),Parameter(K) {}
    DialogMTComment(const DialogMTComment& T):Comment(T.Comment),Parameter(T.Parameter) {}
};

class DialogMTData
{
public:
    std::deque<std::string> StringBuffer;
    std::deque<DialogMTComment> CommentBuffer;
    DialogMTData() {}
};

class StaticDialogMT:public ThreadPart
{
public:
    ExtraBuffer<DialogMTData> Buffer;
    //ThreadMessage<DialogMTComment> Message;
    class Render:public ThreadPart::Work<StaticDialogMT>,public StaticDialog
    {
        ExtraBuffer<DialogMTData>::Receive Buffer;
        //ThreadMessage<DialogMTComment>::Sent Message;
        void RespondDetail(DialogMTData*);
    public:
        void EntrustGet(StaticDialogMT* P)  { Entrust(Buffer.GetPoint(&P->Buffer));/*Entrust(Message.GetPoint(&P->Message));*/}
        void WorkSynchroIn() {}
        void WorkSynchroOut() {}
        bool Demand(){return Buffer;}
        Render(unsigned int U):StaticDialog(U) {}
        void Respond(){Buffer.Read(&Render::RespondDetail,this);}
    };
    class Logic:public ThreadPart::Work<StaticDialogMT>
    {
        ExtraBuffer<DialogMTData>::Sent Buffer;
        //ThreadMessage<DialogMTComment>::Receive Message;
        void WriteDetail(DialogMTData* DD,const std::string& K)   {DD->StringBuffer.push_back(K);}
        void SentComment(DialogMTData* K, const DialogMTComment& T) {K->CommentBuffer.push_back(T);}
    public:
        void EntrustGet(StaticDialogMT* P) {Entrust(Buffer.GetPoint(&P->Buffer));}
        void WorkSynchroOut() {}
        void WorkSynchroIn() {}
        void Insert(const std::string& K){Buffer.Write(&Logic::WriteDetail,this,K);Buffer.Write(&Logic::SentComment,this,DialogMTComment(DMT_ADS));}
        void Clear(){Buffer.Write(&Logic::SentComment,this,DialogMTComment(DMT_CLE));}
        Logic(){}
    };
};

class DynamicDialogMT:public ThreadPart
{
public:
    ExtraBuffer<DialogMTData> Buffer;
    class Render:public ThreadPart::Work<DynamicDialogMT>,public DynamicDialog
    {
        ExtraBuffer<DialogMTData>::Receive Buffer;
        //ThreadMessage<DialogMTComment>::Sent Message;
        void RespondDetail(DialogMTData*);
    public:
        void EntrustGet(DynamicDialogMT* P)  { Entrust(Buffer.GetPoint(&P->Buffer));/*Entrust(Message.GetPoint(&P->Message));*/}
        void WorkSynchroIn() {}
        void WorkSynchroOut() {}
        bool Demand(){return Buffer;}
        //Render(unsigned int U):StaticDialog(U) {}
        void Respond(){Buffer.Read(&Render::RespondDetail,this);}
    };
    class Logic:public ThreadPart::Work<DynamicDialogMT>
    {
        ExtraBuffer<DialogMTData>::Sent Buffer;
        //ThreadMessage<DialogMTComment>::Receive Message;
        void WriteDetail(DialogMTData* DD,const std::string& K)   {DD->StringBuffer.push_back(K);}
        void SentComment(DialogMTData* K, const DialogMTComment& T) {K->CommentBuffer.push_back(T);}
    public:
        void EntrustGet(DynamicDialogMT* P){Entrust(Buffer.GetPoint(&P->Buffer));}
        void WorkSynchroOut() {}
        void WorkSynchroIn() {}
        void Insert(const std::string& K){Buffer.Write(&Logic::WriteDetail,this,K);Buffer.Write(&Logic::SentComment,this,DialogMTComment(DMT_ADS));}
        void Clear(){Buffer.Write(&Logic::SentComment,this,DialogMTComment(DMT_CLE));}
        Logic(){}
    };
};

class EditDialogMT:public ThreadPart
{
public:
    ExtraBuffer<DialogMTData> Buffer;
    class Render:public ThreadPart::Work<EditDialogMT>,protected EditDialog
    {
        ExtraBuffer<DialogMTData>::Receive Buffer;
        void RespondDetail(DialogMTData*);
    public:
        void EntrustGet(EditDialogMT* P)  { Entrust(Buffer.GetPoint(&P->Buffer));}
        void WorkSynchroIn() {}
        void WorkSynchroOut() {}
        bool Demand(){return Buffer;}
        void Respond(){Buffer.Read(&Render::RespondDetail,this);}
        Render(){}
        void Display(float SX,float EX,float SY,unsigned int W,unsigned long Interval,float R=1.0,float G=1.0,float B=1.0,float A=1.0)
                    {EditDialog::Display(SX,EX,SY,W,Interval,R,G,B,A);}
    };
    class Logic:public ThreadPart::Work<EditDialogMT>,protected EditDialog
    {
        ExtraBuffer<DialogMTData>::Sent Buffer;
        void WriteDetail(DialogMTData* DD,const std::string& K)   {DD->StringBuffer.push_back(K);}
        void SentComment(DialogMTData* K, const DialogMTComment& T) {K->CommentBuffer.push_back(T);}
    public:
        void CursorFront(){EditDialog::CursorFront();Buffer.Write(&Logic::SentComment,this,DialogMTComment(DMT_CUF));}
        void CursorBack(){EditDialog::CursorBack();Buffer.Write(&Logic::SentComment,this,DialogMTComment(DMT_CUB));}
        void CursorDelete(){EditDialog::Delete();Buffer.Write(&Logic::SentComment,this,DialogMTComment(DMT_CUD));}
        std::string GetString(){return EditDialog::GetString();}
        void EntrustGet(EditDialogMT* P){Entrust(Buffer.GetPoint(&P->Buffer));}
        void WorkSynchroOut() {}
        void WorkSynchroIn() {}
        void Insert(const std::string& K){EditDialog::Insert(K);Buffer.Write(&Logic::WriteDetail,this,K);Buffer.Write(&Logic::SentComment,this,DialogMTComment(DMT_ADS));}
        void Clear(){EditDialog::Clear();Buffer.Write(&Logic::SentComment,this,DialogMTComment(DMT_CLE));}
        Logic(){}
    };
};

void StaticDialogMT::Render::RespondDetail(DialogMTData* DD)
{
    for(std::deque<DialogMTComment>::iterator Po=DD->CommentBuffer.begin();Po!=DD->CommentBuffer.end();++Po)
    {
        switch(Po->Comment)
        {
        case DMT_CLE:
            Clear();
        break;
        case DMT_ADS:
            {
                if(!DD->StringBuffer.empty())
                {
                    StaticDialog::Insert(*DD->StringBuffer.begin());
                    DD->StringBuffer.pop_front();
                }
            }
            break;
        }
    }
    DD->CommentBuffer.clear();
    DD->StringBuffer.clear();
}

void DynamicDialogMT::Render::RespondDetail(DialogMTData* DD)
{
    for(std::deque<DialogMTComment>::iterator Po=DD->CommentBuffer.begin();Po!=DD->CommentBuffer.end();++Po)
    {
        switch(Po->Comment)
        {
        case DMT_CLE:
            Clear();
            break;
        case DMT_ADS:
            {
                if(!DD->StringBuffer.empty())
                {
                    DynamicDialog::Insert(*DD->StringBuffer.begin());
                    DD->StringBuffer.pop_front();
                }
            }
            break;
        }
    }
    DD->CommentBuffer.clear();
    DD->StringBuffer.clear();
}

void EditDialogMT::Render::RespondDetail(DialogMTData* DD)
{
    for(std::deque<DialogMTComment>::iterator Po=DD->CommentBuffer.begin();Po!=DD->CommentBuffer.end();++Po)
    {
        switch(Po->Comment)
        {
        case DMT_CLE:
            Clear();
            break;
        case DMT_ADS:
                if(!DD->StringBuffer.empty())
                {
                    Insert(*DD->StringBuffer.begin());
                    DD->StringBuffer.pop_front();
                }
            break;
        case DMT_CUF:
            CursorFront();
            break;
        case DMT_CUB:
            CursorBack();
            break;
        case DMT_CUD:
            Delete();
            break;
        }
    }
    DD->CommentBuffer.clear();
    DD->StringBuffer.clear();
}
#endif // DIALOGMT_H

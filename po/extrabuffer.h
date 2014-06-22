#ifndef EXTRABUFFER_H
#define EXTRABUFFER_H
#include "threadpart.h"
template <class T>
class ExtraBuffer:public ThreadPart
{
    T DD[2];
    bool BeenWrite[2];
    bool BeenRead[2];
    bool WriteFinish[2];
    int AUsing;
    int MUsing;
    void AddA()    {AUsing++;AUsing%=2;}
    void AddM()   {MUsing++;MUsing%=2;}
    bool Same()   {return AUsing==MUsing;}
    T& CallM()     {return DD[MUsing];}
    T& CallA()      {return DD[AUsing];}
public:
    ExtraBuffer():AUsing(0),MUsing(0)
    {
        BeenWrite[0]=false;
        BeenWrite[1]=false;
        BeenRead[0]=false;
        BeenRead[1]=false;
    }

    class Receive:public ThreadPart::Work<ExtraBuffer>
    {
        bool ReadyToRead;
        bool ReadFinish;
        void WorkSynchroIn()
        {
            if(!ReadyToRead)
            {
                if(this->Call()->WriteFinish[this->Call()->MUsing]&&(!this->Call()->BeenWrite[this->Call()->MUsing]))
                {
                    ReadyToRead=true;
                    this->Call()->WriteFinish[this->Call()->MUsing]=false;
                    this->Call()->BeenRead[this->Call()->MUsing]=true;
                }
            }
        }
        void WorkSynchroOut()
        {
            if(ReadyToRead&&ReadFinish)
            {
                this->Call()->AddM();
                ReadFinish=false;
                ReadyToRead=false;
                this->Call()->BeenRead[this->Call()->MUsing]=false;
            }
        }
    public:
        operator bool()const  {return ReadyToRead;}

        void Read(void(*Tem)(T*))
        {
            if(ReadyToRead)
            {
                (*Tem)(&this->Call()->CallM());
                ReadFinish=true;
            }
        }

        template <class K>
        void Read(void(K::*Tem)(T*),K* P)
        {
            if(ReadyToRead)
            {
                (P->*Tem)(&this->Call()->CallM());
                ReadFinish=true;
            }
        }

        template <class K,class P>
        void Read(void(K::*Tem)(T*,P),K* Tc,P Te)
        {
            if(ReadyToRead)
            {
                (Tc->*Tem)(&this->Call()->CallM(),Te);
                ReadFinish=true;
            }
        }
        Receive():ReadFinish(false),ReadyToRead(false) {}
    };

    class Sent:public ThreadPart::Work<ExtraBuffer>
    {
        bool WriteFinish;
        void WorkSynchroOut()
        {
            this->Call()->BeenWrite[this->Call()->AUsing]=false;
            if(WriteFinish)
            {
                this->Call()->WriteFinish[this->Call()->AUsing]=true;
                if(this->Call()->Same()) this->Call()->AddA();
                WriteFinish=false;
            }
        }
        void WorkSynchroIn()
        {
            if((this->Call()->Same()&&this->Call()->WriteFinish[this->Call()->AUsing])||this->Call()->BeenRead[this->Call()->AUsing])
                this->Call()->AddA();
            this->Call()->BeenWrite[this->Call()->AUsing]=true;
        }
    public:
        void Finish()    {WriteFinish=true;}
        Sent():WriteFinish(false) {}
        void Write(void(*Tem)(T*))
        {
            (*Tem)(&this->Call()->CallA());
            WriteFinish=true;
        }
        template <class K>
        void Write(void(K::*Tem)(T*),K* I)
        {
            (I->*Tem)(&this->Call()->CallA());
            WriteFinish=true;
        }

        template <class K,class P>
        void Write(void(K::*Tem)(T*,const P&),K* I,const P& Te)
        {
            (I->*Tem)(&this->Call()->CallA(),Te);
            WriteFinish=true;
        }
    };

};

#endif // EXTRABUFFER_H

#ifndef POSERIAL_H
#define POSERIAL_H
#include <windows.h>
#include "../po/pomutex.h"
#include "../po/scriptionanalyze.h"
#include "winserialport.h"
#include "serialtype.h"
#include <sstream>
class POSerial
{
    POMutex POM;
    std::string Name;
    SDL_Thread* ThreadForSerial;
    bool Close;
    WinSerialPort WSP;
public:
    SerialCommentList SentComment;
    SerialData ReceiveBuffer;
    POSerial() {}
    ~POSerial();
    void Lock()
    {
        POM.Lock();
    }
    void UnLock()
    {
        POM.UnLock();
    }
    operator bool()
    {
        return WSP;
    }
    bool CloseSerial();

    bool CreatSerial(const std::string& Tr,unsigned int Bit=9600);
    std::string GetName()
    {
        return Name;
    }
    void SentData(const SerialData& U)
    {
        SentComment.Insert(U);
    }
    void Wait(long T)
    {
        SentComment.Insert(STC_WAI,T);
    }
    void LoopFor(long T)
    {
        SentComment.Insert(STC_LOO,T);
    }
    void DeadLoop()
    {
        SentComment.Insert(STC_DLO);
    }
    void CleanComment()
    {
        SentComment.Clear();
    }
    bool GetData(SerialData& I)
    {
        if(ReceiveBuffer.Size()>0)
        {
            I=ReceiveBuffer;
            return true;
        }
        return false;
    }
    bool SentCommentEmpty()
    {
        return SentComment.Empty();
    }
    void Clear()
    {
        ReceiveBuffer.Clear();
    }
    static int ThreadFunction(void*);
};

bool POSerial::CreatSerial(const std::string& Tr,unsigned int Bit)
{
    CloseSerial();
    Name=Tr;
    if(!WSP.CreatSerial(Tr,Bit))
    {
        return false;
    }
    else
    {
        SentComment.Clear();
        ReceiveBuffer.Clear();
        ThreadForSerial=SDL_CreateThread(&POSerial::ThreadFunction,"Thread For Serial",static_cast<void*>(this));
        return true;
    }
}

bool POSerial::CloseSerial()
{
    if(WSP)
    {
        Lock();
        Close=true;
        UnLock();
        int T;
        SDL_WaitThread(ThreadForSerial,&T);
        WSP.CloseSerial();
        return true;
    }
    return false;
}

POSerial::~POSerial()
{
    CloseSerial();
}

int POSerial::ThreadFunction(void * Go)
{
    POSerial* POS=static_cast<POSerial*>(Go);
    long CountTarget=0;
    long CountNow=0;
    unsigned int Order=0;
    bool Wait=false;
    bool Loop=false;
    bool DeadLoop=false;
    long CountLoop=0;
    unsigned int StartOrder=0;
    while(1)
    {
        POS->Lock();
        if(POS->Close)
        {
            POS->UnLock();
            break;
        }
        if(!POS->SentComment.Empty())
        {
            if(Wait)
            {
                CountNow+=1;
                if(CountNow>=CountTarget)
                {
                    Wait=false;
                }
            }
            else
            {
                unsigned int P=POS->SentComment.Size();
                while(Order<P)
                {

                    if(POS->SentComment[Order].IsCertain())
                    {
                        DWORD I;
                        POS->WSP.Write((char*)(POS->SentComment.GetSerialData(Order)),POS->SentComment.GetSerialData(Order).Size(),I);
                        ++Order;
                    }
                    else if(POS->SentComment[Order].IsWait())
                    {
                        Wait=true;
                        CountTarget=POS->SentComment.GetParameter(Order);
                        CountNow=0;
                        ++Order;
                        break;
                    }
                    else if(POS->SentComment[Order].IsDeadLoop())
                    {
                        Order=StartOrder;
                        DeadLoop=true;
                        break;
                    }
                    else if(POS->SentComment[Order].IsLoop())
                    {
                        if(Loop)
                        {
                            CountLoop-=1;
                            if(CountLoop<=1)
                            {
                                ++Order;
                                StartOrder=Order;
                                Loop=false;
                            }
                            else
                            {
                                Order=StartOrder;
                                break;
                            }
                        }
                        else
                        {
                            Loop=true;
                            CountLoop=POS->SentComment.GetParameter(Order);
                            Order=StartOrder;
                            break;
                        }
                    }
                    else
                        ++Order;
                }
                if(!(Wait||Loop||DeadLoop))
                {
                    POS->SentComment.Clear();
                    CountTarget=0;
                    CountNow=0;
                    Order=0;
                    Wait=false;
                    Loop=false;
                    DeadLoop=false;
                    CountLoop=0;
                    StartOrder=0;
                }
            }
        }
        POS->UnLock();
        DWORD I;
        if(POS->WSP.GetState(I))
        {
            unsigned int P=POS->WSP.ReadBuffer();
            if(P!=0)
            {
                char* Ty=new char[P];
                POS->WSP.ScanfInput(Ty,P,I);
                POS->Lock();
                POS->ReceiveBuffer.Insert(SerialData(Ty,P));
                POS->UnLock();
                delete [](Ty);
            }
        }
        SDL_Delay(1);
    }
    return 0;
}

#endif // POSERIAL_H

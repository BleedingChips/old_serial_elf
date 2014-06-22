#ifndef WINSERIALPORT_H
#define WINSERIALPORT_H
#include <string>
class WinSerialPort
{
    HANDLE hCom;
    DCB DCBPoint;
    COMMTIMEOUTS TimeOutPoint;
    bool Open;
    std::string Name;
    COMSTAT State;
public:
    operator bool()
    {
        return Open;
    }
    bool CreatSerial(const std::string& N,unsigned int Bit=9600);
    bool CloseSerial();
    WinSerialPort():Open(false) {}
    bool WriteFinish();
    bool ReadSomeThing();
    bool Write(char* data,DWORD Size,DWORD& WS)
    {
        return WriteFile(hCom,data,Size,&WS,NULL);
    }
    bool ScanfInput(char* data,DWORD Size,DWORD& RS)
    {
        return ReadFile(hCom,data,Size,&RS,NULL);
    }
    bool GetState(DWORD& K)
    {
        return ClearCommError(hCom,&K,&State);
    }
    int ReadBuffer()
    {
        return State.cbInQue;
    }
    int WriteBuffer()
    {
        return State.cbOutQue;
    }
    ~WinSerialPort()
    {
        CloseSerial();
    }
};
bool WinSerialPort::CloseSerial()
{
    if(Open)
    {
        CloseHandle(hCom);
        Open=false;
        return true;
    }
    return false;
}
bool WinSerialPort::CreatSerial(const std::string& Tr,unsigned int Bit)
{
    CloseSerial();
    hCom=CreateFile(Tr.c_str(),GENERIC_READ | GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL/*|FILE_FLAG_OVERLAPPED*/,NULL);
    if(hCom==INVALID_HANDLE_VALUE)
        return false;
    Open=true;
    Name=Tr;
    if(!SetupComm(hCom,1024,1024))
        return false;
    if(!GetCommState(hCom,&DCBPoint))
        return false;
    DCBPoint.ByteSize=8;
    DCBPoint.Parity=NOPARITY;
    DCBPoint.StopBits=ONESTOPBIT;
    DCBPoint.BaudRate=Bit;
    SetCommMask(hCom,EV_RXCHAR);
    if(!SetCommState(hCom,&DCBPoint))
        return false;
    if(!GetCommTimeouts(hCom,&TimeOutPoint))
        return false;
    TimeOutPoint.ReadIntervalTimeout=10; //读间隔超时
    TimeOutPoint.ReadTotalTimeoutMultiplier=50; //读时间系数
    TimeOutPoint.ReadTotalTimeoutConstant=50; //读时间常量
    TimeOutPoint.WriteTotalTimeoutMultiplier=50; // 写时间系数
    TimeOutPoint.WriteTotalTimeoutConstant=10; //写时间常量
    if(!SetCommTimeouts(hCom,&TimeOutPoint))
        return false;
    return true;
}

#endif // WINSERIALPORT_H

#ifndef POSERIALREPORT_H
#define POSERIALREPORT_H
#include "winserialport.h"
enum SerialReportStyle
{
    SER_DINNOT_OPEN,
    SER_OPEN_FALSE,
    SER_AUTO_CLOSE,
    SER_CLOSE,
    SER_DCB_FALSE,
    SER_GET_DCB_FALSE,
    SER_BUFFER_FALSE,
    SER_TIME_OUT_FALSE,
    SER_GET_TIME_OUT_FALSE,
    SER_FASLE_WRITE,
    SER_FALSE_READ,
    SER_OPEN
};

class SerialReportData
{
public:
    SerialReportStyle Scription;
    int WrongNumber;
    SerialReportData():WrongNumber(0) {}
    SerialReportData(SerialReportStyle P,int Wn):Scription(P),WrongNumber(Wn) {}
    SerialReportData(const SerialReportData& D):Scription(D.Scription),WrongNumber(D.WrongNumber) {}
};

typedef std::deque<SerialReportData> SerialReportList;

class POSerialReport
{
    POMutex ReportMutex;
    SerialReportList SE;
public:
    bool HaveReport()
    {
        ReportMutex.Lock();
        bool Tem=!SE.empty();
        ReportMutex.UnLock();
        return Tem;
    }
    void GetReport(SerialReportStyle &P,int &C);
    void PushReport(SerialReportStyle P,int O=-1)
    {
        ReportMutex.Lock();
        SE.push_back(SerialReportData(P,O));
        ReportMutex.UnLock();
    }
    static std::string Translate(SerialReportStyle K);
    //void GetReport(WinSerialPortError W);
};

void POSerialReport::GetReport(SerialReportStyle& P,int& I)
{
    ReportMutex.Lock();
    if(!SE.empty())
    {
        SerialReportList::iterator Po=SE.begin();
        if(Po!=SE.end())
        {
            P=Po->Scription;
            I=Po->WrongNumber;
            SE.pop_front();
        }
    }
    ReportMutex.UnLock();
}


std::string POSerialReport::Translate(SerialReportStyle K)
{
    switch(K)
    {
    case SER_AUTO_CLOSE:
        return "Serial Been Closed Due to Open A New Serial.";
        break;
    case SER_BUFFER_FALSE:
        return "Creat Buffer false for Serial.";
        break;
    case SER_CLOSE:
        return "Serial is Successfully Colsed.";
        break;
    case SER_DCB_FALSE:
        return "Setting DCB for Serial False.";
        break;
    case SER_DINNOT_OPEN:
        return "Serial Did Not Open.";
        break;
    case SER_FALSE_READ:
        return "Serial False In Reading.";
        break;
    case SER_FASLE_WRITE:
        return "Serial False In Writting.";
        break;
    case SER_GET_DCB_FALSE:
        return "Serial False In Getting DCB.";
        break;
    case SER_GET_TIME_OUT_FALSE:
        return "Serial False In Getting TimeOut.";
        break;
    case SER_OPEN:
        return "Serial Is Successfully Open.";
        break;
    case SER_OPEN_FALSE:
        return "Serial False In Openning.";
        break;
    case SER_TIME_OUT_FALSE:
        return "False In Setting TimeOut Data.";
        break;
    }
}
#endif // POSERIALREPORT_H

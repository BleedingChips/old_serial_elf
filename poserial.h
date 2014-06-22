#ifndef POSERIAL_H_INCLUDED
#define POSERIAL_H_INCLUDED
#include "po/plugin.h"
#include "po/po.h"
#include "poextre/serialteminal.h"
#include "po/dialogmt.h"
const float Location[]={  -0.95,0.95,  -0.95,0.855,  -0.57,0.855,   -0.57,0.95, //Com 0~3
                              -0.494,0.95,   -0.494,0.855,   -0.304,0.855,  -0.304,0.95,//Deb1 4~7
                              -0.228,0.95,   -0.228,0.855,   -0.038,0.855,  -0.038,0.95,//Deb2 8~11
                               0.038,0.95,    0.038,0.855,     0.304,0.855,   0.304,0.95,//Tran1 12~15
                               0.38,0.95,    0.38,0.855,     0.646,0.855,   0.646,0.95,//Tran2 16~19
                               0.722,0.95,    0.722,0.855,     0.874,0.855,   0.874,0.95,//Paint 20~23
                               -0.95,-0.55,   -0.95,-0.95,      0.65,-0.95,   0.65,-0.55, //Debug Out 24~27
                               0.7,-0.55,      0.7,-0.675,      0.9,-0.675,    0.9,-0.55,//SentButton 28~31
                               -0.95,0.85,      -0.95,-0.15,          0.85,-0.15,       0.85,0.85,   //Data for Deb1 32~35
                               -0.95,-0.2,      -0.95,-0.5,          0.85,-0.5,           0.85,-0.2, //DataInput for Deb1 36~39
                               -0.95,0.85,      -0.95,0.45,          0.85,0.45,       0.85,0.85,   //Data for Deb2 and Tran1 40~43,

                               -0.9,0.425,    -0.9,0.325,       0.7,0.325,       0.7,0.425, //DataInput1 for Deb2 44~47
                               -0.9,0.275,    -0.9,0.175,       0.7,0.175,       0.7,0.275, //DataInput2 for Deb2 48~51
                               -0.9,0.125,    -0.9,0.025,       0.7,0.025,       0.7,0.125, //DataInput3 for Deb2 52~55
                               -0.9,-0.025,    -0.9,-0.125,       0.7,-0.125,       0.7,-0.025, //DataInput4 for Deb2 56~59
                               -0.9,-0.175,    -0.9,-0.275,       0.7,-0.275,       0.7,-0.175, //DataInput5 for Deb2 60~63
                               -0.9,-0.325,    -0.9,-0.425,       0.7,-0.425,       0.7,-0.325, //DataInput6 for Deb2 64~67

                               0.8,0.425,    0.8,0.325,       0.9,0.325,       0.9,0.425, //DataInput1State for Deb2 68~71
                               0.8,0.275,    0.8,0.175,       0.9,0.175,       0.9,0.275, //2 for Deb2 72~75
                               0.8,0.125,    0.8,0.025,       0.9,0.025,       0.9,0.125, //3 for Deb2 76~79
                               0.8,-0.025,    0.8,-0.125,       0.9,-0.125,       0.9,-0.025, //4 for Deb2 80~83
                               0.8,-0.175,    0.8,-0.275,       0.9,-0.275,       0.9,-0.175, //5 for Deb2 84~87
                               0.8,-0.325,    0.8,-0.425,       0.9,-0.425,       0.9,-0.325, //6 for Deb2 88~91

                               -0.768,0.4,      -0.768,0.28,    -0.576,0.28,     -0.576,0.4,//All for Tran1 92~95
                               -0.384,0.4,      -0.384,0.28,    -0.24,0.28,        -0.24,0.4,//T1 for Tran1 96~99
                               -0.048,0.4,      -0.048,0.28,    0.096,0.28,       0.096,0.4,//T2 for Tran1 100~103
                               0.288,0.4,       0.288,0.28,     0.432,0.28,       0.432,0.4,//T3 for Tran1 104~107
                               0.624,0.4,        0.624,0.28,      0.768,0.28,     0.768,0.4, //T4 for Tran1 108~111

                                -0.3,0.0,         -0.3,-0.3,      -0.15,-0.3,        -0.15,0.0,//  S1 for Tran1 112~115
                                -0.15,0.0,        -0.15,-0.3,   0.0, -0.3,          0.0,0.0,//S2 for Tran1 116~119
                                0.4,0.0,              0.4,-0.3,      0.7,-0.3,          0.7,0.0, //C for Tran1 120~123
                                0.0,0.0,           0.0, -0.3,       0.15,-0.3,          0.15,0.0,//N1 for Tran1 124~127
                                0.15,0.0,           0.15, -0.3,       0.3,-0.3,          0.3,0.0//N2 for Tran1 128~131

                               };//Paint 20~23
#define COM_L 0
#define DEB1_L 8
#define DEB2_L 16
#define TRAN1_L 24
#define TRAN2_L 32
#define PAINT_L 40
#define DEBOUT_L 48
#define SENB_L 56
#define DATFORDEB1_L 64
#define DATAINPUTDEB1_L 72
#define DATFORDEB2_L 80
#define DATAINPUTDEB2_L 88
#define DATAINPUTSTATEDEB2_L 136
#define TRANLIGHT_L 184
#define TRANSYMPLE1_L 224
#define TRANNUMBER1_L 248
#define TRANCOLOR1_L 240


#define EDITM_NORMAL 0
#define EDITM_COM 1
#define EDITM_DATABLOCKFORDEB1 2
#define EDITM_DBDEB2 3
#define EDITM_DBDEB2_MAX 8
/*unsigned int Order[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,
                                     40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,
                                     76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92};*/
char T1Define[5][30]={" %ef; -w 100 "," %01; -w 100 "," %02; -w 100 ", " %03; -w 100 ", " %04; -w 100 "};

class TestSerial:public Plugin
{
    DynamicDialogMT Com;uile
    EditDialogMT EDMT;
    StaticDialogMT Debug;
    DynamicDialogMT Data,SentBlockDeb1,SentBlockDeb2[6];
    unsigned int InputMode,T1P[6];
    bool OpenSerial;
    bool Editing,Sentting;
    bool UseSenData[6];
    unsigned int EditMode;
public:
    ~TestSerial() {}
    TestSerial():InputMode(1) ,OpenSerial(false),EditMode(0)
    {
        for(int i=0;i<6;++i)
            T1P[i]=0;
    }
    class Render;
    class Logic;
};
class TestSerial::Render:public Plugin::Render
{
    POTextStatic ModeSelect,SentButton,TLight;
    DynamicDialogMT::Render Com;
    DynamicDialogMT::Render Data,SentBlockDeb1,SentBlockDeb2[6];
    PONumber T1N;
    StaticDialogMT::Render Debug;
    POPicture Symple[5];
    //StaticDialogMT::Render Debug;
    EditDialogMT::Render EDMT;
    TestSerial* TS;
    PONumber PON;
    unsigned int InputMode,T1P[6];
    bool OpenSerial,Editing,Senting,UseSenData[6];
    unsigned int EditMode;
    unsigned int *Order;
public:
    void Layer2D(PORender * P)
    {
        glColor3f(0.2,0.4,0.0);
        glBegin(GL_QUADS);
        glVertex2f(-1.0,1.0);
        glVertex2f(-1.0,-1.0);
        glVertex2f(1.0,-1.0);
        glVertex2f(1.0,1.0);
        glEnd();
        glVertexPointer(2,GL_FLOAT,0,Location);
        glEnableClientState(GL_VERTEX_ARRAY);
        if(OpenSerial) glColor3f(0.0,0.0,0.8);else glColor3f(0.8,0,0);
        glDrawElements(GL_QUADS,4, GL_UNSIGNED_INT,Order);
        glColor3f(0.54,0.27,0.07);
        glDrawElements(GL_QUADS,4, GL_UNSIGNED_INT,Order+InputMode*4);
        if(Senting)  glColor3f(1.0,0.0,0.0); else glColor3f(0.0,0.0,1.0);
        glDrawElements(GL_QUADS,4, GL_UNSIGNED_INT,Order+SENB_L/2);
        glColor3f(0.8,1.0,0.0);
        glDrawElements(GL_LINE_LOOP,4, GL_UNSIGNED_INT,Order+DEBOUT_L/2);

        switch(InputMode)
        {
        case 1:
            {
                glColor3f(0.0,0.0,0.5);
                glDrawElements(GL_QUADS,4, GL_UNSIGNED_INT,Order+DATAINPUTDEB1_L/2);
                glColor3f(0.8,1.0,0.0);
                glDrawElements(GL_LINE_LOOP,4, GL_UNSIGNED_INT,Order+DATFORDEB1_L/2);
                if(Data.Demand()) Data.Respond();
                if(SentBlockDeb1.Demand()) SentBlockDeb1.Respond();
                unsigned int Line=Data.CountLine(45);
                if(Line>10)
                {
                    Data.PopFront((Line-10)*45);
                    Line=10;
                }
                Data.Display(Location[DATFORDEB1_L],Location[DATFORDEB1_L+4],Location[DATFORDEB1_L+3]+Line*0.1,45);
                //SentBlockDeb1.Display(Location[DATAINPUTDEB1_L],Location[DATAINPUTDEB1_L+4],Location[DATAINPUTDEB1_L+1],45);
            }
            break;
        case 2:
            {
                glColor3f(0.0,0.0,0.5);
                for(int i=0;i<6;++i)
                    glDrawElements(GL_QUADS,4, GL_UNSIGNED_INT,Order+(DATAINPUTDEB2_L+i*8)/2);
                for(int i=0;i<6;++i)
                {
                    if(UseSenData[i]) glColor3f(0.0,1.0,0.0); else glColor3f(0.2,0.2,0.2);
                    glDrawElements(GL_QUADS,4, GL_UNSIGNED_INT,Order+(DATAINPUTSTATEDEB2_L+i*8)/2);
                }
                glColor3f(0.8,1.0,0.0);
                glDrawElements(GL_LINE_LOOP,4, GL_UNSIGNED_INT,Order+DATFORDEB2_L/2);
                if(Data.Demand()) Data.Respond();
                unsigned int Line=Data.CountLine(45);
                for(int i=0;i<6;++i)
                {
                    if(SentBlockDeb2[i].Demand()) SentBlockDeb2[i].Respond();
                    SentBlockDeb2[i].Display(Location[DATAINPUTDEB2_L+i*8],Location[DATAINPUTDEB2_L+i*8+4],Location[DATAINPUTDEB2_L+i*8+1],40);
                }
                if(Line>5)
                {
                    Data.PopFront((Line-10)*45);
                    Line=5;
                }
                Data.Display(Location[DATFORDEB2_L],Location[DATFORDEB2_L+4],Location[DATFORDEB2_L+3]+Line*0.1,45);
                //Deb2[0].Display(XYF(Location[DATAINPUT1DEB2_L]-0.05,Location[DATAINPUT1DEB2_L+1]),XYF(1.0,0.0),0.125);
            }
            break;
        case 3:
            {
                glColor3f(0.8,1.0,0.0);
                glDrawElements(GL_LINE_LOOP,4, GL_UNSIGNED_INT,Order+DATFORDEB2_L/2);
                glColor3f(1.0,0.0,0.0);
                glDrawElements(GL_QUADS,4, GL_UNSIGNED_INT,Order+(TRANLIGHT_L+T1P[5]*8)/2);
                if(T1P[2]==0) glColor3f(0.5,0.0,0.0); else if(T1P[2]==1) glColor3f(0.5,0.5,0.0); else glColor3f(1.0,0.5,0.0);
                glDrawElements(GL_QUADS,4, GL_UNSIGNED_INT,Order+(TRANCOLOR1_L)/2);
                TLight.Display(XYF(Location[TRANLIGHT_L],Location[TRANLIGHT_L+1]),XYF(1.0,0.0),0.12);
                if(Data.Demand()) Data.Respond();
                unsigned int Line=Data.CountLine(45);
                if(Line>5)
                {
                    Data.PopFront((Line-10)*45);
                    Line=5;
                }
                Data.Display(Location[DATFORDEB2_L],Location[DATFORDEB2_L+4],Location[DATFORDEB2_L+3]+Line*0.1,45);
                for(int i=0;i<2;++i)
                    Symple[T1P[i]].Display(XYF(Location[TRANSYMPLE1_L +i*8],Location[TRANSYMPLE1_L +i*8+1]),
                                     XYF(Location[TRANSYMPLE1_L +4+i*8]-Location[TRANSYMPLE1_L +i*8],0.0),
                                     XYF(0.0,Location[TRANSYMPLE1_L +i*8+3]-Location[TRANSYMPLE1_L +i*8+1]));
                for(int i=3;i<5;++i)
                    T1N.Display(T1P[i],XYF(Location[TRANNUMBER1_L+(i-3)*8],Location[TRANNUMBER1_L+(i-3)*8+1]),XYF(1.0,0.0),0.3);
            }
            break;
        }
        if(Debug.Demand()) Debug.Respond();
        while(Debug.Line()>5)
            Debug.PopFront();
        Debug.Display(Location[DEBOUT_L],Location[DEBOUT_L+4],Location[DEBOUT_L+3]+0.08*Debug.Line());

        SentButton.Display(XYF(Location[SENB_L],Location[SENB_L+1]),XYF(1.0,0.0),0.125);

        if(Editing)
        {
            glVertexPointer(2,GL_FLOAT,0,Location);
            glEnableClientState(GL_VERTEX_ARRAY);
            glColor4f(0.2,0.2,0.2,0.6);
            glDrawElements(GL_QUADS,4, GL_UNSIGNED_INT,Order+24);
            if(EDMT.Demand()) EDMT.Respond();
            EDMT.Display(Location[DEBOUT_L],Location[DEBOUT_L+4],Location[DEBOUT_L+1],50,P->Time());
        }



        if(Com.Demand()) Com.Respond(); Com.Display(Location[COM_L],Location[COM_L+4],Location[COM_L+1],10);
        ModeSelect.Display(XYF(Location[DEB1_L],Location[DEB1_L+1]),XYF(1.0,0.0),0.095);

        //float X,Y;
        //P->GetFloatMouseLocation(X,Y);
        //PON.Display(Y,XYF(X,Y+0.08),XYF(1.0,0.0),0.08);
        //PON.Display(X,XYF(X,Y+0.16),XYF(1.0,0.0),0.08);
    }
    void Init(PORender* P)
    {
        glEnable(GL_BLEND);
        ModeSelect.CreatText(Translate("调试1  调试2  交通灯1  交通灯2  涂鸦"));
        SentButton.CreatText(Translate("发送"));
        TLight.CreatText(Translate("全体    灯1    灯2    灯3    灯4"));
        Symple[0].Creat("resource/front.png");
        Symple[1].Creat("resource/left.png");
        Symple[2].Creat("resource/right.png");
        Symple[3].Creat("resource/frontright.png");
        Symple[4].Creat("resource/frontleft.png");
    }
    void PluginConnect(Plugin *PS)
    {
        TS=dynamic_cast<TestSerial*>(PS);
        Entrust(Com.GetPoint(&TS->Com));
        Entrust(EDMT.GetPoint(&TS->EDMT));
        Entrust(Debug.GetPoint(&TS->Debug));
        Entrust(Data.GetPoint(&TS->Data));
        Entrust(SentBlockDeb1.GetPoint(&TS->SentBlockDeb1));
        for(int i=0;i<6;++i)
            Entrust(SentBlockDeb2[i].GetPoint(&TS->SentBlockDeb2[i]));
    }
    void PluginSynchroIn()
    {
        InputMode=TS->InputMode;
        OpenSerial=TS->OpenSerial;
        Editing=TS->Editing;
        EditMode=TS->EditMode;
        Senting=TS->Sentting;
        if(InputMode==2)
            for(int i=0;i<6;++i)
                UseSenData[i]=(TS->UseSenData)[i];
        if(InputMode==3)
        {
            for(int i=0;i<6;++i)
                T1P[i]=TS->T1P[i];
        }
    }
    Render():InputMode(1),Debug(50)
    {
        Order=new unsigned int [132];
        for(int i=0;i<132;++i)
            Order[i]=i;
    }
    ~Render() {delete[] (Order);}
};
class TestSerial::Logic:public Plugin::Logic
{
    DynamicDialogMT::Logic Com;
    TestSerial* TS;
    unsigned int InputMode,Light,T1P[6];
    SerialTeminal ST;
    EditDialogMT::Logic EDMT;
    StaticDialogMT::Logic Debug;
    DynamicDialogMT::Logic Data,SentBlockDeb1,SentBlockDeb2[6];
    bool Editing,Sentting,UseSenData[6];
    unsigned int EditMode;
    std::string SentDataForDeb1,SentDataDeb2[6];
public:
    ~Logic() {}
    void PluginSynchroOut()
    {
        TS->InputMode=InputMode;
        TS->OpenSerial=ST.ComReady();
        TS->Editing=Editing;
        TS->EditMode=EditMode;
        TS->Sentting=Sentting;
        if(InputMode==2)
            for(int i=0;i<6;++i)
                TS->UseSenData[i]=UseSenData[i];
        if(InputMode==3)
        {
            for(int i=0;i<6;++i)
                TS->T1P[i]=T1P[i];
        }
    }
    void Respond(const Information* Pi,POLogic* POA)
    {
        if((!Editing)&&Pi->IsMouseButtonUp())
        {
            float X,Y;
            POA->GetFloatMouse(X,Y);
            if(Y>Location[DEB1_L+3]&&Y<=Location[DEB1_L+1])
            {
                if(Pi->IsMouseButtonLeft())
                {
                    if(X>Location[DEB1_L]&&X<=Location[DEB1_L+4])
                        InputMode=1;
                    else if(X>Location[DEB2_L]&&X<=Location[DEB2_L+4])
                        InputMode=2;
                    else if(X>Location[TRAN1_L]&&X<=Location[TRAN1_L+4])
                        InputMode=3;
                    else if(X>Location[TRAN2_L]&&X<=Location[TRAN2_L+4])
                        InputMode=4;
                    else if(X>Location[PAINT_L]&&X<=Location[PAINT_L+4])
                        InputMode=5;
                    else if(X>Location[COM_L]&&X<=Location[COM_L+4])
                    {
                        Editing=true;
                        EditMode=EDITM_COM;
                        Com.Clear();
                    }
                }else if(Pi->IsMouseButtonRight())
                {
                    if(X>Location[COM_L]&&X<=Location[COM_L+4])
                    {
                        Com.Clear();
                        Com.Insert(Translate("打开串口"));
                        ST.GetComment("close serial");
                        Debug.Insert("close serial");
                    }
                }
            }else if(Y>Location[SENB_L+3]&&Y<=Location[SENB_L+1]&&X>Location[SENB_L]&&X<=Location[SENB_L+4])
            {
                if(Pi->IsMouseButtonLeft()&&(Sentting))
                {
                    switch(InputMode)
                    {
                    case 1:
                        Debug.Insert("*:write "+SentDataForDeb1);
                        ST.GetComment("write "+SentDataForDeb1);
                        break;
                    case 2:
                        {
                            std::string Tem;
                            for(int i=0;i<6;++i)
                            {
                                if(UseSenData[i])
                                    Tem+=" "+SentDataDeb2[i];
                            }
                            Debug.Insert("*:write "+Tem);
                            ST.GetComment("write "+Tem);
                        }
                        break;
                    case 3:
                        {
                            std::string Tem;
                            Tem+=T1Define[T1P[5]];
                            Tem+=" %b1; -w 15 %";
                            for(int i=0;i<5;++i)
                            {
                                Tem+="0";
                                char P='0';
                                P+=T1P[i];
                                if(i<3)
                                    ++P;
                                Tem+=P;
                                if(i!=4)
                                    Tem+=",";
                                else Tem+=";";
                            }
                            Debug.Insert("*:write"+Tem);
                            ST.GetComment("write "+Tem);
                        }
                        break;
                    }
                }else if(Pi->IsMouseButtonRight()&&(!Sentting))
                {
                    Debug.Insert("close channel");
                    ST.GetComment("close channel");
                }
            }else{
                    switch(InputMode)
                    {
                    case 1:
                        if(Y>Location[DATAINPUTDEB1_L+3]&&Y<=Location[DATAINPUTDEB1_L+1]&&X>Location[DATAINPUTDEB1_L]&&X<=Location[DATAINPUTDEB1_L+4])
                        {
                            if(Pi->IsMouseButtonLeft())
                            {
                                EDMT.Insert(SentDataForDeb1);
                                SentBlockDeb1.Clear();
                                Editing=true;
                                EditMode=EDITM_DATABLOCKFORDEB1;
                            }else if(Pi->IsMouseButtonRight())
                            {
                                SentDataForDeb1.clear();
                                SentBlockDeb1.Clear();
                            }
                        }
                        break;
                    case 2:
                        if(X>Location[DATAINPUTDEB2_L]&&X<=Location[DATAINPUTDEB2_L+4])
                        {
                            if(Pi->IsMouseButtonLeft())
                            {
                                for(int i=0;i<6;++i)
                                {
                                    if(Y<=Location[DATAINPUTDEB2_L+i*8+1]&&Y>Location[DATAINPUTDEB2_L+i*8+3])
                                    {
                                        EDMT.Insert(SentDataDeb2[i]);
                                        SentBlockDeb2[i].Clear();
                                        Editing=true;
                                        EditMode=EDITM_DBDEB2+i;
                                        break;
                                    }
                                }
                            }else if(Pi->IsMouseButtonRight())
                            {
                                for(int i=0;i<6;++i)
                                {
                                    if(Y<=Location[DATAINPUTDEB2_L+i*8+1]&&Y>Location[DATAINPUTDEB2_L+i*8+3])
                                    {
                                        SentDataDeb2[i].clear();
                                        SentBlockDeb2[i].Clear();
                                        break;
                                    }
                                }
                            }
                        }else if(X>Location[DATAINPUTSTATEDEB2_L]&&X<=Location[DATAINPUTSTATEDEB2_L+4])
                            if(Pi->IsMouseButtonLeft())
                                for(int i=0; i<6; ++i)
                                    if(Y<=Location[DATAINPUTSTATEDEB2_L+i*8+1]&&Y>Location[DATAINPUTSTATEDEB2_L+i*8+3])
                                    {
                                        UseSenData[i]=!UseSenData[i];
                                        break;
                                    }
                        break;
                    case 3:
                        if(Y<=Location[TRANLIGHT_L+1]&&Y>=Location[TRANLIGHT_L+3])
                        {
                            if(Pi->IsMouseButtonLeft())
                            {
                                for(int i=0;i<5;++i)
                                    if(X>Location[TRANLIGHT_L+i*8]&&X<Location[TRANLIGHT_L+i*8+4])
                                    {
                                        T1P[5]=i;
                                        break;
                                    }
                            }
                        }else if(Y<=Location[TRANSYMPLE1_L +1]&&Y>=Location[TRANSYMPLE1_L +3])
                        {
                            if(Pi->IsMouseButtonLeft())
                            {
                                for(int i=0;i<5;++i)
                                {
                                    if(X>Location[TRANSYMPLE1_L+i*8]&&X<=Location[TRANSYMPLE1_L+i*8+4])
                                    {
                                        if(i<2)
                                        {
                                            if(T1P[i]<4) ++T1P[i]; else T1P[i]=0;
                                        }else if(i<3)
                                        {
                                            if(T1P[i]<2) ++T1P[i];else T1P[i]=0;
                                        }else{
                                            if(T1P[i]<9) ++T1P[i];else T1P[i]=0;
                                        }
                                        break;
                                    }
                                }
                            }else  if(Pi->IsMouseButtonRight())
                            {
                                for(int i=0;i<5;++i)
                                {
                                    if(X>Location[TRANSYMPLE1_L+i*8]&&X<=Location[TRANSYMPLE1_L+i*8+4])
                                    {
                                        if(i<2)
                                        {
                                            if(T1P[i]>0) --T1P[i]; else T1P[i]=4;
                                        }else if(i<3)
                                        {
                                            if(T1P[i]>0) --T1P[i];else T1P[i]=2;
                                        }else{
                                            if(T1P[i]>0) --T1P[i];else T1P[i]=9;
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                        break;
                    }
                }
        }else if(Pi->IsKeyDown())
        {
            if(Pi->IsSpecialKeyEnter())
            {
                if(!Editing)
                    Editing=true;
                else
                {
                    std::string Tem=EDMT.GetString();
                    if(!Tem.empty())
                    {
                        switch(EditMode)
                        {
                        case EDITM_NORMAL:
                            Debug.Insert("*:"+Tem);
                            ST.GetComment(Tem);
                            EDMT.Clear();
                            break;
                        case EDITM_COM:
                            Com.Insert(Tem);
                            Tem="open "+Tem;
                            Debug.Insert("*:"+Tem);
                            ST.GetComment(Tem);
                            EDMT.Clear();
                            break;
                        case EDITM_DATABLOCKFORDEB1:
                            SentDataForDeb1=Tem;
                            SentBlockDeb1.Insert(Tem);
                            EDMT.Clear();
                            break;
                        default:
                            if(EditMode>=EDITM_DBDEB2&&EditMode<=EDITM_DBDEB2_MAX)
                            {
                                unsigned int P=EditMode-EDITM_DBDEB2;
                                SentDataDeb2[P]=Tem;
                                SentBlockDeb2[P].Insert(Tem);
                                EDMT.Clear();
                            }
                            break;

                        }
                    }else if(EditMode==EDITM_COM)
                    {
                        Com.Insert(Translate("打开串口"));
                    }
                    EDMT.Clear();
                    Editing=false;
                    EditMode=EDITM_NORMAL;
                }
            }else if(Editing)
            {
                if(Pi->IsKeySeem()||Pi->IsSpecialKeySpace())
                {
                    std::string K;
                    K+=static_cast<char>(Pi->GetKey());
                    EDMT.Insert(K);
                }else if(Pi->IsSpecialKeyDelete())
                    EDMT.CursorDelete();
                else if(Pi->IsSpecialKeyLeft())
                    EDMT.CursorFront();
                else if(Pi->IsSpecialKeyRight())
                    EDMT.CursorBack();
            }
        }
    }
    void Init(POLogic *PA)
    {
        OpenKey();
        OpenMouse();
        Com.Insert(Translate("打开端口"));
    }
    void Work(POLogic *PA)
    {
        using namespace std;
        string Go;
        while(ST.GetCommentOut(Go))
            Debug.Insert(Go);
        SerialData SD;
        while(ST.GetData(SD))
        {
            string K=SD.ValueString();
            Data.Insert(K);
            SD.Clear();
        }
        Sentting=ST.Sentting();
    }
    void PluginConnect(Plugin *PS)
    {
        TS=dynamic_cast<TestSerial*>(PS);
        Entrust(Com.GetPoint(&TS->Com));
        Entrust(EDMT.GetPoint(&TS->EDMT));
        Entrust(Debug.GetPoint(&TS->Debug));
        Entrust(Data.GetPoint(&TS->Data));
        Entrust(SentBlockDeb1.GetPoint(&TS->SentBlockDeb1));
        for(int i=0;i<6;++i)
            Entrust(SentBlockDeb2[i].GetPoint(&TS->SentBlockDeb2[i]));
    }
    Logic():InputMode(1)
    {
        for(int i=0;i<6;++i)
            T1P[i]=0;
    }
};
void TestSerialInitFunction(Handle<Plugin::Render>& P,Handle<Plugin::Logic>& A,Plugin*& PS)
{
    P=new TestSerial::Render;
    A=new TestSerial::Logic;
    PS=new TestSerial;
}

#endif // POSERIAL_H_INCLUDED

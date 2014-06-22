#ifndef TEXTPICTURE_H_INCLUDED
#define TEXTPICTURE_H_INCLUDED
#include "po/po.h"
#include "po/plugin.h"
#include "po/popicture.h"
#include "po/dialogmt.h"
#include "po/glsimplify.h"
#include "po/potext.h"
#include "po/extrabuffer.h"
#include "po/dialog.h"
class TextPictureP:public Plugin
{
public:
    ExtraBuffer<int> K;
    StaticDialogMT SDM;
    DynamicDialogMT DDM;
    EditDialogMT EDMT;
};

class TextPicture:public Plugin::Render
{
    POPicture Try;
    POTextStatic POT;
    POTextDynamic POD;
    PONumber PON;
    ExtraBuffer<int>::Receive K;
    unsigned long T;
    StaticDialog SD;
    StaticDialogMT::Render SDM;
    DynamicDialog DD;
    DynamicDialogMT::Render DDM;
    EditDialogMT::Render EDMT;
public:
    void PluginConnect(Plugin* P)
    {
        TextPictureP* TP=dynamic_cast<TextPictureP*>(P);
        Entrust(K.GetPoint(&TP->K));
        Entrust(SDM.GetPoint(&TP->SDM));
        Entrust(DDM.GetPoint(&TP->DDM));
        Entrust(EDMT.GetPoint(&TP->EDMT));
    }
    void Layer2D(PORender* P)
    {
        //Try.Display(XYF(-1.0,1.0),XYF(0.8,0.0),XYF(0.0,-2.0));
        T+=P->Time();
        if(K){
            K.Read(&TextPicture::Read,this);
            T=0;
        }
        POT.Display(XYF(-1.0,1.0),XYF(1.0,0.0));
        if(T<1000)
            POD.Display(XYF(-1.0,0.9),XYF(1.0,0.0),0.1,0.5,0.5,0.5,0.8);
        if(DDM.Demand()) DDM.Respond();
        if(SDM.Demand()) SDM.Respond();
        SDM.Display(-1.0,1.0,-0.3);
        //DD.Display(-1.0,1.0,1.0,20,1.0,1.0,1.0,1.0);
            DDM.Display(-1.0,1.0,0.0,60);
            PON.Display(T,XYF(-1.0,0.5),XYF(1.0,0.0));
        if(EDMT.Demand()) EDMT.Respond();
        EDMT.Display(-1.0,1.0,0.4,60,P->Time());
    }
    void Init(PORender* P)
    {
        POT.CreatText("lala");
        POD.CreatText(Translate("你好啊"));
        DD.Insert("GoGo");
        //SD.Insert(Translate("你好啊"));
        //SD.Insert(Translate("我是谁？"));
        //SD.Insert(Translate("我试试看dsf草果楼市个字符到dasf底需要怎样的处理adsf方式啦啦啦啦啦啦啦啦啦asdf啦啦啦啦啦啦啦啦"));
    }
    void Read(int* I){}
    TextPicture():T(2000),SD(20),SDM(60){}
};
class TextPictureL:public Plugin::Logic
{
    ExtraBuffer<int>::Sent K;
    StaticDialogMT::Logic SDM;
    DynamicDialogMT::Logic DDM;
    EditDialogMT::Logic EDMT;
public:
    void Wr(int* I){}
    void Init(POLogic* P){OpenKey();}
    void PluginConnect(Plugin* P)
    {
        TextPictureP* TP=dynamic_cast<TextPictureP*>(P);
        Entrust(K.GetPoint(&TP->K));
        Entrust(SDM.GetPoint(&TP->SDM));
        Entrust(DDM.GetPoint(&TP->DDM));
        Entrust(EDMT.GetPoint(&TP->EDMT));
    }
    void Respond(const Information* I,POLogic * PO)
    {
        if(I->IsKeyUp())
        {
            if(I->IsSpecialKeyLeft())
                EDMT.CursorFront();
            else if(I->IsSpecialKeyRight())
                EDMT.CursorBack();
            else if(I->IsSpecialKeyDelete())
                EDMT.CursorDelete();
            else if(I->IsSpecialKeyEnter())
            {
                std::string Tem=EDMT.GetString();
                EDMT.Clear();
                SDM.Insert(Tem);
            }else if(I->IsKeySeem()||I->IsSpecialKeySpace())
            {
                std::string Tem;
                Tem+=static_cast<char>(I->GetKey());
                EDMT.Insert(Tem);
            }
        }
    }
};
void TextFunction(Handle<Plugin::Render>& P,Handle<Plugin::Logic>& A,Plugin*& Pl)
{
    P=new TextPicture;
    Pl=new TextPictureP;
    A=new TextPictureL;
}


#endif // TEXTPICTURE_H_INCLUDED

#ifndef POPICTURE_H
#define POPICTURE_H
#include <iostream>
#include "gltexture.h"
#include "baseclass.h"
#include "picturemanager.h"
#include <map>
#include <string>
class POPicture:public GLTexture
{
public:
    POPicture(const POPicture& C):GLTexture(C){}
    POPicture():GLTexture(0,0,0){}
    operator bool(){return IsAvalible();}
    void Display(XYF P,XYF T,XYF U);
    bool Creat(const std::string& P){return PictureManager::CreatPicture(P,*this);}
    bool Free(){return PictureManager::FreeTexture(*this);}
    ~POPicture(){Free();}
};

void POPicture::Display(XYF P, XYF T, XYF U)
{
    P.OutFloat(GLTexture::Location);
    (P+U).OutFloat(GLTexture::Location+2);
    (P+T+U).OutFloat(GLTexture::Location+4);
    (P+T).OutFloat(GLTexture::Location+6);
    bool Tem=glIsEnabled(GL_BLEND);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    GLTexture::Display(GLTexture::Texture,2,GLTexture::Location,2,GLTexture::Order,6);
    if(!Tem)
        glDisable(GL_BLEND);
}
#endif // POPICTURE_H

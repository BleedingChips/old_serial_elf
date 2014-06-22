#ifndef GLTEXTURE_H
#define GLTEXTURE_H
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
class GLTexture
{
    unsigned int GLPoint;
    unsigned int TextureW;
    unsigned int TextureH;
protected:
    void FreeTexture();
public:
    GLTexture(unsigned int P,unsigned int W,unsigned int H):GLPoint(P),TextureW(W),TextureH(H){}
    GLTexture(const GLTexture& K):GLPoint(K.GLPoint),TextureW(K.TextureW),TextureH(K.TextureH){}
    void Display(float* TxP,int TSize,float* LoP,int LSize,unsigned int* Or,unsigned int M);
    void Display(float* TxP,int TSize,float* LoP,int LSize,float* CoP,int CSize,unsigned int* Or,unsigned int M);
    unsigned int GetPoint() const {return GLPoint;}
    unsigned int GetTextureW() const {return TextureW;}
    unsigned int GetTextureH() const {return TextureH;}
    unsigned int Set(unsigned int P,unsigned int W,unsigned int H){GLPoint=P;TextureW=W;TextureH=H;}
    bool IsAvalible() const {return glIsTexture(GLPoint)==GL_TRUE;}
    static bool CreatTexture(SDL_Surface*,unsigned int& P,unsigned int& W,unsigned int& H);
    static bool CreatTexture(SDL_Surface* K,GLTexture& P){return CreatTexture(K,P.GLPoint,P.TextureW,P.TextureH);}


    static float Location[12];
    static float Color[16];
    static float Texture[8];
    static unsigned int Order[6];
    static void SetColor(float R,float G,float B);
    static void SetColor(float R,float G,float B,float A);

    static void StartTexture(unsigned int L,unsigned int T);
    static void StartTextureWithColor(unsigned int L,unsigned int T,unsigned int C);
    static void CloseTexture();
    static void CloseTextureWithColor();
};

float GLTexture::Location[12];
float GLTexture::Color[16];
float GLTexture::Texture[8]={0.0,0.0,0.0,1.0,1.0,1.0,1.0,0.0};
unsigned int GLTexture::Order[6]={0,1,2,3,2,0};

void GLTexture::SetColor(float R,float G,float B)
{
    for(int i=0; i<4; ++i)
    {
        Color[i*3]=R;
        Color[i*3+1]=G;
        Color[i*3+2]=B;
    }
}
void GLTexture::SetColor(float R,float G,float B,float A)
{
    for(int i=0; i<4; ++i)
    {
        Color[i*4]=R;
        Color[i*4+1]=G;
        Color[i*4+2]=B;
        Color[i*4+3]=A;
    }
}
void GLTexture::FreeTexture()
{
    if(IsAvalible())
    {
        glDeleteTextures(1,&GLPoint);
        GLPoint=0;
    }
}

void GLTexture::Display(float *TxP, int TSize, float *LoP, int LSize,unsigned int* Order,unsigned int All)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(LSize,GL_FLOAT,0,LoP);
    glTexCoordPointer(TSize,GL_FLOAT,0,TxP);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,GLPoint);
    glDrawElements(GL_TRIANGLES,All, GL_UNSIGNED_INT,Order);
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void GLTexture::Display(float *TxP, int TSize, float *LoP, int LSize,float* CoP,int CSize,unsigned int* Order,unsigned int All)
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(LSize,GL_FLOAT,0,LoP);
    glColorPointer(CSize,GL_FLOAT,0,CoP);
    glTexCoordPointer(TSize,GL_FLOAT,0,TxP);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,GLPoint);
    glDrawElements(GL_TRIANGLES,All, GL_UNSIGNED_INT,Order);
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}
bool GLTexture::CreatTexture(SDL_Surface * Picture,unsigned int& P,unsigned int& W,unsigned int& H)
{
    if(Picture)
    {
        bool Wrong=false;
        glGenTextures(1,&P);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,P);
        unsigned int Fag=Picture->format->format;
        W=Picture->w;
        H=Picture->h;
        switch(Fag)
        {
        case SDL_PIXELFORMAT_BGR888:
            gluBuild2DMipmaps(GL_TEXTURE_2D,4,Picture->w,Picture->h,GL_RGB,GL_UNSIGNED_BYTE,Picture->pixels);
            break;
        case SDL_PIXELFORMAT_ABGR8888:
            gluBuild2DMipmaps(GL_TEXTURE_2D,4,Picture->w,Picture->h,GL_RGBA,GL_UNSIGNED_BYTE,Picture->pixels);
            break;
        default:
            {
                SDL_Surface* Picturenew=SDL_ConvertSurfaceFormat(Picture,SDL_PIXELFORMAT_ABGR8888,0);
                if(Picturenew)
                {
                    gluBuild2DMipmaps(GL_TEXTURE_2D,4,Picturenew->w,Picturenew->h,GL_RGBA,GL_UNSIGNED_BYTE,Picturenew->pixels);
                    W=Picturenew->w;
                    H=Picturenew->h;
                    SDL_FreeSurface(Picturenew);
                }else Wrong=true;
            }
            break;
        }
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        if(Wrong)
        {
            P=0;W=0;H=0;
            return false;
        }else return true;
    }else{
        P=0;
        W=0;
        H=0;
        return false;
    }
}

void GLTexture::StartTexture(unsigned int L,unsigned int T)
{
    glVertexPointer(L,GL_FLOAT,0,GLTexture::Location);
    glTexCoordPointer(T,GL_FLOAT,0,GLTexture::Texture);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
}

void GLTexture::StartTextureWithColor(unsigned int L,unsigned int T,unsigned int C)
{
    glVertexPointer(L,GL_FLOAT,0,GLTexture::Location);
    glTexCoordPointer(T,GL_FLOAT,0,GLTexture::Texture);
    glColorPointer(C,GL_FLOAT,0,GLTexture::Color);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
}

void GLTexture::CloseTexture()
{
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void GLTexture::CloseTextureWithColor()
{
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}
#endif // POGIDATA_H

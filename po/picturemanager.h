#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <map>
#include <string>
#include "gltexture.h"
#include <SDL2/SDL_image.h>
namespace PictureManager
{
    class PictureData:public GLTexture
    {
        unsigned int Count;
        std::string Scription;
public:
        PictureData(unsigned int P,unsigned int W,unsigned int H,const std::string& S):GLTexture(P,W,H),Count(0),Scription(S) {}
        PictureData(const PictureData& P):GLTexture(P),Count(P.Count),Scription(P.Scription) {}
        void Use(){Count+=1;}
        void Unuse(){Count-=1;}
        bool NoneUse(){return Count==0;}
        std::string GetScription(){return Scription;}
        void FreeTexture(){GLTexture::FreeTexture();}
    };
    std::map<unsigned int,PictureData> TextureMap;
    std::map<std::string,unsigned int> TextureDefine;
    bool HavePicture(const std::string& P){return TextureDefine.find(P)!=TextureDefine.end();}
    bool HavePicture(unsigned int P){return TextureMap.find(P)!=TextureMap.end();}
    bool FindAdress(const GLTexture& K,std::string& S);
    bool CreatPicture(const std::string& U,GLTexture&);
    bool FreeTexture(GLTexture&);
    void TextureMangerExit();
};

bool PictureManager::FindAdress(const GLTexture& K, std::string& S)
{
    if(K.IsAvalible())
    {
        std::map<unsigned int,PictureData>::iterator Po=TextureMap.find(K.GetPoint());
        if(Po!=TextureMap.end())
        {
            S=Po->second.GetScription();
            return true;
        }
    }
    return false;
}

bool PictureManager::FreeTexture(GLTexture& T)
{
    using namespace std;
    if(T.IsAvalible())
    {
        map<unsigned int,PictureData>::iterator Po=TextureMap.find(T.GetPoint());
        if(Po!=TextureMap.end())
        {
            Po->second.Unuse();
            if(Po->second.NoneUse())
            {
                std::string K;
                K=Po->second.GetScription();
                Po->second.FreeTexture();
                TextureMap.erase(Po);
                map<string,unsigned int>::iterator Po2=TextureDefine.find(K);
                if(Po2!=TextureDefine.end())
                    TextureDefine.erase(Po2);
            }
        }
        T.Set(0,0,0);
        return true;
    }
    return false;
}

bool PictureManager::CreatPicture(const std::string& U, GLTexture& T)
{
    using namespace std;
    typedef map<unsigned int,PictureData> Map;
    typedef map<string,unsigned int> Define;

    bool Ready=false;
    GLTexture Tem(0,0,0);
    Define::iterator De=TextureDefine.find(U);
    if(De==TextureDefine.end())
    {
        SDL_Surface* Picture = IMG_Load(U.c_str());
        if(Picture)
        {
            if(GLTexture::CreatTexture(Picture,Tem))
            {
                TextureDefine[U]=Tem.GetPoint();
                PictureData PPDT(Tem.GetPoint(),Tem.GetTextureW(),Tem.GetTextureH(),U);
                PPDT.Use();
                TextureMap.insert(make_pair(Tem.GetPoint(),PPDT));
                Ready=true;
            }
            SDL_FreeSurface(Picture);
        }
    }else{
        Map::iterator Ma=TextureMap.find(De->second);
        if(Ma!=TextureMap.end())
        {
            Tem.Set(Ma->second.GetPoint(),Ma->second.GetTextureW(),Ma->second.GetTextureH());
            Ma->second.Use();
            Ready=true;
        }else
            TextureDefine.erase(De);
    }
    FreeTexture(T);
    if(Ready)
        T.Set(Tem.GetPoint(),Tem.GetTextureW(),Tem.GetTextureH());
    return Ready;
}

void PictureManager::TextureMangerExit()
{
    using namespace std;
    for(map<unsigned int , PictureData>::iterator Po=TextureMap.begin();Po!=TextureMap.end();++Po)
        Po->second.FreeTexture();
    TextureMap.clear();
    TextureDefine.clear();
}
#endif // TEXTUREMANAGER_H

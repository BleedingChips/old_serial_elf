#ifndef DIY_LANGUAGE_H
#define DIY_LANGUAGE_H
#include <string>
#ifdef _WIN32
#include <windows.h>
void UnicodeToUTF_8(char* pOut,wchar_t* pText)
{
    // 注意 WCHAR高低字的顺序,低字节在前，高字节在后
    char* pchar = (char *)pText;
    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));
    return;
}
void Gb2312ToUnicode(wchar_t* pOut,const char *gbBuffer)
{
    MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
    return ;
}
void GB2312ToUTF_8(std::string& pOut,const char *pText, int pLen)
{
    char buf[4];
    int nLength = pLen* 3;
    char* rst = new char[nLength];
    memset(buf,0,4);
    memset(rst,0,nLength);
    int i = 0;
    int j = 0;
    while(i < pLen)
    {
        //如果是英文直接复制就可以
        if( *(pText + i) >= 0)
        {
            rst[j++] = pText[i++];
        }
        else
        {
            wchar_t pbuffer;
            Gb2312ToUnicode(&pbuffer,pText+i);
            UnicodeToUTF_8(buf,&pbuffer);
            unsigned short int tmp = 0;
            tmp = rst[j] = buf[0];
            tmp = rst[j+1] = buf[1];
            tmp = rst[j+2] = buf[2];
            j += 3;
            i += 2;
        }
    }
    rst[j] = '\0';
    //返回结果
    pOut = rst;
    delete []rst;
    return;
}
std::string Translate(std::string Tem)
{
    std::string Temp;
    GB2312ToUTF_8(Temp,Tem.c_str(),Tem.size()+1);
    return Temp;
}
#else
#ifdef FOUSE_WINDOWS
void UnicodeToUTF_8(char* pOut,wchar_t* pText)
{
    // 注意 WCHAR高低字的顺序,低字节在前，高字节在后
    char* pchar = (char *)pText;
    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));
    return;
}
void Gb2312ToUnicode(wchar_t* pOut,const char *gbBuffer)
{
    MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
    return ;
}
void GB2312ToUTF_8(std::string& pOut,const char *pText, int pLen)
{
    char buf[4];
    int nLength = pLen* 3;
    char* rst = new char[nLength];
    memset(buf,0,4);
    memset(rst,0,nLength);
    int i = 0;
    int j = 0;
    while(i < pLen)
    {
        //如果是英文直接复制就可以
        if( *(pText + i) >= 0)
        {
            rst[j++] = pText[i++];
        }
        else
        {
            wchar_t pbuffer;
            Gb2312ToUnicode(&pbuffer,pText+i);
            UnicodeToUTF_8(buf,&pbuffer);
            unsigned short int tmp = 0;
            tmp = rst[j] = buf[0];
            tmp = rst[j+1] = buf[1];
            tmp = rst[j+2] = buf[2];
            j += 3;
            i += 2;
        }
    }
    rst[j] = '\0';
    //返回结果
    pOut = rst;
    delete []rst;
    return;
}
std::string Translate(std::string Tem)
{
    std::string Temp;
    GB2312ToUTF_8(Temp,Tem.c_str(),Tem.size()+1);
    return Temp;
}
#else
inline std::string Translate(std::string Tem)
{
    return Tem;
}
#endif
#endif
#endif

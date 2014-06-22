#ifndef DIY_BASECLASS_H
#define DIY_BASECLASS_H
#define XYZ XYZ<float>
#include <math.h>
#include <fstream>
class XYF
{
public:
    float X,Y;
    const float Distance(const XYF& Tem) const
    {
        return sqrt((X-Tem.X)*(X-Tem.X)+(Y-Tem.Y)*(Y-Tem.Y));
    }
    const float Distance(const XYF& T1,const XYF& T2) const;
    const float Long(void) const
    {
        return sqrt(X*X+Y*Y);
    }
    const float Long2(void) const
    {
        return X*X+Y*Y;
    }
    void Set(const float a, const float b)
    {
        X=a;
        Y=b;
    }
    const XYF Uint(const float Multiply) const;
    XYF(float x=0.0 , float y=0.0 ):X(x),Y(y) {}
    XYF(const XYF& T):X(T.X),Y(T.Y) {}
    const XYF& operator+=(const XYF& B)
    {
        X+=B.X;
        Y+=B.Y;
        return *this;
    }
    const XYF& operator-=(const XYF& B)
    {
        X-=B.X;
        Y-=B.Y;
        return *this;
    }
    const XYF& operator=(const XYF& B)
    {
        X=B.X;
        Y=B.Y;
        return *this;
    }
    void OutFloat(float* C)const {C[0]=X;C[1]=Y;}
};
class XYZF
{
public:
    float X,Y,Z;
    void Turn(float x,float y,float z);//弧度制，以该轴负方向为正方向，逆时针旋转
    void Set(float x,float y,float z)
    {
        X=x;
        Y=y;
        Z=z;
    }
    const float Distance(const XYZF& Tem) const
    {
        return sqrt((X-Tem.X)*(X-Tem.X)+(Y-Tem.Y)*(Y-Tem.Y)+(Z-Tem.Z)*(Z-Tem.Z));
    }
    const float Distance(const XYZF& T1,const XYZF& T2) const;
    const float Long(void) const
    {
        return sqrt(X*X+Y*Y+Z*Z);
    }
    const float Long2(void) const
    {
        return X*X+Y*Y+Z*Z;
    }
    const XYZF Uint(const float Multiple=1.0)  const
    {
        float Line=sqrt(X*X+Y*Y+Z*Z);
        if(Line!=0)
        {
            XYZF Tem;
            Tem.X=X/Line*Multiple;
            Tem.Y=Y/Line*Multiple;
            Tem.Z=Z/Line*Multiple;
            return Tem;
        }
        else
        {
            XYZF Tem(0.0,0.0,0.0);
            return Tem;
        }
    }
    XYZF(const XYZF& T)
    {
        X=T.X;
        Y=T.Y;
        Z=T.Z;
    }
    XYZF(float x=0.0,float y=0.0,float z=0.0):X(x),Y(y),Z(z) {}
    const XYZF ToCoordinate(const XYZF& x,const XYZF& y,const XYZF& z)const
    {
        XYZF Tem2((*this)*x,(*this)*y,(*this)*z);    //将原始坐标系中的点或向量转换成目标坐标系中的坐标
        return Tem2;
    }
    const XYZF FromCoordinate(const XYZF& x,const XYZF& y,const XYZF& z)const
    {
        XYZF Tem(x*X+y*Y+z*Z);    //将目标坐标系中的点或向量转换成原始坐标系中的坐标
        return Tem;
    }
    const float operator* (const XYZF& Temp)const
    {
        return X*Temp.X+Y*Temp.Y+Z*Temp.Z;
    }
    const XYZF operator* (const float t)const
    {
        XYZF tem(X*t,Y*t,Z*t);
        return tem;
    }
    const XYZF operator+ (const XYZF& b)const
    {
        XYZF tem(X+b.X , Y+b.Y , Z+b.Z);
        return tem;
    }
    const XYZF operator- (const XYZF& b)const
    {
        XYZF tem(X-b.X , Y-b.Y , Z-b.Z);
        return tem;
    }
    const XYZF operator& (const XYZF& b)const
    {
        XYZF tem(Y*b.Z-Z*b.Y,Z*b.X-X*b.Z,X*b.Y-Y*b.X);
        return tem;
    }
    const XYZF& operator= (const XYZF& B)
    {
        Set(B.X,B.Y,B.Z);
        return *this;
    }
    const XYZF& operator+=(const XYZF& B)
    {
        X+=B.X;
        Y+=B.Y;
        Z+=B.Z;
        return *this;
    }
    const XYZF& operator-=(const XYZF& B)
    {
        X-=B.X;
        Y-=B.Y;
        Z-=B.Z;
        return *this;
    }
    void OutFloat(float* C)const {C[0]=X;C[1]=Y;C[2]=Z;}
};
class Coordinate
{
public:
    XYZF Left,Location,Up,Front;
    const XYZF ToCoordinate(const XYZF&) const ;
    const XYZF VectorToCoordinate(const XYZF&) const ;
    const Coordinate ToCoordinate(const Coordinate*) const;
    const XYZF FromCoordinate(const XYZF&) const ;
    const XYZF VectorFromCoordinate(const XYZF&) const ;
    const Coordinate FromCoordinate(const Coordinate*) const;
    inline void Reflash(void);
    void Set(Coordinate&);
    void Set(Coordinate*);
    void SetFront(const XYZF& T)
    {
        Front=T;
    }
    void SetLeft(const XYZF& T)
    {
        Left=T;
    }
    void SetUp(const XYZF& T)
    {
        Up=T;
    }
    void SetLocation(const XYZF& T)
    {
        Location=T;
    }
    const Coordinate& operator=(const Coordinate& Tem)
    {
        Left=Tem.Left;
        Location=Tem.Location;
        Up=Tem.Up;
        Front=Tem.Front;
        return *this;
    }
    Coordinate():Left(1.0,0.0,0.0),Up(0.0,1.0,0.0),Front(0.0,0.0,1.0),Location(0.0,0.0,0.0) {}
    Coordinate(const XYZF& Loc,const XYZF& Fro,const XYZF& Lef,const XYZF& U):Left(Lef),Up(U),Front(Fro),Location(Loc) {}
    Coordinate(const XYZF& Loc):Left(1.0,0.0,0.0),Up(0.0,1.0,0.0),Front(0.0,0.0,1.0),Location(Loc) {}
    Coordinate(const Coordinate& C):Left(C.Left),Up(C.Up),Front(C.Front),Location(C.Location) {}
    //operator Coordinate(){return *this;}
};
const float XYF::Distance(const XYF& T1,const XYF& T2) const
{
    XYF P(T2.X-T1.X,T2.Y-T1.Y);
    float A;
    float B;
    char S=0;
    if(P.X==0.0)
    {
        S+=1;
    }
    if(P.Y==0.0)
    {
        S+=2;
    }
    switch(S)
    {
    case 0:
        B=1.0/(T1.Y-P.Y/P.X*T1.X);
        A=-B*P.Y/P.X;
        break;
    case 1:
        B=0.0;
        A=-1.0/T1.X;
        break;
    case 2:
        A=0.0;
        B=-1.0/T1.Y;
        break;
    case 3:
        return Distance(T1);
    }
    return fabs(A*X+B*Y+1.0)/sqrt(pow(A,2.0)+pow(B,2.0));
}

const XYF XYF::Uint(const float Multiply=1.0) const
{
    float Line=Long();
    XYF Tem;
    if(Line!=0)
    {
        Tem.X=X/Line*Multiply;
        Tem.Y=Y/Line*Multiply;
        //return Tem;
    }
    return Tem;
}
void XYZF::Turn(float x,float y,float z)//弧度制，以该轴负方向为正方向，逆时针旋转
{
    XYZF Tem(X,Y,Z);
    XYZF RT=Tem;
    Tem.Set(RT.X,RT.Y*cos(x)-RT.Z*sin(x),RT.Z*cos(x)+RT.Y*sin(x));
    RT=Tem;
    Tem.Set(RT.X*cos(y)+RT.Z*sin(y),RT.Y,RT.Z*cos(y)-RT.X*sin(y));
    RT=Tem;
    Tem.Set(RT.X*cos(z)-RT.Y*sin(z),RT.Y*cos(z)+RT.X*sin(z),RT.Z);
    *this=Tem;
}
std::ostream& operator <<(std::ostream& C,XYZF& T)
{
    C<<"<"<<T.X<<","<<T.Y<<","<<T.Z<<">";
    return C;
}
std::ostream& operator<<(std::ostream& C,Coordinate& P)
{
    C<<P.Location<<" , "<<P.Up<<" , "<< P.Left <<" , "<<P.Front;
    return C;
}
const float XYZF::Distance(const XYZF& T1,const XYZF& T2) const
{
    XYZF L1=T2-T1;
    XYZF L2=*this-T1;
    float P=L2*L1.Uint();
    return (L1*P-L2).Long();
}

inline void Coordinate::Reflash(void)//the Up is the standard;
{
    Left=(Up&Front).Uint();
    Front=(Left&Up).Uint();
}
const XYZF Coordinate::ToCoordinate(const XYZF& Tem) const//point to Coordinate
{
    XYZF Ter=Tem-Location;
    Ter=Ter.ToCoordinate(Left,Up,Front);
    return Ter;
}
const XYZF Coordinate::VectorToCoordinate(const XYZF& Tem) const//vector to Coordinate
{
    XYZF Ter;
    Ter=Ter.ToCoordinate(Left,Up,Front);
    return Ter;
}
const XYZF operator* (const float t,const XYZF& a)
{
    XYZF tem(a.X*t,a.Y*t,a.Z*t);
    return tem;
}
const float operator* (const XYF& Tem,const XYF& Temp)
{
    return Tem.X*Temp.X+Tem.Y*Temp.Y;
}
const XYF operator* (const XYF& a,const float t)
{
    XYF tem(a.X*t,a.Y*t);
    return tem;
}
const XYF operator* (const float t,const XYF& a)
{
    XYF tem(a.X*t,a.Y*t);
    return tem;
}
const XYF operator+ (const XYF& a,const XYF& b)
{
    XYF tem(a.X+b.X , a.Y+b.Y);
    return tem;
}
const XYF operator- (const XYF& a,const XYF& b)
{
    XYF tem(a.X-b.X , a.Y-b.Y);
    return tem;
}
/*Coordinate Coordinate::ToCoordinate(const Coordinate* Temc) const
{
    Coordinate Tem;
    Tem.Location=Location-Temc->Location;
    Tem.Up=Temc->Up.ToCoordinate(Left,Up,Front);
    Tem.Front=Temc->Front.ToCoordinate(Left,Up,Front);
    Tem.Reflash();
    return Tem;
}*/
void Coordinate::Set(Coordinate* C)
{
    Up=C->Up;
    Front=C->Front;
    Left=C->Left;
    Location=C->Location;
}
void Coordinate::Set(Coordinate& C)
{
    Up=C.Up;
    Front=C.Front;
    Left=C.Left;
    Location=C.Location;
}
const XYZF Coordinate::FromCoordinate(const XYZF& Tem) const
{
    XYZF Ter=Tem;
    Ter=Ter.FromCoordinate(Left,Up,Front)+Location;
    return Ter;
}
const XYZF Coordinate::VectorFromCoordinate(const XYZF& Tem) const
{
    XYZF Ter=Tem;
    Ter=Ter.FromCoordinate(Left,Up,Front);
    return Ter;
}
const Coordinate Coordinate::FromCoordinate(const Coordinate* Temc) const
{
    Coordinate Tem;
    Tem.Location=Temc->Location-Location;
    Tem.Up=Temc->Up.FromCoordinate(Left,Up,Front);
    Tem.Front=Temc->Front.FromCoordinate(Left,Up,Front);
    Tem.Reflash();
    return Tem;
}
#endif

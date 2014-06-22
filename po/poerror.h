#ifndef POERROR_H
#define POERROR_H
class POError
{
public:
    static void GetError(std::string,int) {}
    static bool NeedToStop()
    {
        return true;
    }
};

#endif // POERROR_H

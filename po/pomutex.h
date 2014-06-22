#ifndef POMUTEX_H
#define POMUTEX_H
#include <SDL2/SDL_mutex.h>
class POMutex
{
public:
    SDL_semaphore *LockSem;
    void Lock()
    {
        SDL_SemWait(LockSem);
    }
    void UnLock()
    {
        SDL_SemPost(LockSem);
    }
    ~POMutex()
    {
        SDL_DestroySemaphore(LockSem);
    }
    POMutex()
    {
        LockSem=SDL_CreateSemaphore(1);
    }
    POMutex(const POMutex&)
    {
        LockSem=SDL_CreateSemaphore(1);
    }
    void operator=(POMutex& K) {}
};
#endif // PUBLICVALUE_H

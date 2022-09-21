#pragma once
#include "Common.h"

class Timer
{
public:
    void StartTimer(float Time, bool Loop, function<void()> Function, float Delay=0.f);
    void Update(float DeltaTime);
    bool HasFinished()const;

private:
    float            m_Time = 0.f;
    float            m_StartingTimer = 0.f;
    function<void()> m_Function;
    bool             m_Finished = false;
    bool             m_Loop = false;

};

class TimerManager
{
public:
    void StartTimer(float Time, bool Loop, function<void()> Function, float Delay=0.f);
    void Update(float DeltaTime);

private:
    vector<shared_ptr<Timer>> m_Timers;
};
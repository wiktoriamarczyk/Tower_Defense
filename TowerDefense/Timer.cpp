#include "Timer.h"

void Timer::StartTimer(float Time, bool Loop, function<void()> Function, float Delay)
{
    m_Time = Time + Delay;
    m_StartingTimer = Time;
    m_Loop = Loop;
    m_Function = Function;
}

void Timer::Update(float DeltaTime)
{
    m_Time -= DeltaTime;

    if (m_Time <= 0)
    {
        m_Function();

        if (!m_Loop)
            m_Finished = true;
        else 
            m_Time += m_StartingTimer;
    }
}

bool Timer::HasFinished()const
{
    return m_Finished;
}

void TimerManager::StartTimer(float Time, bool Loop, function<void()> Function, float Delay)
{
    auto pTimer = make_shared<Timer>();
    pTimer->StartTimer(Time, Loop, Function, Delay);
    m_Timers.push_back(pTimer);
}

void TimerManager::Update(float DeltaTime)
{
   for (size_t i = 0; i < m_Timers.size();)
    {
        m_Timers[i]->Update(DeltaTime);

        if (m_Timers[i]->HasFinished())
            m_Timers.erase(m_Timers.begin() + i);

        else
            ++i;
    }
}

void TimerManager::StopAllTimers()
{
    m_Timers.clear();
}
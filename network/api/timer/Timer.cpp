//
// Created by fufeng on 2024/1/20.
//

#include "Timer.h"
#include <time.h>

int64_t Timer::generateId()
{
    int64_t tmpId;
    s_mutex.lock();
    ++s_initialId;
    tmpId = s_initialId;
    s_mutex.unlock();

    return tmpId;
}

Timer::Timer(int32_t repeatedTimes, int64_t interval, const TimerCallback& timerCallback)
{
    m_repeatedTimes = repeatedTimes;
    m_interval = interval;

    //当前时间加上触发间隔得到下一次的过期时间
    m_expiredTime = (int64_t)time(nullptr) + interval;

    m_callback = timerCallback;

    //生成一个唯一的id
    m_id = Timer::generateId();
}

bool Timer::isExpired() const
{
    int64_t now = time(nullptr);
    return now >= m_expiredTime;
}

void Timer::run()
{
    m_callback();

    if (m_repeatedTimes >= 1)
    {
        --m_repeatedTimes;
    }

    m_expiredTime += m_interval;
}

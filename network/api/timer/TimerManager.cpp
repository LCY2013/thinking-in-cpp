//
// Created by fufeng on 2024/1/20.
//

#include "TimerManager.h"

int64_t TimerManager::addTimer(int32_t repeatedCount, int64_t interval, const TimerCallback& timerCallback)
{
    Timer* pTimer = new Timer(repeatedCount, interval, timerCallback);

    m_listTimers.push_back(pTimer);

    return pTimer->getId();
}

bool TimerManager::removeTimer(int64_t timerId)
{
    for (auto iter = m_listTimers.begin(); iter != m_listTimers.end(); ++iter)
    {
        if ((*iter)->getId() == timerId)
        {
            m_listTimers.erase(iter);
            return true;
        }
    }

    return false;
}

void TimerManager::checkAndHandleTimers()
{
    Timer* deletedTimer;
    for (auto iter = m_listTimers.begin(); iter != m_listTimers.end(); )
    {
        if ((*iter)->isExpired())
        {
            (*iter)->run();

            if ((*iter)->getRepeatedTimes() == 0)
            {
                //定时器不需要触发从集合中移除该对象
                deletedTimer = *iter;
                iter = m_listTimers.erase(iter);
                delete deletedTimer;
                continue;
            }
            else
            {
                ++iter;
            }
        }
    }
}

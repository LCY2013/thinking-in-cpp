//
// Created by mac257 on 2024/1/20.
//

#ifndef THINKING_IN_CPP_TIMERMANAGER_H
#define THINKING_IN_CPP_TIMERMANAGER_H


#include <stdint.h>
#include <list>

#include "Timer.h"

void defaultTimerCallback()
{

}

class TimerManager
{
public:
    TimerManager() = default;
    ~TimerManager() = default;

    /** 添加定时器
     * @param repeatedCount 重复次数
     * @param interval      触发间隔
     * @param timerCallback 定时器回调函数
     * @return              返回创建成功的定时器id
     */
    int64_t addTimer(int32_t repeatedCount, int64_t interval, const TimerCallback& timerCallback);

    /** 移除指定id的定时器
     * @param timerId 待移除的定时器id
     * @return 成功移除定时器返回true，反之返回false
     */
    bool removeTimer(int64_t timerId);

    /** 检测定时器是否到期，如果到期则触发定时器函数
     */
    void checkAndHandleTimers();


private:
    std::list<Timer*> m_listTimers;
};


#endif //THINKING_IN_CPP_TIMERMANAGER_H

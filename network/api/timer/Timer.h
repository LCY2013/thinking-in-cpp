//
// Created by mac257 on 2024/1/20.
//

#ifndef THINKING_IN_CPP_TIMER_H
#define THINKING_IN_CPP_TIMER_H


#include <functional>
#include <mutex>

using namespace std;

typedef std::function<void()> TimerCallback;

class Timer
{
public:
    /**
     * @param repeatedTimes 定时器重复次数，设置为-1表示一直重复下去
     * @param interval      下一次触发的时间间隔
     * @param timerCallback 定时器触发后的回调函数
     */
    Timer(int32_t repeatedTimes, int64_t interval, const TimerCallback& timerCallback);
    ~Timer();

    int64_t getId()
    {
        return m_id;
    }

    bool isExpired() const;

    int32_t getRepeatedTimes()
    {
        return m_repeatedTimes;
    }

    void run();

    //其他实现暂且省略

public:
    //生成一个唯一的id
    static int64_t generateId();

private:
    //定时器的id，唯一标识一个定时器
    int64_t                     m_id;
    //定时器的到期时间
    time_t                      m_expiredTime;
    //定时器重复触发的次数
    int32_t                     m_repeatedTimes;
    //定时器触发后的回调函数
    TimerCallback               m_callback;
    //触发时间间隔
    int64_t                     m_interval;

    //定时器id基准值，初始值为 0
    static int64_t     			s_initialId{0};
    //保护s_initialId的互斥体对象
    static std::mutex  			s_mutex{};
};


#endif //THINKING_IN_CPP_TIMER_H

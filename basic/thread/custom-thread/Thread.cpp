//
// Created by fufeng on 2023/12/21.
//
#include "Thread.h"

#ifdef WIN32
DWORD WINAPI CThread::_ThreadEntry(LPVOID pParam)
#else
void* CThread::_ThreadEntry(void* pParam)
#endif
{
    CThread *pThread = (CThread *)pParam;
    if(pThread->InitInstance())
    {
        pThread->Run();
    }

    pThread->ExitInstance();

    return NULL;
}

CThread::CThread()
{
    m_hThread = (THREAD_HANDLE)0;
    m_IDThread = 0;
}

CThread::~CThread()
{
}

bool CThread::Create()
{
    if (m_hThread != (THREAD_HANDLE)0)
    {
        return true;
    }
    bool ret = true;
#ifdef WIN32
    m_hThread = ::CreateThread(NULL,0,_ThreadEntry,this,0,&m_IDThread);
	if(m_hThread==NULL)
	{
		ret = false;
	}
#else
    ret = (::pthread_create(&m_hThread,NULL,&_ThreadEntry , this) == 0);
#endif
    return ret;
}

bool CThread::InitInstance()
{
    return true;
}

void CThread::ExitInstance()
{
}

void CThread::OSSleep(int seconds)
{
#ifdef WIN32
    ::Sleep(seconds*1000);
#else
    ::sleep(seconds);
#endif
}

void CThread::SleepMs(int nMilliseconds)
{
#ifdef WIN32
    ::Sleep(nMilliseconds);
#else
    ::usleep(nMilliseconds);
#endif
}

bool CThread::IsCurrentThread()
{
#ifdef WIN32
    return ::GetCurrentThreadId() == m_IDThread;
#else
    return ::pthread_self() == m_hThread;
#endif
}

bool CThread::Join()
{
    THREAD_HANDLE hThread = GetHandle();
    if(hThread == (THREAD_HANDLE)0)
    {
        return true;
    }
#ifdef WIN32
    return (WaitForSingleObject(hThread,INFINITE) != 0);
#else
    return (pthread_join(hThread, NULL) == 0);
#endif
}

void CThread::ExitThread()
{
#ifdef WIN32
    ::ExitThread(0);
#else
#endif
}


#ifdef WIN32
typedef HANDLE THREAD_HANDLE ;
#else
typedef pthread_t THREAD_HANDLE ;
#endif

/**定义了一个线程对象
 */
class  CThread
{
public:
    /**构造函数
     */
    CThread();

    /**析构函数
     */
    virtual ~CThread();

    /**创建一个线程
     * @return true:创建成功 false:创建失败
     */
    virtual bool Create();

    /**获得本线程对象存储的线程句柄
     * @return 本线程对象存储的线程句柄线程句柄
     */
    THREAD_HANDLE GetHandle();

    /**线程睡眠seconds秒
     * @param seconds 睡眠秒数
     */
    void OSSleep(int nSeconds);

    void SleepMs(int nMilliseconds);

    bool Join();

    bool IsCurrentThread();

    void ExitThread();

private:
#ifdef WIN32
    static DWORD WINAPI _ThreadEntry(LPVOID pParam);
#else
    static void* _ThreadEntry(void* pParam);
#endif

    /**虚函数，子类可做一些实例化工作
     * @return true:创建成功 false:创建失败
     */
    virtual bool InitInstance();

    /**虚函数，子类清楚实例
     */
    virtual void ExitInstance();

    /**线程开始运行，纯虚函数，子类必须继承实现
     */
    virtual void Run() = 0;

private:
#ifdef WIN32
    DWORD          m_IDThread;
#else
    //线程句柄
    THREAD_HANDLE  m_hThread;
#endif
};
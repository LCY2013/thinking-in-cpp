//
// Created by fufeng on 2023/12/6.
//

class CSocketClient {
public:
    CSocketClient() {
        m_pImpl = new Impl();
    };

    ~CSocketClient() {
        delete m_pImpl;
    };

public:
    void SetProxyWnd(HWND hProxyWnd);

    bool Init(CNetProxy *pNetProxy);

    bool Uninit();

    int Register(const char *pszUser, const char *pszPassword);

    void GuestLogin();

    BOOL IsClosed();

    BOOL Connect(int timeout = 3);

    void AddData(int cmd, const std::string &strBuffer);

    void AddData(int cmd, const char *pszBuff, int nBuffLen);

    void Close();

    BOOL ConnectServer(int timeout = 3);

    BOOL SendLoginMsg();

    BOOL RecvLoginMsg(int &nRet);

    BOOL Login(int &nRet);

private:
    class Impl;
    Impl *m_pImpl;
};

class CSocketClient::Impl {
public:
    Impl() {
        //TODO: 你可以在这里对成员变量做一些初始化工作
    }

    ~Impl() {
        //TODO: 你可以在这里做一些清理工作
    }

public:
    SOCKET m_hSocket;
    short m_nPort;
    char m_szServer[64];
    long m_nLastDataTime;        //最近一次收发数据的时间
    long m_nHeartbeatInterval;   //心跳包时间间隔，单位秒
    CRITICAL_SECTION m_csLastDataTime;       //保护m_nLastDataTime的互斥体
    HANDLE m_hSendDataThread;      //发送数据线程
    HANDLE m_hRecvDataThread;      //接收数据线程
    std::string m_strSendBuf;
    std::string m_strRecvBuf;
    HANDLE m_hExitEvent;
    bool m_bConnected;
    CRITICAL_SECTION m_csSendBuf;
    HANDLE m_hSemaphoreSendBuf;
    HWND m_hProxyWnd;
    CNetProxy *m_pNetProxy;
    int m_nReconnectTimeInterval;    //重连时间间隔
    time_t m_nLastReconnectTime;        //上次重连时刻
    CFlowStatistics *m_pFlowStatistics;
};


//
// Created by fufeng on 2023/12/6.
//
// C++中对类的这种封装方式，我们称之为pimpl惯用法，即Pointer to Implementation（也有人认为是Private Implementation）
// 在实际的开发中，Impl类的声明和定义既可以使用class关键字也可以使用struct关键字。
// 在C++语言中，struct类型可以定义成员方法，但struct所有成员变量和方法默认都是public的。
//

class CSocketClient {
public:
    CSocketClient();

    ~CSocketClient();

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
    void LoadConfig();

    static UINT CALLBACK
    SendDataThreadProc(LPVOID
    lpParam);
    static UINT CALLBACK
    RecvDataThreadProc(LPVOID
    lpParam);

    bool Send();

    bool Recv();

    bool CheckReceivedData();

    void SendHeartbeatPackage();

private:
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
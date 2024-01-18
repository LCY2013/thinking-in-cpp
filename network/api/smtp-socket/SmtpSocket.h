//
// Created by fufeng on 2024/1/18.
//

/**
 * 发送邮件类，SmtpSocket.h
 */

#ifndef THINKING_IN_CPP_SMTPSOCKET_H
#define THINKING_IN_CPP_SMTPSOCKET_H


#include <string>
#include <vector>
#include "Platform.h"

class SmtpSocket final
{
public:
    static bool sendMail(const std::string& server, short port, const std::string& from, const std::string& fromPassword,
                         const std::vector<std::string>& to, const std::string& subject, const std::string& mailData);

public:
    SmtpSocket(void);
    ~SmtpSocket(void);

    bool isConnected() const { return m_hSocket; }
    bool connect(const char* pszUrl, short port = 25);
    bool logon(const char* pszUser, const char* pszPassword);
    bool setMailFrom(const char* pszFrom);
    bool setMailTo(const std::vector<std::string>& sendTo);
    bool send(const std::string& subject, const std::string& mailData);

    void closeConnection();
    void quit();	//退出

private:
    /**
     *	验证从服务器返回的前三位代码和传递进来的参数是否一样
     */
    bool checkResponse(const char* recvCode);

private:
    bool		                m_bConnected;
    SOCKET		                m_hSocket;
    std::string                 m_strUser;
    std::string                 m_strPassword;
    std::string                 m_strFrom;
    std::vector<std::string>    m_strTo;
};
#endif //THINKING_IN_CPP_SMTPSOCKET_H

//
// Created by fufeng on 2024/1/18.
//
/**
 * 封装curl的http库, CurlClient.h
 */

#ifndef THINKING_IN_CPP_CURLCLIENT_H
#define THINKING_IN_CPP_CURLCLIENT_H


#include <string>

#include "curl.h"

class CCurlClient final
{
public:
    CCurlClient();
    ~CCurlClient();

    CCurlClient(const CCurlClient& rhs) = delete;
    CCurlClient& operator=(const CCurlClient& rhs) = delete;

    /**
     * 初始化libcurl
     * 非线程安全函数，建议在程序初始化时调用一次，以免出现多线程调用curl_easy_init出现崩溃问题
     */
    static void init();
    /**
     * 反初始化libcurl
     * 非线程安全函数，建议在程序退出时调用一次
     */
    static void uninit();

    /** 发送http get请求
     * @param url 请求的网址
     * @param headers 随请求发送的自定义http头信息，多个自定义头之间使用\r\n分割，最后一个以\r\n结束，无自定义http头信息则设置为nullptr
     * @param response 如果请求成功则存储http请求结果(注意：response在函数调用中是追加模式，也就是说如果上一次response的值不清空，调用这个函数时会追加，而不是覆盖)
     * @param autoRedirect 请求得到http 3xx的状态码是否自动重定向至新的url
     * @param bReserveHeaders 请求的结果中（存储于response），是否保留http头部信息
     * @param connTimeout 连接超时时间，单位为秒（对于某些http URI资源不好的，总是返回超时可以将该参数设置大一点）
     * @param readTimeout 读取数据超时时间，单位为秒（对于某些http URI资源不好的，总是返回超时可以将该参数设置大一点）
     */
    bool get(const char* url, const char* headers, std::string& response, bool autoRedirect = false, bool bReserveHeaders = false, int64_t connTimeout = 1L, int64_t readTimeout = 5L);

    /** 发送http post请求
     * @param url 请求的网址
     * @param headers 随请求发送的自定义http头信息，多个自定义头之间使用\r\n分割，最后一个以\r\n结束，无自定义http头信息则设置为nullptr
     * @param postParams post参数内容
     * @param response 如果请求成功则存储http请求结果(注意：response在函数调用中是追加模式，也就是说如果上一次response的值不清空，调用这个函数时会追加，而不是覆盖)
     * @param autoRedirect 请求得到http 3xx的状态码是否自动重定向至新的url
     * @param bReserveHeaders 请求的结果中（存储于response），是否保留http头部信息
     * @param connTimeout 连接超时时间，单位为秒（对于某些http URI资源不好的，总是返回超时可以将该参数设置大一点）
     * @param readTimeout 读取数据超时时间，单位为秒（对于某些http URI资源不好的，总是返回超时可以将该参数设置大一点）
     */
    bool post(const char* url, const char* headers, const char* postParams, std::string& response, bool autoRedirect = false, bool bReserveHeaders = false, int64_t connTimeout = 1L, int64_t readTimeout = 5L);


private:
    static bool  m_bGlobalInit;
};


#endif //THINKING_IN_CPP_CURLCLIENT_H

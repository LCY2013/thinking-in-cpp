//
// Created by fufeng on 2024/1/18.
//

/**
 * 封装curl的http库, CurlClient.cpp
 */

#include "CurlClient.h"
#include <iostream>

void* returnback = NULL;
// reply of the requery

size_t req_reply(void* ptr, size_t size, size_t nmemb, void* stream)
{
    std::string* str = (std::string*)stream;
    (*str).append((char*)ptr, size * nmemb);
    return size * nmemb;
}

bool CCurlClient::m_bGlobalInit = false;

CCurlClient::CCurlClient()
{
}

CCurlClient::~CCurlClient()
{
}

void CCurlClient::init()
{
    if (!m_bGlobalInit)
    {
        curl_global_init(CURL_GLOBAL_ALL);
        m_bGlobalInit = true;
    }
}

void CCurlClient::uninit()
{
    if (m_bGlobalInit)
    {
        curl_global_cleanup();
        m_bGlobalInit = false;
    }
}

// http GET
bool CCurlClient::get(const char* url, const char* headers, std::string& response,
                      bool autoRedirect/* = false*/, bool bReserveHeaders/* = false*/, int64_t connTimeout/* = 1L*/, int64_t readTimeout/* = 5L*/)
{
    // init curl
    CURL* curl = curl_easy_init();
    if (curl == nullptr)
        return false;

    // set params
    curl_easy_setopt(curl, CURLOPT_URL, url); // url
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);
    //禁用SIGALRM+sigsetjmp/siglongjmp的超时机制，
    //采用其他超时机制，因为该机制修改了一个全局变量，在多线程下可能会出现问题
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connTimeout); // set transport and time out time
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, readTimeout);

    //遇到 http 3xx 状态码是否自定重定位
    if (autoRedirect)
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    else
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);

    if (bReserveHeaders)
        curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
    else
        curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

    //添加自定义头信息
    if (headers != nullptr)
    {
        struct curl_slist* chunk = NULL;
        chunk = curl_slist_append(chunk, headers);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    }

    // start req
    CURLcode res = curl_easy_perform(curl);

    // release curl
    curl_easy_cleanup(curl);

    return res == CURLcode::CURLE_OK;
}

// http POST
bool CCurlClient::post(const char* url, const char* headers, const char* postParams, std::string& response,
                       bool autoRedirect/* = false*/, bool bReserveHeaders/* = false*/, int64_t connTimeout/* = 1L*/, int64_t readTimeout/* = 5L*/)
{
    // init curl
    CURL* curl = curl_easy_init();
    if (curl == nullptr)
        return false;

    // set params
    curl_easy_setopt(curl, CURLOPT_POST, 1L); // post req
    curl_easy_setopt(curl, CURLOPT_URL, url); // url
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postParams); // params
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false); // if want to use https
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false); // set peer and host verify false
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, req_reply);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)& response);
    //禁用SIGALRM+sigsetjmp/siglongjmp的超时机制，
    //采用其他超时机制，因为该机制修改了一个全局变量，在多线程下可能会出现问题
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connTimeout);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, readTimeout);

    //遇到 http 3xx 状态码是否自定重定位
    if (autoRedirect)
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    else
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);

    if (bReserveHeaders)
        curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
    else
        curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

    //添加自定义头信息
    if (headers != nullptr)
    {
        struct curl_slist* chunk = NULL;
        chunk = curl_slist_append(chunk, headers);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
    }

    // start req
    CURLcode res = curl_easy_perform(curl);

    // release curl
    curl_easy_cleanup(curl);

    return res == CURLcode::CURLE_OK;
}

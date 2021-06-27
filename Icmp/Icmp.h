#pragma once

#include <string>
#include <netinet/in.h>

class noncopyable
{
public:
    noncopyable(const noncopyable &) = delete;
    void operator=(const noncopyable &) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

struct ResultPacket
{
    int bytes = 0;
    std::string ip;
    uint16_t seq;
    int ttl = 0;
    double time = 0; //ms
};

class Icmp : noncopyable
{
public:
    Icmp(const std::string &destAddr);

    virtual ~Icmp();

    size_t sendIcmpPacket();

    bool receiveIcmpPacket(struct ResultPacket &resultPacket) const;

    std::string errorString() const;

    int sockfd() const { return m_sockfd; }

private:
    int m_sockfd = -1;
    bool m_error = false;
    int m_pid = 0;
    int m_nsend = 0;
    struct sockaddr_in m_destAddr;
};

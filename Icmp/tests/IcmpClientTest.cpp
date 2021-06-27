#include <Icmp/Icmp.h>

#include <unistd.h>
#include <signal.h>
#include <string>
#include <vector>
#include <algorithm>
#include <netdb.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

std::string hostname;
std::vector<double> timerVec;
int nsend = 0;
int nreceived = 0;
struct timeval tvStart;

void quit(int signo)
{
    struct timeval tvEnd;
    gettimeofday(&tvEnd, NULL);
    long int t = 1000 * 1000 * (tvEnd.tv_sec - tvStart.tv_sec) +
                 (tvEnd.tv_usec - tvStart.tv_usec);

    std::sort(timerVec.begin(), timerVec.end());
    double min = timerVec[0];
    double max = timerVec[timerVec.size() - 1];
    double avg = 0;
    for (double ms : timerVec)
    {
        avg += ms;
    }
    avg = avg / timerVec.size();

    printf("\n--------------------%s ping statistics-------------------\n",
           hostname.c_str());
    printf("%d packets transmitted, %d received , %%%d lost time %.3f ms\n"
           "rtt min/avg/max = %.3f/%.3f/%.3f ms\n",
           nsend, nreceived, (nsend - nreceived) / nsend * 100, double(t) / 1000.0,
           min, avg, max);
    exit(1);
}

std::string getIp(char *name)
{
    std::string ip;
    /*判断是主机名还是ip地址*/
    if (inet_addr(name) == INADDR_NONE)
    {
        struct hostent *host = gethostbyname(name);
        if (NULL == host) /*是主机名*/
        {
            perror("gethostbyname error");
            return ip;
        }
        for (int i = 0; host->h_addr_list[i]; i++)
        {
            ip = inet_ntoa(*(struct in_addr *)host->h_addr_list[i]);
            printf("IP addr %d: %s\n", i + 1, ip.c_str());
            break;
        }
    }
    else
    {
        ip = name;
    }
    return ip;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("usage:%s hostname/IP address\n", argv[0]);
        return -1;
    }
    hostname = argv[1];

    signal(SIGINT, quit);

    std::string ip(getIp(argv[1]));
    /*判断是主机名还是ip地址*/
    printf("PING %s(%s): %d bytes data in ICMP packets.\n",
           argv[1], ip.c_str(), 56);

    gettimeofday(&tvStart, NULL);

    Icmp icmp(ip);
    fd_set fds;
    struct timeval tv;
    tv.tv_sec = 10; //10s超时设置
    tv.tv_usec = 0;
    ResultPacket resultPacket;
    while (1)
    {
        nsend++;
        icmp.sendIcmpPacket();
        FD_ZERO(&fds);               //每次循环都要清空集合，否则不能检测描述符变化
        FD_SET(icmp.sockfd(), &fds); //添加描述符
        ssize_t ret = select(icmp.sockfd() + 1, &fds, nullptr, nullptr, &tv);
        if (ret == -1)
        {
            perror("select error: ");
            break;
        }
        else if (FD_ISSET(icmp.sockfd(), &fds))
        {
            if (!icmp.receiveIcmpPacket(resultPacket))
            {
                break;
            }
            nreceived++;
            printf("%d byte from %s: icmp_seq=%u ttl=%d rtt=%.3f ms\n",
                   resultPacket.bytes,
                   resultPacket.ip.c_str(),
                   resultPacket.seq,
                   resultPacket.ttl,
                   resultPacket.time);
            timerVec.push_back(resultPacket.time);
        }
        sleep(1);
    }
}

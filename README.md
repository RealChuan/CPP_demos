# CPP_demos  

C/C++写的一些基础demos。   

**部分写法比较脑残：比如多线程读写同一个IO，而且语法上用的也是过时的标准，在有IO复用的模式下，文件描述符也没有设置为非阻塞，看个大概意思明白就可以了。**   

------



1. Algorithms-------------算法；  

      1. BracketMatch-------------括号匹配，字符串处理（删除多余空格只保留一个和清除前后空格）；  

2. Client-------------select客户端；  

3. CrashHandler-------------崩溃、异常捕捉；  

4. Crc-------------各种CRC计算方法;  

5. Icmp------------- Icmp(Ping实现);  

6. List-------------单项链表及反转;  

7. process--------------系统单个进程cpu和memory获取；  

8. Server-------------单线程服务端Tcp；  
   1. server_select-------------select模型；  

   2. server_poll--------------poll模型；  
   3. server_epoll-------------epoll模型；  

9. WatchDirectory-------------目录监控；   
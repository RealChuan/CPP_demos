#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define VMRSS_LINE 17 //ARM 15
//#define VMSIZE_LINE 13
#define PROCESS_ITEM 14
#define BUF_SIZE 1024

using namespace std;

struct config
{
	int pid;
	int model;
	int looptime;
	int systemtime;
};

struct tool
{
	int pid;
	int memory;
	float cpu;
	char name[50];
};

struct Total_Cpu_Occupy_t
{
	unsigned long user;
	unsigned long nice;
	unsigned long system;
	unsigned long idle;
};

struct Proc_Cpu_Occupy_t
{
	unsigned int pid;
	unsigned long utime; //user time
	unsigned long stime; //kernel time
	unsigned long cutime; //all user time
	unsigned long cstime; //all dead time
};

config readCfg() {
	config c;
	memset(&c, 0, sizeof(config));
	FILE *fp = fopen("config.ini", "r");
	if (fp == nullptr)
	{
		perror("配置文件为空:");
		exit(-1);
	}
	char line[1024] = {0};
	while (!feof(fp))
	{
		/*初始化line*/
		memset(line, 0, 1024);
		fgets(line, 1024, fp);
		size_t len = strlen(line);
		/*查找等号的位置*/
		char *pos = strchr(line, '=');
		if (pos == nullptr)
			continue;
		char key[64] = {0};
		char val[64] = {0};
		/*消除不必要的换行*/
		int offset = 1;
		if (line[len - 1] == '\n')
			offset = 2;
		/*截取key,val值*/
		strncpy(key, line, static_cast<size_t>(pos - line));
		strncpy(val, pos + 1, static_cast<size_t>(line + len - offset - pos));
		if (strstr(key, "PID"))
			c.pid = (atoi(val));
		if (strstr(key, "LOOPTIME"))
			c.looptime = static_cast<int>(atoi(val));
		if (strstr(key, "SYSTEMTIME"))
			c.systemtime = static_cast<int>(atoi(val));
		if (strstr(key, "MODEL")) {
			if (strstr(val, "A"))
				c.model = 1;
			else if (strstr(val, "B"))
				c.model = 2;
		}
		//printf("%s=%s\n", key, val);
	}
	fclose(fp);
	fp = nullptr;
	return c;
}

const char* get_items(const char*buffer, unsigned int item) {
	const char *p = buffer;
	size_t len = strlen(buffer);
	size_t count = 0, i = 0;
	for (i = 0; i < len; i++)
	{
		if (' ' == *p)
		{
			count++;
			if (count == item - 1)
			{
				p++;
				break;
			}
		}
		p++;
	}
	return p;
}

//获取总的CPU时间
unsigned long get_cpu_total_occupy()
{
	FILE *fd;
	char buff[1024] = {0};
	Total_Cpu_Occupy_t t;
	fd = fopen("/proc/stat", "r");
	if (nullptr == fd)
		return 0;
	fgets(buff, sizeof (buff), fd);
	char name[64] = {0};
	sscanf(buff, "%s %ld %ld %ld %ld", name, &t.user, &t.nice, &t.system, &t.idle);
	fclose(fd);
	return (t.user + t.nice + t.system + t.idle);
}

//获取进程的CPU时间
unsigned long get_cpu_proc_occupy(int pid)
{
	char file_name[64] = {0};
	Proc_Cpu_Occupy_t t;
	FILE *fd;
	char line_buff[1024] = {0};
	sprintf(file_name, "/proc/%d/stat", pid);
	fd = fopen(file_name, "r");
	if (nullptr == fd)
		return 0;
	fgets(line_buff, sizeof (line_buff), fd);
	sscanf(line_buff, "%u", &t.pid);
	const char *q = get_items(line_buff, PROCESS_ITEM);
	sscanf(q, "%ld %ld %ld %ld", &t.utime, &t.stime, &t.cutime, &t.cstime);
	fclose(fd);
	return (t.utime + t.stime + t.cutime + t.cstime);
}

//获取CPU占用率
float get_proc_cpu1(int pid)
{
	static unsigned long totalcputime1 = 0, procputime1 = 0;
	unsigned long totalcputime2, procputime2;
	totalcputime2 = get_cpu_total_occupy();
	procputime2 = get_cpu_proc_occupy(pid);
	float pcpu = 0.0;
	if (totalcputime1 == 0 || procputime1 == 0)
		pcpu = -2;
	else if (totalcputime2 - totalcputime1)
		pcpu = 100 * (static_cast<float>(procputime2)- static_cast<float>(procputime1))
			   / (static_cast<float>(totalcputime2) - static_cast<float>( totalcputime1));
	totalcputime1 = totalcputime2;
	procputime1 = procputime2;
	return pcpu;
}

//获取进程占用内存
int get_proc_mem(int pid)
{
	char file_name[64] = {0};
	FILE *fd;
	char line_buff[512] = {0};
	int i = 0;
	sprintf(file_name, "/proc/%d/status", pid);
	fd = fopen(file_name, "r");
	if (nullptr == fd)
		return 0;
	char name[64];
	int vmrss;
	for (i = 0; i < VMRSS_LINE - 1; i++)
		fgets(line_buff, sizeof (line_buff), fd);
	fgets(line_buff, sizeof (line_buff), fd);
	sscanf(line_buff, "%s %d", name, &vmrss);
	fclose(fd);
	return vmrss;
}

void getNameByPid(pid_t pid, char *task_name) {
	char file_name[64] = {0};
	char line_buff[512] = {0};
	sprintf(file_name, "/proc/%d/status", pid);
	FILE* fp = fopen(file_name, "r");
	if (nullptr != fp)
	{
		if (fgets(line_buff, sizeof (line_buff), fp) == nullptr)
			fclose(fp);
		fclose(fp);
		sscanf(line_buff, "%*s %s", task_name);
	}
	else
	{
		printf("PID ERROR\n");
		exit(0);
	}
}

void format(char* buf,tool* t)
{
	time_t t1;
	struct tm * lt;
	time(&t1); //获取Unix时间戳。
	lt = localtime(&t1); //转为时间结构。
	snprintf(buf,1024,"%02d-%02d-%02d,%02d:%02d:%02d,%d,%s,%dKB,%.2f%%\n",
			 lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday,lt->tm_hour, lt->tm_min, lt->tm_sec,
			 t->pid, t->name, t->memory, static_cast<double>(t->cpu));
}

FILE* openFile()
{
	FILE* fp = nullptr;
	if ((fp = fopen("tool.csv", "a")) == nullptr)
		perror("cant open the file");
	return fp;
}

static FILE* fp;

void closeFile()
{
	if(nullptr==fp)
		return;
	fclose(fp);
	fp=nullptr;
}

//写数据到.csv
void writecsv(tool* t)
{
	char buf[BUF_SIZE];
	memset(buf,0,1024);
	format(buf,t);
	printf("%s",buf);
	fprintf(fp,"%s",buf);
}

static volatile bool loop=true;

void quit(int)
{
	loop=false;
	cout<<"时间到，程序结束."<<endl;
}

int main()
{
	config c;
	memset(&c, 0, sizeof (config));
	c = readCfg(); //读取配置

	fp = openFile();

	tool t;
	memset(&t, 0, sizeof (tool)); //初始化数据结构体

	int mnow = 0;
	float cpunow = 0;

	signal(SIGALRM, quit);
	alarm(static_cast<unsigned int>(c.systemtime));
	while (loop)
	{
		t.pid = c.pid;
		getNameByPid(t.pid, t.name);//获取进程名称
		t.memory = get_proc_mem(t.pid);//获取进程内存大小
		t.cpu = get_proc_cpu1(t.pid);//
		if (c.model == 1)
			writecsv(&t);
		else if (c.model == 2)
		{
			if (mnow != t.memory || cpunow <= 0)
				writecsv(&t);
		}
		mnow = t.memory;
		cpunow = t.cpu;
		sleep(static_cast<unsigned int>(c.looptime));
	}
	closeFile();
	fp=nullptr;
	return 0;
}

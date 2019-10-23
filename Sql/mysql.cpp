#include <stdio.h>
#include <mysql/mysql.h>

int main()
{
	MYSQL mysql;    //一个数据库结构体
	MYSQL_RES* results; //一个结果集结构体
	MYSQL_ROW row;  //char** 二维数组，存放一条条记录
	//初始化数据库
	mysql_init(&mysql);
	//设置编码方式
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "utf8");
	//连接数据库
	//判断如果连接失败就输出连接失败。
	//注意你连接的账户名密码
	if (mysql_real_connect(&mysql, "localhost", "root", "123456", "example", 3306, nullptr, 0) == nullptr)
	{
		mysql_error(&mysql);
		perror("连接失败！\n");
		return -1;
	}
	printf("连接成功!\n");

	//const char *sql;
	//sql = "SELECT * FROM test";                           //查
	//sql = "INSERT INTO test(Name,Age) VALUES('Json','33')";    //增
	//sql = "DELETE FROM test where name='小李'";           //删
	//sql = "UPDATE test SET Age='22' where name='小王'";   //改
	//sql = "SELECT * FROM test ORDER BY id";              //排序
	//if(mysql_query(&mysql, sql)<0)
	//{
	//	perror("query failed:");
	//	return -1;
	//}

	//查询数据
	mysql_query(&mysql, "select * from test");
	//获取结果集
	results = mysql_store_result(&mysql);

	//给ROW赋值，判断ROW是否为空，不为空就打印数据。
	while ((row = mysql_fetch_row(results))!=nullptr)
	{
		printf("%s  ", row[0]);//打印ID
		printf("\t%s  ", row[1]);//打印姓名
		printf("\t%s  ", row[2]);//打印
		printf("\n");
	}
	//释放结果集
	mysql_free_result(results);
	//关闭数据库
	mysql_close(&mysql);
	return 0;
}

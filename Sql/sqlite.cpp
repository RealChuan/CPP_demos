#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <wchar.h>
#include <sqlite3.h>

int main()
{
	sqlite3 *db;
	char *zErrMsg = nullptr;
	int rc,result;

	rc = sqlite3_open("example.db", &db);

	if (rc)
	{
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return -1;
	}
	printf("Opened database successfully\n");
	const char *sql;
	sql = "SELECT * FROM test";                           //查
	//sql = "INSERT INTO students(id,name) VALUES('4','小丁')";    //增
	//sql = "DELETE FROM students where name='小李'";           //删
	//sql = "UPDATE students SET c='1111'where name='小王'";   //改
	//sql = "SELECT * FROM students ORDER BY id";              //排序

	result=sqlite3_exec(db,sql,nullptr,nullptr,&zErrMsg);
	if (result!=SQLITE_OK)
	{
		fprintf(stderr, "%s\n", zErrMsg);
		return -1;
	}

	sql = "SELECT * FROM test";
	int nrow = 0, ncolumn = 0;
	char **azResult; //二维数组存放结果
	result = sqlite3_get_table(db, sql, &azResult, &nrow, &ncolumn, &zErrMsg);
	if(result<0)
		printf("error:%s\n",zErrMsg);

	printf("row:%d column=%d \n", nrow, ncolumn);
	printf("\nThe result of querying is : \n");

	for (int i = 0; i <(nrow + 1)*ncolumn; i++)
	{
		printf("%s\t",azResult[i]);
		if((i+1)%ncolumn==0)
			printf("\n");
	}

	sqlite3_free_table(azResult);
	sqlite3_close(db);
	return 0;
}

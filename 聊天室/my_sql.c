#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "my_mysql.h"
#include "my_err.h"

int main()
{
	MYSQL mysql;
	char a[100] = "select *from user_data";
	mysql = accept_mysql();
	use_mysql(a, mysql);
	use_mysql(a, mysql);
	close_mysql(mysql);
	return 0;
}

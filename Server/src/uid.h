/*
 * uid.h
 *
 *  Created on: 2018年1月22日
 *      Author: Void
 */

#ifndef UID_H_
#define UID_H_

#define UID_CONF "/home/root/uid.conf"
#include <stdio.h>
#include <stdlib.h>

//获取设备id传入BUF 成功返回0 错误返回值<0
int get_uid(char *buf)
{
	int fd;
	fd = open(UID_CONF, O_RDONLY);
	if (fd < 0)
	{
		perror("open()");
		return -1;
	}
	if (read(fd, buf, 14) != 14)
	{
		perror("read() and uid != 14");
		return -1;
	}
	return 0;
}

#endif /* UID_H_ */

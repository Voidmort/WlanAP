/*
 * uid.h
 *
 *  Created on: 2018��1��22��
 *      Author: Void
 */

#ifndef UID_H_
#define UID_H_

#define UID_CONF "/home/root/uid.conf"
#include <stdio.h>
#include <stdlib.h>

//��ȡ�豸id����BUF �ɹ�����0 ���󷵻�ֵ<0
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

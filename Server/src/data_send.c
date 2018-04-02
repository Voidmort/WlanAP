/*
 * data_send.c
 *
 *  Created on: 2017年4月6日
 *      Author: Void
 */
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <glob.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>

#include "wifi_control.h"
#include "data_send.h"

static struct Asleep asleep;
static struct Sfile sfile;
static struct Download download;
static struct VerInfo verInfo;

int send_flag = 0;  //为0时没有数据发送  为1时有数据正在发送
int wifi_flag = 0;  //为0时休眠  为1时正在连接
int file_flag = 0;  //为0时没有失败文件需要发送，为1时有文件未发送

static int send_data(http_data data);
static char* wpa_sta;

static void asleep_str(char *a){

	memset(a, 0, BUFSIZE);
	strcat(a, "?uid=");
	strcat(a, asleep.uid);

	strcat(a, "&mdts=");
	strcat(a, asleep.mdts);

	strcat(a, "&dt=");
	strcat(a, asleep.dt);

	strcat(a, "&data=");
	strcat(a, asleep.data);

	strcat(a, "&age=");
	strcat(a, asleep.age);

	strcat(a, "&sex=");
	strcat(a, asleep.sex);

	strcat(a, "&wht=");
	strcat(a, asleep.wht);

	strcat(a, "&cm=");
	strcat(a, asleep.cm);


	strcat(a, "&nba=");
	strcat(a, asleep.nba);

	strcat(a, "&sa=");
	strcat(a, asleep.sa);

	strcat(a, "&ga=");
	strcat(a, asleep.ga);

	strcat(a, "&zw=");
	strcat(a, asleep.zw);

	strcat(a, "&cgr=");
	strcat(a, asleep.cgr);

	strcat(a, "&amnesia=");
	strcat(a, asleep.amnesia);

}

static void sfile_str(char *b){

	memset(b, 0, BUFSIZE);
	strcat(b, "?uid=");
	strcat(b, sfile.uid);

	strcat(b, "&dt=");
	strcat(b, sfile.dt);

	strcat(b, "&rsi=");
	strcat(b, sfile.rsi);

	strcat(b, "&myka=");
	strcat(b, sfile.myka);

	strcat(b, "&mykb=");
	strcat(b, sfile.mykb);

	strcat(b, "&sdi=");
	strcat(b, sfile.sdi);

	strcat(b, "&kk=");
	strcat(b, sfile.kk);

	strcat(b, "&nkk=");
	strcat(b, sfile.nkk);


	strcat(b, "&ibk=");
	strcat(b, sfile.ibk);

	strcat(b, "&tdo=");
	strcat(b, sfile.tdo);

	strcat(b, "&hu=");
	strcat(b, sfile.hu);

	strcat(b, "&sl=");
	strcat(b, sfile.sl);

	strcat(b, "&al=");
	strcat(b, sfile.al);

	strcat(b, "&err=");
	strcat(b, sfile.err);

	strcat(b, "&ver=");
	strcat(b, sfile.ver);
}

static void download_str(char *c){

	memset(c, 0, BUFSIZE);
	strcat(c, "?ver=");
	strcat(c, download.ver);

	strcat(c, "&line=");
	strcat(c, download.line);
}

static void verInfo_str(char *d){

	memset(d, 0, BUFSIZE);
	strcat(d, "?ID=");
	strcat(d, verInfo.ID);

	strcat(d, "&Kind=");
	strcat(d, verInfo.Kind);

	strcat(d, "&Ver=");
	strcat(d, verInfo.Ver);
}

static void message_str(char *s1, char *s2)
{
	if (strcmp(s1, "GET /date.php") == 0){
		memset(s2, 0, BUFSIZE);
		strcat(s2, "\r\n\r\n");
	}
	else if (strcmp(s1, "GET /alsleep.php") == 0){
		asleep_str(s2);
		strcat(s2, "\r\n\r\n");
	}
	else if (strcmp(s1, "GET /sfile.php") == 0){
		sfile_str(s2);
		strcat(s2, "\r\n\r\n");
	}
	else if (strcmp(s1, "GET /SleepCareUpdate/VerInfo.php") == 0){
		verInfo_str(s2);
		strcat(s2, "\r\n\r\n");
	}
	else if (strcmp(s1, "GET /SleepCareUpdate/download.php") == 0){
		download_str(s2);
		strcat(s2, "\r\n\r\n");
	}
	else{
		printf("wrong file name!\n");
		return;
	}
}

static int mk_dir(char* path)
{
 	if(access(path,F_OK) == -1)
 	{
 		if (mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
 		{
 			printf("Mkdir %s error", path);
 			return -1;
 		}
 		return 1;
 	}
 	return 0;
}

/////////////////////////////////////////////////////////////////////
//把未发送成功的保存到本地
//返回保存成功的文件名字
//失败返回NULL
static char* write_sfile(struct Sfile file)
{
	FILE *fp=NULL;
	time_t lt;
	time (&lt);
	static char name[16];
	char path[36] = SFILE_PATH;

	if (mk_dir(path) == -1)
		return NULL;
	sprintf(name, "/%d.sbin", lt);
	strcat(path, name);

//	printf("%s\n",path);

	fp=fopen(path, "wb" );
	if( fp == NULL )
	{
		printf("open file for write error!\n");
		return NULL;
	}

	fwrite( &file, sizeof(struct Sfile), 1, fp );
	fclose(fp);

	return name;
}
static char* write_asleep(struct Asleep sleep)
{
	FILE *fp=NULL;
	time_t lt;
	time (&lt);
	static char name[16];
	char path[36] = ASLEEP_PATH;

	mk_dir(path);

	sprintf(name, "/%d.abin", lt);
	strcat(path, name);
	fp=fopen(path, "wb" );
	if( fp == NULL )
	{
		printf("open file for write error\n");
		return NULL;
	}

	fwrite( &sleep, sizeof(struct Asleep), 1, fp );
	fclose(fp);

	return name;
}
//读取文件并发送
//发送成功删除文件
//返回值：
//	>0 	成功发送的个数
//	-1	错误
static int read_send_data(char* file_path)
{
	send_flag = 1;
	glob_t buf;
	int i, j = 0, ret_http;
	FILE *fp=NULL;
	struct Sfile file;
	char path[32] = {};
	http_data data;

	strcat(path, file_path);
	strcat(path, "/*");
	glob(path, GLOB_NOSORT, NULL, &buf);
	if (buf.gl_pathc == 0)
	{
		printf("no file!\n");
		file_flag = 0;
		globfree(&buf);
		return 0;
	}
	else
		file_flag = 1;

	for (i = 0; i < buf.gl_pathc; i++)
	{
		fp=fopen( buf.gl_pathv[i], "rb" );
	//	printf("-----%s\n", buf.gl_pathv[i]);
		if( fp == NULL )
		{
			printf("open file for read error!\n");
			return -1;
		}
		fread( &file, sizeof(struct Sfile), 1, fp );
		data.sfile = &file;
	//	printf("uid=%s dt=%s ver =%s\n", data.sfile->uid, data.sfile->dt, data.sfile->ver );
		ret_http = send_data(data);
		if (ret_http == 0)
		{
			remove(buf.gl_pathv[i]);
			j++;
		}
		else
		{
			return -2;
		}
		fclose(fp);
	}

	globfree(&buf);
	return j;
}

/*建立SOCKET连接 成功返回sockfd 失败返回值小于零*/
static int buildsocket(int domain, int type, int protocol)
{
	struct sockaddr_in servaddr;
	int sockfd;

	sockfd = socket(domain, type, protocol);
	if (sockfd < 0 )
	{
		printf("socket error!\n");
		return -1;
	};
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);

	if (inet_pton(domain, IPSTR, &servaddr.sin_addr) <= 0 )
	{
		printf("inet_pton error!\n");
		close(sockfd);
		return -2;
	}
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
        printf("socket connect error!\n");
        close(sockfd);
        return -3;
	}
//	printf("connect success \n");
	return sockfd;
}

//
static void *_wifi_control()
{
	wpa_status_st wpa_sta_st;
	wpa_sta_st = wifi_control();
	wpa_sta = wpa_sta_st.ssid;
	pthread_exit(NULL);
}

//如果网络断开，尝试进行WIFI连接，成功返回NULL，失败返回错误信息
static char* wait_wifi_connect()
{
	pthread_t t_wifi;

	printf("Initiating connection to wifi. Please wait...!\n");
	wifi_flag = 1;
	if (pthread_create(&t_wifi, NULL, _wifi_control,NULL) != 0)
	{
		perror("t_wifi pthread_create():");
		wifi_flag = 0;
		return "t_wifi pthread_create():";
	}

	pthread_join(t_wifi, NULL);

	if (wpa_sta != NULL)
	{
		wifi_flag = 0;
		return wpa_sta;
	}

	wifi_flag = 0;
	return NULL;
}

/*
 *发送成功返回0
 *失败返回值<0
 *返回值 -1 socket连接错误
 *返回值 -2 服务器返回错误信息
 *返回值 -3 函数send() 失败
*/
static int send_data(http_data data)
{
	int sockfd;
	char str1[BUFSIZE], str2[BUFSIZE], str3[BUFSIZE], buf[BUFSIZE];;
	int ret,h;
	fd_set t_set;
	struct timeval tv;
	char* re;

	send_flag = 1;
	if ((sockfd = buildsocket(AF_INET, SOCK_STREAM, 0)) < 0 )
 	{
		if (wifi_flag == 0)
		{
			re = wait_wifi_connect();
			if (re != NULL)
			{
				printf("The network connection : %s\n",re);
				close(sockfd);
				return -1;
			}
			else
				printf("The network connection is normal!\n");
		}
		else
		{
			close(sockfd);
			return -5;
		}
 	}
	memset(str1, 0, BUFSIZE);

	if (data.sfile != NULL)
	{
		sfile = *data.sfile;
		strcat(str1,"GET /sfile.php");

	}
	else if (data.asleep != NULL)
	{
		asleep = *data.asleep;
		strcat(str1, "GET /alsleep.php");
	}
	else if (data.downlod)
	{
		download = *data.downlod;
		strcat(str1, "GET /SleepCareUpdate/download.php");
	}
	else if (data.verinfo)
	{
		verInfo = *data.verinfo;
		strcat(str1, "GET /SleepCareUpdate/VerInfo.php");
	}
	else if (data.date != NULL)
		strcat(str1, "GET /date.php");
	else
	{
		printf("http_data data error!\n");
		close(sockfd);
		return -1;
	}
	memset(str3, 0, BUFSIZE);
	strcat(str3, "HTTP/1.0\r\n");
	strcat(str3, "Accept: */*\r\n");
	strcat(str3, "Accept-Language: cn\r\n");
	strcat(str3, "User-Agent: Mozilla/4.0\r\n");
	strcat(str3, "Host: "IPSTR"\r\n");
	strcat(str3, "Connection: Keep-Alive\r\n");
	strcat(str3, "\r\n\r\n");

	message_str(str1, str2);

	strcat(str1, str2);
	strcat(str1, str3);
	strcat(str1, "\r\n\r\n");
	//printf("------------------------------------------------>>>\n%s\n",str1);

	ret = send(sockfd,(void *)str1,strlen(str1),0);
	if (ret < 0)
	{
		printf("send error %d，Error message'%s'\n",errno, strerror(errno));
		close(sockfd);
		return -3;
	}
	else
	{
		printf("send success ,total send %d \n", ret);
	}

	//printf("uid=%s dt=%s ver =%s\n", data.sfile->uid, data.sfile->dt, data.sfile->ver );
	//获取HTTP返回响应

	while(1)
	{
		tv.tv_sec= 0;
		tv.tv_usec= 0;
		h= 0;
		FD_ZERO(&t_set);
		FD_SET(sockfd, &t_set);

		h= select(sockfd +1, &t_set, NULL, NULL, &tv);

		if (h == 0) continue;
		if (h < 0)
		{
			close(sockfd);
			printf("Select error!\n");
			return -4;;
		};

		if (h > 0)
		{
			memset(buf, 0, BUFSIZE);
			if (recv(sockfd, (void *)buf, BUFSIZE,0) == 0)
			{
				close(sockfd);
				break;
			}
			printf("server return:%s\n", buf);//HTTP返回值
			if (data.sfile != NULL)
			{
				if (strcmp(buf, "ok"))

				{	close(sockfd);
					return -2;

				}
			}
		}
	}
	return 0;
}


//发送数据
//成功返回0
//写入数据返回>0
//出错返回值小于0
int http_send(http_data data)
{
	struct Sfile file;
	struct Asleep sleep;

	if (send_flag == 0)
	{
		if (send_data(data) == 0)
		{
			send_flag = 0;
			if (data.sfile != NULL)
			{
				if (read_send_data(SFILE_PATH) >= 0)
				{
					send_flag = 0;
					return 0;
				}
				else
				{
					send_flag = 0;
					return -6;
				}
			}
			else if (data.asleep != NULL)
			{
				if (read_send_data(ASLEEP_PATH) >= 0)
				{
					send_flag = 0;
					return 0;
				}
				else
				{
					send_flag = 0;
					return -6;
				}
			}
			else
			{
				send_flag = 0;
				return -2;
			}

		}

		else
		{
			send_flag = 0;
			printf("send erron write data!\n");
			if (data.sfile != NULL)
			{
				file = *data.sfile;
				if (write_sfile(file) != NULL)
					return 1;
				else
				{
					printf("write data failed!\n");
					return -1;
				}
			}
			else if (data.asleep != NULL)
			{
				sleep = *data.asleep;
				if (write_asleep(sleep) != NULL)
					return 2;
				else
				{
					printf("write data failed!\n");
					return -2;
				}
			}
			else
			{
				//没有文件写入
				return 3;
			}
		}

	}
	else
	{
		printf("write data\n");
		if (data.sfile != NULL)
		{
			file = *data.sfile;
			if (write_sfile(file) != NULL)
				return 1;
			else
			{
				printf("write data failed!\n");
				return -1;
			}
		}
		else if (data.asleep != NULL)
		{
			sleep = *data.asleep;
			if (write_asleep(sleep) != NULL)
				return 2;
			else
			{
				printf("write data failed!\n");
				return -2;
			}
		}
		else
		{
			//没有文件写入
			return 3;
		}
	}
	return 0;
}

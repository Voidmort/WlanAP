/*
 * wifi_ap.c
 *
 *  Created on: 2017年12月28日
 *      Author: Void
 */

#include "wifi_control.h"

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
// 服务器端口号
#define SERVERPORT        "666"

#define FMT_STAMP        "%s"

static int service_sd;

struct login{
	char ssid[64];
	char password[64];
};
static pthread_t T1_wait_ap;
static pthread_t T2_ap_ctl;
static void server_job(int sd)
{
    char buf[128];
    int len;

    len = sprintf(buf,FMT_STAMP,"OK!");

    if(send(sd,buf,len,0) < 0)
    {
        perror("send()");
        exit(1);
    }

    return ;
}

//服务端
char* server_main()
{
    int newsd,i;
    struct sockaddr_in laddr,raddr;
    socklen_t raddr_len;
	static char buf[128] = {};
    // 选择 TCP 协议
	service_sd = socket(AF_INET,SOCK_STREAM,0/*IPPROTO_TCP,IPPROTO_SCTP*/);
    if(service_sd < 0)
    {
        perror("socket()");
        exit(1);
    }

    // SO_REUSEADDR 用来设置端口被释放后可立即被重新使用
    int val = 1;
    if(setsockopt(service_sd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val)) < 0)
    {
        perror("setsockopt()");
        exit(1);
    }

    laddr.sin_family = AF_INET;
    // 指定服务端使用的端口号
    laddr.sin_port = htons(atoi(SERVERPORT));
    inet_pton(AF_INET,"0.0.0.0",&laddr.sin_addr.s_addr);

    // 绑定端口
    if(bind(service_sd,(void *)&laddr,sizeof(laddr)) < 0)
    {
        perror("bind()");
        exit(1);
    }

    // 开始监听端口
    if(listen(service_sd,200) < 0)
    {
        perror("listen()");
        exit(1);
    }

    raddr_len = sizeof(raddr);

    while(1)
    {
        // 阻塞等待新消息传入
        newsd = accept(service_sd,(void *)&raddr,&raddr_len);

        if(newsd < 0)
        {
            if(errno == EINTR || errno == EAGAIN)
                continue;
            perror("newsd()");
            break;
        }
		i = recv(newsd,buf,sizeof(buf), 0);
		if( i < 0)
		{
			perror("recv:");
			return NULL;
		}
		else
		{
			//printf("buf = %s\n",buf);
			break;
		}
    }
	server_job(newsd);
    close(service_sd);
	close(newsd);
    return buf;
}

//打开AP功能
int wifi_ap()
{
	char cmd[128] = "";
	if (wifi_init() != NULL)
		return -1;
	//strcat(cmd, "cp ");
	//strcat(cmd, HOSTAPD_CONF);
	//strcat(cmd, " /etc/hostapd/hostapd.conf");
	//printf("%s\n",cmd);
	//system(cmd);
	system("systemctl start hostapd");

	return 0;
}

static struct login ret_login(char str[64])
{
	struct login login;

	strcpy(login.ssid, strtok(str, "\n"));
	strcpy(login.password, strtok(NULL, "\n"));
	return login;
}

static void* wait_ap()
{
	FILE *fstream=NULL;
	while (1)
	{
		printf("wait ap!\n");
		sleep(60);
		if(!(fstream = popen("hostapd_cli all | grep dot11RSNAStatsSTAAddress", "r")))
		{
			perror("popen()\n");
			return "error popen()";
		}
		if(EOF==fgetc(fstream))
		{
			printf("exit wifi ap mod!\n");
			system("systemctl stop hostapd");
			pthread_cancel(T2_ap_ctl);
			break;
		}
	}
	printf("wait ap exit!\n");
	pclose (fstream);
	return 0;
}

static void* wifi_ap_ctl_()
{
	char *buf;
	struct login login;
	wifi_ap();
	buf = server_main();
	if (buf == NULL)
		return "error server_main()";
	login = ret_login(buf);
	system("systemctl stop hostapd");
	printf("connect wifi ssid = %s, password = %s\n",login.ssid,login.password);
	if (connect2wifi(login.ssid,login.password) == NULL)
	{
		shutdown(service_sd,2);
		pthread_cancel(T1_wait_ap);
	}
	else
		return "error connect2wifi()";
	return 0;
}


int _wifi_ap_ctl()
{
	pthread_create(&T1_wait_ap, NULL, wait_ap, NULL);
	pthread_create(&T2_ap_ctl, NULL, wifi_ap_ctl_, NULL);

	pthread_join(T1_wait_ap,NULL);
	pthread_join(T2_ap_ctl,NULL);
	return 0;
}

int wifi_ap_ctl()
{
	_wifi_ap_ctl();
	sleep(3);
	system("systemctl restart wpa_supplicant");
	return 0;
}

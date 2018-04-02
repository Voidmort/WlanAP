/* Author: 
   Copyright (c) 2016 Basesys Corporation.
*/

/*******************************************************************************
 **
 ** Name:           wifi_control.c
 **
 ** Description:    This program is to implementing the WiFi function.
 **
 *******************************************************************************/

//#include "sc500.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "wifi_control.h"

#define BUFSIZE				521
#define RET_STATUS_ERROR	"Get WIFI status error!"
#define SUCCEED				1
#define ERROR				0
#define SSID_LEN			32
#define PASSWORD_LEN		32

/*
int inttochar(int nnum, char *strfromint)
{
	int i=0, k;
	char indexi[]="0123456789";
    char temp;

 //转换
 	 do{
 		 strfromint[i++]=indexi[nnum % 10];
 		 nnum/= 10;
 	 }while(nnum);
	 
 	strfromint[i]='\0';

 	for(k=0; k<(i/2); k++)
	{
	   temp = strfromint[k];
	   strfromint[k] = strfromint[i-1-k];
	   strfromint[i-1-k] = temp;
	}
//if num >1000?
//	return str;
}
*/
//判断连接状态,返回1已经连接，0未连接
int get_connect_status()
{
	wpa_status_st sta;
	int i;
	sta = ret_status();
	if (strcmp(sta.fault,"null"))
		return ERROR;
	for (i = 0; i < 10 ; i++)
	{
		//if (strcmp(sta.ip_address,"") && strcmp(sta.ssid,"") && !strcmp(sta.wpa_state,"COMPLETED"))
		if (!strcmp(sta.wpa_state,"COMPLETED"))
			return SUCCEED;
		sleep(2);
		sta = ret_status();
	}
		return ERROR;
}

//判断文件是否为空 为空返回1 非空返回0
static int fgetc_eof(FILE* fp)
{
	if (fgetc(fp) == EOF)
		return SUCCEED;
	else
		return ERROR;
}

//初始化wlan，成功返回NULL，失败返回错误原因
static char* wlan_init(void)
{
	FILE *fpe, *fpstart, *fpstop;
	char *enable = "systemctl enable wpa_supplicant",//  (设置wifi上电自动连接)
		 *start = "systemctl start wpa_supplicant",//(打开wpa)
		 *stop = "systemctl stop connman";//(关闭connman避免冲突)
	if(NULL == (fpe = popen(enable,"r")))
	{
		pclose(fpe);
		return "popen enable wpa_supplicant error!";
	}
	else if (!fgetc_eof(fpe))
	{
		pclose(fpe);
		return "systemctl enable wpa_supplicant error!";
	}

	if(NULL == (fpstart = popen(start,"r")))
	{
		pclose(fpe);
		pclose(fpstart);
		return "popen start wpa_supplicant error!";
	}
	else if (!fgetc_eof(fpstart))
	{
		pclose(fpe);
		pclose(fpstart);
		return "systemctl start wpa_supplicant error!";
	}

	if(NULL == (fpstop = popen(stop,"r")))
	{
		pclose(fpe);
		pclose(fpstart);
		pclose(fpstop);
		return "popen stop connman error!";
	}
	else if (!fgetc_eof(fpstop))
	{
		pclose(fpe);
		pclose(fpstart);
		pclose(fpstop);
		return "systemctl stop connman error!";
	}
	return NULL;
}

//初始化WIFI，成功返回NULL，失败返回错误原因，初始化未成功会执行3次，间隔1秒，超时错误
char* wifi_init(void)
{
	int i = 0;
	char* ret;
	while (i < 3)
	{
		ret = wlan_init();
		if (ret == NULL)
			return NULL;
		i++;
		sleep(1);
	}
	return ret;
}

//判断命令是否执行失败，失败返回0，成功返回1
static int cmd_faild(void)
{
	FILE *fp;
    char* cmd = "wpa_cli static | grep Failed";


    if(NULL == (fp = popen(cmd,"r")))
    {
    	pclose(fp);
    	return 0;
    }
    else
    {
    	if (fgetc(fp) == EOF)
    	{
    		pclose(fp);
    		return 1;
    	}
    	else
    	{
    		pclose(fp);
    		return 0;
    	}
    }
}

//获取 = 后的字符,失败返回NULL
static char* ret_wpa_equal(char* buf)
{
	char tmp[128];
	strcpy(tmp,buf);
	if (!strcmp (strtok(buf,"="), tmp))
		return NULL;
	return strtok(NULL,"=");
}

//字符串结尾位\n去除
static void ret_end(char* str)
{
	int len = strlen(str);
	if(!strcmp(str+len-1,"\n"))
		str[len - 1] = '\0';
}

//读取wpa响应的状态，成功返回状态行，失败返回NULL
static char *read_wpa_status(char* p)
{
	FILE *fstream=NULL;
	static char buf[BUFSIZE];

    memset(buf,0,sizeof(buf));
    char str[BUFSIZE] = "wpa_cli status | grep ^";

    if(!cmd_faild())
    {
    	return NULL;
    }
    else
    {
    	strcat(str, p);
    	if(NULL==(fstream = popen(str,"r")))
    	{
    		pclose(fstream);
    		return NULL;
    	}
    	if(NULL!=fgets(buf, sizeof(buf), fstream))
    	{
    		pclose(fstream);
    		ret_end(buf);
    		return buf;
    	}
    	else
    	{
    		pclose(fstream);
    		return NULL;
    	}
    }
}


//获取WIFI信号强度，没有信号返回“-”，获取失败返回NULL
char* get_signal_level(char *ssid)
{
	int flag = 0;
	char *p;
	FILE *fstream=NULL;
	char buf[BUFSIZE];

    ret_end(ssid);
	if (cmd_faild())
	{
		memset(buf,0,sizeof(buf));
		char str[BUFSIZE] = "wpa_cli -i wlan0 scan_results | grep ";
		strcat(str, ssid);
		strcat(str, "$");
		system("wpa_cli -i wlan0 scan > /dev/null");
		if(NULL==(fstream=popen(str,"r"))){
	    	return "-";
		}
		if(NULL!=fgets(buf, sizeof(buf), fstream)){
			pclose(fstream);
	    	p=strtok(buf,"\t");
	    	while (flag<2){
	        	flag++;
	        	p=strtok(NULL,"\t");
	    	}
		}
		else{
		pclose(fstream);
	   	   return "-";
		}
		return p;
	}
	else
		return NULL;
}
//获取WIFI的状态，返回wpa_status_st结构体，出错时.fault返回错误信息，正常.fault返回“null”
wpa_status_st ret_status()
{
	wpa_status_st sta = { };
	char *bssid, *ssid, *key_mgmt, *ip_address, *wpa_state, *signal_level;

	strcpy(sta.fault, "null");
	wpa_state = read_wpa_status("wpa_state");
	if (wpa_state != NULL)
		strcpy(sta.wpa_state , ret_wpa_equal(wpa_state));
	else{
			strcpy(sta.fault, wpa_state);
			return sta;
	}

	bssid = read_wpa_status("bssid");
	if (bssid != NULL)
		strcpy(sta.bssid, ret_wpa_equal(bssid));
	else{
		strcpy(sta.fault, RET_STATUS_ERROR);
		return sta;
	}

	ssid = read_wpa_status("ssid");
	if (ssid != NULL)
		strcpy(sta.ssid , ret_wpa_equal(ssid));
	else{
			strcpy(sta.fault, "Not connect wifi!");
			return sta;
	}

	key_mgmt = read_wpa_status("key_mgmt");
	if (key_mgmt != NULL)
		strcpy(sta.key_mgmt , ret_wpa_equal(key_mgmt));
	else{
			strcpy(sta.fault, "ket mgmt error!");
			return sta;
	}
	ip_address = read_wpa_status("ip_address");
	if (ip_address != NULL)
		strcpy(sta.ip_address , ret_wpa_equal(ip_address));
	else{
			strcpy(sta.fault, "ip address error!" );
			return sta;
	}

	signal_level = get_signal_level(sta.ssid);
	if (signal_level != NULL)
		strcpy(sta.signal_level , signal_level);
	else{
				strcpy(sta.fault, "get signal level error!");
				return sta;
		}
	return sta;
}

//尝试连接,执行成功返回NULL，失败返回错误信息
static void wifi_connect(char *ssid,char *password)
{
	char wpopen[64] = WPOPEN;
	char wpa2[64] = WPA2;
	char *space = " ";

//	printf("\n%s %d ssid = %s password = %s\n",__func__,__LINE__,ssid,password);

	if (password == NULL)
	{
		strcat(wpopen,ssid);
		strcat(wpopen," > /dev/null");
		system(wpopen);
		sleep(1);
		return;
	}
	else
	{
		strcat(wpa2,ssid);
		strcat(wpa2,space);
		strcat(wpa2,password);
		strcat(wpa2," > /dev/null");

	//	printf("\nwpa2 = %s\n",wpa2);

		system(wpa2);
		sleep(1);
		return ;
	}

}

//判断ssid是否正确，扫描失败返回错误信息，扫描5次间隔3秒，扫描超时返回错误信息，成功返回NULL
char* read_ssid(char* ssid)
{
	char *ret;
	int count = 0;

	ret = get_signal_level(ssid);

	if (ret == NULL)
		return "scan error!";
	for( ;!strcmp(ret, "-"); count++)
	{
		sleep(3);
		ret = get_signal_level(ssid);
		if (count == 5)
			return "Restarting the wlan and check up ssid!";
	}
	return NULL;
}

//连接成功返回NULL，失败返回失败原因,尝试连接3次，间隔10秒
char* connect2wifi(  char* ssid,  char* password)
{
	wpa_status_st sta;
	int count = 0 ,i;

	printf("\n^^^ ssid = %s ^ password = %s ^^^\n",ssid,password);

	wifi_connect(ssid,password);
	sleep(10);
	for(count = 0 ,i = 0;!get_connect_status() ; count++ )
	{
		wifi_connect(ssid,password);
		sleep(10+i);
		i += 2;
		sta = ret_status();
		if (count == 3)
		{
			if (!strcmp(sta.wpa_state, "SCANNING"))
				return "password is incorrect!";
			else if (sta.signal_level == NULL)
				return "scan error!";
			else if (!strcmp(sta.signal_level,"-"))
				return "signals are weak!";
			else
				return "The requested command cannot be completed because an unknown error occurred!";
		}
	}
	return NULL;
}

//读取文件的SSID 或PSK 成功返回读取到的信息，失败返回NULL
char* read_file(char* mod)
{

	FILE* fstream;
	char buf[128];
	char str[32] = "cat ";
	char *ret;

    if((access(WIFI_CONF,F_OK)) == -1)
    	return NULL;

	strcat(str, WIFI_CONF);
	strcat(str, " | grep ^");
	strcat(str, mod);

	if (NULL==(fstream=popen(str,"r")))
	{
		pclose(fstream);
		return NULL;
	}

	if (fgetc_eof(fstream))
	{
		pclose(fstream);
		return NULL;
	}

	if (NULL==fgets(buf, sizeof(buf), fstream))
	{
		pclose(fstream);
		return NULL;
	}

	ret = ret_wpa_equal(buf);
	if (ret == NULL)
	{
		pclose(fstream);
		return NULL;
	}
	ret_end(ret);
	return ret;
}
/*
 interface=wlan0 driver=nl80211 ssid=test channel=1
  */

//wifi控制函数，成功返回wpa_status_st，失败.fault返回错误信息
wpa_status_st wifi_control(void)
{
	wpa_status_st sta = { };
	char *ret;
	static char ssid[SSID_LEN];
	static char password[PASSWORD_LEN];
	int i;

	//初始化
	ret = wifi_init();
	if (ret != NULL)
	{
		strcpy(sta.fault, ret);
		return sta;
	}

	//判断连接状态
	sta = ret_status();
	if (get_connect_status())
		return sta;

//	printf( "\ninit suceed^^^^^^^^^^^^^^ %s . %d\n\n", __func__, __LINE__  );
	//获取SSID和password
	strcpy(ssid,read_file("ssid"));
	for (i = 0;ssid == NULL ; i++)
	{
		sleep(1);
		strcpy(ssid,read_file("ssid"));
		if(i < 3)
			strcpy(sta.fault, "ssid read error!");
				return sta;
	}
	strcpy(password,read_file("psk"));

	//判断密码
	if ((strlen(password) < 8) && (password != NULL))
	{
		strcpy(sta.fault, "Password error greater than or equal 8 digits password!");
		return sta;
	}

//	printf("\n%s %d ssid = %s password = %s\n",__func__,__LINE__,ssid,password);

	//读取ssid是否存在
	ret = read_ssid(ssid);
	if (ret != NULL)
	{
		strcpy(sta.fault, ret);
		return sta;
	}

	//连接
	ret = connect2wifi(ssid, password);
	if (ret != NULL)
	{
		strcpy(sta.fault, ret);
		return sta;
	}
	sta = ret_status();
	return sta;
}

//断开WIFI连接
void rm_wifi(void)
{
	system("wpa_cli -i wlan0 remove_network all > /dev/null");
}



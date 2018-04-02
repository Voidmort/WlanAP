/* Author: 
   Copyright (c) 2016 Basesys Corporation.
*/

/*******************************************************************************
 **
 ** Name:           wifi_control.h
 **
 ** Description:    This program is the definition of the WiFi function.
 **
 *******************************************************************************/
//启动edison的ap模式
int wifi_ap();
/*
//asleeep
struct Asleep{
	char uid[15];		//ID, 14 decimal
	char mdts[13];	//Sleep Start TIme
	char dt[11];		//Sleep Aanlyzing Tiem
	char data[100];	//Sleep Result Data    ????????
	char age[3];		//Age
	char sex[2];		//Gender
	char wht[4];		//Weight
	char cm[2];		//Communication Setup
	char nba[2];		//Alert Setup
	char sa[5];		//Sleep Time
	char ga[5];		//Wake-up Time
	char zw[3];		//Reserved
	char cgr[6];		//Reserved
	char amnesia[9];	//认知障碍指数:D型 A型
}asleep;

//sfile
struct Sfile{
	char uid[15];		//ID, 14 decimal
	char dt[13];		//Calculated time, yyyymmddhhmm - 12 decimal
	char rsi[4];		//reserved
	char myka[4];	//reserved
	char mykb[4];	//Heart Rate
	char sdi[4];		//Body Movement Time
	char kk[4];		//Breath Rate
	char nkk[3];		//Apnea Time: 
	char ibk[3];		//Snore Time
	char tdo[3];		//Pressure
	char hu[4];		//In/Off Bed
	char sl[4];		//Sleeping Start
	char al[4];		//Alert
	char err[11];		//Machine Status
	char ver[4];		//SW Version
}sfile;

//download
struct Download{

	char ver[9];
	char line[2];

}download;

//verInfo
struct VerInfo{

	char ID[15];
	char Kind[5];
	char Ver[5];

}verInfo;

//char ending_symbol[4]="\r\n\r\n";

extern char asleep_str(char*);
extern char sfile_str(char*);
extern char download_str(char*);
extern char verInfo_str(char*);
//send message
extern char message_str(char*, char*);
int http_send(int heart_rate_s, int respiration_rate_s);
*/
int buildSocket(int domain, int type, int protocol);

int inttochar(int nnum, char *strfromint);
extern void get_current_minute(char *current_minute);

#ifndef WIFI_H_
#define WIFI_H_

//#define WIFI_CONF 	"/home/root/wlan/wifi.ini"
#define WIFI_CONF 	"/home/root/sc500.ini"

#define WPOPEN 		"sh wpacli_connect_open.sh "
#define WPA2		"sh wpacli_connect_wpa2.sh "
#define LENG		24

typedef struct {
	char fault[128];  			//错误信息，是null时无错误
	char bssid[LENG]; 			//bssid
	char ssid[LENG];			//ssid
	char key_mgmt[LENG];		//加密方式
	char wpa_state[LENG];		//wifi状态
	char ip_address[LENG];		//IP地址
	char signal_level[LENG];	//信号强度
}wpa_status_st;

//初始化WIFI，成功返回NULL，失败返回错误原因
char* wifi_init(void);

//连接成功返回NULL，失败返回失败原因,尝试连接3次，间隔5秒
char* connect2wifi(char* ssid,char* password);

//获取WIFI的状态，返回wpa_status_st结构体，出错时.fault返回错误信息，正常.fault返回“null”
wpa_status_st ret_status();

//获取WIFI信号强度，没有信号返回“-”，获取失败返回NULL
char* get_signal_level(char *ssid);

//获取连接状态,返回1已经连接，0未连接
int get_connect_status();

//wifi控制函数，成功返回wpa_status_st，失败.fault返回错误信息
wpa_status_st wifi_control(void);

//读取文件的SSID 或PSK 成功返回读取到的信息，失败返回NULL
char* read_file(char* mod);

//断开WIFI连接
void rm_wifi(void);

#endif /* WIFI_H_ */



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
//����edison��apģʽ
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
	char amnesia[9];	//��֪�ϰ�ָ��:D�� A��
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
	char fault[128];  			//������Ϣ����nullʱ�޴���
	char bssid[LENG]; 			//bssid
	char ssid[LENG];			//ssid
	char key_mgmt[LENG];		//���ܷ�ʽ
	char wpa_state[LENG];		//wifi״̬
	char ip_address[LENG];		//IP��ַ
	char signal_level[LENG];	//�ź�ǿ��
}wpa_status_st;

//��ʼ��WIFI���ɹ�����NULL��ʧ�ܷ��ش���ԭ��
char* wifi_init(void);

//���ӳɹ�����NULL��ʧ�ܷ���ʧ��ԭ��,��������3�Σ����5��
char* connect2wifi(char* ssid,char* password);

//��ȡWIFI��״̬������wpa_status_st�ṹ�壬����ʱ.fault���ش�����Ϣ������.fault���ء�null��
wpa_status_st ret_status();

//��ȡWIFI�ź�ǿ�ȣ�û���źŷ��ء�-������ȡʧ�ܷ���NULL
char* get_signal_level(char *ssid);

//��ȡ����״̬,����1�Ѿ����ӣ�0δ����
int get_connect_status();

//wifi���ƺ������ɹ�����wpa_status_st��ʧ��.fault���ش�����Ϣ
wpa_status_st wifi_control(void);

//��ȡ�ļ���SSID ��PSK �ɹ����ض�ȡ������Ϣ��ʧ�ܷ���NULL
char* read_file(char* mod);

//�Ͽ�WIFI����
void rm_wifi(void);

#endif /* WIFI_H_ */



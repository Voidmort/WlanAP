/*
 * data_send.h
 *
 *  Created on: 2017年4月6日
 *      Author: Void
 */

#ifndef WIFICTL_DATA_SEND_CTL_H_
#define WIFICTL_DATA_SEND_CTL_H_

#define IPSTR 				"203.195.170.235"   //get from parameter file later
#define PORT 80
#define BUFSIZE 			1024

#define SFILE_PATH 			"/tmp/sfile"
#define ASLEEP_PATH 		"/tmp/asleep"


//asleeep
struct Asleep{

	char uid[15];	//14桁の機器ＩＤ文字列
	char mdts[13];	//睡眠開始日時
	char dt[11];	//睡眠解析日時
	char data[100];	//睡眠解析データ（睡眠段階データ）
	char age[3];		//被験者年齢
	char sex[2];//被験者年齢
	char wht[4];	//被験者体重
	char cm[2];	//通信設定
	char nba[2];	//アラーム設定
	char sa[5];	//目標就寝時刻
	char ga[5];	//目標起床時刻
	char zw[3];	//未使用
	char cgr[6];	//未使用
	char amnesia[9];	//認知症指数D型A型

};

//sfile
struct Sfile{

	char uid[15];	//14桁の機器ＩＤ文字列
	char dt[13];	//データ取得日時
	char rsi[4];	//未使用
	char myka[4];	//未使用
	char mykb[4];	//脈拍数
	char sdi[4];	//体動大きさ
	char kk[4];		//呼吸数
	char nkk[3];	//無呼吸時間
	char ibk[3];	//いびき発生時間
	char tdo[3];	//圧力体動
	char hu[4];		//人体有無
	char sl[4];		//睡眠開始フラグ
	char al[4];		//アラーム
	char err[11];	//機器ステータス
	char ver[4];	//プログラムバージョン

};

//download
struct Download{

	char ver[9];
	char line[2];

};

//verInfo
struct VerInfo{

	char ID[15];
	char Kind[5];
	char Ver[5];

};

//所要HTTP发送的数据类型
typedef struct{
	struct Asleep *asleep;
	struct Sfile *sfile;
	struct Download *downlod;
	struct VerInfo *verinfo;
	int *date;
}http_data;

int http_send(http_data data);

/*
void asleep_str(char*);
void sfile_str(char*);
void download_str(char*);
void verInfo_str(char*);

void message_str(char*, char*);

int buildSocket(int domain, int type, int protocol);
int http_send(http_data data);
int send_data(http_data data);

int inttochar(int nnum, char *strfromint);

//补传机制
int read_send_data(char* file_path);
char* write_sfile(struct Sfile file);
char* write_asleep(struct Asleep sleep);
*/

#endif /* WIFICTL_DATA_SEND_CTL_H_ */


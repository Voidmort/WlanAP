/*
 * data_send.h
 *
 *  Created on: 2017��4��6��
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

	char uid[15];	//14��ΙC���ɣ�������
	char mdts[13];	//˯���_ʼ�Օr
	char dt[11];	//˯�߽����Օr
	char data[100];	//˯�߽����ǩ`����˯�߶��A�ǩ`����
	char age[3];		//���Y�����h
	char sex[2];//���Y�����h
	char wht[4];	//���Y������
	char cm[2];	//ͨ���O��
	char nba[2];	//����`���O��
	char sa[5];	//Ŀ�˾��ޕr��
	char ga[5];	//Ŀ���𴲕r��
	char zw[3];	//δʹ��
	char cgr[6];	//δʹ��
	char amnesia[9];	//�Jָ֪֢��D��A��

};

//sfile
struct Sfile{

	char uid[15];	//14��ΙC���ɣ�������
	char dt[13];	//�ǩ`��ȡ���Օr
	char rsi[4];	//δʹ��
	char myka[4];	//δʹ��
	char mykb[4];	//�}����
	char sdi[4];	//��Ӵ󤭤�
	char kk[4];		//������
	char nkk[3];	//�o�����r�g
	char ibk[3];	//���Ӥ��k���r�g
	char tdo[3];	//�R�����
	char hu[4];		//�����Пo
	char sl[4];		//˯���_ʼ�ե饰
	char al[4];		//����`��
	char err[11];	//�C�����Ʃ`����
	char ver[4];	//�ץ����Щ`�����

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

//��ҪHTTP���͵���������
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

//��������
int read_send_data(char* file_path);
char* write_sfile(struct Sfile file);
char* write_asleep(struct Asleep sleep);
*/

#endif /* WIFICTL_DATA_SEND_CTL_H_ */


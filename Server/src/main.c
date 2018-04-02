/*
 * main.c
 *
 *  Created on: 2018年1月19日
 *      Author: Void
 */
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
#include <string.h>
#include <fcntl.h>
/*
#include "wifi_control.h"
#include "data_send.h"
#include "uid.h"

struct Asleep asleep = {"18686000000248", "160830130000", "1608301437", "6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,4,6,3,3,3,3,2,1,5,5,6,7,7,7,7,7,7,7,7,7,7,", "20", "M", "060", "0", "0", "2000", "0700", "00", "00000", "D000A000"};
struct Sfile sfile = {"18686000000248", "201608311600", "000", "000", "070", "071", "014", "01", "00", "13", "HU1", "SL", "AL", "ER10CA0000", "133"};
struct Download download = {"SUB_0006", "1"};
struct VerInfo verInfo = {"18686000000248", "main", "0001"};

int main()
{
	http_data data = {NULL, NULL, NULL, NULL,"1"};
	char buf[64] = {};
	printf("helo word!\n");
	//wifi_ap_ctl();
	//strcat(data.date,"1");
	http_send(data);
	get_uid(buf);
	printf("%s\n",buf);
	return 0;
}

void get_current_minute(char *current_minute);

void get_current_minute1(char *current_minute)
{
       int i;
//       long int time_curr;
	char ltime[13];
	time_t lt;
	struct tm *ptr;
//	char *str;

//       i = sizeof(lt);

//	time_curr = time (&lt);
	time (&lt);
	ptr = localtime(&lt);
//	str = (char*)malloc(32);
	ptr->tm_min +=1;
	strftime(ltime, sizeof(ltime), "%G%m%d%H%M",ptr);
    for(i=0; i<12; i++)
    {
         current_minute[i] = ltime[i];
    }
	current_minute[12] = '\0';
//	printf("%s\n",current_minute);
//	sprintf(str,"%s",ltime);
}
void get_current_minute(char *current_minute)
{
       int i;
//       long int time_curr;
	char ltime[13];
	time_t lt;
	struct tm *ptr;
//	char *str;

//       i = sizeof(lt);

//	time_curr = time (&lt);
	time (&lt);
	ptr = localtime(&lt);
//	str = (char*)malloc(32);
	strftime(ltime, sizeof(ltime), "%G%m%d%H%M",ptr);
       for(i=0; i<12; i++)
       {
            current_minute[i] = ltime[i];
       }
	current_minute[12] = '\0';
//	printf("%s\n",current_minute);
//	sprintf(str,"%s",ltime);
}
*/

int inttochar(int nnum, char *strfromint)
{
	int i=0, k;
	char indexi[]="0123456789";
    char temp;

 /*转换*/
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

char inttochar1(int num)
{
	return num+48;
}

//通过串口发送数据
 void serial_send()
 {
	 static int i = 0;
	 char buf[16] = {};
	 static int sd;
	 memset(buf, 0, 16);

	 static int j = 0;
	 sprintf(buf, "[%d]\n", j++);
	 if(i == 0)
	 {
		 sd = open("/dev/ttyMFD2", O_WRONLY);
		 i++;
	 }
	 if(sd < 0)
	 {
		 perror("/dev/ttyMFD2 open()");
		 return ;
	 }
	 printf("write %s\n",buf);

	 write(sd, buf, 12);

	 return ;
 }
int main()
{
	/*
	char date_now[13] = {};
	get_current_minute(date_now);
	printf("%s\n",date_now);
	get_current_minute1(date_now);
	printf("%s\n",date_now);
	*/
	char arr[12];
	while(1)
	{
		serial_send();
		sleep(1);
	}
	return 0;
}

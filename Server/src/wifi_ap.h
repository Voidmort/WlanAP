/*
 * wifi_ap.h
 *
 *  Created on: 2017��12��28��
 *      Author: Void
 */

//hostapd �����ļ�
#define HOSTAPD_CONF "./hostapd.conf"

//wifi AP ģʽ������
int wifi_ap_ctl();

//��AP����
int wifi_ap();

//�����
char* server_main();

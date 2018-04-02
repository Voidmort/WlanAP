/*
 * wifi_ap.h
 *
 *  Created on: 2017年12月28日
 *      Author: Void
 */

//hostapd 配置文件
#define HOSTAPD_CONF "./hostapd.conf"

//wifi AP 模式控制器
int wifi_ap_ctl();

//打开AP功能
int wifi_ap();

//服务端
char* server_main();

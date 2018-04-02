WIFI AP模块：
	服务端：
		监听APP发送的数据 接受成功返回OK
		需要 启动 hostapd 服务
		修改hostapd.conf配置文件，写入wifi密码与名称，DHCP等
		接受到连接WIFI命令会关闭AP功能，接连接收到的WIFI名与密码
		如果一分钟没有设备连接则关闭AP功能，自动连接已保存的WIFI（如果有WIFI信号）
	
	客户端：
		当WIFI连接到设备则可以向设备发送WIFI名与密码
	
设备ID模块：
	uid.conf保存ID号
	get_uid()获取设备ID

数据发送：
	通过HTTP发送睡眠数据
	发送失败保存到本地
	网络正常再次发送
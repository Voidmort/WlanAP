if [ $# != 1 ] ; then
	echo "$0 <SSID>"
	exit
	fi
wpa_cli -iwlan0 disconnect
wpa_cli -iwlan0	remove_network all
wpa_cli -iwlan0	add_network
wpa_cli -iwlan0 add_network 0 mode 0
wpa_cli -iwlan0 add_network 0 ssid \"$1\"
wpa_cli -iwlan0 add_network 0 auth_alg OPEN
wpa_cli -iwlan0 add_network 0 key_mgmt NONE
wpa_cli -iwlan0 add_network 0 scan_ssid 1
wpa_cli -iwlan0	select_network 0
wpa_cli -iwlan0	enable_network 0
wpa_cli -iwlan0 reassociate
wpa_cli -iwlan0 status


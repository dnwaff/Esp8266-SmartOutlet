	#ifndef INCLUDE_CLIENT_H_
	#define INCLUDE_CLIENT_H_

	#include "wifi.h"

	//#define DNS_ENABLE
	#define SEC 1000
	#define NET_DOMAIN "api.parse.com"

    #define packet_size   (2 * 1024)

	os_timer_t reconn_timer;
    ip_addr_t tcp_server_ip;

    void ICACHE_FLASH_ATTR user_tcp_recv_cb(void *arg, char *pusrdata, unsigned short length);
    void user_tcp_sent_cb(void *arg);
    void user_tcp_discon_cb(void *arg);
    void user_tcp_connect_cb(void * arg);
    void user_tcp_recon_cb(void * arg, sint8 err);

    #endif

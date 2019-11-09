#include "wifi.h"


	void (*WifiCallback)(void) = NULL;

	/******************************************************************************
     * FunctionName : user_set_station_config
     * Description  : set the router info which ESP8266 station will connect to
     * Parameters   : none
     * Returns      : none
    *******************************************************************************/
    void ICACHE_FLASH_ATTR
    user_set_station_config(void (*gotIp_cb)(void))
    {
    	struct station_config st;
    	WifiCallback = gotIp_cb;
    	os_sprintf(st.ssid, "%s",SSID);
    	os_sprintf(st.password, "%s", PASSWORD);
    	ets_uart_printf("ssid: %s and password: %s \n",st.ssid,st.password);
    	wifi_station_set_config_current(&st);
    	wifi_station_set_auto_connect(1);
    	wifi_station_set_reconnect_policy(1);
    	wifi_station_connect();

       //set a timer to check whether got ip from router succeed or not.
        os_timer_disarm(&test_timer);
        os_timer_setfn(&test_timer, (os_timer_func_t *)user_check_ip, NULL);
        os_timer_arm(&test_timer, 1000, 0);

    }

    /******************************************************************************
     * FunctionName : user_check_ip
     * Description  : check whether get ip addr or not
     * Parameters   : none
     * Returns      : none
    *******************************************************************************/
    void ICACHE_FLASH_ATTR
    user_check_ip(void )
    {
    	struct ip_info ipConfig;
    	//disarm timer first
        os_timer_disarm(&test_timer);
    	switch(wifi_station_get_connect_status())
    	{
    		case STATION_GOT_IP:
    			if(ipConfig.ip.addr != 0) {
    			wifi_get_ip_info(STATION_IF, &ipConfig);
    			WifiCallback();
    			return;
    			}
    			break;
    		case STATION_WRONG_PASSWORD:

    			#ifdef PLATFORM_DEBUG
    			ets_uart_printf("WiFi connecting error, wrong password\r\n");
    			#endif
    			break;
    		case STATION_NO_AP_FOUND:

    			#ifdef PLATFORM_DEBUG
    			ets_uart_printf("WiFi connecting error, ap not found\r\n");
    			#endif
    			break;
    		case STATION_CONNECT_FAIL:

    			#ifdef PLATFORM_DEBUG
    			ets_uart_printf("WiFi connecting fail\r\n");
    			#endif
    			break;
    		default:

    			#ifdef PLATFORM_DEBUG
    			ets_uart_printf("WiFi connecting...\r\n");
    			#endif
    	}
                os_timer_setfn(&test_timer, (os_timer_func_t *)user_check_ip, NULL);
                os_timer_arm(&test_timer, 1000, 0);
   }

#include "wifi.h"
#include "client.h"
#include "commands.h"
#include "gpio.h"

#define RELAY1_MUX PERIPHS_IO_MUX_GPIO2_U
#define RELAY1_FUNC FUNC_GPIO2
#define RELAY1_PIN 2


#define	RELAY2_MUX PERIPHS_IO_MUX_GPIO0_U
#define RELAY2_PIN 0
#define RELAY2_FUNC FUNC_GPIO0


 void ICACHE_FLASH_ATTR
relay1(uint8 state)
{
    state &= 0x01;
    //Set SDA line to state
    if (state)
        gpio_output_set(1 << RELAY1_PIN, 0, 1 << RELAY1_PIN, 0);
    else
        gpio_output_set(0, 1 << RELAY1_PIN, 1 << RELAY1_PIN, 0);
}

/**
 * Set SCK to state
 */
 void ICACHE_FLASH_ATTR
relay2(uint8 state)
{
    //Set SCK line to state
    if (state)
        gpio_output_set(1 << RELAY2_PIN, 0, 1 << RELAY2_PIN, 0);
    else
        gpio_output_set(0, 1 << RELAY2_PIN, 1 << RELAY2_PIN, 0);
}

/**
 * I2C init function
 * This sets up the GPIO io
 */
void ICACHE_FLASH_ATTR
relay_init(void)
{
    //Disable interrupts
    ETS_GPIO_INTR_DISABLE();

    //Set pin functions
    PIN_FUNC_SELECT(RELAY1_MUX, RELAY1_FUNC);
    PIN_FUNC_SELECT(RELAY2_MUX, RELAY2_FUNC);

    //Set SDA as open drain
    GPIO_REG_WRITE(
        GPIO_PIN_ADDR(GPIO_ID_PIN(RELAY1_PIN)),
        GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(RELAY1_PIN))) |
        GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE)
    );

    GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | (1 << RELAY1_PIN));

    //Set SCK as open drain
    GPIO_REG_WRITE(
        GPIO_PIN_ADDR(GPIO_ID_PIN(RELAY2_PIN)),
        GPIO_REG_READ(GPIO_PIN_ADDR(GPIO_ID_PIN(RELAY2_PIN))) |
        GPIO_PIN_PAD_DRIVER_SET(GPIO_PAD_DRIVER_ENABLE)
    );

    GPIO_REG_WRITE(GPIO_ENABLE_ADDRESS, GPIO_REG_READ(GPIO_ENABLE_ADDRESS) | (1 << RELAY2_PIN));

    //Turn interrupt back on
    ETS_GPIO_INTR_ENABLE();

    relay1(0);
    relay2(0);
    return;
}




	bool ICACHE_FLASH_ATTR validate(char * data, unsigned short length){

		if (os_strcmp(data,"on1") == 0){
			relay1(1);
			ets_uart_printf("on1");
			return true;
		}
		else
			if (os_strcmp(data,"on2") == 0){
				relay2(1);
				ets_uart_printf("on2");
				return true;
			}
			else
				if (os_strcmp(data,"off1") == 0){
					relay1(0);
					ets_uart_printf("off1");
					return true;
				}
				else
					if (os_strcmp(data,"off2") == 0){
						relay2(0);
						ets_uart_printf("off2");
						return true;
					}
					else
					{
						ets_uart_printf("nah");
						return false;
					}
	}

    user_rf_pre_init(){

    }

    void task(os_event_t* e){


    	switch(e->sig){

    		case CLICK_RIGHT:
				break;

			case CLICK_LEFT:
				break;

			case CLICK_MIDDLE:
				break;

			default:
				ets_uart_printf("not a valid command \n");
				break;

    	}

    }

    void gotIp(void){
    	ets_uart_printf("got ip");
    }

    void ICACHE_FLASH_ATTR init_done(){
    	user_set_station_config(gotIp);
    }

    void uart_cb(void * string){

    	char* command = (char * ) string;
    	ets_uart_printf("message to int : %d \n", atoi(command));
    	system_os_post(USER_TASK_PRIO_0, atoi(command), 0);

    }

     void wifi_event_cb(System_Event_t *evt){
    	switch (evt->event){

    		case EVENT_SOFTAPMODE_STACONNECTED:

    			ets_uart_printf("station: " MACSTR "join, AID = %d\n",
    			MAC2STR(evt->event_info.sta_connected.mac),
    			evt->event_info.sta_connected.aid);

    		break;

    		case EVENT_SOFTAPMODE_STADISCONNECTED:

				ets_uart_printf("station disconnected : " MACSTR "join, AID = %d\n",
				MAC2STR(evt->event_info.sta_connected.mac),
				evt->event_info.sta_connected.aid);

			break;
    	}
    }



    /******************************************************************************
     * FunctionName : user_init
     * Description  : entry of user application, init user function here
     * Parameters   : none
     * Returns      : none
    *******************************************************************************/
    void user_init(void){

    	uart_init(BIT_RATE_115200, BIT_RATE_115200);
    	os_delay_us(1000000);


    	uartCbp = uart_cb;

        ets_uart_printf("SDK version: %s\n", system_get_sdk_version());
        ets_uart_printf("free heap size: %d\n", system_get_free_heap_size());

       //Set softAP + station mode
       //wifi_set_opmode(STATIONAP_MODE);

        wifi_set_opmode((wifi_get_opmode()|STATIONAP_MODE)&USE_WIFI_MODE);

    	struct espconn* server = (struct espconn*) os_zalloc(sizeof (struct espconn));
    	os_memset(server, 0 , sizeof(struct espconn));

    	//espconn_create(server);
    	server->type = ESPCONN_TCP;
    	server->state = ESPCONN_NONE;

    	server->proto.tcp = (esp_tcp* )os_zalloc(sizeof(esp_tcp));
    	server->proto.tcp->local_port =80;

    	espconn_regist_connectcb(server, user_tcp_connect_cb);

    	espconn_accept(server);

    	espconn_regist_time(server,150,0);

        wifi_station_dhcpc_stop();
 	    struct ip_info info;
        IP4_ADDR(&info.ip, 192, 168, 2, 100);
        IP4_ADDR(&info.netmask, 255, 255, 255, 0);
        IP4_ADDR(&info.gw, 192, 168, 2, 1);
        wifi_set_ip_info(STATION_IF, &info);

       wifi_set_event_handler_cb(wifi_event_cb);

       //ESP8266 connect to router
       // system_init_done_cb(init_done);
       system_init_done_cb(init_done);
       relay_init();

    }

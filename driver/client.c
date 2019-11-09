#include "client.h"

	unsigned char *default_certificate;
    unsigned int default_certificate_len = 0;
    unsigned char *default_private_key;
    unsigned int default_private_key_len = 0;




    /******************************************************************************
     * FunctionName : user_tcp_recv_cb
     * Description  : receive callback.
     * Parameters   : arg -- Additional argument to pass to the callback function
     * Returns      : none
    *******************************************************************************/
    void  user_tcp_recv_cb(void *arg, char *pusrdata, unsigned short length){

    	char s[] = "success";
		char f[] =  "failed";

       //received some data from tcp connection

    	struct espconn *pespconn = arg;

        ets_uart_printf("tcp recv !!! %s \r\n", pusrdata);

        if (validate(pusrdata, length) == true){
        	espconn_sent(pespconn,s,os_strlen(s));
        }
        else
        	espconn_sent(pespconn,f,os_strlen(f));
    }
    /******************************************************************************
     * FunctionName : user_tcp_sent_cb
     * Description  : data sent callback.
     * Parameters   : arg -- Additional argument to pass to the callback function
     * Returns      : none
    *******************************************************************************/
    void ICACHE_FLASH_ATTR
    user_tcp_sent_cb(void *arg){

       //data sent successfully
        ets_uart_printf("tcp sent succeed !!! \r\n");
    }
    /******************************************************************************
     * FunctionName : user_tcp_discon_cb
     * Description  : disconnect callback.
     * Parameters   : arg -- Additional argument to pass to the callback function
     * Returns      : none
    *******************************************************************************/
    void ICACHE_FLASH_ATTR
    user_tcp_discon_cb(void *arg){

       //tcp disconnect successfully

        ets_uart_printf("tcp disconnect succeed !!! \r\n");
    }

    /******************************************************************************
     * FunctionName : user_tcp_connect_cb
     * Description  : A new incoming tcp connection has been connected.
     * Parameters   : arg -- Additional argument to pass to the callback function
     * Returns      : none
    *******************************************************************************/
    void ICACHE_FLASH_ATTR
    user_tcp_connect_cb(void *arg){

        struct espconn *pespconn = arg;

        ets_uart_printf("connect succeed !!! \r\n");

        os_timer_disarm(&reconn_timer);

        espconn_regist_recvcb(pespconn, user_tcp_recv_cb);
        espconn_regist_sentcb(pespconn, user_tcp_sent_cb);
        espconn_regist_disconcb(pespconn, user_tcp_discon_cb);

        //user_sent_data(pespconn);
    }

    /******************************************************************************
     * FunctionName : user_tcp_recon_cb
     * Description  : reconnect callback, error occured in TCP connection.
     * Parameters   : arg -- Additional argument to pass to the callback function
     * Returns      : none
    *******************************************************************************/
    void ICACHE_FLASH_ATTR
    user_tcp_recon_cb(void *arg, sint8 err)
    {
    	os_timer_arm(&reconn_timer, 5*SEC , 1);

    	ets_uart_printf("reconnect callback, error code %d !!! \r\n",err);

    }

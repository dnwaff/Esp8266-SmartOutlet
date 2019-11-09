#include "osapi.h"
#include "timer.h"

    void null_function(void * arg){
    	ETSTimer * this = (ETSTimer *) arg;

    	ets_uart_printf("NULL \n");


    	destroy_timer(this);
    }

    void set_timer(ETSTimer* timer, int time , bool repeat, ETSTimerFunc* cb )
    {
    	ets_timer_disarm(timer);

    	if (cb == NULL)
    		os_timer_setfn(timer,(ETSTimerFunc*)null_function, timer);
    	else
    		os_timer_setfn(timer,(ETSTimerFunc *)cb, timer);

    	ets_timer_arm_new( timer, time, repeat , 1);
    }

    ETSTimer* create_timer(void){
    	ETSTimer * timer = (ETSTimer *)os_zalloc(sizeof(ETSTimer));
    	return  timer;
    }

    void destroy_timer(ETSTimer* timer){
    	ets_timer_disarm(timer);
    	os_free(timer);
    }

#ifndef TIMER_H
#define TIMER_H

void set_timer(ETSTimer* timer, int time , bool repeat, ETSTimerFunc* cb );
ETSTimer* create_timer(void);
void destroy_timer(ETSTimer* timer);

#endif

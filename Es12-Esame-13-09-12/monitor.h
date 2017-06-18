/*******Monitor.h**********/

#ifndef MONITOR_H
#define MONIOTR_H

typedef struct {
	int n_var_cond;
	
	int id_mutex;
	int id_var_cond;	
	int id_shm_cond_counts;
	
	int *cond_counts;			
	
}Monitor;

void init_monitor(Monitor *,int);
void destroy_monitor(Monitor *);

void enter_monitor(Monitor *);
void leave_monitor(Monitor *);

void wait_condition(Monitor *,int);
void signal_condition(Monitor *,int);
int  queue_condition(Monitor *,int);

#endif

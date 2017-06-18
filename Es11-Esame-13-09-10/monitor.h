#ifndef MONITOR_H
#define MONITOR_H



typedef struct {
	int id_mutex;
	
	int n_var_cond;
	
	int id_conds;
	
	int *cond_counts;
	int id_shm_cond_counts;
}Monitor;

void init_monitor(Monitor *,int);
void enter_monitor(Monitor *);
void leave_monitor(Monitor *);
void remove_monitor(Monitor *);
void wait_condition(Monitor *,int);
void signal_condition(Monitor *,int);
int  queue_condition(Monitor *,int);

#endif

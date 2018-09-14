#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "topology.h"
#include "sim_engine.h"

struct intlist {
  int val;
  struct intlist *next;
};

/*
 * activeflowhead[i][j]: the head of the list of active flows using link graph[i][j]
 * activeflowtail[i][j]: the tail of the list
 * ave_bw: average bandwidth on each link = bandwidth/load
 *         used in simulation to speed up rate calculation
 */

static struct intlist *activeflowhead[MAX_NODE][MAX_DEGREE];
static struct intlist *activeflowtail[MAX_NODE][MAX_DEGREE];
static long long int ave_bw[MAX_NODE][MAX_DEGREE]; /* bandwidth/load */

/* simulation related variables */

#define MAX_ACTIVE_FLOW 3000000
/*  a buffer of 3000000 used as circular array */

static long long active_start = 0;
static long long active_end = 0;
static long long start[MAX_ACTIVE_FLOW];
static long long lastupdatetime[MAX_ACTIVE_FLOW];
static unsigned short src[MAX_ACTIVE_FLOW];
static unsigned short dst[MAX_ACTIVE_FLOW];
static int size[MAX_ACTIVE_FLOW];
static int wsize[MAX_ACTIVE_FLOW];
static long long end[MAX_ACTIVE_FLOW];
static long long rate[MAX_ACTIVE_FLOW];
/* static char flag[MAX_ACTIVE_FLOW]; */
static char flag1[MAX_ACTIVE_FLOW];
static long long currentend[MAX_ACTIVE_FLOW];
static short int activepath[MAX_ACTIVE_FLOW][MAX_PATH_LEN];
static char hopcount[MAX_ACTIVE_FLOW];

/* managing af_count is a bit tricky
 * it is reset everything the rate is calculated
 */

int af_count = 0;
static int affectedflow[MAX_ACTIVE_FLOW * MAX_PATH_LEN+1];

static long long current_t =0;
static long long next_t=BIG_TIME;

static int num_entered = 0;
static int num_completed=0;

static void insert_flow(int node, int degree, int flow)
{
  struct intlist *tmp;

  tmp = (struct intlist *)malloc(sizeof (struct intlist));
  if (tmp == NULL) {
    printf("Memory allocation error.\n");
    exit(0);
  }
  tmp->val = flow;
  tmp->next = NULL;

  if (activeflowtail[node][degree] == NULL) {
    activeflowtail[node][degree] = activeflowhead[node][degree] = tmp;
  } else {
    activeflowtail[node][degree]->next = tmp;
    activeflowtail[node][degree] = tmp;
  }
}


static void delete_flow(int node, int degree, int flow)
{
  struct intlist *tmp, *tmp1;

  tmp = activeflowhead[node][degree];
  if (tmp == NULL) {
    printf("Something is wrong in delete_flow 0.\n");
    exit(0);
  } else if (tmp->val == flow) {
    if (activeflowhead[node][degree] == activeflowtail[node][degree]) {
      free(tmp);
      activeflowtail[node][degree] = activeflowhead[node][degree] = NULL;
    } else {
      activeflowhead[node][degree] = tmp->next;
      free(tmp);
    }
  } else {
    tmp1=tmp->next;
    while ((tmp1 != NULL) && (tmp1->val != flow)) {
      tmp = tmp1; tmp1 = tmp->next;
    }
    if (tmp1 == NULL) {
      printf("Something is wrong in delete_flow 1.\n");
      exit(0);
    }
    if (tmp1 == activeflowtail[node][degree]) {
      tmp->next = tmp1->next;
      free(tmp1);
      activeflowtail[node][degree] = tmp;
    } else {
      tmp->next = tmp1->next;
      free(tmp1);
    }
  }
}

/* this assumes that topology has been initialized */

void simulation_init() {
  int i, j;

  current_t = 0;
  next_t = BIG_TIME;
  active_start = active_end = 0;

  for (i=0; i<totNode; i++) {
    for (j=0; graph[i][j] != -1; j++) {
      activeflowhead[i][j] = activeflowtail[i][j] = NULL;
    }
  }  
  for (i=0; i<MAX_ACTIVE_FLOW; i++) end[i] = -1;
}
  
long long network_current_time() {
  return current_t;
}

long long network_next_time() {
  return next_t;
}

long long smallest_active_finishing_time()
{
  int i;
  long long t[128], tt;

  for (i=0; i<128; i++) t[i] = BIG_TIME;
  /* t = BIG_TIME; */

  {
    int ii = 0; 
    for (i=active_start; i<active_end; i++) {
      if (end[i] >=0) continue;
      if (currentend[i] < t[ii]) 
	t[ii] = currentend[i];
    }
  }

  tt = BIG_TIME;
  for (i=0; i<128; i++)
    if (tt > t[i]) tt = t[i];
  return tt;
}

/* flag1 is supposed to be 0 initialized before collecting affected flows
 * this routing assume the af_count is set correctly
 */
void collect_affected_flow(int src, int port) 
{
  struct intlist *t;
  if (graph[src][port] < 0) {
    printf("This isn't right.\n");
    exit(0);
  }
  t = activeflowhead[src][port];
  while (t != NULL) {
    if (flag1[t->val] == 0) {
      affectedflow[af_count++] = t->val;
      flag1[t->val] = 1; /* set the flag */
    }
    t = t->next;
  }
}

void clear_affected_flow_flag()
{
  int i;
  for (i=0; i<af_count; i++)
    flag1[affectedflow[i]] = 0;

  af_count = 0;
}

/* simulate one time step: advance network time from current_t to next_t
 * not optimized
 */
void simulate_one_step() 
{
  simulate_to_time_t(next_t);
}


/* simulate to time t */
void simulate_to_time_t(long long t) 
{
  int j, k;
  short unsigned int sss;
  int path[MAX_PATH_LEN];

  if (t<next_t) return;

  while (next_t <= t) {
    for (j=active_start; j<active_end; j++) {
      if ((end[j] < 0) && (currentend[j] <= next_t)) {
        wsize[j] = 0;
        end[j] = next_t + hopcount[j]*per_hop_latency + software_overhead;
	  {
          num_completed++;
	  }          
          for (k=0; k<MAX_PATH_LEN;k++) path[k] = activepath[j][k];
	  for (k=1, sss = path[0]; path[k] != -1; k++) {
	    {
	    load_graph[sss][path[k]] --;
	    if (load_graph[sss][path[k]] > 0) {
	      ave_bw[sss][path[k]] = bandwidth[sss][path[k]] / 
		load_graph[sss][path[k]];
            }
            delete_flow(sss, path[k], j);
            collect_affected_flow(sss, path[k]);
	    }
            sss = graph[sss][path[k]];
	  }
      }
    }
    
    while ((end[active_start] > 0) && (active_start != active_end)) 
      active_start = (active_start + 1) % MAX_ACTIVE_FLOW;     

    progress_affected_flow(next_t);
    calculate_rate_and_next_t();
    clear_affected_flow_flag();
  }
}

/* simulate to time t */
void simulate_to_time_t_with_no_rate_change(long long t) 
{
  int j, k;
  short unsigned int sss;
  int path[MAX_PATH_LEN];

  if (t == current_t) return;
  if (next_t < t) {
    printf("Inconsistent simulation.\n");
    exit(0);
  }

  for (j=active_start; j<active_end; j++) {
    if (end[j] < 0) {
      wsize[j] = wsize[j] - (long long)(((double)rate[j]/ONEG) * 
					(t - current_t));
      if (wsize[j] <= 0) {
	end[j] = t + hopcount[j]*per_hop_latency + software_overhead;
	{
        num_completed++;
	}
	for (k=0; k<MAX_PATH_LEN;k++) path[k] = activepath[j][k];
	/* routing_algorithm(src[j], dst[j], path); */
	for (k=1, sss=path[0]; path[k] != -1; k++) {
	  {
	  load_graph[sss][path[k]] --;
	  if (load_graph[sss][path[k]] > 0) {
	    ave_bw[sss][path[k]] = bandwidth[sss][path[k]] / 
	      load_graph[sss][path[k]];
	  }
	  }
          sss = graph[sss][path[k]];
	}
      }
    }
  }

  while ((end[active_start] > 0) && (active_start != active_end)) 
    active_start = (active_start + 1) % MAX_ACTIVE_FLOW;

  current_t = t;
  /* next_t = current_t + smallest_active_finishing_time(); 
   * next_t should not change too */
}

void add_a_new_active_flow_without_calculate_rate(unsigned short s, 
						  unsigned short d, 
						  unsigned int ss, 
						  long long t)
{
  int k, kk;
  int path[MAX_PATH_LEN];

  /*
  if (t != current_t) {
    printf("This is incorrect.\n");
    exit(0);
  }
  */


  num_entered ++;
  src[active_end] = s;
  dst[active_end] = d;
  size[active_end] = ss;
  wsize[active_end] = ss;
  start[active_end] = t;
  end[active_end] = -1;
  lastupdatetime[active_end] = t;

  if (s == d) {  
    end[active_end] = t;
    active_end = (active_end+1) % MAX_ACTIVE_FLOW;
    if (active_end == active_start) {
      printf("MAX_ACTIVE_FLOW %d too small. Exit.\n", MAX_ACTIVE_FLOW);
    }
    return;
  }

  routing_algorithm(s, d, path);

  hopcount[active_end] = 0;
  for (k=2; path[k] != -1; k++) hopcount[active_end]++;

  for (k=0; k<MAX_PATH_LEN; k++) 
    activepath[active_end][k] = path[k];

  for (k=0; path[k+1] != -1; k++) {
    for (kk=0; (graph[path[k]][kk] != -1) 
	   && (graph[path[k]][kk] != path[k+1]); kk++);
    if (graph[path[k]][kk] != path[k+1]) {
      printf("Something wrong.\n");
      exit(0);
    }
    activepath[active_end][k+1] = kk;
    insert_flow(path[k], kk, active_end);
    load_graph[path[k]][kk] ++;
    collect_affected_flow(path[k], kk);

    ave_bw[path[k]][kk] = bandwidth[path[k]][kk] / 
      load_graph[path[k]][kk];
  }   

  active_end = (active_end+1) % MAX_ACTIVE_FLOW;
  if (active_end == active_start) {
    printf("MAX_ACTIVE_FLOW %d too small. Exit.\n", MAX_ACTIVE_FLOW);
  }
  current_t = t;
  
  progress_affected_flow(t);
}

void progress_affected_flow(long long t)
{
  int i, j;

  for (i=0; i<af_count; i++) {
    j = affectedflow[i];
    if ((end[j] <0)) {
      wsize[j] = wsize[j] - (long long)(((double)rate[j]/ONEG) * 
					(t - lastupdatetime[j]));
      lastupdatetime[j] = t;
    }
  }
}

void calculate_rate_and_next_t(long long t)
{

  int i, j, k;
  long long min_load_rate;
  short unsigned int sss;

  for (i=0; i<af_count; i++) {
    j = affectedflow[i];
    if ((end[j] <0)) {
      min_load_rate = BIG_RATE;
      
      for (k=1, sss= activepath[j][0]; activepath[j][k] != -1; k++) {
	short int ttt = activepath[j][k];
        long long int myrate = ave_bw[sss][ttt];
	if (myrate < min_load_rate) {
	  min_load_rate = myrate;
	}
        sss = graph[sss][ttt];
      }
      rate[j] = min_load_rate;
      currentend[j] = lastupdatetime[j] + 
	(long long) ((wsize[j] / (double)rate[j])*ONEG) + 1;
      
    }
  }

  next_t = smallest_active_finishing_time();
}

void output_simulation_result(FILE *fd)
{
  static int old_active_start = 0;
  if (active_start == old_active_start) return;

  while (old_active_start != active_start) {
    fprintf(fd, "%d %d %d %lld %lld\n", src[old_active_start],
	    dst[old_active_start], size[old_active_start],
	    start[old_active_start], end[old_active_start]);
    old_active_start = (old_active_start + 1) % MAX_ACTIVE_FLOW;
  };
  fflush(0);
}

void simulation_status(int *entered, int *completed, int *inprogress)
{
  while (end[active_start] >= 0) {
    active_start = (active_start +1) % MAX_ACTIVE_FLOW;
  }

  *entered = num_entered;
  *completed = num_completed;
  *inprogress = ((active_end + MAX_ACTIVE_FLOW) - active_start) % 
    MAX_ACTIVE_FLOW;
}

/* this is a sample application of the simulator interface
 * should not use any detailed information
 * should only use the simulator public interface
 */
void simulate_from_trace_file(char *filename)
{
  FILE *fd, *ofd;
  int next_s, next_d, next_ss;
  long long next_st;
  long long int tt, t;  
  int i;
  int enter=0, complete=0, inprogress=0;
  int old_enter=0, old_complete=0, old_inprogress=0;


  if ((fd = fopen(filename, "r")) == NULL) {
    printf("file %s does not exist.\n", filename);
    exit(0);
  }

  if ((ofd = fopen("sim_result", "w")) == NULL) {
    printf("Can't open sim_result for write.\n");
    exit(0);
  }

  i = fscanf(fd, "%d %d %d %lld", &next_s, &next_d, &next_ss, &next_st);

  while ((next_s != -1) || (active_start != active_end)) {
  
    simulation_status(&enter, &complete, & inprogress);
    if (enter + complete > old_enter+old_complete+100000) {
      printf("entered: %d, complete: %d, inprogress: %d\n", enter, 
	     complete, inprogress);fflush(0);
      old_enter = enter; old_complete = complete; old_inprogress = inprogress;
      i=system("date");
    }
    /* if (enter > 10000) exit(0); */
    if ((next_s == -1) && (active_start == active_end)) {
      output_simulation_result(ofd);
      break;
    }

    tt = network_next_time();
    if ((next_s != -1) && (tt > next_st)) {
      t = next_st;
      while(t == next_st) {
	add_a_new_active_flow_without_calculate_rate(next_s, next_d,
						     next_ss, next_st);
	/* get next flow */
	i = fscanf(fd, "%d %d %d %lld", &next_s, &next_d, &next_ss, &next_st);
      } 
      calculate_rate_and_next_t(next_st);
      clear_affected_flow_flag();
    } else
      simulate_to_time_t(tt);
      
    output_simulation_result(ofd);
  }
  fclose(fd);
  fclose(ofd);
}



#ifndef SIM_ENGINE_H
#define SIM_ENGINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "topology.h"

#define BIG_TIME 1000000000000000
#define BIG_RATE 1000000000000000
#define ONEG 1000000000

long long network_current_time();
long long network_next_time();
long long smallest_active_finishing_time(); /* omp code */
void simulation_init();
void simulate_one_step(); /* omp code */
void simulate_to_time_t(long long t); /* omp code */
void simulate_to_time_t_with_no_rate_change(long long t); /* omp code */
void add_a_new_active_flow(unsigned short s, unsigned short d, 
			   unsigned int ss, long long t); 
void add_a_new_active_flow_without_calculate_rate(unsigned short s, 
						  unsigned short d, 
						  unsigned int ss, 
						  long long t);
void calculate_rate_and_next_t();
void output_simulation_result(FILE *fd);
void simulation_status(int *entered, int *completed, int *inprogress);
void simulate_from_trace_file(char *filename);
void progress_affected_flow(long long t);

#endif



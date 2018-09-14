/*
 * This file defines the common constants and extern variables 
 * for the topology
 */

#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODE 65537
#define MAX_DEGREE 49
#define MAX_NPROCS 32769
#define TRUE 1
#define FALSE 0

#ifdef _TORUS
#define MAX_PATH_LEN 64
#else
#define MAX_PATH_LEN 20
#endif

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif

/*
 * totNode: total number of nodes (PE+switches in the network)
 * totPE: total number of processing nodes (PEs)
 * nprocs = totPE
 *
 * graph:      network connectivity, set at initialization
 * bandwidth:  the bandwidth on each link, set at initialization
 * load_graph: the current link load, used in simulation to keep track
 *             of link load changes, all adaptive routing adapts based on 
 *             the value in this array.
 */

extern int totNode;
extern int totPE;
extern int totSE;
extern int nprocs;

extern int per_hop_latency;
extern int software_overhead;

/* node i connects to node graph[i][j] */
extern int graph[MAX_NODE][MAX_DEGREE]; 
extern long long int bandwidth[MAX_NODE][MAX_DEGREE]; 
/* // bandwidth from i to graphp[i][j] */
extern int load_graph[MAX_NODE][MAX_DEGREE];

/* define the routing scheme */

#define XGFT_DMODK_ROUTING    1
#define XGFT_RANDOM_ROUTING   2
#define XGFT_ADAPTIVE_ROUTING 3
#define DRAGONFLY_MIN_ROUTING 4
#define DRAGONFLY_VAL_ROUTING 5
#define DRAGONFLY_GLOBALADAPTIVE_ROUTING 6
#define DRAGONFLY_MIN1_ROUTING 11
#define DRAGONFLY_VAL1_ROUTING 12
#define DRAGONFLY_GLOBALADAPTIVE1_ROUTING 13
#define HCUBE_DIMENSIONORDER_ROUTING 7
#define HCUBE_LOCALADAPTIVE_ROUTING 8
#define TORUS_DIMENSIONORDER_ROUTING 9
#define TORUS_LOCALADAPTIVE_ROUTING 10

extern int routing;   
void (* routing_algorithm)(int src, int dst, int *path);


/* xgft related routines */

void xgft_build_topology(int h, int *M, int *W, long long int *BW);
void xgft_print_topology();
void xgft_dmodk_routing(int src, int dst, int *path);
void xgft_random_routing(int src, int dst, int *path);
void xgft_adaptive_routing(int src, int dst, int *path);
void (* xgft_routing_algorithm)(int src, int dst, int *path);
int  xgft_check_path(int *path);
void xgft_print_path(int *path);
void xgft_topology_init(int h, int *m, int *w, long long int *bw, int r);

#endif



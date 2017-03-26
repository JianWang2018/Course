#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "topology.h"
#include "sim_engine.h"

int totNode;
int totSE;
int totPE;
int nprocs;

int per_hop_latency;
int software_overhead;

int graph[MAX_NODE][MAX_DEGREE]; 
long long int bandwidth[MAX_NODE][MAX_DEGREE]; 
int load_graph[MAX_NODE][MAX_DEGREE];
int routing;

int main(int argc, char *argv[]) 
{

  int i;
  if (argc < 2) {
    printf("Usage: \n  ./a.out xgft h m1 m2 ... mh w1 w2 ...wh bw1 bw2 ... bwh sw_l sw_o routing traffic_file\n");
    printf("  ./a.out dragonfly p a h g bw_p bw_a bw_h routing sw_l sw_o traffic_file\n");
    printf("  ./a.out torus Num_Dimension Num_Terminals_Per_Switch \
    a1 a2 ... an bw0 bw1 bw2... bwn routing sw_l sw_o traffic_file\n");
    printf("  ./a.out hcube num_dimension num_term_per_switch \
    bw0 bw1 bw2 ... bwn routing sw_l sw_o traffic_file\n");
    exit(0);
  }

  if (strcmp(argv[1], "xgft") == 0) {
#define MAX_H 10
    int h;
    int M[MAX_H];
    int W[MAX_H];
    long long int BW[MAX_H];

    h = atoi(argv[2]);

    if (argc != 3*h+7) {
      printf("Usage: ./a.out xgft h m1 m2 ... mh w1 w2 ...wh bw1 bw2 ... bwh routing sw_l sw_o traffic_file\n");
      exit(0);
    }

    for (i=0; i<h; i++) M[i] = atoi(argv[2+i+1]);
    for (i=0; i<h; i++) W[i] = atoi(argv[2+h+i+1]);
    for (i=0; i<h; i++) BW[i] = atoi(argv[2+2*h+i+1])*((long long int)1000000);
  
    if (strcmp(argv[2+3*h+1], "DMODK") == 0) routing = XGFT_DMODK_ROUTING;
    else if (strcmp(argv[2+3*h+1], "RANDOM") == 0) routing = XGFT_RANDOM_ROUTING;
    else if (strcmp(argv[2+3*h+1], "ADAPTIVE") == 0) routing = XGFT_ADAPTIVE_ROUTING
;
    else {
      printf("Routing scheme %s not supported on xgft.\n", argv[2+3*h]);
      exit(0);
    }

    per_hop_latency = atoi(argv[3+3*h+1]);
    software_overhead = atoi(argv[3+3*h+2]);

    srand(12481);
    xgft_topology_init(h, M, W, BW, routing);
    simulation_init();
    simulate_from_trace_file(argv[3+3*h+3]);
    printf("\nSimulation done. Output in file sim_result.\n");
  }else{
    printf("Error : Topology is not supported.\n\n");
    printf("Currently supported topologies are:\n");
    printf("Extended Generalized Fat Tree : xgft\n");
    exit(0);
  }
			     
  return 0;
}



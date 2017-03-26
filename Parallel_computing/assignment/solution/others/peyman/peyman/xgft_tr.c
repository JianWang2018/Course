#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "topology.h"

#define MAX_H 4

static int xgft_h;
static int xgft_M[MAX_H];
static int xgft_W[MAX_H];
static long long int xgft_BW[MAX_H];  /* bandwidth in each layer */
                                      /* in the unit of bps */
static int baseL[MAX_H+1];
static int sizeL[MAX_H+1];

/* some local function */
static void compute_baseL(int h, int *M, int *W);
static void compute_label(int node, int label[MAX_H+1]);
static void copy_label(int *dst, int *src);
static void print_label(int label[MAX_H+1]);

static void copy_label(int *dst, int *src) 
{
  int i;
  int h = xgft_h;
  for (i=0; i<=h; i++) {
    dst[i] = src[i];
  }
}

/* compute_baseL computes the base index for each layer of nodes
// L[0] are processing nodes while L[i] are switches
*/

static void compute_baseL(int h, int *M, int *W)
{
  int i, j;
  int tmp;

  for (i=0; i<=h; i++) {
    tmp = 1;
    for(j=0; j<i; j++) {
      tmp *= W[j];
    }
    for (j=i; j<h; j++)
      tmp *=M[j];
    sizeL[i] = tmp;
  }

  tmp = 0;
  for (i=0; i<=h; i++) {
    baseL[i] = tmp;
    tmp += sizeL[i];
  }
  totPE = sizeL[0];
  totNode = baseL[h] + sizeL[h];
  nprocs = totPE;
}

static void compute_label(int nodeid, int label[MAX_H+1])
{
  int i;
  int localid;  
  int K[MAX_H];
  int h;

  h = xgft_h;

  if ((nodeid < 0) || (nodeid > baseL[h] + sizeL[h])) {
    printf("Nodeid %d out of bound.\n", nodeid);
    exit(0);
  }

  for (i=0; i<=h; i++) {
    if (baseL[i] > nodeid) break;
  }
  label[0] = i-1;  /* determine the level */
  
  localid = nodeid - baseL[label[0]];

  for (i=0; i<h; i++) {
    if (i<label[0]) K[i] = xgft_W[i];
    else K[i] = xgft_M[i]; 
  }

  /* compute the encoding of localid */
  for (i=0; i<h; i++) {
    label[i+1] = localid % K[i];
    localid = localid / K[i];
  }
}

static void compute_nodeid(int * nodeid, int label[MAX_H+1])
{
  int i, j;
  int localid;  
  int K[MAX_H];
  int h;

  h = xgft_h;

  if ((label[0] < 0)  || (label[0] > h)) {
    printf("level %d does not exist.\n", label[0]);
    exit(0);
  }

  for (i=0; i<h; i++) {
    if (i<label[0]) K[i] = xgft_W[i];
    else K[i] = xgft_M[i]; 
  }

  for (i=0; i<h; i++) {
    if (label[i+1] > K[i]) {
      printf("label[%d] = %d out of bound %d\n", i+1, label[i+1], K[i]);
      for (j=0; j<=h; j++) {
	printf("label[%d] = %d\n", j, label[j]);
      }
      for (j=0; j<h; j++) {
	printf("K[%d] = %d\n", j, K[j]);
      }
      exit(0);
    }
  }

  localid = label[h];

  for (i=h-1; i>=1; i--) {
    localid = localid * K[i-1] + label[i];
  }

  *nodeid = localid + baseL[label[0]];
}

/*
// build_xgft builds the extended generalized fat-tree
*/
void xgft_build_topology(int h, int *M, int *W, long long int *BW)
{
  int i, j, k;
  int nodeid;
  int label[MAX_H+1];
  int code[MAX_H+1];


  for (i=0; i<totNode; i++) 
    for (j=0; j<MAX_DEGREE; j++) {
      graph[i][j] = -1;
    }

  for (i=0; i<totNode; i++) {
    /* compute connection for each node */
    compute_label(i, label);
    /* up first then down */
    j = 0;
    if (label[0] != h) { /* can go up */
      for (; j<W[label[0]]; j++) {
        copy_label(code, label);
	/*        print_label(label); printf("xxxxx\n"); */
        code[code[0]+1] = j;
	code[0] ++;
	/*        print_label(code); printf("yyyy\n"); */
	compute_nodeid(&nodeid, code);
	graph[i][j] = nodeid;
        bandwidth[i][j] = BW[label[0]];
      }
    }

    k=0;
    if (label[0] != 0) {
      for (; k<M[label[0]-1]; k++) {
        copy_label(code, label);
        code[0] --;
        code[code[0]+1] = k;
	/*        print_label(code); printf("zzzzz\n"); */
	compute_nodeid(&nodeid, code);
	graph[i][j+k] = nodeid;
        bandwidth[i][j+k] = BW[code[0]];
      }
    }
  }
}

static void print_label(int label[MAX_H+1])
{
  int i;
  int h;
  
  h = xgft_h;
  printf("(");
  for (i=0; i<h; i++) printf("%d, ", label[i]);
  printf("%d)", label[i]);
}

void xgft_print_topology()
{
  int i, j;
  int label[MAX_H+1];

  for (i=0; i<totNode; i++) {
    compute_label(i, label);
    printf("Node %d, ", i);
    print_label(label);
    printf(", connects to\n");
    for (j=0; graph[i][j] != -1; j++) {
      printf("  node %d (bw=%lld), ", graph[i][j], bandwidth[i][j]);
      compute_label(graph[i][j], label);
      print_label(label);
      printf("\n");
    }
  }
}

void xgft_print_path(int *path)
{
  int i = 0;
  int label[MAX_H+1];
  while (path[i] != -1) {
    compute_label(path[i], label);
    printf("node %d, ", path[i]);
    print_label(label);
    printf("\n");
    i++;
  }
}

void xgft_dmodk_routing(int src, int dst, int *path)
{
  int src_label[MAX_H+1];
  int dst_label[MAX_H+1];
  int r_label[MAX_H+1];
  int i;
  int high;  
  int tdst;
  int h;

  h = xgft_h;

  compute_label(src, src_label);
  compute_label(dst, dst_label);

  if (src_label[0] != 0) {
    printf("Node %d, ", src);
    print_label(src_label);
    printf(" is not a processing node.\n");
    exit(0);
  }

  if (dst_label[0] != 0) {
    printf("Node %d, ", dst);
    print_label(dst_label);
    printf(" is not a processing node.\n");
    exit(0);
  }
  if (src == dst){
    path[0] = src;
    path[1] = -1;
    return;
  }

  high = h;
  tdst = dst;
  while (src_label[high] == dst_label[high]) high --;
  if (high <= 0) { 
    printf("This can't be right.\n");
    exit(0);
  } else {
    /* printf("high = %d\n", high); */
    path[0] = src;
    copy_label(r_label, src_label);
    r_label[0] = 1;
    r_label[1] = 0;
    compute_nodeid(&path[1], r_label);
    /* up */
    for (i=1; i<high; i++) {
      /*      r_label[i+1] = dst_label[i] % W[i];  */
      r_label[i+1] = tdst % xgft_W[i]; 
      tdst = tdst / xgft_W[i];
      r_label[0] ++;
      compute_nodeid(&path[i+1], r_label);
    }
    /* down path */
    for (i = high -1; i>=0; i--) {
      r_label[i+1] = dst_label[i+1];
      r_label[0] --;
      compute_nodeid(&path[high-1 - i+high+1], r_label);
    }
    path[high-1-i+high+1] = -1;
  }
  /*  print_path(path);
  //  if (check_path(path)) {
    //  printf("Path exists.\n");
  //  }
  */
}

void xgft_random_routing(int src, int dst, int *path)
{
  int src_label[MAX_H+1];
  int dst_label[MAX_H+1];
  int r_label[MAX_H+1];
  int i;
  int high;  
  int tdst;
  int h;

  h = xgft_h;

  compute_label(src, src_label);
  compute_label(dst, dst_label);

  if (src_label[0] != 0) {
    printf("Node %d, ", src);
    print_label(src_label);
    printf(" is not a processing node.\n");
    exit(0);
  }

  if (dst_label[0] != 0) {
    printf("Node %d, ", dst);
    print_label(dst_label);
    printf(" is not a processing node.\n");
    exit(0);
  }
  if (src == dst){
    path[0] = src;
    path[1] = -1;
    return;
  }

  high = h;
  tdst = dst;
  while (src_label[high] == dst_label[high]) high --;
  if (high <= 0) { 
    printf("This can't be right.\n");
    exit(0);
  } else {
    /* printf("high = %d\n", high); */
    path[0] = src;
    copy_label(r_label, src_label);
    r_label[0] = 1;
    r_label[1] = 0;
    compute_nodeid(&path[1], r_label);
    /* up */
    for (i=1; i<high; i++) {
      /*      r_label[i+1] = dst_label[i] % W[i];  */
      r_label[i+1] = rand() % xgft_W[i]; 
      /*      tdst = tdst / W[i]; */
      r_label[0] ++;
      compute_nodeid(&path[i+1], r_label);
    }
    /* down path */
    for (i = high -1; i>=0; i--) {
      r_label[i+1] = dst_label[i+1];
      r_label[0] --;
      compute_nodeid(&path[high-1 - i+high+1], r_label);
    }
    path[high-1-i+high+1] = -1;
  }
  /*  print_path(path);
  //  if (check_path(path)) {
    //  printf("Path exists.\n");
  //  }
  */
}

/*
 // adaptive_routing computes the route based on the metrices stored
 // on load_graph
 */

void xgft_adaptive_routing(int src, int dst, int *path)
{
  int src_label[MAX_H+1];
  int dst_label[MAX_H+1];
  int r_label[MAX_H+1];
  int smallest_a[MAX_DEGREE+1];
  int smallest;
  int s_c;
  int i, j;
  int high;  
  int tdst;
  int h;

  h = xgft_h;

  compute_label(src, src_label);
  compute_label(dst, dst_label);

  if (src_label[0] != 0) {
    printf("Node %d, ", src);
    print_label(src_label);
    printf(" is not a processing node.\n");
    exit(0);
  }

  if (dst_label[0] != 0) {
    printf("Node %d, ", dst);
    print_label(dst_label);
    printf(" is not a processing node.\n");
    exit(0);
  }
  if (src == dst){
    path[0] = src;
    path[1] = -1;
    return;
  }

  high = h;
  tdst = dst;
  while (src_label[high] == dst_label[high]) high --;
  if (high <= 0) { 
    printf("This can't be right.\n");
    exit(0);
  } else {
    /*    printf("high = %d\n", high); */
    path[0] = src;

    copy_label(r_label, src_label);
    r_label[0] = 1;
    r_label[1] = 0;

    compute_nodeid(&path[1], r_label);
    /* up */

    for (i=1; i<high; i++) {

      smallest = 1000000;
      for (j=0; j<xgft_W[i]; j++) 
	if (load_graph[path[i]][j] < smallest) smallest = load_graph[path[i]][j];


      s_c = 0; 
      for (j=0; j<xgft_W[i]; j++) 
        if (load_graph[path[i]][j] == smallest) {
	  smallest_a[s_c] = j;
	  s_c++;
	}

      if (s_c == 0) { 
	printf("This cannot be right.\n");
	exit(0);
      }
      
      /*      r_label[i+1] = dst_label[i] % W[i];  */
      r_label[i+1] = smallest_a[rand() % s_c]; 
      /* r_label[i+1] = smallest_a[0]; 
      //      graph_r[path[i]][r_label[i+1]] ++;   */
      /* can't update the load graph, should change the load when others
         are set-up
      */
      /*      tdst = tdst / W[i]; */
      r_label[0] ++;
      compute_nodeid(&path[i+1], r_label);
    }
    /* down path */
    for (i = high -1; i>=0; i--) {
      r_label[i+1] = dst_label[i+1];
      r_label[0] --;
      compute_nodeid(&path[high-1 - i+high+1], r_label);
    }
    path[high-1-i+high+1] = -1;
  }
  /*
  //  print_path(path);
  if (check_path(path)) {
    //  printf("Path exists.\n");
  }     */

}


int xgft_check_path(int *path)
{
  int i, j;
  for (i=0; path[i+1] != -1; i++) {
    for (j=0; graph[path[i]][j] != -1; j++)
      if (graph[path[i]][j] == path[i+1]) break;
    if (graph[path[i]][j] == -1) {
      xgft_print_path(path);
      printf(" does not exist!!");
      exit(0);
      return FALSE;
    }
  }
  return TRUE;
}

void xgft_topology_init(int h, int *m, int *w, long long int *bw, int r) 
{
  int i, j;

  xgft_h = h;
  if (MAX_H < xgft_h) {
    printf("MAX_H %d is too small, less than xgft_h %d\n", MAX_H, xgft_h);
    exit(0);
  }

  for(i=0; i<xgft_h; i++) {
    xgft_M[i] = m[i];
    xgft_W[i] = w[i];
    xgft_BW[i] = bw[i];
  }
  compute_baseL(h, m, w);
  xgft_build_topology(h, m, w, bw);

  /* reset the load graph when topology is initialized */
  for (i=0; i<totNode; i++) {
    for (j=0; graph[i][j] != -1; j++)
      load_graph[i][j] = 0;
  }

  printf("Simulating XGFT with parameters: height = %d\n  m = [", h);
  for (i=0; i<h-1; i++) printf("%d, ", m[i]);
  printf("%d]\n  w = [", m[i]);

  for (i=0; i<h-1; i++) printf("%d, ", w[i]);
  printf("%d]\n  bw = [", w[i]);

  for (i=0; i<h-1; i++) printf("%lld, ", bw[i]);
  printf("%lld]\n", bw[i]);

  for (i=0; i<=h; i++) {
    printf("sizeL[%d] = %d\n", i, sizeL[i]);
  }
  for (i=0; i<=h; i++) {
    printf("baseL[%d] = %d\n", i, baseL[i]);
  }

  printf("%d PEs, %d switches, %d nodes (switch+PE)\n", 
	 totPE, totNode-totPE, totNode);
  
  routing = r;
  if (r == XGFT_DMODK_ROUTING) {
    routing_algorithm = xgft_routing_algorithm = xgft_dmodk_routing;
    printf("Routing algorithm: Destination mod K routing\n");
  } else if (r == XGFT_RANDOM_ROUTING) {
    routing_algorithm = xgft_routing_algorithm = xgft_random_routing;
    printf("Routing algorithm: Random routing\n");
  } else if (r == XGFT_ADAPTIVE_ROUTING) {
    routing_algorithm = xgft_routing_algorithm = xgft_adaptive_routing;
    printf("Routing algorithm: Adaptive routing\n");
  } else {
    printf("Routing scheme %d not supported on xgft.\n", routing);
    exit(0);
  }

  if (DEBUG_LEVEL > 0) {
    xgft_print_topology();
  }  
}
  



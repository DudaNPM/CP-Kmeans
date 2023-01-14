#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

typedef struct Point {
  float x;  // x coord
  float y;  // y coord
} Point;


typedef struct Cluster {
  int size;       // cluster size
  Point pos;      // centroid position
  Point * points; // points that belong to the cluster
} Cluster;

int N;  // number of samples
int K;  // number of clusters
int T;  // number of threads

Point * points;       // N points coordinates
Cluster * clusters;   // Simulates an matrix[T][K] but contiguous in memory
                      // row major ordering -> int offset = i * cols + j;


#endif
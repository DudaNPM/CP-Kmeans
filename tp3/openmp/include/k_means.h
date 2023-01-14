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
} Cluster;

int N;  // number of samples
int K;  // number of clusters
int T;  // number of threads

Point * points;       // N points coordinates
Cluster * clusters;   // K clusters

float * sum_x;
float * sum_y;
int * size;

#endif
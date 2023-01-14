#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 10000000
#define K 4


typedef struct Point {
  float x;  // x coord
  float y;  // y coord
} Point;


typedef struct Cluster {
  int size;       // cluster size
  Point pos;      // centroid position
  Point old_pos;  // last centroid position
  Point * points; // points that belong to the cluster
} Cluster;


Point * points;      // N points coordinates
Cluster clusters[K]; // K clusters

#endif
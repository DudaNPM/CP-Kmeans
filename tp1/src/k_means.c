#include "../include/k_means.h"


// Strategy:
    // -for each point calculate closest cluster
    // -add point to points array of the cluster
    // -calculate centroid based on each point array of the clusters
    // -store previous centroid position and the new one
    // -check if the centroid position changed
    // -repeat if yes


// Final print

void print_info(int iterations)  {
    printf("N = %d, K = %d\n", N, K);
    for(int i = 0; i < K; i++)
        printf("Center: (%.3f, %.3f) : Size: %d\n", clusters[i].pos.x, clusters[i].pos.y, clusters[i].size);
    printf("Iterations: %d\n", iterations);
}


// Initialization: N points and K clusters

void initialize() {
    points = (Point *) malloc(N*sizeof(Point));

    srand(10);
    for(register int i = 0; i < N; i++) {
        points[i].x = (float) rand() / RAND_MAX;
        points[i].y = (float) rand() / RAND_MAX;
    }
    for(register int i = 0; i < K; i++) {
        clusters[i].points = (Point *) malloc(N*sizeof(Point));
        clusters[i].pos.x = points[i].x;
        clusters[i].pos.y = points[i].y;
    }
}


// Euclidean distance between 2 points
// (no need to use sqrt, we dont need the right dist)

float dist(Point a, Point b) {
	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}


// Associates all N points to the respective clusters
// Calculates the closest cluster to a Point p
// Adds Point p to Cluster points array
// Inc Cluster size

// The problem is here -> 87.55% Overhead
void cluster_points() {
    for (register int i = 0; i < N; i++) {

        register float lowest_dist = dist(points[i],clusters[0].pos);
        register int cluster_id = 0;

        for (register int j = 1; j < K; j++) {
            register float distance = dist(points[i],clusters[j].pos);

            if (distance < lowest_dist) {
                lowest_dist = distance;
                cluster_id = j;
            }
        }

        clusters[cluster_id].points[clusters[cluster_id].size] = points[i];
        clusters[cluster_id].size++;
    }
}


// Calculates centroid based on the Cluster points

void reevaluate_centers() {
    for (register int i = 0; i < K; i++) {
        register float x = 0;
        register float y = 0;
        
        // vectorizable
        for (register int j = 0; j < clusters[i].size; j++) {
            x += clusters[i].points[j].x;
            y += clusters[i].points[j].y;
        }
        
        clusters[i].pos.x = x / (float) clusters[i].size;
        clusters[i].pos.y = y / (float) clusters[i].size;
    }
}


// Checks if the solution has converged
// Verification: cluster has changed its position or not ?

int has_converged() {
    for(register int i = 0; i < K; i++) {
        if (!(clusters[i].old_pos.x == clusters[i].pos.x && clusters[i].old_pos.y == clusters[i].pos.y))
            return 0;
    }
    return 1;
}


// Updates previous position of the Cluster centroid
// Size returns to 0

void update_old_pos_clusters() {
    for (register int i = 0; i < K; i++) {
        clusters[i].old_pos.x = clusters[i].pos.x;
        clusters[i].old_pos.y = clusters[i].pos.y;
        clusters[i].size = 0;
    }
}


// Main

int main() {
    register int iterations = -1; // initial iteration is inside the while
    initialize();

    while (!has_converged()) {
        update_old_pos_clusters();  // Update previous position of the clusters centroid
        cluster_points();           // Associate points to clusters
        reevaluate_centers();       // Calculate new centroids
        iterations++;
    }
    
    print_info(iterations);
    return 0;
}


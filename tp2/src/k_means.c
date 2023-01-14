#include "../include/k_means.h"



// Final print

void print_info(int iterations)  {
    printf("N = %d, K = %d\n", N, K);
    
    for (register int k = 0; k < K; k++) {
        register int size = 0;
        for(register int t = 0; t < T; t++)
            size += clusters[t * K + k].size;
        printf("Center: (%.3f, %.3f) : Size: %d\n", clusters[k].pos.x, clusters[k].pos.y, size);
    }
    
    printf("Iterations: %d\n", iterations);
}



// Initialization: N points and K clusters

void initialize() {
    points = (Point *) malloc(N * sizeof(Point));
    clusters = (Cluster *) malloc(T * K * sizeof(Cluster));

    srand(10);
    for(register int i = 0; i < N; i++) {
        points[i].x = (float) rand() / RAND_MAX;
        points[i].y = (float) rand() / RAND_MAX;
    }
    
    for(register int t = 0; t < T; t++) {
        for(register int k = 0; k < K; k++) {
            clusters[t * K + k].points = (Point *) malloc(((N/T)+1) * sizeof(Point));
            clusters[t * K + k].pos.x = points[k].x;
            clusters[t * K + k].pos.y = points[k].y;
        }
    }
}



// Euclidean distance between 2 points
// (no need to use sqrt, we dont need the right dist)

float dist(Point a, Point b) {
	return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}



// Associate each point to closest cluster
// Overhead :: 90%
void cluster_points() {
    #pragma omp parallel for // (same as) #pragma omp parallel for schedule(static, N/T)
    for (register int i = 0; i < N; i++) {
        register int cluster_id = 0;
        register int t = omp_get_thread_num();
        register float lowest_dist = dist(points[i],clusters[t * K].pos); // k = 0

        for (register int k = 1; k < K; k++) {
            register float distance = dist(points[i],clusters[t * K + k].pos);
            if (distance < lowest_dist) {
                lowest_dist = distance;
                cluster_id = k;
            }
        }

        clusters[t * K + cluster_id].points[clusters[t * K + cluster_id].size++] = points[i];
    }
}



// Calculate new centroids

void reevaluate_centers() {
    #pragma omp parallel for
    for (register int k = 0; k < K; k++) {
        register float x = 0, y = 0, s = 0;

        for (register int t = 0; t < T; t++) {
            for (register int j = 0; j < clusters[t * K + k].size; j++) {
                x += clusters[t * K + k].points[j].x;
                y += clusters[t * K + k].points[j].y;
            }
            s += clusters[t * K + k].size;
        }

        for (register int t = 0; t < T; t++) {
            clusters[t * K + k].pos.x = x / s;
            clusters[t * K + k].pos.y = y / s;
        }
    }
}



// Set all clusters size to 0

void update_clusters_size() {
    for (register int t = 0; t < T; t++) {
        #pragma omp parallel for
        for (register int k = 0; k < K; k++)
            clusters[t * K + k].size = 0;
    }
}



// Main

int main(int argc, char *argv[]) {
    
    N = atoi(argv[1]);                      // number of samples
    K = atoi(argv[2]);                      // number of clusters
    T = (argc == 4) ? atoi(argv[3]) : 1;    // number of threads

    omp_set_num_threads(T);                 // set number of threads

    register int iterations = -1;           // initial iteration is inside the while
    initialize();                           // initialize samples and clusters

    while (iterations < 20) {
        update_clusters_size();             // set clusters size to 0
        cluster_points();                   // associate each point to closest cluster
        reevaluate_centers();               // calculate new centroids
        iterations++;
    }

    free(points);
    print_info(iterations);
    free(clusters);
    return 0;
}


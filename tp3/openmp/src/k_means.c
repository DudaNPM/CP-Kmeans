#include "../include/k_means.h"



// Final print

void print_info(int iterations)  {
    printf("N = %d, K = %d\n", N, K);
    for (register int k = 0; k < K; k++)
        printf("Center: (%.3f, %.3f) : Size: %d\n", clusters[k].pos.x, clusters[k].pos.y, clusters[k].size);
    printf("Iterations: %d\n", iterations);
}



// Initialization: N points and K clusters

void initialize() {
    points = (Point *) malloc(N * sizeof(Point));
    clusters = (Cluster *) malloc(K * sizeof(Cluster));

    srand(10);
    for(register int i = 0; i < N; i++) {
        points[i].x = (float) rand() / RAND_MAX;
        points[i].y = (float) rand() / RAND_MAX;
    }

    for(register int k = 0; k < K; k++) {
        clusters[k].pos.x = points[k].x;
        clusters[k].pos.y = points[k].y;
    }
}



// Euclidean distance between 2 points
// (no need to use sqrt, we dont need the right dist)

float dist(Point a, Point b) {
        return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}



// Associate each point to closest cluster and calculate new centroids and sizes

void k_means_it() {
    float * sum_x = calloc(K,sizeof(float));    // array of size K with sum of all x of each cluster
    float * sum_y = calloc(K,sizeof(float));    // array of size K with sum of all y of each cluster
    int   * size  = calloc(K,sizeof(int  ));    // array of size K with clusters size

    // each thread will have is own variables (sum_x,sum_y,size) so there will be no data races
    // precision may decrease since the order of the adds is changed
    #pragma omp parallel for reduction(+:sum_x[:K]) reduction(+:sum_y[:K]) reduction(+:size[:K])
    for (register int i = 0; i < N; i++) {
        register int cluster_id = 0;
        register float lowest_dist = dist(points[i],clusters[0].pos);

        for (register int k = 1; k < K; k++) {
            register float distance = dist(points[i],clusters[k].pos);
            if (distance < lowest_dist) {
                lowest_dist = distance;
                cluster_id = k;
            }
        }

        // increment size of the closest cluster
        size[cluster_id]++;
        // increment x and y sums of the closest cluster
        sum_x[cluster_id] += points[i].x;
        sum_y[cluster_id] += points[i].y;
    }

    // calculate new centroids after reduction its done
    for (register int k = 0; k < K; k++) {
        clusters[k].pos.x = sum_x[k] / size[k];
        clusters[k].pos.y = sum_y[k] / size[k];
        clusters[k].size  = size[k];
    }

    // Free memory
    free(sum_x);
    free(sum_y);
    free(size);
}


// Main

// omp melhor tempo no cluster para N=10000000 K=4 T=8 cpu=15 time=0.600s
// omp melhor tempo no      pc para N=10000000 K=4 T=8 cpu=-- time=0.391s

int main(int argc, char *argv[]) {
    N = atoi(argv[1]);                      // number of samples
    K = atoi(argv[2]);                      // number of clusters
    T = (argc == 4) ? atoi(argv[3]) : 1;    // number of threads

    omp_set_num_threads(T);                 // set number of threads

    register int iterations = -1;           // initial iteration is inside the while
    initialize();                           // initialize samples and clusters

    while (iterations < 20) {
        k_means_it();
        iterations++;
    }

    // Print info
    print_info(iterations);


    // Free memory
    free(points);
    free(clusters);

    return 0;
}
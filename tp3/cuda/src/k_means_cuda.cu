#include "../include/k_means_cuda.h"



// Final print

__host__ void print_info(int iterations)  {
    printf("N = %d, K = %d\n", NUM_SAMPLES, NUM_CLUSTERS);
    for (register int k = 0; k < NUM_CLUSTERS; k++)
        printf("Center: (%.3f, %.3f) : Size: %d\n", h_clusters[k].pos.x, h_clusters[k].pos.y, h_clusters[k].size);
    printf("Iterations: %d\n", iterations);
}



// Initialization: N points and K clusters

__host__ void initialize() {
    h_points   = (Point *)   malloc(NUM_SAMPLES * sizeof(Point)  );
    h_clusters = (Cluster *) malloc(NUM_CLUSTERS * sizeof(Cluster));

    h_sum_x = (float *) malloc(NUM_CLUSTERS * sizeof(float));
    h_sum_y = (float *) malloc(NUM_CLUSTERS * sizeof(float));
    h_size  = (int *)   malloc(NUM_CLUSTERS * sizeof(int)  );

    srand(10);
    for(register int i = 0; i < NUM_SAMPLES; i++) {
        h_points[i].x = (float) rand() / RAND_MAX;
        h_points[i].y = (float) rand() / RAND_MAX;
    }
    
    for(register int k = 0; k < NUM_CLUSTERS; k++) {
        h_clusters[k].pos.x = h_points[k].x;
        h_clusters[k].pos.y = h_points[k].y;
    }
}



// Euclidean distance between 2 points
// (no need to use sqrt, we dont need the right dist)

__device__ float dist(Point a, Point b) {
  return (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y);
}



__global__ void k_means_it_kernel(Point * d_points, Cluster * d_clusters, float * d_sum_x, float * d_sum_y, int * d_size) {
    register int i = blockIdx.x * blockDim.x + threadIdx.x;  // global thread id
    if (i >= NUM_SAMPLES) return;                            // in case there are more threads than points

    register int cluster_id = 0;
    register float lowest_dist = dist(d_points[i],d_clusters[0].pos);

    for (register int k = 1; k < NUM_CLUSTERS; k++) {
        register float distance = dist(d_points[i],d_clusters[k].pos);
        if (distance < lowest_dist) {
            lowest_dist = distance;
            cluster_id = k;
        }
    }

    atomicAdd(&d_size[cluster_id] , 1            );
    atomicAdd(&d_sum_x[cluster_id], d_points[i].x);
    atomicAdd(&d_sum_y[cluster_id], d_points[i].y);
}



__host__ void k_means_it(Point * d_points, Cluster * d_clusters, float * d_sum_x, float * d_sum_y, int * d_size) {
    // Iniatilize/clear device variabels to 0
    cudaMemset(d_size , 0, NUM_CLUSTERS * sizeof(int)  );
    cudaMemset(d_sum_x, 0, NUM_CLUSTERS * sizeof(float));
    cudaMemset(d_sum_y, 0, NUM_CLUSTERS * sizeof(float));

    // Copy memory to the device
    cudaMemcpy(d_clusters, h_clusters, NUM_CLUSTERS * sizeof(Cluster), cudaMemcpyHostToDevice);

    // Launch the kernel
    k_means_it_kernel<<<NUM_BLOCKS, NUM_THREADS_PER_BLOCK>>>(d_points, d_clusters, d_sum_x, d_sum_y, d_size);

    // Copy memory to the host
    cudaMemcpy(h_size , d_size , NUM_CLUSTERS * sizeof(int)  , cudaMemcpyDeviceToHost);
    cudaMemcpy(h_sum_x, d_sum_x, NUM_CLUSTERS * sizeof(float), cudaMemcpyDeviceToHost);
    cudaMemcpy(h_sum_y, d_sum_y, NUM_CLUSTERS * sizeof(float), cudaMemcpyDeviceToHost);

    // Calculate centroids and size of the clusters
    for (register int k = 0; k < NUM_CLUSTERS; k++) {
        h_clusters[k].pos.x = h_sum_x[k] / h_size[k];
        h_clusters[k].pos.y = h_sum_y[k] / h_size[k];
        h_clusters[k].size  = h_size[k];
    }
}



// Main

__host__ int main(int argc, char *argv[]) {
    initialize();                   // initialize samples and clusters
    register int iterations = -1;   // initial iteration is inside the while

    // Device variables
    int * d_size;           // array of size K with clusters size
    float * d_sum_x;        // array of size K with sum of all x of each cluster
    float * d_sum_y;        // array of size K with sum of all y of each cluster
    Point * d_points;       // N samples
    Cluster * d_clusters;   // K clusters

    // Allocate memory on the device
    cudaMalloc((void**) &d_size    , NUM_CLUSTERS * sizeof(int)    );
    cudaMalloc((void**) &d_sum_x   , NUM_CLUSTERS * sizeof(float)  );
    cudaMalloc((void**) &d_sum_y   , NUM_CLUSTERS * sizeof(float)  );
    cudaMalloc((void**) &d_points  , NUM_SAMPLES  * sizeof(Point)  );
    cudaMalloc((void**) &d_clusters, NUM_CLUSTERS * sizeof(Cluster));

    // Copy memory to the device
    cudaMemcpy(d_points, h_points, NUM_SAMPLES * sizeof(Point), cudaMemcpyHostToDevice);

    while (iterations < 20) {
        k_means_it(d_points,d_clusters,d_sum_x,d_sum_y,d_size);
        iterations++;
    }

    // Print info
    print_info(iterations);

    // Free the device memory
    cudaFree(d_sum_x);
    cudaFree(d_sum_y);
    cudaFree(d_size);
    cudaFree(d_points);
    cudaFree(d_clusters);

    // Free the host memory
    free(h_points);
    free(h_clusters);
    free(h_sum_x);
    free(h_sum_y);
    free(h_size);

    return 0;
}

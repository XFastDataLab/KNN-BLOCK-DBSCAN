

#include "../flann/flann.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

float* read_points(const char* filename, int rows, int cols)
{
	float* data;
	float *p;
	FILE* fin;
	int i,j;

    fin = fopen(filename,"r");
    if (!fin) {
        printf("cannot open input file.\n");
        exit(1);
    }

    data = (float*) malloc(rows*cols*sizeof(float));
    if (!data) {
        printf("cannot allocate memory.\n");
        exit(1);
    }
    p = data;

    for (i=0;i<rows;++i) {
        for (j=0;j<cols;++j) {
            fscanf(fin,"%g ",p);
            p++;
        }
    }

    fclose(fin);

    return data;
}

void write_results(const char* filename, int *data, int rows, int cols)
{
	FILE* fout;
	int* p;
	int i,j;

    fout = fopen(filename,"w");
    if (!fout) {
        printf("cannot open output file.\n");
        exit(1);
    }

    p = data;
    for (i=0;i<rows;++i) {
        for (j=0;j<cols;++j) {
            fprintf(fout,"%d ",*p);
            p++;
        }
        fprintf(fout,"\n");
    }
    fclose(fout);
}



int main(int argc, char** argv)
{
	float* dataset;
	float* testset=(float*)malloc(10000);
	int nn;
	int* result;
	float* dists;
	struct FLANNParameters p;


//    int rows = 9000;
//    int cols = 128;
//    int tcount = 1000;
    int rows = 789;
    int cols =2;
    int tcount = 1;

    printf("reading input data file.\n");
    dataset = read_points("agg.txt", rows, cols);
    printf("reading test data file.\n");
//    testset = read_points("../testset.dat", tcount, cols);

    nn = 5;
    result = (int*) malloc(tcount*nn*sizeof(int));
    dists = (float*) malloc(tcount*nn*sizeof(float));

//    for(int i = 0;i<10000;i++)
//	{
//        printf("%f",dataset[i]);
//	}

    testset[0] = 15.55;
    testset[1] = 28.65;
   // p = DEFAULT_FLANN_PARAMETERS;
//    p.algorithm = FLANN_INDEX_AUTOTUNED;
//    p.target_precision = 0.9;
    p.algorithm = FLANN_INDEX_KDTREE;
    p.trees = 1;
    p.log_level = FLANN_LOG_INFO;
	p.checks = 8;

    printf("computing index.\n");
  //  flann_index_t index_id = flann_build_index(dataset, rows, cols, &speedup, &p);
    flann_find_nearest_neighbors(dataset, rows, cols,testset, tcount, result, dists, nn, &p);
    write_results("results.txt",result, tcount, nn);

 //   flann_free_index(index_id, &p);
    free(dataset);
    free(testset);
    free(result);
    free(dists);

    return 0;
}

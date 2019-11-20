/*
 *
 * Copyright (C) 2017-  Yewang Chen<ywchen@hqu.edu.cn;nalandoo@gmail.com;1611414017@hqu.edu.cn>
 * License: GPL v1
 * This software may be modified and distributed under the terms
 * of license.
 *
 */
#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include "DataPoint.h"
#include "flann/flann.h"
using namespace std;
//Cluster analysis type.
class ClusterAnalysis
{
private:
	vector<DataPoint> dadaSets;        //the dataset.
	vector< vector< int > > CBs;
	vector< vector< int > > NCBs;
	float* CBs_radius;
	float* NCBs_radius;
	unsigned int dimNum;
	double radius;
	unsigned int dataNum;
	unsigned int minPTs;
	float* dataSet;
    flann_index_t t;
    struct FLANNParameters p;
    int it1;
    int it2;
    int count_noise_blocks;
    unsigned long clusterId;
    pair<int,int>* CAND1;
    pair<int,int>* CAND2;
    int cand1Id;
    int cand2Id;

	float GetDistance(DataPoint& dp1, DataPoint& dp2);                    //distance function
	void SetArrivalPoints(DataPoint& dp);
public:
	ClusterAnalysis(){}                //Default constructor.
	void Retrieve_Borders();
	bool Init(char* fileName, float radius, int minPTs);    //Initialization operation
	bool DoDBSCANRecursive();            //do KNN_BLOCK_DBSCAN
	bool WriteToFile(char* fileName);    //save the result
	void MergeCoreBlocks();
    void AssignNonCoreBlocks();
    void Merge(int a,int b);

};

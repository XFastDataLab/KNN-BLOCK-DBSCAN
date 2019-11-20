/*
 *
 * Copyright (C) 2017-  Yewang Chen<ywchen@hqu.edu.cn;nalandoo@gmail.com;1611414017@hqu.edu.cn>
 * License: GPL v1
 * This software may be modified and distributed under the terms
 * of license.
 *
 */
#include "ClusterAnalysis.h"
#include <fstream>
#include <iosfwd>
#include <math.h>
#include <cstdlib>


bool ClusterAnalysis::Init(char* fileName, float radius, int minPTs){
	this->radius = radius;        //set the radius
	this->minPTs = minPTs;        //set the minPTs
	this->dimNum = DIME_NUM;     //set the dim of the dataset
	ifstream ifs(fileName);
	dataSet=(float*) malloc(dimNum*1000000*sizeof(float));
	if (!ifs.is_open())
	{
		cout << "Error opening file";
		exit(-1);
	}
    cout<<"allocing the data..."<<endl;
	unsigned long i = 0;
	int m = 0;
	while (!ifs.eof())
	{
		DataPoint tempDP;
		double tempDimData[DIME_NUM];
		for (int j = 0; j<DIME_NUM; j++)
		{
			ifs >> tempDimData[j];
	 		dataSet[m] = tempDimData[j];
	 		m++;;
		}
		tempDP.SetDimension(tempDimData);
		tempDP.SetDpId(i);
		tempDP.SetVisited(false);
		tempDP.SetClusterId(-1);
		dadaSets.push_back(tempDP);
		i++;
	}
	ifs.close();
	dataNum = i-1;
	cout << "the total dataNum is: " << dataNum<< endl;
    cout << "building the trees..."<< endl;

    p.algorithm = FLANN_INDEX_KDTREE;
    p.trees =1;
    p.log_level = FLANN_LOG_INFO;
	p.checks = 128;

	float* speedup= (float*) malloc(10);
    t = flann_build_index_float(dataSet,dataNum,DIME_NUM,speedup,&p);
    free(speedup);
    it1 = 0;
    it2 = 0;
    clusterId = 0;
    CBs_radius = (float*) malloc(100000);
    NCBs_radius = (float*) malloc(500000);
    CAND1 = (pair<int,int>*) malloc(100000);
    CAND2 = (pair<int,int>*) malloc(500000);
	return true;
}

bool ClusterAnalysis::WriteToFile(char* fileName){
	ofstream of1(fileName);
	for (unsigned long i = 0; i<dataNum; i++){
		for (int d = 0; d<DIME_NUM; d++)
			of1 << dadaSets[i].GetDimension()[d] << '\t';
		of1 << dadaSets[i].GetClusterId() << endl;
	}
	of1.close();
	return true;
}

void ClusterAnalysis::SetArrivalPoints(DataPoint& dp){
	float* dists = (float*) malloc(minPTs*sizeof(float));
    int* result = (int*) malloc(minPTs*sizeof(int));
    float* point=(float*) malloc(DIME_NUM*sizeof(float));
	for(int i = 0;i<dimNum;i++){
       point[i]  = dp.GetDimension()[i];
	}
    flann_find_nearest_neighbors_index_float(t,point,1,result, dists,minPTs,&p);
    float dist_k = sqrt(dists[minPTs-1]);
    dp.SetVisited(true);
    if(dist_k <= radius){
        vector <int> cbs;
        if(dist_k <= radius/2)
        {
            for(int i = 0; i < minPTs; i++)
            {
                cbs.push_back(result[i]);
                dadaSets[result[i]].SetVisited(true);
            }
            CBs_radius[CBs.size()] = dist_k;
            CBs.push_back(cbs);
            cbs.clear();
        }
        else
        {
            for(int i = 0;i < minPTs;i++)
            {
                cbs.push_back(result[i]);
                dadaSets[result[i]].SetVisited(true);
            }
            CBs_radius[CBs.size()] = 0;
            CBs.push_back(cbs);
            cbs.clear();
        }
    }
    else if(dist_k > radius && dist_k <= 2*radius)
    {
        vector <int> ncbs;
        for(int i = 0; i < minPTs; i++)
        {
            if(sqrt(dists[i]) <= (dist_k -radius))
            {
                ncbs.push_back(result[i]);
                dadaSets[result[i]].SetVisited(true);
            }

        }
        NCBs_radius[NCBs.size()] = dist_k - radius;
        NCBs.push_back(ncbs);
        ncbs.clear();
    }
    else if(dist_k > 2*radius)
    {
        for(int i = 0; i < minPTs; i++)
        {
            if(sqrt(dists[i]) < (dist_k-2*radius))
            {
                dadaSets[result[i]].SetVisited(true);
            }
        }
        count_noise_blocks++;
    }
    free(point);
	free(result);
	free(dists);
}

bool ClusterAnalysis::DoDBSCANRecursive(){
    count_noise_blocks = 0;
    for (unsigned long i = 0; i<dataNum-1; i++){
	    if(dadaSets[i].isVisited()==false)
        SetArrivalPoints(dadaSets[i]);
	}
	free(dataSet);
    cout<<"the number of Core-blocks is: "<<CBs.size()<<endl;
    cout<<"the number of Non-Core-blocks is: "<<NCBs.size()<<endl;
    cout<<"the number of noise-blocks is: "<<count_noise_blocks<<endl;
  ofstream of1("data\\CBs_radius.txt");
	for (unsigned long i = 0; i<CBs.size(); i++)
	{
            if(CBs_radius[i] != 0)
            {
            for (int d = 0; d<DIME_NUM; d++)
                of1 << dadaSets[CBs[i][0]].GetDimension()[d] << '\t';
            of1<<CBs_radius[i]<<endl;
            }
	}
	of1.close();
    cout<<"Merge the core blocks ..."<<endl;
    MergeCoreBlocks();
    for(int i = 0;i<CBs.size();i++)
    {
        if(CBs[i].size()> 1)
        {
            clusterId++;
            for(int j=0;j<CBs[i].size();j++)
            {
                dadaSets[CBs[i][j]].SetClusterId(clusterId);
            }
        }
    }
    cout<<"Assign the non core blocks ..."<<endl;
    AssignNonCoreBlocks();
    free(CBs_radius);
    free(NCBs_radius);
    free(CAND1);
    free(CAND2);
    cout<<"Retrieve the Borders ..."<<endl;
    Retrieve_Borders();
	cout << "the result contains " << clusterId << " clusters" << endl;
	return true;
}

void ClusterAnalysis::MergeCoreBlocks(){
    for(int i = 0; i < CBs.size(); i++){
        if(dadaSets[CBs[i][0]].GetClusterId() == -1)
        {
            clusterId++;
            for(int it = 0;it < CBs[i].size();it++)
            {
                dadaSets[CBs[i][it]].SetClusterId(clusterId);
            }

            for(int j = i+1; j < CBs.size();j++)
            {
                float dist = GetDistance(dadaSets[CBs[i][0]], dadaSets[CBs[j][0]]);
                if(dist <= 7.8*radius)
                {
                    for(int n = 0;n < CBs[j].size();n++)
                    {
                        if(dadaSets[CBs[j][n]].GetClusterId() == -1)
                        dadaSets[CBs[j][n]].SetClusterId(clusterId);
                    }
                }
            }
        }
    }
}

void ClusterAnalysis::AssignNonCoreBlocks(){
    cand2Id = 0;
    for(int i=0 ;i<CBs.size();i++){
        for(int j=0;j<NCBs.size();j++){
             float dist = GetDistance(dadaSets[CBs[i][0]], dadaSets[NCBs[j][0]]);
             if(dist<=radius - NCBs_radius[j])
             {
                for(int m=0;m<NCBs[j].size();m++)
                {
                    dadaSets[NCBs[j][m]].SetClusterId(dadaSets[CBs[i][0]].GetClusterId());
                }
             }
             else if(dist> (radius-NCBs_radius[j]) && dist<=(radius + NCBs_radius[j]))
             {
                CAND2[cand2Id].first = i;
                CAND2[cand2Id].second = j;
                cand2Id++;
             }
        }
    }
    for(int i=0; i< cand2Id;i++){
       for(int j=0;j < NCBs[CAND2[i].second].size();j++)
       {
           if(dadaSets[NCBs[CAND2[i].second][j]].GetClusterId() == -1)
           {
                for(int m=0;m < CBs[CAND2[i].first].size();m++)
                {
                    float dist = GetDistance(dadaSets[CBs[CAND2[i].first][m]], dadaSets[NCBs[CAND2[i].second][j]]);
                    if(dist<=radius)
                    dadaSets[NCBs[CAND2[i].second][j]].SetClusterId(dadaSets[CBs[CAND2[i].first][m]].GetClusterId());
                }
           }
       }
    }
}

void ClusterAnalysis::Merge( int cbs_1, int cbs_2){
    float mindist = GetDistance(dadaSets[CBs[cbs_1][0]],dadaSets[CBs[cbs_2][0]]);
    int nearestId1 = 0;
    int nearestId2 = 0;
    for(int i = 0;i<=CBs[cbs_1].size();i++){
        float dist = GetDistance(dadaSets[CBs[cbs_1][0]],dadaSets[CBs[cbs_2][i]]);
        if(dist < mindist)
        {
            mindist = dist;
            nearestId1 = i;
        }
    }

    for(int j = 0;j<=CBs[cbs_2].size();j++){
        float dist = GetDistance(dadaSets[CBs[cbs_1][j]],dadaSets[CBs[cbs_2][nearestId1]]);
        if(dist < mindist)
        {
            mindist = dist;
            nearestId2 = j;
        }
    }

    if(mindist <= radius){
        for(int i = 0 ;i<= CBs[cbs_2].size();i++)
        {
            CBs[cbs_1].push_back(CBs[cbs_2][i]);
        }
        CBs[cbs_2].clear();
    }
}

void ClusterAnalysis::Retrieve_Borders(){
    int it = 0;
    pair<int,int> a[10000];
    for(int i = 0;i<dataNum;i++){
        if (dadaSets[i].GetClusterId()!=-1)
        {
            for(int j = 0;j <10000;j++)
            {
                if(a[j].first == dadaSets[i].GetClusterId())
                {
                    dadaSets[i].SetClusterId(a[j].second);
                    goto LoopRetrieve;
                }
            }
            it++;
            a[it].first = dadaSets[i].GetClusterId();
            a[it].second = it;
            dadaSets[i].SetClusterId(a[it].second);
        }
        LoopRetrieve:;
    }
    clusterId = it;
}

float ClusterAnalysis::GetDistance(DataPoint& dp1, DataPoint& dp2){
	float distance = 0;
	for (int i = 0; i<DIME_NUM; i++)
	{
		distance += pow(dp1.GetDimension()[i] - dp2.GetDimension()[i], 2);
	}
	return pow(distance, 0.5);
}


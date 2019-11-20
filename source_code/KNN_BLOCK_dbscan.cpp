/*
 *
 * Copyright (C) 2017-  Yewang Chen<ywchen@hqu.edu.cn;nalandoo@gmail.com;1611414017@hqu.edu.cn>
 * License: GPL v1
 * This software may be modified and distributed under the terms
 * of license.
 *
 */
#include "ClusterAnalysis.h"
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "cyw_timer.h"


using namespace std;

int main()
{
	ClusterAnalysis myClusterAnalysis;                        //Clustering algorithm object declaration.
	cout<<"loading the file..."<<endl;
	myClusterAnalysis.Init("data\\aps_norm.txt", 6000,10);        //Algorithm initialization eps =1.35 minpts=7
	cout<<"clusting the data..."<<endl;
    CYW_TIMER build_timer;
    build_timer.start_my_timer();
	myClusterAnalysis.DoDBSCANRecursive();                    //Perform KNN_BLOCK_DBSCAN.
	build_timer.stop_my_timer();
    printf("the running time is %f\n",build_timer.get_my_timer());
//	myClusterAnalysis.WriteToFile("data\\result.txt");//Save the result.
	return 0;
}

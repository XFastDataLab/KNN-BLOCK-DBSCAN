/*
 *
 * Copyright (C) 2017-  Yewang Chen<ywchen@hqu.edu.cn;nalandoo@gmail.com;1611414017@hqu.edu.cn>
 * License: GPL v1
 * This software may be modified and distributed under the terms
 * of license.
 *
 */
#include <vector>

using namespace std;

const int DIME_NUM = 170;        //set the DIME_NUME as 2
class DataPoint
{
private:
    int dpID;                //the ID of the data
	double dimension[DIME_NUM];
	long clusterId;                    //the cluster ID
	bool isKey;
	bool visited;
	vector<unsigned long> arrivalPoints;
public:

	DataPoint();
	DataPoint(unsigned long dpID, double* dimension, bool isKey);

	unsigned long GetDpId();
	void SetDpId(int dpID);
	double* GetDimension();
	void SetDimension(double* dimension);
	bool IsKey();
	void SetKey(bool isKey);
	bool isVisited();
	void SetVisited(bool visited);
	long GetClusterId();
	void SetClusterId(int classId);
	vector<unsigned long>& GetArrivalPoints();
};

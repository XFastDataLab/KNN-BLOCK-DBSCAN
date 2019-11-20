/*
 *
 * Copyright (C) 2017-  Yewang Chen<ywchen@hqu.edu.cn;nalandoo@gmail.com;1611414017@hqu.edu.cn>
 * License: GPL v1
 * This software may be modified and distributed under the terms
 * of license.
 *
 */
#include "DataPoint.h"

//Default consructor
DataPoint::DataPoint()
{
}

//consructor
DataPoint::DataPoint(unsigned long dpID, double* dimension, bool isKey) :isKey(isKey), dpID(dpID)
{
	for (int i = 0; i<DIME_NUM; i++)
	{
		this->dimension[i] = dimension[i];
	}
}

//set the data dimension
void DataPoint::SetDimension(double* dimension)
{
	for (int i = 0; i<DIME_NUM; i++)
	{
		this->dimension[i] = dimension[i];
	}
}

//get the data dimension
double* DataPoint::GetDimension()
{
	return this->dimension;
}

bool DataPoint::IsKey()
{
	return this->isKey;
}

void DataPoint::SetKey(bool isKey)
{
	this->isKey = isKey;
}

unsigned long DataPoint::GetDpId()
{
	return this->dpID;
}

void DataPoint::SetDpId(int dpID)
{
	this->dpID = dpID;
}

bool DataPoint::isVisited()
{
	return this->visited;
}


void DataPoint::SetVisited(bool visited)
{
	this->visited = visited;
}

long DataPoint::GetClusterId()
{
	return this->clusterId;
}

void DataPoint::SetClusterId(int clusterId)
{
	this->clusterId = clusterId;
}

vector<unsigned long>& DataPoint::GetArrivalPoints()
{
	return arrivalPoints;
}

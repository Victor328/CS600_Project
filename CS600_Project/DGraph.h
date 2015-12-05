#pragma once
#include <fstream>
#include <iostream>
class DGraph
{
private:
	int size;
	int source;
	int sink;
	int *adjacentMatrix;
	int *maxFlowMatrix;
	bool empty;
	int residualCapacity(int* flow, int *path);
	void pushFlow(int* flow, int *path, int amount);
public:
	DGraph();
	~DGraph();
	void readFromFile(char* fileName);
	void print();
	void printMaxFlow();
	void maxFlow(int* (*pathFunc)(int* flow, int* network, int size, int source, int sink));
};


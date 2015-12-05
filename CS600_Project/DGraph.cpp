#include "DGraph.h"



DGraph::DGraph()
{
	empty = true;
}


DGraph::~DGraph()
{
	delete[] adjacentMatrix;
	delete[] maxFlowMatrix;
}

void DGraph::readFromFile(char* fileName)
{
	std::ifstream data(fileName);
	if (!empty)
	{
		delete[] adjacentMatrix;
	}
	empty = false;
	data >> size;
	adjacentMatrix = new int[size*size];
	data >> source;
	data >> sink;
	int count = 0;
	while (!data.eof())
	{
		data >> adjacentMatrix[count];
		count++;
	}
	data.close();
}

void DGraph::print()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf_s("%2d ", adjacentMatrix[i*size + j]);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void DGraph::printMaxFlow()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf_s("%2d/%2d ", maxFlowMatrix[i*size + j], adjacentMatrix[i*size + j]);
		}
		std::cout << std::endl;
	}
}

void DGraph::maxFlow(int *(*pathFunc)(int* flow, int* network, int size,int source, int sink))
{
	int temp = size*size;
	if (maxFlowMatrix != nullptr)delete[] maxFlowMatrix;
	int* actrualFlow = new int[temp];                      //Creating an empty matrix from adjacent matrix to store
	for (int i = 0; i < temp; i++)                         //actrual flow in network.
	{
		if (adjacentMatrix[i] == -1)actrualFlow[i] = -1;
		else actrualFlow[i] = 0;
	}

	int* augmentingPath = pathFunc(actrualFlow,adjacentMatrix,size,source,sink);
	while (augmentingPath != NULL)
	{
		int rc = residualCapacity(actrualFlow, augmentingPath);
		pushFlow(actrualFlow, augmentingPath, rc);
		delete[] augmentingPath;
		augmentingPath = pathFunc(actrualFlow,adjacentMatrix,size,source,sink);
	}

	maxFlowMatrix = actrualFlow;
}

int DGraph::residualCapacity(int * flow,int* path)
{
	int length = path[0];
	int re = 0;
	int temp = 0;
	if (flow[path[1] + size*path[2]] != -1)
	{
		re = adjacentMatrix[path[1] + size*path[2]] - flow[path[1] + size*path[2]];
	}
	else
	{
		re = flow[path[2] + size*path[1]];
	}
	for (int i = 2; i < length; i++)
	{
		if (flow[path[i] + size*path[i+1]] != -1)
		{
			temp = adjacentMatrix[path[i] + size*path[i+1]] - flow[path[i] + size*path[i+1]];
		}
		else
		{
			temp = flow[path[i+1] + size*path[i]];
		}
		if (temp < re)re = temp;
	}
	return re;
}

void DGraph::pushFlow(int * flow, int * path, int amount)
{
	int length = path[0];
	for (int i = 1; i < length; i++)
	{
		if (flow[path[i] + size*path[i + 1]] != -1)flow[path[i] + size*path[i + 1]] += amount;
		else flow[path[i + 1] + size*path[i]] -= amount;
	}
}

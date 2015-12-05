#include "DGraph.h"
#include <vector>
int count_DFS = 0;
int count_Edmonds_Karp = 0;
int count_R_DFS = 0;

int* pathfunc_DFS(int* flow, int* network, int size, int source, int sink)
{
	count_DFS++;
	std::vector<int> path;
	bool* edge_visited = new bool[size*size];
	for (int i = 0; i < size*size; i++)edge_visited[i] = false;
	int current_vertex = source;
	int next_vertex = current_vertex;
	do
	{
		path.push_back(current_vertex);
		for (int i = 0; i < size; i++)
		{
			if (!edge_visited[current_vertex + i*size] &&
				network[current_vertex + i*size] != -1 &&
				(network[current_vertex+i*size]-flow[current_vertex+i*size])>0)
			{
				next_vertex = i;
				edge_visited[current_vertex + next_vertex*size] = true;
				break;
			}
			if (!edge_visited[i + current_vertex*size]
				&&network[i + current_vertex*size]!= -1 
				&& flow[i+current_vertex*size]>0)
			{
				next_vertex = i;
				edge_visited[next_vertex + current_vertex*size] = true;
				break;
			}
		}
		if (next_vertex == current_vertex)
		{
			if (path.size() == 1)return NULL;
			path.pop_back();
			next_vertex = path.back();
			path.pop_back();
		}
		current_vertex = next_vertex;
	} while (current_vertex != sink);
	path.push_back(sink);

	delete[] edge_visited;

	int* re = new int[path.size()+1];
	re[0] = path.size();
	for (int i = 1; i <= re[0]; i++)re[i] = path[i - 1];
	return re;
}

int* pathfunc_Edmonds_Karp(int* flow, int* network, int size, int source, int sink)
{
	count_Edmonds_Karp++;
	struct node
	{
		int value;
		int parent;
		node(int n,int p)
		{
			value = n;
			parent = p;
		}
	};
	int current_vertex;
	int queue_front = 0;
	bool* edge_visited = new bool[size*size];
	bool found = false;
	for (int i = 0; i < size*size; i++)edge_visited[i] = false;
	std::vector<node> todo_queue;
	todo_queue.push_back(node(source, -1));
	while (queue_front<todo_queue.size())
	{
		current_vertex = todo_queue[queue_front].value;
		for (int i = 0; i < size; i++)
		{
			if (!edge_visited[current_vertex + i*size] &&
				network[current_vertex + i*size] != -1 &&
				(network[current_vertex + i*size] - flow[current_vertex + i*size])>0)
			{
				todo_queue.push_back(node(i, queue_front));
				edge_visited[current_vertex + i*size] = true;
				if (i == sink)
				{
					queue_front = todo_queue.size()-1;
					found = true;
					break;
				}
			}
			if (!edge_visited[i + current_vertex*size]
				&& network[i + current_vertex*size] != -1
				&& flow[i + current_vertex*size]>0)
			{
				todo_queue.push_back(node(i, queue_front));
				edge_visited[i + current_vertex*size] = true;
				if (i == sink)
				{
					queue_front = todo_queue.size()-1;
					found = true;
					break;
				}
			}
		}
		queue_front++;
	}
	if (!found)return NULL;
	queue_front--;
	std::vector<int> path;
	do
	{
		path.push_back(todo_queue[queue_front].value);
		queue_front = todo_queue[queue_front].parent;
	} while (queue_front != -1);

	int* re = new int[path.size()+1];
	re[0] = path.size();
	for (int i = 1; i <= re[0]; i++)re[i] = path[re[0]-i];
	return re;
}

int* pathfunc_R_DFS(int* flow, int* network, int size, int source, int sink)
{
	count_R_DFS++;
	int* residual_graph = new int[size*size];
	for (int i = 0; i < size*size; i++)residual_graph[i] = -1;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (network[i + j*size] != -1)
			{
				residual_graph[i + j*size] = network[i + j*size] -flow[i + j*size];
				residual_graph[j + i*size] = flow[i + j*size];
				if (residual_graph[i + j*size] == 0)residual_graph[i + j*size] = -1;
				if (residual_graph[j + i*size] == 0)residual_graph[j + i*size] = -1;
			}

		}
	}

	std::vector<int> path;
	bool* edge_visited = new bool[size*size];
	for (int i = 0; i < size*size; i++)edge_visited[i] = false;
	int current_vertex = source;
	int next_vertex = current_vertex;
	do
	{
		path.push_back(current_vertex);
		for (int i = 0; i < size; i++)
		{
			if (!edge_visited[current_vertex + i*size] &&
				residual_graph[current_vertex + i*size] != -1)
			{
				next_vertex = i;
				edge_visited[current_vertex + next_vertex*size] = true;
				break;
			}
		}
		if (next_vertex == current_vertex)
		{
			if (path.size() == 1)return NULL;
			path.pop_back();
			next_vertex = path.back();
			path.pop_back();
		}
		current_vertex = next_vertex;
	} while (current_vertex != sink);
	path.push_back(sink);

	delete[] edge_visited;

	int* re = new int[path.size() + 1];
	re[0] = path.size();
	for (int i = 1; i <= re[0]; i++)re[i] = path[i - 1];
	return re;

	return NULL;
}

void main()
{
	using namespace std;
	DGraph flowMap;
	cout << "CS 600 Project P-8.2" << endl << "Ford Fulkerson algorithm" << endl;
	cout << "Zijie Zhang 10400100" << endl << endl;
	cout << "Reading flow network from network.txt...";
	flowMap.readFromFile("network.txt");
	cout << "done" << endl<<endl;
	cout << "Adjacent Matrix of network:" << endl << endl;
	flowMap.print();
	cout << "Calculating Maximum flow with DFS..." ;
	flowMap.maxFlow(pathfunc_DFS);
	cout << "done. Iterations used:" << count_DFS << endl;
	flowMap.printMaxFlow();
	cout << endl << "Calculating Maximum flow with Edmonds Karp algorithm...";
	flowMap.maxFlow(pathfunc_Edmonds_Karp);
	cout << "done. Iterations used:" << count_Edmonds_Karp << endl;
	flowMap.printMaxFlow();
	cout << endl << "Calculating Maximum flow using DFS with Residual Graph...";
	flowMap.maxFlow(pathfunc_R_DFS);
	cout << "done. Iterations used:" << count_R_DFS << endl;
	flowMap.printMaxFlow();

	system("pause");
}
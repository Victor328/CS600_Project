#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stack>
using namespace std;

struct node
{
	string str;
	vector<node*> successor;
	vector<int> index;
	node() {};
	node(string str)
	{
		this->str = str;
	}
	node(string str, int index)
	{
		this->str = str;
		this->index.push_back(index);
	}
};

class Tries
{
private:
	vector<int> empty;
	node* root;
	void print(node* p);
	void compress(node* p);
	void savetoFile(node *p, ofstream& file,int lvl);
	void lowercase(string& str);
	void clear(node* p);
	string url_file;
public:
	int match(string lhs, string rhs);
	const vector<int>& search(string keyword, bool print = false);
	void loadFromFile(const char* filename);
	void savetoFile(const char* filename);
	void compress();
	void print();
	void insert(string keyword, int index);
	void set_url_file(string filename);
	void clear();
	Tries();
	~Tries();
};


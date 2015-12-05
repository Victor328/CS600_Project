#include "Tries.h"



Tries::Tries()
{
	root = new node();
}


Tries::~Tries()
{
	clear();
	delete root;
}

void Tries::lowercase(string& str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] <= 'Z' && str[i] >= 'A')str[i] += 32;
	}
}

void Tries::print(node* p)
{
	cout << p->str<< " ";
	if (!p->successor.empty())
	{
		for (auto it = p->successor.begin(); it != p->successor.end(); ++it)
		{
			print(*it);
		}
	}
	cout << endl;
}

void Tries::print()
{
	if (!root->successor.empty())
	{
		for (auto it = root->successor.begin(); it != root->successor.end(); ++it)
		{
			print(*it);
		}
	}
}

int Tries::match(string lhs, string rhs)
{
	int i = 0;
	if (lhs.length() < rhs.length())return 0;
	for (; i < rhs.length(); i++)
	{
		if (lhs[i] != rhs[i])return i;
	}
	return i;
}

const vector<int>& Tries::search(string keyword,bool print=false)
{
	lowercase(keyword);
	int* re;
	node* p = root;
	int match_result = 0;
	string match_string = keyword;
	bool match_flag = false;
	while (true)
	{
		match_flag = false;
		for (auto it = p->successor.begin(); it != p->successor.end(); ++it)
		{
			match_result = match(match_string, (*it)->str);
			if (match_result <= 0) continue;
			else if (match_result == match_string.length())
			{
				if (print)
				{
					cout << keyword << ": ";
					for (auto index_it = (*it)->index.begin(); index_it != (*it)->index.end(); ++index_it)cout << (*index_it) << " ";
					cout << endl;
				}
				return (*it)->index;
			}
			else
			{
				match_string = match_string.substr(match_result, string::npos);
				p = (*it);
				match_flag = true;
				break;
			}
		}
		if (!match_flag)return empty;
	}

	return empty;
}
void Tries::loadFromFile(char* filename)
{
	clear();
	ifstream file(filename);
	int current_lvl = 1;
	int bash_count = 1;
	string input;
	stack<node*> parent_stack;
	node* current_parent = root;
	node* last_node=nullptr;
	file >> input;
	bool name_done = false;
	bool first_bash = true;
	while (file >> input)
	{
		lowercase(input);
		if (input == "-")
		{
			if (first_bash)
			{
				bash_count = 0;
				first_bash = false;
			}
			bash_count++;
			name_done = false;
		}
		else
		{
			first_bash = true;
			if (current_lvl == bash_count)
			{
				if (!name_done)
				{
					name_done = true;
					last_node = new node(input);
					current_parent->successor.push_back(last_node);
				}
				else
				{
					int i = atoi(input.c_str());
					last_node->index.push_back(i);
				}
			}
			else if (bash_count > current_lvl)
			{
				current_lvl = bash_count;
				parent_stack.push(current_parent);
				current_parent = last_node;
				if (!name_done)
				{
					name_done = true;
					last_node = new node(input);
					current_parent->successor.push_back(last_node);
				}
				else
				{
					int i = atoi(input.c_str());
					last_node->index.push_back(i);
				}
			}
			else
			{			
				for (int i = 0; i < (current_lvl - bash_count); i++)
				{
					current_parent = parent_stack.top();
					parent_stack.pop();
				}
				current_lvl = bash_count;
				if (!name_done)
				{
					name_done = true;
					last_node = new node(input);
					current_parent->successor.push_back(last_node);
				}
				else
				{
					int i = atoi(input.c_str());
					last_node->index.push_back(i);
				}
			}
		}
	}
}
void Tries::savetoFile(char* filename)
{
	ofstream file(filename);
	file << "root" << endl;
	if (!root->successor.empty())
	{
		for (auto it = root->successor.begin(); it != root->successor.end(); ++it)
		{
			savetoFile(*it, file, 1);
		}
	}
}

void Tries::savetoFile(node * p,ofstream& file,int lvl)
{
	for (int i = 0; i < lvl; i++)file << "- ";
	file << p->str<<" ";
	for (auto it = p->index.begin(); it != p->index.end(); ++it)file << *it<<" ";
	file << endl;
	if (!p->successor.empty())
	{
		for (auto it = p->successor.begin(); it != p->successor.end(); ++it)
		{
			savetoFile(*it, file, lvl + 1);
		}
	}
}

void Tries::compress()
{
	for (auto it = root->successor.begin(); it != root->successor.end(); ++it)
	{
		compress(*it);
	}
}

void Tries::compress(node* p)
{
	if (p->successor.empty())return;
	if (p->successor.size() == 1)
	{
		compress(p->successor[0]);
		if (p->successor[0]->successor.empty())
		{
			p->str = p->str + p->successor[0]->str;
			p->index.swap(p->successor[0]->index);
			p->successor.pop_back();
		}
		else
		{
			node* temp = p->successor[0];
			p->successor.clear();
			p->successor.swap(temp->successor);
			p->str = p->str + temp->str;
			delete temp;
		}
	}
	else
	{
		for (auto it = p->successor.begin(); it != p->successor.end(); ++it)
		{
			compress(*it);
		}
	}
}

void Tries::insert(string keyword, int index)
{
	lowercase(keyword);
	if (root->successor.size() == 0)
	{
		node* p = root;
		node* next;
		for (int i = 0; i < keyword.length(); i++)
		{
			next = new node(keyword.substr(i, 1));
			p->successor.push_back(next);
			p = next;
		}
		p->index.push_back(index);
	}
	else
	{
		node* p = root;
		node* next;
		bool found = false;
		for (int i = 0; i < keyword.length(); i++)
		{
			found = false;
			for (auto it = p->successor.begin(); it != p->successor.end(); ++it)
			{
				if ((*it)->str == keyword.substr(i, 1))
				{
					p = (*it);
					found = true;
					break;
				}
			}
			if (!found)
			{
				next = new node(keyword.substr(i, 1));
				p->successor.push_back(next);
				p = next;
			}
		}
		p->index.push_back(index);
	}
}

void Tries::set_url_file(string filename)
{
	url_file = filename;
}

void Tries::clear()
{
	if (!root->successor.empty())for (auto it = root->successor.begin(); it != root->successor.end(); ++it)clear(*it);
	root->successor.clear();
}

void Tries::clear(node* p)
{
	if(!p->successor.empty())for (auto it = p->successor.begin(); it != p->successor.end(); ++it)clear(*it);
	delete p;
}

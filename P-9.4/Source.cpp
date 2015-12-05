#include "Tries.h"
#include "HTTP_Client.h"
#include <algorithm>

static int url_count = 0;
int max_level = 2;
string host_url = "http://www.cplusplus.com/";

void create_dictionary(Tries& dict, HTTP_Client& net,string current_url="http://www.cplusplus.com/",int lvl=0)
{
	if (lvl > max_level || url_count>300)return;
	vector<string> re;
	string temp_file_name = "Tempfile.txt";
	net.fetch(current_url,temp_file_name);
	net.get_keyword_list(re, temp_file_name);
	for (auto it = re.begin(); it != re.end(); ++it)
	{
		dict.insert(*it, url_count);
	}
	url_count++;
	std::fstream fs;
	if(lvl!=0)fs.open("url_list.txt", std::fstream::out | std::fstream::app);
	else fs.open("url_list.txt", std::fstream::out);
	fs << current_url << endl;
	fs.close();
	net.get_url_list(host_url,re, temp_file_name);
	remove(temp_file_name.c_str());
	
	for (auto it = re.begin(); it != re.end(); ++it)                            //Recursively search all urls mentioned
	{
		fs.open("url_list.txt", std::fstream::in);                              //Eliminate duplicates
		string existing_url;
		bool flag = false;
		while (fs >> existing_url)
		{
			if (existing_url == *it)
			{
				flag = true;
				break;
			}
		}
		fs.close();
		if(!flag)create_dictionary(dict, net, *it, lvl + 1);
	}
}
void show_help()
{
	cout << "exit" << endl;
	cout << "    Close the program" << endl;
	cout << "makedict"<<endl;
	cout << "    Fetch data from the internet and make dictionary for cplusplus.com."<<endl;
	cout << "    Keyword will be used to create a Tries, and urls saved in url_list.txt." << endl;
	cout << "load [filename]" << endl;
	cout << "    Load saved dictionary (keywords) into compressed Tries" << endl;
	cout << "save [filename]" << endl;
	cout << "    Save keywords in Tries into [filename]." << endl;
	cout << "search [keyword1] [keyword2] ..." << endl;
	cout << "    Search for matching result of given keywords." << endl;
	cout << "depth [maximum]" << endl;
	cout << "    Set the maximum search depth when making dictionary. Default is 2. The" << endl;
	cout << "    program will end searching if depth exceeds [maximum] or the number of" << endl;
	cout << "    urls saved is over 300" << endl;
}

void main()
{
	Tries dict;
	HTTP_Client net;
	cout << "CS 600 Project P-9.2" << endl << "Search program for cplusplus.com" << endl;
	cout << "Zijie Zhang 10400100" << endl<<endl;
	cout << "Type \"help\" for available command"<<endl<<endl;
	string cmd = "";
	string filename = "";
	while (cmd != "exit")
	{
		cin >> cmd;
		if (cmd == "help")
		{
			show_help();
		}
		else if (cmd == "makedict")
		{
			create_dictionary(dict, net);
			cout << "Dicionary successfully created." << endl;
		}
		else if (cmd == "load")
		{
			cin >> filename;
			dict.loadFromFile(filename.c_str());
		}
		else if (cmd == "save")
		{
			cin >> filename;
			dict.savetoFile(filename.c_str());
		}
		else if (cmd == "search")
		{
			string line;
			vector<string> keyword;
			vector<int> re;
			getline(cin, line);
			line = line.substr(1);
			line = line + "  ";
			for (int i = 0; i < line.size(); i++)
			{
				if (line[i] == ' ')
				{
					keyword.push_back(line.substr(0, i));
					line = line.substr(i+1);
					i = 0;					
				}
			}
			for (auto key : keyword)
			{
				if(re.empty())re=dict.search(key);
				else
				{
					vector<int> temp = dict.search(key);
					vector<int> intersection(20);
					sort(re.begin(), re.end());
					sort(temp.begin(), temp.end());

					auto it = std::set_intersection(re.begin(), re.end(), temp.begin(), temp.end(), intersection.begin());
					intersection.resize(it - intersection.begin());
					re = intersection;
				}
			}
			vector<string> urls;
			ifstream file("url_list.txt");
			int count = 0;
			while (file >> line)
			{
				for (auto it = re.begin(); it != re.end(); ++it)
				{
					if (count == *it)
					{
						urls.push_back(line);
						break;
					}
				}
				count++;
			}
			for(auto it:urls)cout << "  " << it << endl;
		}
		else if (cmd == "depth")
		{
			cin >> max_level;
		}
	}
}
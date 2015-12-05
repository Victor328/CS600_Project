#include "Tries.h"
#include "HTTP_Client.h"

static int url_count = 0;
int max_level = 2;

void create_dictionary(Tries& dict, HTTP_Client& net,string current_url="http://www.cplusplus.com/",int lvl=0)
{
	if (lvl > max_level)return;
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
	net.get_url_list(current_url,re, temp_file_name);
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
	cout << "search [keyword]" << endl;
	cout << "    Search for matching result of [keyword]s." << endl;
	cout << "depth [maximum]" << endl;
	cout << "    Set the maximum search depth when making dictionary. Default is 2." << endl;
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
			cin >> filename;
			dict.search(filename, true);
		}
		else if (cmd == "depth")
		{
			cin >> max_level;
		}
	}
}
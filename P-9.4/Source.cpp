#include "Tries.h"
#include "HTTP_Client.h"

static int url_count = 0;

void create_dictionary(Tries& dict, HTTP_Client& net,string current_url="http://www.cplusplus.com/",int lvl=0)
{
	if (lvl > 1)return;
	vector<string> re;
	string temp_file_name = net.fetch(current_url);
	net.get_keyword_list(re, temp_file_name);
	for (auto it = re.begin(); it != re.end(); ++it)
	{
		dict.insert(*it, url_count);
	}
	url_count++;
	std::fstream fs;
	fs.open("url_list.txt", std::fstream::out | std::fstream::app);
	fs << current_url << endl;
	fs.close();
	net.get_url_list(current_url,re, temp_file_name);
	//remove(temp_file_name.c_str());
	for (auto it = re.begin(); it != re.end(); ++it)
	{
		create_dictionary(dict, net, *it, lvl + 1);
	}
}

void main()
{
	Tries dict;
	HTTP_Client net;
	create_dictionary(dict, net);
	dict.compress();
	dict.savetoFile("dict.txt");
	string cmd = "";
	//while (cmd != "exit")
	//{
	//	cin >> cmd;
	//	dict.search(cmd, true);
	//}
	//dict.insert("TExt", 5);
	//dict.insert("text", 6);
	//dict.insert("tEst", 4);
	//dict.insert("apPle", 3);
	//dict.insert("appLY", 8);
	//dict.insert("apPle", 90);
	//dict.insert("aPPle", 88);
	//dict.insert("ApPle", 66);
	dict.loadFromFile("dict.txt");
	while (cmd != "exit")
	{
		cin >> cmd;
		dict.search(cmd, true);
	}
	//dict.compress();
	//dict.print();
	//dict.search("apple", true);
	//dict.loadFromFile("keyword.txt");
	//dict.print();
	//dict.savetoFile("duplicate.txt");
	//vector<string> re;
	//net.fetch("http://www.cplusplus.com/");
	//net.get_url_list("http://www.cplusplus.com/", re, "tempfile_0.txt");
	//for (auto it = re.begin(); it != re.end(); ++it)cout << *it << endl;
	//net.get_keyword_list(re, "tempfile_0.txt");
	//for (auto it = re.begin(); it != re.end(); ++it)cout << *it << endl;
	system("pause");
}
#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <iterator>
#include <vector>
#pragma comment(lib,"ws2_32.lib")
using namespace std;

class HTTP_Client
{
private:
	static int temp_file_count;
public:
	HTTP_Client();
	~HTTP_Client();
	void get_url_list(string host_addr, vector<string>& result, string filename);
	void get_keyword_list(vector<string>& result, string filename);
	int fetch(string url, string file_name);
};


#include "HTTP_Client.h"

int HTTP_Client::temp_file_count = 0;

HTTP_Client::HTTP_Client()
{
}


HTTP_Client::~HTTP_Client()
{
}

void HTTP_Client::get_url_list(string host_addr, vector<string>& result, string filename)
{
	result.clear();
	string line;
	smatch re;
	string str;
	regex rex_expr_href("<a href=[^ ]*?\">");
	ifstream html_file(filename.c_str());
	while (!html_file.eof())
	{
		getline(html_file, line);
		regex_search(line, re, rex_expr_href);
		if (!re.empty())
		{
			for (auto x : re)
			{
				str = x.str();
				str = str.substr(9);
				str = str.substr(0, str.length() - 2);
				if (str[0] == '/')
				{
					str = str.substr(1);
					str = host_addr + str;
				}
				if (str.length() >= host_addr.length())
				{
					if (str.substr(0, host_addr.length()) == host_addr)
					{
						result.push_back(str);
					}
				}
			}
		}
	}
	html_file.close();
}

void HTTP_Client::get_keyword_list(vector<string>& result, string filename)
{
	result.clear();
	string line;
	smatch re;
	string str;
	regex rex_expr_keyword("[> -][A-Za-z+]+[< -]");
	ifstream html_file(filename.c_str());
	bool first_word = true;
	while (!html_file.eof())
	{
		getline(html_file, line);
		line = " " + line;
		while (regex_search(line, re, rex_expr_keyword))
		{
			if (!re.empty())
			{
				auto x = re[0];
				str = x.str();
				while (str[0] == '>' || str[0] == ' '||str[0]=='-')str = str.substr(1);
				while (str[str.length() - 1] == '<' || str[str.length() - 1] == ' ' || str[str.length() - 1] == '-')str = str.substr(0, str.length() - 1);
				result.push_back(str);
				line = re.suffix().str();
			}
		}
	}
	html_file.close();
}

int HTTP_Client::fetch(string url, string file_name)
{
	cout << "fetching " << url << "...";
	string host_addr;
	string html_path;
	host_addr = url.substr(7);
	int i = 0;
	while (host_addr[i] != '/' && i<host_addr.length())i++;
	html_path = host_addr.substr(i);
	host_addr = host_addr.substr(0, i);
	if (html_path == "")html_path = "/";

	string get_cmd;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		cout << "WSAStartup failed.\n";
		system("pause");
		return -1;
	}

	SOCKET Socket = INVALID_SOCKET;

	addrinfo hints;
	addrinfo *result = NULL, *ptr = NULL;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo(host_addr.c_str(), "80", &hints, &result);

	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons(80);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)result->ai_addr);

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		Socket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (Socket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return -1;
		}

		// Connect to server.		
		if (connect(Socket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR) {
			closesocket(Socket);
			Socket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);

	ofstream file(file_name.c_str());

	get_cmd = "GET "+html_path+" HTTP/1.1\r\nHost: " + host_addr + "\r\nConnection: close\r\n\r\n";
	send(Socket,get_cmd.c_str() ,get_cmd.length(), 0);
	char buffer[10000];
	int nDataLength;
	while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0) {
		int i = 0;
		while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {
			file << buffer[i];
			i += 1;
		}
	}
	cout << "Done.\n";
	file.close();
	closesocket(Socket);
	WSACleanup();
	return 0;
}
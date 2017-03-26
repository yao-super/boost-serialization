#include<WS2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>   
#include <boost/archive/text_oarchive.hpp>  
#include <boost/archive/text_iarchive.hpp>  
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>  
using namespace std;
#define BUF_SIZE 64

#pragma comment(lib,"ws2_32.lib")

class Package
{

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & num;
	}
public:
	vector<int> num;
	//int num;
};

int main()
{
	WSADATA wsaData;
	SOCKET SendSocket;
	char buf[BUF_SIZE];
	int retVal;
	//初始化套接字动态库
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return 1;
	}                                    
	//创建Socket
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SendSocket == INVALID_SOCKET)
	{
		cout << "socket failed!" << endl;
		WSACleanup();
		return -1;
	}                  

	sockaddr_in recvAddr;
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port = htons(8888);
	//servAddr.sin_addr.S_un.S_addr = inet_addr("192.168.10.3");
	inet_pton(AF_INET, "192.168.10.255", (void*)&recvAddr.sin_addr.S_un.S_addr);//设置服务器地址 (255)为广播
	int AddrLen = sizeof(recvAddr);

	Package pack;
	stringstream ss; 
	boost::archive::text_oarchive oar(ss);
	while (true)
	{
		cout << "Please input a string to send: " << endl;
		int str;
		cin>>str;
		//将用户输入的数据复制到buf中
		//ZeroMemory(buf, BUF_SIZE);
		//strcpy_s(buf, str.size() + 1, str.c_str());
		(pack.num).push_back(str);	
		oar << pack;
		string st=ss.str();
		const char *a=st.c_str();
		cout<<a<<endl;
		int n = sendto(SendSocket,(char*)a,strlen(a)+1, 0, (sockaddr *)&recvAddr, AddrLen); //返回发送数据的字符数
		cout<<n<<endl;
		if (-1 == n)
		{
			cout << "send failed" << endl;
			closesocket(SendSocket);
			WSACleanup();
			return -1;
		}

		//oar.clear();
	}
	closesocket(SendSocket);
	WSACleanup();
	system("pause");
	return 0;
}
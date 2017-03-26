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
#define  BUF_SIZE 65535
#pragma comment(lib,"WS2_32.lib")

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
};

int main()
{
	WSADATA wsaData;
	SOCKET RecvSocket;
	char buf[BUF_SIZE];
	int retVal;
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		return 1;
	}
	RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == RecvSocket)
	{
		cout << "socket failed !" << endl;
		WSACleanup();
		return -1;
	}

	sockaddr_in sendAddr;
	int len = sizeof(sendAddr);
	sockaddr_in recvAddr; //服务器地址
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port = htons(8888);
	//inet_pton(AF_INET, "192.168.10.111", (void*)&recvAddr.sin_addr.S_un.S_addr);
	recvAddr.sin_addr.s_addr = htonl(INADDR_ANY); //任意地址
	retVal =::bind(RecvSocket, (sockaddr*)&recvAddr, sizeof(recvAddr));
	if (SOCKET_ERROR == retVal)
	{
		cout << "bind error!" << endl;
		closesocket(RecvSocket);
		WSACleanup();
		return -1;
	}

	Package pack;
	stringstream iss;
	while (true)
	{
		//ZeroMemory(buf, BUF_SIZE);
		//第三个参数要缓冲区大小要设置大一些 返回接收数据的字符数
		int n = recvfrom(RecvSocket,(char*)buf, BUF_SIZE,0, (sockaddr *)&sendAddr, &len); 
		    iss<<buf;
			cout<<iss.str()<<endl;

		boost::archive::text_iarchive iar(iss);
		iar>>pack;
		if (n == -1)
		{
			cout << "recvfrom failed";
			closesocket(RecvSocket);
			WSACleanup();
			return 0;
		}

		cout << "Message received " << pack.num[0] << endl;
	}
	closesocket(RecvSocket);
	WSACleanup();
	system("pause");
	return 0;
}

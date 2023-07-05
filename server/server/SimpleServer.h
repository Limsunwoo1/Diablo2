#pragma once
#include "Engine.h"
#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>>
#include <list>
#include <map>
#include <string>
#include <stdio.h>
#include <thread>

#define LOCAL_HOST "127.0.0.1"
#define PORT_NUMBER 8080

using namespace std;
using SocketList = list<SOCKET>;
using SocketPair = pair<SOCKET, SOCKET>;
using SocketMap = map<string, SOCKET>;


class SimpleServer
{
	SINGLE(SimpleServer)
public:
	void Initalize();
	void WSAStartUp();
	void CreateScoket();
	void BindSocket();
	void ListenSocket();
	void Accept();

	INT SendData(SOCKET sock, void* data, int dataLen, int flags = 0);
	INT ReceiveData(SOCKET sock, void* data, int dataLen, int flags = 0);



	void EXIT();

	bool ClientHandler(SOCKET sock);

private:
	WORD			mVersion;
	WSADATA			mWSAData;

	SOCKET			mServerSocket;
	
	SocketList		mAllSocketList;
	SocketMap		mAllSocketMap;
	SocketList		mChatLoomSocket;
	
	sockaddr_in		mSocketAddr;
};


#include "SimpleServer.h"
#include <bitset>
#include <string>


void SimpleServer::Initalize()
{
	WSAStartUp();
	CreateScoket();
	BindSocket();
	ListenSocket();
}

void SimpleServer::WSAStartUp()
{
	// ���� ���̺귯�� ���� 2.2
	mVersion = MAKEWORD(2, 2);
	INT iWSA = WSAStartup(mVersion, &mWSAData);
	if (iWSA != 0)
	{
		cout << "WSAStartup() Fail..!" << endl;
		exit(1);
	}

	cout << "WSAStartup() Access..!" << endl;
}

void SimpleServer::CreateScoket()
{
	// ���� ����
	mServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (mServerSocket == INVALID_SOCKET)
	{
		cout << "SocketCreateFail...!" << endl;
		EXIT();
	}

	cout << "SocketCreateAccess..!" << endl;
}

void SimpleServer::BindSocket()
{
	mSocketAddr.sin_family = AF_INET;
	mSocketAddr.sin_port = htons(PORT_NUMBER);
	mSocketAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	// ������ �ּ� ���� (INADDR_ANY Ű����� ��� ��Ʈ��ũ �������̽����� ������ ������ ����)

	if (bind(mServerSocket, (sockaddr*)&mSocketAddr, sizeof(mSocketAddr)) == SOCKET_ERROR)
	{
		std::cout << "BindFail...!" << std::endl;

		EXIT();
	}

	cout << "BindAccess..!" << endl;
}

void SimpleServer::ListenSocket()
{
	if (listen(mServerSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "ListenFail...!" << std::endl;

		EXIT();
	}

	std::cout << "Running..." << std::endl;
}

void SimpleServer::Accept()
{
	while (1)
	{
		sockaddr_in clientAddr;
		SOCKET clientSocket;
		socklen_t clientSize = (socklen_t)sizeof(clientAddr);

		clientSocket = accept(mServerSocket, (sockaddr*)&clientAddr, &clientSize);
		mAllSocketList.emplace_back(clientSocket);

		if (clientSocket == INVALID_SOCKET)
		{
			int error = WSAGetLastError();
			std::cout << error << std::endl;

			closesocket(clientSocket);

			exit(1);
		}

		thread clientHandle([this, &clientSocket]()
		{
			cout << clientSocket << " - ���Ϲ�ȣ " << endl;
			ClientHandler(clientSocket);
		});
		clientHandle.detach();
	}
}

INT SimpleServer::SendData(SOCKET sock, void* data, int dataLen, int flags)
{
	INT iSend = send(sock, reinterpret_cast<char*>(data), dataLen, flags);
	if (iSend == SOCKET_ERROR)
	{
		std::cout << "SendError...!" << std::endl;
	}

	return iSend;
}

INT SimpleServer::ReceiveData(SOCKET sock, void* data, int dataLen, int flags)
{
	INT iReceive = recv(sock, reinterpret_cast<char*>(data), dataLen, flags);
	if (iReceive == SOCKET_ERROR)
	{
		std::cout << sock << " - ������ ������ ���� �մϴ�.." << std::endl;
		
		SocketList::iterator listiter = std::find(mAllSocketList.begin(), mAllSocketList.end(), sock);
		if (listiter != mAllSocketList.end())
		{

			mAllSocketList.erase(listiter);
		}

		for (auto& mapIter : mAllSocketMap)
		{
			if (mapIter.second == sock)
			{
				// �ʿ� find �� ã��iterator�� erase
				mAllSocketMap.erase(mAllSocketMap.find(mapIter.first));
				break;
			}
		}

		return SOCKET_ERROR;
	}
	
	int* dataType = reinterpret_cast<int*>(data);
	int type = *dataType;


	switch ((ServerDataType)type)
	{
	case ServerDataType::LoginData:
	{
		Login_Packet* packet = reinterpret_cast<Login_Packet*>(data);
		mAllSocketMap.insert(make_pair(packet->name, sock));

		// ������� �Լ� ����
		std::thread ChatThread([this, packet, sock, data, dataLen]() {
			string name = packet->name;
			std::cout << name << " ���� �����Ͽ����ϴ�" << std::endl;

			for (SOCKET _sock : mAllSocketList)
			{
				SendData(_sock, data, sizeof(Login_Packet));
			}

			});
		ChatThread.join();
	}
		break;
	case ServerDataType::LogoutData:
	{
		Logout_Packet* packet = reinterpret_cast<Logout_Packet*>(data);

		SocketList::iterator listiter = std::find(mAllSocketList.begin(), mAllSocketList.end(), sock);
		if (listiter != mAllSocketList.end())
		{
			mAllSocketList.erase(listiter);
		}

		SocketMap::iterator mapiter = mAllSocketMap.find(packet->name);
		if (mapiter != mAllSocketMap.end())
		{
			mAllSocketMap.erase(mapiter);
		}

		// ������� �Լ� ����
		std::thread ChatThread([this, packet, sock, data, dataLen]() {
			std::cout << packet->name << " ���� �����Ͽ����ϴ�" << std::endl;

			for (SOCKET _sock : mAllSocketList)
			{
				if (sock == _sock)
					continue;

				SendData(_sock, data, sizeof(Logout_Packet));
			}

			});
		ChatThread.join();
	}
		break;
	case ServerDataType::ChatMessege:
	{
		ChatMassege_Packet* packet = reinterpret_cast<ChatMassege_Packet*>(data);
		// ������� �Լ� ����
		std::thread ChatThread([this, packet, sock, data, dataLen]() {
			std::cout << packet->name << " ���� �޼��� : " << packet->Messege << std::endl;

			for (SOCKET _sock : mAllSocketList)
			{
				if (_sock == sock)
					continue;

				SendData(_sock, data, sizeof(ChatMassege_Packet));
			}
			});
		ChatThread.join();
	}
		break;

	case ServerDataType::WhisperMessege:
	{
		WhisperMessege_Packet* packet = reinterpret_cast<WhisperMessege_Packet*>(data);
		// ������� �Լ� ����
		std::thread ChatThread([this, packet, sock, data, dataLen]() {
			SocketMap::iterator iter = mAllSocketMap.find(packet->otherName);
			if (iter != mAllSocketMap.end())
			{
				std::cout << packet->name << " ���� �ӼӸ� �޼��� : " << packet->Messege << std::endl;
				send(iter->second, reinterpret_cast<char*>(data), dataLen, 0);
			}

			});
		ChatThread.join();
	}
		break;

	case ServerDataType::DamegeData:

		break;
	case ServerDataType::PositionData:

		break;

	case ServerDataType::AnimationData:

		break;

	case ServerDataType::RigidbodyData:

		break;
	default:
		break;
	}
	
	

	return iReceive;
}

void SimpleServer::EXIT()
{
	cout << "EXIT..." << endl;

	mAllSocketList.clear();

	closesocket(mServerSocket);
	WSACleanup();
}

bool SimpleServer::ClientHandler(SOCKET sock)
{
	char buf[1024] = {};

	while (1)
	{
		INT iReceive = ReceiveData(sock, buf, 1024, 0);

		if (iReceive == SOCKET_ERROR)
		{
			return false;
		}

		ZeroMemory(buf, sizeof(buf), 0);
	}

	return false;
}

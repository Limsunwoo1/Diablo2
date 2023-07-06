#pragma once
#include "def.h"
#include <string>
#include <WinSock2.h>

namespace Server
{
	struct Vec3
	{
		float x;
		float y;
		float z;

		Vec3(){};
		Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
	};


	enum class ServerDataType : int
	{
		LoginData,
		LogoutData,

		ChatMessege,
		WhisperMessege,
		PositionData,
		AnimationData,
		DamegeData,
		RigidbodyData,
	};

	struct Login_Packet
	{
		ServerDataType type;
		SOCKET sock;

		std::string name;
		std::string password;
	};

	struct Logout_Packet
	{
		ServerDataType type;
		SOCKET sock;

		std::string name;
	};

	struct ChatMassege_Packet
	{
		ServerDataType type;

		std::string name;
		std::string Messege;
	};

	struct WhisperMessege_Packet
	{
		ServerDataType type;

		std::string name;
		std::string Messege;

		std::string otherName;
	};

	struct Position_Packet
	{
		ServerDataType type;
		SOCKET sock;

		Vec3 position;
		int animationIdx;
		std::string animationName;
	};

	struct Animation_Packet
	{
		ServerDataType type;
		SOCKET sock;

		int animationIDX;
		std::string animationName;

	};
}
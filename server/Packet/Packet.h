#pragma once
#include "def.h"
#include <string>

using namespace std;

struct Vec3
{
	float x;
	float y;
	float z;
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

	string name;
	string password;
};

struct Logout_Packet
{
	ServerDataType type;

	string name;
};

struct ChatMassege_Packet
{
	ServerDataType type;

	string name;
	string Messege;
};

struct WhisperMessege_Packet
{
	ServerDataType type;

	string name;
	string Messege;

	string otherName;
};

struct Position_Packet
{
	ServerDataType type;

	Vec3 position;
};

struct Animation_Packet
{
	ServerDataType type;

	int animationIDX;
	wstring animationName;
};
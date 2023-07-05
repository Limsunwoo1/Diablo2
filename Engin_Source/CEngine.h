#pragma once

// WIndows ��� ���� ���� �����ؾ��Ѵ�
// Winsock.h �� Winscok2.h �� �浹�� �߻��ϹǷ� 
// Winsock ����� ������� �ʰڴٴ� ��ũ��
#define _WINSOCKAPI_

// c++17 ���� ���� ��������� windows ����� byte �� std::byte �� �̸��� ����
// using namespace std ���� windows ����� byte �� �켱�Ǿ� std::byte�� ��ȣ�ϴٴ� �����������
// �׷��� Windows ��� ���� ������ HAS_STD_BYTE 0 �� �����Ѵ�
#define _HAS_STD_BYTE  0
#include <string>
#include <Windows.h>
#include <assert.h>
#include <iostream>

#include <functional>
#include <vector>
#include <list>
#include <map>
#include <bitset>
#include <set>
#include <unordered_map>
#include <stack>
#include <queue>

#include <cmath>
#include <algorithm>
#include <limits>
#include <memory>
#include <filesystem>


#include "CommonInclude.h"
#include "CEnums.h"
#include "def.h"
#include "Rtti.h"
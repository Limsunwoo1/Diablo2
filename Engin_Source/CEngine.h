#pragma once

// WIndows 헤더 선언 전에 선언해야한다
// Winsock.h 와 Winscok2.h 의 충돌이 발생하므로 
// Winsock 헤더는 사용하지 않겠다는 매크로
#define _WINSOCKAPI_

// c++17 버전 부터 생긴오류로 windows 헤더의 byte 와 std::byte 이 이름이 겹쳐
// using namespace std 사용시 windows 헤더의 byte 가 우선되어 std::byte는 모호하다는 에러가생긴다
// 그래서 Windows 헤더 선언 이전에 HAS_STD_BYTE 0 을 선언한다
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
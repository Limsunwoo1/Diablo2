#pragma once
#include <string>
using namespace std;
// RTTI ( Run - Time - Type - Information )
// 실행 시간(런타임) 중에 타입 정보 (부모 클래스 정보) 를 식별하여
// 비교적 속도가 느린 dynamic_cast 대신 빠른 static_cast 및 상속관계아 아닌 경우 nullptr 를 뱉도록 수정
// ClassName 자체는 별 쓸모없는 변수지만 추후 로그남기거나 할때 
// Class 이름을 가져다가 사용할 수 있도록 혹시 해서 만들어두었습니다
class FRTTI
{
private:
	string ClassName;
	const FRTTI* SuperRTTI;

public:
	// explicit 으로 형변환을 강제로 막은건데 큰 의미가 있는 코드는 아님
	explicit FRTTI(const string& InClassName, FRTTI* InSuperRTTI)
		:
		ClassName(InClassName),
		SuperRTTI(InSuperRTTI)
	{}
	virtual ~FRTTI() {}

	const string& GetClassName() const { return ClassName; }
	const FRTTI* GetSuperRTTI() const { return SuperRTTI; }
};

// 부모 , 자식 클래스 모두 필요한 변수 매크로
#define _GENERATED_RTTI                                     \
public:                                                     \
	static FRTTI RTTI;                                      \
	virtual const FRTTI* GetRTTI() const { return &RTTI; }  \
private:

// 자식 클래스 헤더에서 선언해줘야 하는 매크로 ( 다중 상속은 불가능 합니다 )
// 하단의 CPP 클래스도 마찬가지고 상속받은 메인이 되는 부모클래스를 클래스 이름 그대로 SuperClass 자리에 넣어주세요
// SuperClass Super 를 통해 Java 처럼 Super 키워드로 부모에 접근도 가능
#define GENERATED_RTTI( Class, SuperClass )                 \
	_GENERATED_RTTI                                         \
	typedef SuperClass Super;

// 부모 클래스 헤더에서 선언해줘야 하는 매크로
// 
// IsSameClass 는 지금 당장은 쓸모없는 함수긴 한데 혹시 해서 넣어두었습니다 같은 클래스 타입인지 비교하는 용도
// ( 사실 typeid() 로 대체가 가능한 부분 )
// 
// InKindOf 함수 내에서 static FRTTI 의 메모리 주소값을 통해 타입을 비교하도록 구현되어있습니다
// GENERATED_RTTI_ROOT_CPP 매크로를 통해 SuperRTTI == nullptr 이 되기 때문에 while 문으로 구현
#define GENERATED_RTTI_ROOT                                 \
	_GENERATED_RTTI                                         \
public:                                                     \
	bool IsSameClass( FRTTI* InRTTI ) const                 \
	{                                                       \
		return GetRTTI() == InRTTI;                         \
	}                                                       \
	bool InKindOf( FRTTI* InRTTI ) const                    \
	{                                                       \
		const FRTTI* CurRTTI = GetRTTI();                   \
		while( CurRTTI )                                    \
		{                                                   \
			if( CurRTTI == InRTTI ) return true;            \
			CurRTTI = GetSuperRTTI();                       \
		}                                                   \
		return false;                                       \
	}                                                       \
private:                                                    \

// 자식 클래스 CPP 에서 선언해줘야 하는 매크로
// static RTTI 변수를 초기화하는 역활 말고는 없습니다
#define GENERATED_RTTI_CPP( Class, SuperClass )             \
	FRTTI Class::RTTI( TEXT( #Class ), &SuperClass::RTTI );

// 부모 클래스 CPP 에서 선언해줘야 하는 매크로
// static RTTI 변수를 초기화하는 역활 말고는 없습니다
#define GENERATED_RTTI_ROOT_CPP( Class )                    \
	FRTTI Class:RTTI( TEXT( #Class ), nullptr )


// =============================================================
// To클래스가 From클래스의 자식인지 검색
// =============================================================
template<class To, class From>
bool IsKindOfByRTTI(const From* InTarget)
{
	if (!InTarget) return false;

	return InTarget->IsKindOf(&To::RTTI);
}

// =============================================================
// Cast 함수
// 언리얼 Cast 처럼 BClass* b = RTTICast<BClass>(a) 로 사용해주세요
// =============================================================
template<class To, class From>
To* RTTICast(const From* InTarget)
{
	return (IsKindOfByRTTI< To >(InTarget)) ? static_cast<To*>(InTarget) : nullptr;
}


// 사용 예시
/*
// TopClass.h
class TopClass
{
	GENERATED_RTTI_ROOT(TopClass)
private:
	// 그 외 사용 방법은 다른 C++과 동일
};

// TopClass.cpp
 #include "TopClass.h"

 GENERATED_RTTI_ROOT_CPP(TopClass)

 TopClass::TopClass() {}
 TopClass::~TopClass() {}

// BottomClass.h
class BottomClass : public TopClass
{
	GENERATED_RTTI(BottomClass, TopClass)
private:
	// 그 외 사용 방법은 다른 C++과 동일
};

// BottomClass.cpp
 #include "BottomClass.h"

 GENERATED_RTTI_CPP(BottomClass, TopClass)

 BottomClass::BottomClass() {}
 BottomClass::~BottomClass() {}
 */
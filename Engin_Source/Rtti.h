#pragma once
#include <string>
using namespace std;
// RTTI ( Run - Time - Type - Information )
// ���� �ð�(��Ÿ��) �߿� Ÿ�� ���� (�θ� Ŭ���� ����) �� �ĺ��Ͽ�
// ���� �ӵ��� ���� dynamic_cast ��� ���� static_cast �� ��Ӱ���� �ƴ� ��� nullptr �� �񵵷� ����
// ClassName ��ü�� �� ������� �������� ���� �α׳���ų� �Ҷ� 
// Class �̸��� �����ٰ� ����� �� �ֵ��� Ȥ�� �ؼ� �����ξ����ϴ�
class FRTTI
{
private:
	string ClassName;
	const FRTTI* SuperRTTI;

public:
	// explicit ���� ����ȯ�� ������ �����ǵ� ū �ǹ̰� �ִ� �ڵ�� �ƴ�
	explicit FRTTI(const string& InClassName, FRTTI* InSuperRTTI)
		:
		ClassName(InClassName),
		SuperRTTI(InSuperRTTI)
	{}
	virtual ~FRTTI() {}

	const string& GetClassName() const { return ClassName; }
	const FRTTI* GetSuperRTTI() const { return SuperRTTI; }
};

// �θ� , �ڽ� Ŭ���� ��� �ʿ��� ���� ��ũ��
#define _GENERATED_RTTI                                     \
public:                                                     \
	static FRTTI RTTI;                                      \
	virtual const FRTTI* GetRTTI() const { return &RTTI; }  \
private:

// �ڽ� Ŭ���� ������� ��������� �ϴ� ��ũ�� ( ���� ����� �Ұ��� �մϴ� )
// �ϴ��� CPP Ŭ������ ���������� ��ӹ��� ������ �Ǵ� �θ�Ŭ������ Ŭ���� �̸� �״�� SuperClass �ڸ��� �־��ּ���
// SuperClass Super �� ���� Java ó�� Super Ű����� �θ� ���ٵ� ����
#define GENERATED_RTTI( Class, SuperClass )                 \
	_GENERATED_RTTI                                         \
	typedef SuperClass Super;

// �θ� Ŭ���� ������� ��������� �ϴ� ��ũ��
// 
// IsSameClass �� ���� ������ ������� �Լ��� �ѵ� Ȥ�� �ؼ� �־�ξ����ϴ� ���� Ŭ���� Ÿ������ ���ϴ� �뵵
// ( ��� typeid() �� ��ü�� ������ �κ� )
// 
// InKindOf �Լ� ������ static FRTTI �� �޸� �ּҰ��� ���� Ÿ���� ���ϵ��� �����Ǿ��ֽ��ϴ�
// GENERATED_RTTI_ROOT_CPP ��ũ�θ� ���� SuperRTTI == nullptr �� �Ǳ� ������ while ������ ����
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

// �ڽ� Ŭ���� CPP ���� ��������� �ϴ� ��ũ��
// static RTTI ������ �ʱ�ȭ�ϴ� ��Ȱ ����� �����ϴ�
#define GENERATED_RTTI_CPP( Class, SuperClass )             \
	FRTTI Class::RTTI( TEXT( #Class ), &SuperClass::RTTI );

// �θ� Ŭ���� CPP ���� ��������� �ϴ� ��ũ��
// static RTTI ������ �ʱ�ȭ�ϴ� ��Ȱ ����� �����ϴ�
#define GENERATED_RTTI_ROOT_CPP( Class )                    \
	FRTTI Class:RTTI( TEXT( #Class ), nullptr )


// =============================================================
// ToŬ������ FromŬ������ �ڽ����� �˻�
// =============================================================
template<class To, class From>
bool IsKindOfByRTTI(const From* InTarget)
{
	if (!InTarget) return false;

	return InTarget->IsKindOf(&To::RTTI);
}

// =============================================================
// Cast �Լ�
// �𸮾� Cast ó�� BClass* b = RTTICast<BClass>(a) �� ������ּ���
// =============================================================
template<class To, class From>
To* RTTICast(const From* InTarget)
{
	return (IsKindOfByRTTI< To >(InTarget)) ? static_cast<To*>(InTarget) : nullptr;
}


// ��� ����
/*
// TopClass.h
class TopClass
{
	GENERATED_RTTI_ROOT(TopClass)
private:
	// �� �� ��� ����� �ٸ� C++�� ����
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
	// �� �� ��� ����� �ٸ� C++�� ����
};

// BottomClass.cpp
 #include "BottomClass.h"

 GENERATED_RTTI_CPP(BottomClass, TopClass)

 BottomClass::BottomClass() {}
 BottomClass::~BottomClass() {}
 */
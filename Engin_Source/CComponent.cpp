#include "CComponent.h"

Component::Component(eComponentType type)
	: mType(type)
	, mOwner(nullptr)
{
}


Component::~Component()
{
}

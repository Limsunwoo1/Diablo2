#include "CScript.h"

Script::Script()
	: Component(eComponentType::Script)
{
}

Script::~Script()
{
}

void Script::SetPosData(std::stack<Math::Vector2>& pos)
{
	 mPosData = pos;
}

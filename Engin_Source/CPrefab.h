#pragma once
#include "CResource.h"
class CPrefab : public Resource
{
public:
	CPrefab();
	~CPrefab();

	virtual HRESULT Load(const std::wstring& path) override;
private:

};


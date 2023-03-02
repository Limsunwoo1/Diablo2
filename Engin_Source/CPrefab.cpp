#include "CPrefab.h"

CPrefab::CPrefab()
    : Resource(eResourceType::Prefab)
{
}

CPrefab::~CPrefab()
{
}

HRESULT CPrefab::Load(const std::wstring& path)
{
    return E_NOTIMPL;
}

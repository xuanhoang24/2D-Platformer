#ifndef ASSET_CONTROLLER_H
#define ASSET_CONTROLLER_H

#include "../Core/StandardIncludes.h"
#include "../Utils/StackAllocator.h"
#include "../Resources/Asset.h"

class AssetController : public Singleton<AssetController>
{
public:
	//Constructos / Destructors
	AssetController();
	virtual ~AssetController();

	//Methods
	void Initialize(int _stackSize);
	void Clear();
	Asset* GetAsset(string _guid);

	//Members
	static StackAllocator* Stack;

private:
	//Members
	map<string, Asset*> m_assets;
};

#endif // ASSET_CONTROLLER_H
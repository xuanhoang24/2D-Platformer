#ifndef RESOURCE_H
#define RESOURCE_H

#include "Serializable.h"
#include "ObjectPool.h"
#include "AssetController.h"

class Resource :public Serializable
{
public:
	// Constructor/ Destructor
	Resource();
	virtual ~Resource();

	//Methods
	virtual void Serialize(std::ostream& _stream);
	virtual void Deserialize(std::istream& _streamm);
	void AssignNonDefaultValues();
	void ToString();

	//Members
	static ObjectPool<Resource>* Pool;

protected:
	void SerializerPointer(std::ostream& _stream, Resource* _pointer);
	void DeserializePointer(std::istream& _stream, Resource*& _pointer);
	void SerializeAsset(std::ostream& _stream, Asset*& _asset);
	void DeserializeAsset(std::istream& _stream, Asset*& _asset);

private:
	int m_val1;
	double m_val2;
	char m_val3;
	Resource* m_subResource;
	Asset* m_asset;
};

#endif // RESOURCE_H

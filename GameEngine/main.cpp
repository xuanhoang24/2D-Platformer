#include "Resource.h"

int main()
{
	Resource::Pool = new ObjectPool<Resource>();
	Resource* r1 = Resource::Pool->GetResource();
	r1->AssignNonDefaultValues();

	ofstream writeStream("resource.bin", ios::out | ios::binary);
	r1->Serialize(writeStream);
	writeStream.close();
	cout << "r1 values: ";
	r1->ToString();

	Resource* r2 = Resource::Pool->GetResource();
	ifstream readStream("resource.bin", ios::in | ios::binary);
	r2->Deserialize(readStream);
	readStream.close();
	cout << "r2 values: ";
	r2->ToString();

	delete Resource::Pool;
}
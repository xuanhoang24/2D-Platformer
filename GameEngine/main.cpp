#include "FileController.h"
#include "StackAllocator.h"

int main()
{
	FileController* fc = &FileController::Instance();
	StackAllocator* sa = new StackAllocator();
	sa->AllocateStack(100); //Allocate 100 bytes on the heap

	int fs1 = fc->GetFileSize("File1.txt");
	unsigned char* file1 = sa->GetMemory(fs1);
	fc->ReadFile("File1.txt", file1, fs1);
	cout << file1 << endl;

	int fs2 = fc->GetFileSize("File2.txt");
	unsigned char* file2 = sa->GetMemory(fs2);
	fc->ReadFile("File2.txt", file2, fs2);
	sa->Mark();
	cout << file2 << endl;

	int fs3 = fc->GetFileSize("File3.txt");
	unsigned char* file3 = sa->GetMemory(fs3);
	fc->ReadFile("File3.txt", file3, fs3);
	cout << file3 << endl;

	cout << "Bytes used: " << sa->GetBytesUsed() << " Content: " << file1 << endl;
	sa->FreeToMarker();
	cout << file1 << endl;
	cout << "Bytes used: " << sa->GetBytesUsed() << " Content: " << file1 << endl;

	delete sa;
}
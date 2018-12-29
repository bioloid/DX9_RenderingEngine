#ifndef __MEMORY_H__
#define __MEMORY_H__
#define BLOCKSIZE 20
#include <set>
class MEMORY
{
private:
	std::set<void*> address_new;
	std::set<void*> address_malloc;
public:
	MEMORY();
	~MEMORY();	
	void* Allocate(size_t size, int mode);
	void Insert(void* _data, int mode);
	bool Search(void* _data, int mode);
	bool Empty(int mode);
	void Clear(int mode);
	void Release();
};

#endif
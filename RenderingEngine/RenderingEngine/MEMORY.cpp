#include "MEMORY.h"
#include <stdlib.h>
MEMORY::MEMORY()
{
}

MEMORY::~MEMORY()
{
}

// mode : 0  -> malloc
//		  1  -> new
//		 -1  -> All
void* MEMORY::Allocate(size_t size, int mode)
{
	void* ptr;
	if (mode == 0) {
		ptr = malloc(size);
		if (ptr == NULL)
			return ptr;
		address_malloc.insert(ptr);
		return ptr;
	}
	else if (mode == 1) {
		try {
			ptr = new char[size];
		}
		catch (std::bad_alloc &e) {
			return NULL;
		}
		address_new.insert(ptr);
		return ptr;
	}
	return NULL;
}

void MEMORY::Insert(void* _data, int mode)
{
	if (mode == 0)
		address_malloc.insert(_data);
	else if (mode == 1)
		address_new.insert(_data);
}
bool MEMORY::Search(void* _data, int mode)
{
	if (mode == 0 || mode == -1) {
		if (address_malloc.find(_data) == address_malloc.end())
			return false;
		else
			return true;
	}
	if (mode == 1 || mode == -1) {
		if (address_new.find(_data) == address_new.end())
			return false;
		else
			return true;
	}
	return false;
}
bool MEMORY::Empty(int mode)
{
	if (mode == 0 || mode == -1)
		return address_malloc.empty();
	if (mode == 1 || mode == -1)
		return address_new.empty();
	return false;
}

void MEMORY::Clear(int mode)
{
	if (mode == 0 || mode == -1) {
		for (auto ptr = address_malloc.begin(); ptr != address_malloc.end(); ptr++)
		{
			if (*ptr != NULL)
				free(*ptr);
		}
		address_malloc.clear();
	}
	if (mode == 1 || mode == -1) {
		for (auto ptr = address_new.begin(); ptr != address_new.end(); ptr++)
		{
			if (*ptr != NULL)
				delete[] * ptr;
		}
		address_new.clear();
	}
}

void MEMORY::Release()
{
	Clear(-1);
}
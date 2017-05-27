#ifndef ELF_IV_HASH_MAP_H
#define ELF_IV_HASH_MAP_H

typedef struct _ElfIVHashMap ElfIVHashMap;

ElfIVHashMap *elfIVHashMap_new();
ElfIVHashMap *elfIVHashMap_new_withSize(int expectedSize);

/* DOCUMENTATION

typedef ElfIVHashMap;
	A map of key-value pairs, where key is an integer, and value is a void pointer.
	For each integer key, there can only be 1 value.
	This HashMap attemps to keep the load factor below 66% (2/3).
	Upon construction, the user can specify the expected number of elements that will be added to the Map,
	  in which case the inner structure will allocate sufficient memory so that these elements fill
	  66% or less of the map.
	Memory allocated for the Hash is always rounded to the next prime number in our prime number table,
	  which can be at most 2 times higher than the value given as input 'expectedAmount'.

ElfIVHashMap *elfIVHashMap_new();
	Returns a new HashMap without any capacity constraints.

ElfIVHashMap *elfIVHashMap_new_withSize(int expectedSize);
	Returns a new HashMap prepared to always receive a number of elements equal to 'expectedSize' or more.

*/

#endif

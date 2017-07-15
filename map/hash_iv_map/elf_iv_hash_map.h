#ifndef ELF_IV_HASH_MAP_H
#define ELF_IV_HASH_MAP_H

typedef struct _ElfIVHashMap ElfIVHashMap;

ElfIVHashMap *elfIVHashMap_new();
ElfIVHashMap *elfIVHashMap_new_withSize(int expectedSize);

void elfIVHashMap_destroy_F(ElfIVHashMap **elf_p, void (*func)(void *data));
void elfIVHashMap_destroy(ElfIVHashMap **elf_p);

void elfIVHashMap_traverse(ElfIVHashMap *elf, void(*func)(int key, void* value));

void *elfIVHashMap_put(ElfIVHashMap *elf, int key, void *value);
void *elfIVHashMap_remove(ElfIVHashMap *elf, int key);
void *elfIVHashMap_get(const ElfIVHashMap *elf, int key);

int elfIVHashMap_size(const ElfIVHashMap *elf);

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

void elfIVHashMap_destroy_F(ElfIVHashMap **elf_p, void (*func)(void *data));
	Frees the memory allocated for the hashmap.
	Every void* value stored will be passed to the function 'func', e.g. for freeing their memory.

void elfIVHashMap_destroy(ElfIVHashMap **elf_p);
	Frees the memory allocated for the hashmap.

void elfIVHashMap_traverse(ElfIVHashMap *elf, void(*func)(int key, void* value));
	Traverses all the elements stored in the hashmap.
	Each key-value pair is passed through to function 'func'.

void *elfIVHashMap_put(ElfIVHashMap *elf, int key, void *value);
	Adds the given key-value pair to the hashmap.
	Returns NULL if the pair was added and none has been replaced.
	If an old pair with same 'key' existed, this pair is replaced by the new one
	  and its value is returned.

void *elfIVHashMap_remove(ElfIVHashMap *elf, int key);
	Removes the key-value pair whose key is the given 'key'.
	If it exists, returns the associated value.
	If it doesn't exist, returns NULL, and the hashmap remains unaltered.

void *elfIVHashMap_get(const ElfIVHashMap *elf, int key);
	Returns the value associated with the given 'key'.
	Returns NULL if there is no key-value pair whose key is 'key'.

int elfIVHashMap_size(const ElfIVHashMap *elf);
	Returns the number of key-value pairs in the hashMap.
*/

#endif

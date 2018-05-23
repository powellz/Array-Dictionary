
// Implementation of a dictionary using an array and sequential search
// The array will be dynamically resized if necessary

#include <stdlib.h>
#include <string.h>

#include "array-dictionary.h"

// Constructor
ArrayDictionary::ArrayDictionary()
{
	maxEntries = 500;
	currentEntry = 0;

	array = new ArrayDictionaryNode[maxEntries];
}

// Add a record to the dictionary. Returns false if key already exists
bool
ArrayDictionary::addRecord( KeyType key, DataType record)
{
	for (int i = 0; i < currentEntry; i++)
	{
		if (!strcmp(array[i].key, key))
		{
			array[i].data = (DataType*)record;
			return false;
		}
	}

	if (currentEntry == maxEntries)
	{
		int oldSize = maxEntries;
		maxEntries *= 2;

		ArrayDictionaryNode *tmp = new ArrayDictionaryNode[maxEntries];

		for (int i = 0; i < oldSize; i++)
		{
			tmp[i].key = (char*)malloc(50 * sizeof(char));
			strcpy(tmp[i].key, array[i].key);
			tmp[i].data = (DataType*)array[i].data;
		}

		delete [] array;
		array = tmp;
	}

	array[currentEntry].key = (char*)malloc(50 * sizeof(char));
	strcpy(array[currentEntry].key, key);
	array[currentEntry].data = (DataType*)record;
	currentEntry++;

	return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
ArrayDictionary::findRecord(KeyType key)
{
	for (int i = 0; i < currentEntry; i++)
	{
		if (!strcmp(array[i].key, key))
		{
			return (DataType)array[i].data;
		}
	}

	return NULL;
}

// Removes one element from the table
bool
ArrayDictionary::removeElement(KeyType key)
{
    int found = 0;

	for (int i = 0; i < currentEntry; i++)
	{
		if (!strcmp(array[i].key, key))
		{
			found = 1;
		}
	}

	if (found == 0)
	{
		return false;
	}

    ArrayDictionaryNode *tmp = new ArrayDictionaryNode[maxEntries];

    int index;
    index = 0;

	for (int i = 0; i < currentEntry; i++)
	{
        if (!strcmp(array[i].key, key))
            continue;

	    tmp[index].key = (char*)malloc(sizeof(char)*50);
	    strcpy(tmp[index].key, array[i].key);
	    tmp[index].data = (DataType*)array[i].data;

        index++;
	}

	delete [] array;
	array = tmp;
    currentEntry--;

    return true;

}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
ArrayDictionary::keys(int * n)
{
    KeyType* data = new KeyType[currentEntry];

	for (int i = 0; i < currentEntry; i++)
	{
		data[i] = (char*)malloc(20 * sizeof(char));
		strcpy((char*)data[i], array[i].key);
	}

	*n = currentEntry;
	return data;
}

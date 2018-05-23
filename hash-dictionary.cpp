
// Implementation of a dictionary using a hash table

#include <stdlib.h>
#include <string.h>
#include "hash-dictionary.h"

// Constructor
HashDictionary::HashDictionary()
{
  // Add your code here
  //Allocate and initialize every bucket
	for (int i = 0; i < MaxBuckets; i++)
		buckets[i] = NULL;
}

//Using standard class string hash function
int
HashDictionary::hash(KeyType key) {
  // Add your code here
  int sum = 0;
  int len = strlen(key);
  for (int i = 0; i < len; i++) {
		sum += i*key[i];
  }
  return sum%MaxBuckets;
}

// Add a record to the dictionary. Returns false if key already exists
bool
HashDictionary::addRecord( KeyType key, DataType record)
{
  // Add your code here
	int h = hash(key);

	HashNode *e;
	e = buckets[h];

	while (e != NULL)
	{
		if (strcmp(e->key, key) == 0)
		{
			e->data = record;
			return false;
		}

		e = e->next;
	}

	e = new HashNode();
	e->key = strdup(key);
	e->data = record;
	e->next = buckets[h];

	buckets[h] = e;
	nElements++;

	return true;
}

// Find a key in the dictionary and return corresponding record or NULL
DataType
HashDictionary::findRecord( KeyType key)
{
	int h = hash(key);

	HashNode *currentNode;
	currentNode = buckets[h];

	while (currentNode != NULL)
	{
		if (strcmp(currentNode->key, key) == 0)
		{
			return (DataType)currentNode->data;
		}

		currentNode = currentNode->next;
	}

	return NULL;
}

// Removes one element from the table
bool
HashDictionary::removeElement(KeyType key)
{
	int h = hash(key);

	HashNode *currentNode;
	currentNode = buckets[h];

	HashNode *prev;
	prev = NULL;

	while (currentNode != NULL)
	{
		if (strcmp(currentNode->key, key) == 0)
		{
			if (prev != NULL)
			{
				prev->next = currentNode->next;
			}
			else
			{
				buckets[h] = currentNode->next;
			}

			nElements--;

			delete currentNode;
			return true;
		}

		prev = currentNode;
		currentNode = currentNode->next;
	}

	return false;
}

// Returns all the elements in the table as an array of strings.
// *n is the size of the table and it is returned by reference
KeyType *
HashDictionary::keys(int * n)
{
	KeyType * k = (KeyType *) malloc(nElements * sizeof(KeyType));

	*n = nElements;
	int i = 0;
	for (int h = 0; h < MaxBuckets; h++) {
		HashNode * n = buckets[h];
		while (n!=NULL) {
			k[i] = n->key;
			i++;
			n = n->next;
		}
	}
	*n = nElements;
	return k;
}


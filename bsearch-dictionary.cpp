
// Implementation of a dictionary using an array and binary search
// It will inherit from the ArrayDictionary

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bsearch-dictionary.h"

// Constructor
BinarySearchDictionary::BinarySearchDictionary()
{
	sorted = false;
}

bool
BinarySearchDictionary::addRecord( KeyType key, DataType record) {
	sorted = false;
	return ArrayDictionary::addRecord(key, record);
}

//Find a key in the dictionary and return corresponding record or NULL
//Standard binary tree search algorithm
DataType
BinarySearchDictionary::findRecord( KeyType key)
{
	if (!sorted) {
		sort();
		sorted = true;
	}

	int min, max;
	min = 0;
	max = currentEntry - 1;

	while (max >= min)
	{
		int mid = (min + max) / 2;
		int result = strcmp(key, array[mid].key);

		if (result == 0)
		{
			return (DataType)array[mid].data;
		}
		else if (result < 0)
		{
			max = mid - 1;
		}
		else
		{
			min = mid + 1;
		}
	}

	return NULL;
}


// Sort array using heap sort
void
BinarySearchDictionary::sort()
{
	//printf("Before\n");
	//for (int i = 0; i < currentEntry; i++) {
	//	printf("%s\n", array[i].key);
	//}

	// Add your code here

	int i;

	ArrayDictionaryNode temp;

	for (i = currentEntry / 2; i >= 0; i--)
	{
		bool incomplete = true;
		int max;
		int rootIndex = i;
		int botIndex = currentEntry - 1;

		ArrayDictionaryNode temp;

		while ((rootIndex * 2) <= botIndex && incomplete)
		{
			if ((rootIndex * 2) == botIndex)
			{
				max = (rootIndex * 2);
			}
			else if (strcmp(array[(rootIndex * 2)].key, array[(rootIndex * 2) + 1].key) > 0)
			{
				max = (rootIndex * 2);
			}
			else
			{
				max = (rootIndex * 2) + 1;
			}

			if (strcmp(array[rootIndex].key, array[max].key) < 0)
			{
				temp = array[rootIndex];
				array[rootIndex] = array[max];
				array[max] = temp;
				rootIndex = max;
			}
			else
			{
				incomplete = false;
			}
		}
	}

	for (i = currentEntry - 1; i >= 1; i--)
	{
		temp = array[0];
		array[0] = array[i];
		array[i] = temp;

		bool incomplete = true;
		int max;
		int rootIndex = 0;
		int botIndex = i - 1;

		ArrayDictionaryNode temp;

		while ((rootIndex * 2) <= botIndex && incomplete)
		{
			if ((rootIndex * 2) == botIndex)
			{
				max = (rootIndex * 2);
			}
			else if (strcmp(array[(rootIndex * 2)].key, array[(rootIndex * 2) + 1].key) > 0)
			{
				max = (rootIndex * 2);
			}
			else
			{
				max = (rootIndex * 2) + 1;
			}

			if (strcmp(array[rootIndex].key, array[max].key) < 0)
			{
				temp = array[rootIndex];
				array[rootIndex] = array[max];
				array[max] = temp;
				rootIndex = max;
			}
			else
			{
				incomplete = false;
			}
		}
	}
	//printf("After\n");
	//for (int i = 0; i < currentEntry; i++) {
	//	printf("%s\n", array[i].key);
	//}
}


#include <string.h>
#include "search-engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "webcrawl.h"


int searches = 0;
float totaltime = 0.0;

DictionaryType dictType;
SearchEngine::SearchEngine( int port, DictionaryType dictionaryType):
								  MiniHTTPD(port)
{
	// Create dictionary of the indicated type
	if (dictionaryType == ArrayDictionaryType){
		_wordToURLList = new ArrayDictionary();
	}
	else if (dictionaryType == HashDictionaryType){
		_wordToURLList = new HashDictionary();
	}
	else if (dictionaryType == AVLDictionaryType){
		_wordToURLList = new AVLDictionary();
	}
	else if (dictionaryType == BinarySearchDictionaryType){
		_wordToURLList = new BinarySearchDictionary();
	}
	else{
		_wordToURLList = NULL;
	}

	dictType = dictionaryType;

	//Initialize the dictionary of the dictionarytype
	URLRecord **records = new URLRecord*[2000];

	for (int i = 0; i < 2000; i++)
	{
		records[i] = new URLRecord();
	}

    //Read url.txt and store its entries
	FILE * file;
	file = fopen("url.txt", "r");

	char* line = new char[1000];
	char* url = new char[500];
	char* description = new char[500];

	while (fgets(line, 1000, file) != NULL)
	{
		if (strcmp(line, "\n"))
		{
			char* token = new char[500];
			token = strtok(line," \n");
			int index = atoi(token);

			url = strdup(strtok(NULL,"\n"));

			fgets(line,1000,file);
			description = strdup(line);

			records[index]->_url = url;
			records[index]->_description = description;
		}
	}

	delete line;
	fclose(file);

	file = fopen("word.txt", "r");

	line = new char[1000];
	char* token = new char[200];
	char* word = new char[200];
	URLRecordList* URLlist;


    //Read word.txt and store its entries
	while (fgets(line,2000,file)) {

		if (line[0]=='\n')
			continue;

		token = strtok(line," \n");
		strcpy(word, token);

		URLlist = new URLRecordList;
		URLlist = NULL;

		char* position = new char[200];
		token = strtok(NULL," \n");

		while(token){
			int position = atoi(token);
			if (records[position]->_url == NULL)
				continue;

			URLRecordList* entry = new URLRecordList;
			entry->_urlRecord = records[position];
			entry->_next = URLlist;
			URLlist = entry;
			token = strtok(NULL," \n");
		}
		_wordToURLList->addRecord(word,URLlist);

	}

	delete line;
	fclose(file);
}

void
SearchEngine::dispatch( FILE * fout, const char * documentRequested)
{
	if (strcmp(documentRequested, "/")==0) {
		// Send initial form
		fprintf(fout, "<TITLE>CS251 Search</TITLE>\r\n");
		fprintf(fout, "<CENTER><H1><em>Boiler Search</em></H1>\n");
		fprintf(fout, "<H2>\n");
		fprintf(fout, "<FORM ACTION=\"search\">\n");
		fprintf(fout, "Search:\n");
		fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
		fprintf(fout, "</H2>\n");
		fprintf(fout, "</FORM></CENTER>\n");
		return;
	}

	// TODO: The words to search in "documentRequested" are in the form
	// /search?word=a+b+c
	//
	// You need to separate the words before search
	// Search the words in the dictionary and find the URLs that
	// are common for al the words. Then print the URLs and descriptions
	// in HTML. Make the output look nicer.

	//seperate words from documentRequested

	if (strlen(documentRequested) < 13)
		return;
	char * requestpar = new char[strlen(documentRequested)];
	strcpy(requestpar, documentRequested+13);

	char **words = new char*[100];
	for (int i = 0; i < 100; i++)
	{
		words[i] = NULL;
	}

	int numWords = 0;
	char *singleword;
	singleword = strtok(requestpar, "+");

	while (singleword != NULL)
	{
		words[numWords] = new char[50];
		strcpy(words[numWords], singleword);
		numWords++;
		singleword = strtok(NULL, "+");
	}

	//Initialize search string
	char *query = new char[100*50];
	strcpy(query, "");

	for (int i = 0; i < numWords; i++)
	{
		strcat(query, words[i]);

		if (i < numWords - 1)
			strcat(query, " ");
	}

    //Initialize clocks to measure search time
	clock_t time1, time2;
	int counter = 1;
	int listCount = 0;

	URLRecord **list = new URLRecord*[500];

	//Mark first clock time
	time1 = clock();

	for (int i = 0; i < numWords; i++)
	{
		URLRecordList* data;
		data = (URLRecordList*)_wordToURLList->findRecord(words[i]);

		while (data != NULL)
		{
			int exists = 0;

			int j;
			for (j = 0; j < listCount; j++)
			{
				if (list[j] == data->_urlRecord)
				{
					exists = 1;
					break;
				}
			}

			if (exists == 0)
			{
				list[listCount] = data->_urlRecord;
				listCount++;
			}

			data = data->_next;
		}
	}

	for (int i = 0; i < listCount; i++)
	{
		int j;
		for (j = 0; j < numWords; j++)
		{
			URLRecordList* data;
			data = (URLRecordList*)_wordToURLList->findRecord(words[j]);

			int exists = 0;

			while (data != NULL)
			{
				if (data->_urlRecord == list[i])
				{
					exists = 1;
				}

				data = data->_next;
			}

			if (exists == 0)
				list[i] = NULL;
		}
	}

    //Mark finish clock time, add the differnce in time to total
	time2 = clock();
	totaltime += time2 - time1;

	fprintf( stderr, "Search for words: \"%s\"\n", query);

	fprintf( fout, "<TITLE>Search Results</TITLE>\r\n");
	fprintf( fout, "<H1> <Center><em>Boiler Search</em></H1>\n");
	fprintf( fout, "<H2> Search Results for \"%s\"</center></H2>\n", query );

    //Increment counter while printing the numbers for results
	for ( int i = 0; i < listCount; i++ ) {
		if(list[i] ==NULL) continue;
		fprintf(fout, "<h3>%d. <a href=\"%s\">%s</a><h3>\n", counter++, list[i]->_url, list[i]->_url);
		fprintf(fout, "<blockquote>%s<p></blockquote>\n", list[i]->_description);
	}

	//Increment number of searches at end of search
	searches++;

	fprintf(fout, "<BR>\n");
	fprintf(fout, "<h3>Current Search Time: %d ns</h3>\n", (int)(time2 - time1));
	fprintf(fout, "<h3>Average Search Time: %lf ns</h3>\n", (totaltime/(float)searches));

	if (dictType == ArrayDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: Array Dictionary</h3>\n");
	else if (dictType == HashDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: Hash Dictionary</h3>\n");
	else if (dictType == AVLDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: AVL Dictionary</h3>\n");
	else if (dictType == BinarySearchDictionaryType)
		fprintf(fout, "<h3>Dictionary Used: Binary Search Dictionary</h3>\n");
	else
		fprintf(fout, "<h3>Dictionary Used: Unknown Dictionary</h3>\n");

	// Add search form at the end
	fprintf(fout, "<HR><H2>\n");
	fprintf(fout, "<FORM ACTION=\"search\">\n");
	fprintf(fout, "Search:\n");
	fprintf(fout, "<INPUT TYPE=\"text\" NAME=\"word\" MAXLENGTH=\"80\"><P>\n");
	fprintf(fout, "</H2>\n");
	fprintf(fout, "</FORM>\n");
}

void
printUsage()
{
	const char * usage =
			"Usage: search-engine port (array | hash | avl | bsearch)\n"
			"  It starts a search engine at this port using the\n"
			"  data structure indicated. Port has to be larger than 1024.\n";

	fprintf(stderr, "%s", usage);
}

int main(int argc, char ** argv)
{
	if (argc < 3) {
		printUsage();
		return 1;
	}

	// Get port
	int port;
	sscanf( argv[1], "%d", &port);

	// Get DictionaryType
	const char * dictType = argv[2];
	DictionaryType dictionaryType;
	if (!strcmp(dictType, "array")) {
		dictionaryType = ArrayDictionaryType;
	}
	else if (!strcmp(dictType, "hash")) {
		dictionaryType = HashDictionaryType;
	}
	else if (!strcmp(dictType, "avl")) {
		dictionaryType = AVLDictionaryType;
	}
	else if (!strcmp(dictType, "bsearch")) {
		dictionaryType = BinarySearchDictionaryType;
	}
	else {
		printUsage();
		return 0;
	}

	SearchEngine httpd(port, dictionaryType);

	httpd.run();

	return 0;
}

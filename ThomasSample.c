#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "cy_url.h"

#define REMOTE_URL		"http://weather.cayintech.com/weather.xml"
#define INPUT			"./weather.xml"
#define OUTPUT			"./output.txt"
#define DOWNLOAD_PATH	"./"

/*	Debug Function */
#ifdef DEBUG
#define dbgout(s, ...) fprintf(stderr, s, ##__VA_ARGS__)
#else
#define dbgout(s, ...)
#endif

/*	Item Struct */
typedef struct ITEM_T {
	char	*id;
	char	*name;
} ITEM;

/*	Item Methods - initial item members */
void itemInit(ITEM *item)
{
	if (item == NULL) return;
	item->id = NULL;
	item->name = NULL;
}

/*	Item Methods - clear item members */
void itemClear(ITEM *item)
{
	if (item == NULL) return;
	if (item->id) free(item->id);
	if (item->name) free(item->name);
	itemInit(item);
}

char *loadData(char *file, size_t size)
{
	char *buffer = NULL;
	FILE *pFile = NULL;

	buffer = malloc(sizeof(char) * size); // create buffer (Dynamic Memory)
	if (buffer == NULL)
	{
		dbgout("[readFile] Create Buffer Fail!\n");
		return 0;
	}
	memset(buffer, 0, size); // init buffer

	pFile = fopen(file, "r"); // open file
	if (pFile == NULL)
	{
		dbgout("[readFile] Open File Fail!\n");
		return 0;
	}

	fread(buffer, size, 1, pFile);	// read once
	if (pFile) fclose(pFile);	// close file

	return buffer;
}

char *getContent(char *src, char *key)
{
	char *buf = NULL, *token = NULL, *save = NULL, delim[] = "<>/";
	for (buf = src;; buf = NULL)
	{
		token = strtok_r(buf, delim, &save);
		if (token == NULL) break;
		dbgout("[getContent] token: %s\n", token);
		if (strcmp(token, key) != 0) break;
	}
	return token;
}

/*	Parse and Save data */
int handleBuffer(char *buffer)
{
	char *token = NULL, *str = NULL, delim[] = "\r\n\t";
	int count = 0;
	FILE *pOutput = NULL;
	ITEM item;

	if (buffer == NULL) return -1;

	itemInit(&item); // Initial item

	pOutput = fopen(OUTPUT, "w"); // open file
	if (pOutput == NULL)
	{
		dbgout("Open Output File Fail!\n");
		goto out;
	}

	for (str = buffer;; str = NULL)
	{
		token = strtok(str, delim);
		if (token == NULL) break;
		dbgout("[handleBuffer] token: %s\n", token);

		if (strstr(token, "<id>") != NULL)
			item.id = strdup(getContent(token, "id"));
		else if (strstr(token, "<name>") != NULL)
			item.name = strdup(getContent(token, "name"));
		else if (strstr(token, "</item>") != NULL)
		{
			/* save data */
			fprintf(pOutput, "[%d], %s, %s\n", count++, item.id, item.name);
			itemClear(&item); // Release item's Memory
		}
	}

out:
	itemClear(&item);
	if (pOutput) fclose(pOutput); // close file

	return 0;
}

int main()
{
	char *buffer = NULL;
	double size = 0.0;

	//	Step 0: (optional)
	remove(OUTPUT);

	//	Step 1: Download File
	if (Cy_Curl_Download_File(REMOTE_URL, DOWNLOAD_PATH, NULL, &size, NULL) == 0)  return 0;

	//	Step 2: Load Data To Buffer
	buffer = loadData(INPUT, (size_t)size);
	if (buffer == NULL) return 0;

	//	Step 3:	Handle Buffer
	handleBuffer(buffer);

	//	Step 4:	Realse Memory
	if (buffer) free(buffer);

	//	Step 5:	Remove Unused Files
	remove(INPUT);

	return 0;
}
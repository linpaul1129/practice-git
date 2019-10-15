#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(void)
{
    FILE *filein;
	FILE *filein1;
	FILE *fileout;
	char show[4000];
	char *findnspace, *findspace;
	int i=0, count1=0, count2 = 0;
    filein = fopen("../../../../../tmp/Taiwan", "r");
	filein1 = fopen("../../../../../tmp/Taiwan", "r");
    while((fgets(show,4000,filein)) != NULL) {
		findnspace = strcasestr(show,"Taiwan"); // 找尋Taiwan字串不含空白
		while ( findnspace != NULL ) { 
			count1++;
			findnspace+=6;
			findnspace = strcasestr(findnspace,"Taiwan");
		}
    }
	printf( "'Taiwan' with Capitalization appear in the page %d times\n", count1 );
	count1 = 0;
	while((fgets(show,4000,filein1)) != NULL) {
		findspace = strcasestr(show,"Taiwan "); // 找尋Taiwan字串含空白
		while ( findspace != NULL ) { 
			count1++;
			findspace+=7;
			findspace = strcasestr(findspace,"Taiwan ");
		}
    }
	printf( "'Taiwan ' with Capitalization appear in the page %d times\n", count1 );
	fclose(filein);
	close(filein1);
	return 0;
    
}
 

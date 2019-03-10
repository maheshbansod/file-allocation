#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct dent {
	char fname[100];
	int len; //length of file
	int st; //starting address

	struct dent *next;
} Dent;

Dent *dir = NULL, *last = NULL;

int bitvector[100];
int n;

int addFile(Dent *dent) {
	int i=0,freecount=0;
	while(i<n) {
		freecount = 0;
		while(i<n && bitvector[i++]==1) { //mem'i' is free
			freecount++;
			//printf("<%d>\n", freecount);
			if(freecount == dent->len) {
				//allocate

				dent->st = i-freecount; //set start index
				//add directory entry to dir
				if(dir == NULL) dir = last = dent;
				else {
					last->next = dent;
					last = dent;
				}

				for(i=dent->st;i<dent->st+dent->len;i++) {
					bitvector[i]=0; //set allocated
				}

				return 1;
			}
		}
	}

	return 0; //coudn't allocate
}

int main() {

	Dent *tdent = NULL;
	int choice,i;

	printf("Enter size of memory/no. of blocks: ");
	scanf("%d", &n);

	for(i=0;i<n;i++) {
		bitvector[i]=rand()%5;
	}
	
	do {
		printf("1. Show Bit Vector\n2. Create a file\n3. Show directory\n4. Delete file\n0. Exit\n> ");
		scanf("%d", &choice);
		switch(choice) {
		case 1:
			for(i=0;i<n;i++)
				printf("%d", bitvector[i]);
			printf("\n");
			break;
		case 2:
			tdent = (Dent*)malloc(sizeof(Dent));
			tdent->next = NULL;
			printf("Enter file name: ");
			getchar();
			fgets(tdent->fname , 100, stdin);
			//remove extra \n
			tdent->fname[strlen(tdent->fname)-1]='\0';
			printf("Enter file size(in blocks): ");
			scanf("%d", &tdent->len);
			if(addFile(tdent)) {
				printf("Added successfully.\n");
			} else //no contiguous memory of len or not enough memory
				printf("Couldn't add the file.\n");
			break;
		case 3:
			printf("| Filename | Size | StartIndex |\n");
			printf("+----------+------+------------+\n");
			tdent = dir;
			while(tdent != NULL) {
				printf("|%10s|%6d|%12d|\n",tdent->fname, tdent->len, tdent->st);
				tdent = tdent->next;
			}
			printf("+------------------------------+\n");
			break;
		case 4:
			break;
		case 0:
			printf("Exit");
			break;
		default: printf("Invalid choice\n");
			break;
		}
	} while(choice != 0);
}

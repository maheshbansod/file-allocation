#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct dent {
	char fname[100];
	int st;
	int last;

	struct dent *next;
} Dent;

Dent *dir = NULL, *last = NULL;

int mem[100]={-1}; //each entry contains index of next block, it contains -1 if it's last block

int bitvector[100];
int freen; //number of free blocks
int n;

void calculateFreeBlocks() {
	int i;
	freen = 0;
	for(i=0;i<n;i++) if(bitvector[i]==1) freen++;
}

int addFile(Dent *dent, int len) {
	int i=0,j=0,freecount=0;

	if(freen < len) return 0; //not enough space

	freen -= len; //number of free blocks is decreased to allocate the file

	while(i<n && bitvector[i]!=1) i++; //find first free block
	dent->st = i;
	bitvector[i]=0;
	len--;
	
	j=i+1;
	while(j<n && len > 0) {
		if(bitvector[j]==1) {
			bitvector[j]=0;
			mem[i] = j; //set next pointer
			i=j;
			len--;
		}
		j++;
	}
	dent->last = i;

	//Add directory entry to dir
	if(dir == NULL) dir = last = dent;
	else {
		last->next = dent;
		last = dent;
	}
	
	return 1;
}

int main() {

	Dent *tdent = NULL;
	int choice,i,len;

	printf("Enter size of memory/no. of blocks: ");
	scanf("%d", &n);

	for(i=0;i<n;i++) {
		bitvector[i]=rand()%2;
	}

	calculateFreeBlocks();
	
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
			scanf("%d", &len);
			if(addFile(tdent,len)) {
				printf("Added successfully.\n");
			} else //no contiguous memory of len or not enough memory
				printf("Couldn't add the file.\n");
			break;
		case 3:
			printf("| Filename | StartBlockNo | LastBlockNo |\n");
			printf("+----------+--------------+-------------+\n");
			tdent = dir;
			while(tdent != NULL) {
				printf("|%10s|%14d|%13d|\n",tdent->fname, tdent->st, tdent->last);
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

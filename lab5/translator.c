#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PT_SIZE 256
#define BUFFER_SIZE 256
#define MEM_SIZE 256
#define TLB_SIZE 16

typedef struct tlb {
	unsigned char page[16];
	unsigned char frame[16];
	int ind;
} TLB;

void findPage(FILE* out, int virtMem, char* PT, TLB *tlb,  char* pm, int* oFrame);
int readDisk (int pageNum, char *pm, int* oFrame);

int main (int argc, char* argv[]) {
	if (argc < 2){
		printf("Error: Too few arguments\nUsage: ./a.out [filename]\n");
		exit(1);
	} else if (argc > 2) {
      printf("Error: Too many arguments\nUsage: ./a.out [filename]\n");
      exit(1);
   }
	int val;
	FILE* f;
   FILE* out;
	int oFrame = 0;
	unsigned char PageTable[PT_SIZE];
	char physMem[MEM_SIZE][MEM_SIZE]; 
	memset(PageTable, -1, sizeof(PageTable));	
	TLB tlb;	
	memset(tlb.page, -1, sizeof(tlb.page));
	memset(tlb.frame, -1, sizeof(tlb.frame));
	tlb.ind = 0;
   if (!(f = fopen(argv[1], "r"))) {
      printf("Error: File %s failed to open\n", argv[1]);
      exit(1);
   }
   if (!(out = fopen("output.txt", "w"))) {
      printf("Error: Could not open output.txt for writing\n");
      exit(1);
   }
	while (fscanf(f, "%d", &val)==1){
		findPage(out, val, PageTable, &tlb, (char*)physMem, &oFrame);
	}
   fclose(out);
	fclose(f);
	return 0;
}

void findPage(FILE* out, int virtMem, char* PT, TLB *tlb,  char* pm, int* oFrame) {
	unsigned char offset;
	unsigned char pageNum;
	int hit = 0;
	int frame = 0;
	int value;
	int newFrame = 0;
	int i;
   int index;
   fprintf(out,"Virtual address: %d ", virtMem);
   pageNum = (virtMem & 65280) >> 8;
   offset = virtMem & 255;
	for (i = 0; i < TLB_SIZE; i++){
		if(tlb->page[i] == pageNum){
			frame = tlb->frame[i];
			hit = 1;
         break;
		}
	}
	if (!hit){
		if (PT[pageNum] == -1) {
			PT[pageNum] = readDisk(pageNum, pm, oFrame);
		}
		frame = PT[pageNum];
		tlb->page[tlb->ind] = pageNum;
		tlb->frame[tlb->ind] = PT[pageNum];
		tlb->ind = (tlb->ind + 1) % TLB_SIZE;
	}
	index = ((unsigned char)frame * MEM_SIZE) + offset;
	value = *(pm+index);
	fprintf(out,"Physical address: %d Value: %d\n", index, value);
}

int readDisk (int pageNum, char *pm, int* oFrame) {
	char buffer[BUFFER_SIZE];
	int i;
	FILE *backStore;
	memset(buffer, 0, sizeof(buffer));
	if (!(backStore = fopen("BACKING_STORE.bin", "rb"))) {
		printf("Error: File BACKING_STORE.bin could not open\n");
		exit(1);
	}
	if (fseek(backStore, pageNum * MEM_SIZE, SEEK_SET) != 0) {
		printf("Error: fseek\n");
      exit(1);
   }
	if (fread(buffer, sizeof(char), MEM_SIZE, backStore) == 0) {
		printf("Error: fread\n");
      exit(1);
   }
	for(i = 0; i < MEM_SIZE; i++){
		*((pm + (*oFrame) * MEM_SIZE) + i) = buffer[i];
	}
   fclose(backStore);
	(*oFrame)++;
	return ((*oFrame)-1);
}

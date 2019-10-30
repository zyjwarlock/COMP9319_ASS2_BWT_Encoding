#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>




#define _MID_LENGTH 30000000
#define _BWT_LENGTH 2500000
#define _read_AUX 10000

typedef struct {
    char key;
    int quantity;
    int *index;
    int currSize;
    int fillin;
} Bucket;

typedef struct {
    int *index;
    int size;
    int currSize;
} Dlist;

int del;

char *tmp_path;

char *source_path;

char *bwt_path;

char *file_source;

char *path_bwt_1;

char *path_bwt_2;

char *path_aux_tmp;

char *position_path;

char *path_aux;

char *bwt_folder;

int del_qua;

int *del_arr;

char *bwt_save;

int bwt_cur_index;

int filesize;



void merge();

void printtest();

int Cmp(char *source, int left, int right);

int hoare_partition(char *str, int *source, int head, int tail);

void Qsort(char *str, int *source, int head, int tail);

//void encode(char *tmpfolder, int del, char *source, int times);
void encode_large(int start, int end, int times);

void encode_small();

void read_Aux();
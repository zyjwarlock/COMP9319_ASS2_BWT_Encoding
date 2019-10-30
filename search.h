#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include"bitmap.h"
//#include<io.h>

#define SPL 1000

//int *occ_arr;

int del;

char *bwt_source;

char *index_path;

char *option;

char *search_code;

char *occ_path;

char *c_path;

int *c_table;

char *aux_path;

int filesize;



void occ_Table();

void get_c_Table();

void search_a(int *del_arr);

void search_i(int del_left, int del_right, int length);

void search_m();

int search_n(int *del_arr);

//int atoi(char *str);
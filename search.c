#include "search.h"

int get_occ_Table(FILE *b_file, FILE *o_file, char ch, int row){

		

		int index = row/SPL;
		//printf("row: %d\n", row);


		int order = row%SPL;
		//printf("order: %d\n", order);

		int occ[1] = {0};

		fseek(o_file, sizeof(int)*(index*130+ch), SEEK_SET);

		fread(occ, sizeof(int), 1, o_file);

		//printf("%c %d: %d\n", ch, (index+1)*SPL, occ[0]);


		fseek(b_file, index*SPL, SEEK_SET);

		char *bwt_tra = (char *)calloc(sizeof(char), (SPL+1));

		fread(bwt_tra, sizeof(char), SPL, b_file);

		//printf("bwt: %s\n", bwt_tra);

		int _i;

		for (_i=strlen(bwt_tra); _i>order; _i--){
			if(bwt_tra[_i-1] == ch){
				occ[0]--;
			}
		}
		//printf("%c %d: %d\n", ch, index, occ[0]);

		free(bwt_tra);

		return occ[0];

}

int is_exist(int v,int *data){//v是某值，data是数组{
	int i;
	for(i=0;i<sizeof(data)/sizeof(int);i++){
		if(v==data[i]) return 1;//找到值返回1
		if(data[i]==0) return 0;
	}
	return 0;//找不到，返回0
	
}

int hoare_partition(int *arr, int head, int tail){

	int x = arr[head];
    int i = head - 1;
    int j = tail + 1;
    while (1) {
        do {
            j--;
            //printf("x:%d sourcej:%d\n", x, source[j]);
        } while (x < arr[j]);//小的在前
        do {
            i++;
            //printf("x:%d sourcei:%d\n", x, source[j]);
        } while (arr[i] < x);
        if (i < j) {
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        } else return j;
    }

}

void qSort(int *arr, int head, int tail){

	if (head >= tail)
        return;

    int mid = hoare_partition( arr, head, tail);


    qSort(arr, head, mid);
    qSort(arr, mid + 1, tail);
}


void occ_Table(){

	FILE *bwt_file = fopen(bwt_source, "r");

	FILE *occ_file = fopen(occ_path, "w");

	if(bwt_file == NULL){
		printf("BWT file cannot be found\n");
		return;
	}


	fseek(bwt_file, 0, SEEK_END);

	size_t filesize_bwt = ftell(bwt_file);

	fseek(bwt_file, 0, SEEK_SET);

	int bwt_file_length = filesize_bwt;

	int i;

	

	//printf("bwt_file_length: %d\n", bwt_file_length);
	//printf("occ size: %d\n", sizeof(occ_arr));

	//printf("%d\n", bwt_file_length);



	int *occ_arr = (int *)calloc(sizeof(int), 130);

	char ch[1];

	//printf("%s\n", occ_path);

	for(i=1; i<=bwt_file_length; ++i){

		//char *ch = (char *)malloc(sizeof(char)*2);

		fread(ch, sizeof(char), 1, bwt_file);

		occ_arr[ch[0]]++;

		//printf("%s ============== %d\n", ch, occ_arr[ch[0]]);
		if(i%SPL == 0){
			//printf("i :%d\n", i);

			fwrite(occ_arr, sizeof(int), 130, occ_file);
			
		}
	}

	

	fwrite(occ_arr, sizeof(int), 130, occ_file);

	fclose(bwt_file);

	fclose(occ_file);

	FILE *c_file = fopen(c_path, "w");

	int sum = occ_arr[del];

	for (i=0; i<130; i++){

		if(i == del){
			int _del = 0;
			fwrite(&_del, sizeof(int), 1, c_file);
			//printf("%c: %d\n", i, _del);
		}
		else{
			if(occ_arr[i] == 0){
				fwrite(&occ_arr[i], sizeof(int), 1, c_file);
				//printf("%c: %d\n", i, occ_arr[i]);
			}

			else{
				fwrite(&sum, sizeof(int), 1, c_file);
				//printf("%c: %d\n", i, sum);
				sum += occ_arr[i];
			}
		}
		
	}

	fclose(c_file);
	free(occ_arr);

}

void get_c_Table(){

	FILE *file = fopen(c_path, "r");

	c_table = (int *)calloc(sizeof(int), 130);

	fread(c_table, sizeof(int), 130, file);

	fclose(file);
	
	/*
	for(i=0; i<130; i++){
		printf("%c, %d\n",i, c_table[i]);
	}*/

}

int get_Next_c(FILE *b_file, char ch){

	int length = 130;
	int i = (int) ch;
	//int next_c;

	if((int)ch ==del ){
		i = -1;
	}

	for (i=i+1; i<length; i++){
		
		if(c_table[i] != 0){
			
			return c_table[i];
		}
	}

	fseek(b_file, 0, SEEK_END);
	int next_c = ftell(b_file);
	fseek(b_file, 0, SEEK_SET);

	return next_c;

}

void backward_search(int *res){

	int i = strlen(search_code);
	
	char charter = search_code[--i];
	
	int _first = c_table[(int)charter]+1;

	if(_first==1){
		//printf("==============\n");

		return;

	}
	
	//printf("first: %d\n", _first);
	

	FILE *bwt_file = fopen(bwt_source, "r");

	int _last = get_Next_c(bwt_file, charter);
	
	//printf("last: %d\n", _last);
	
	FILE *occ_file = fopen(occ_path, "r");
	


	while((_first<=_last) && (i>=1)){

		charter = search_code[i-1];

		//printf("c: %c\n", charter);
		//printf("ctable: %d\n", c_table[charter]);


		_first = c_table[charter] + get_occ_Table(bwt_file, occ_file, charter, _first-1) + 1;

		//printf("first: %d\n", _first);
		
		
		_last = c_table[charter] + get_occ_Table(bwt_file, occ_file, charter, _last);

		//printf("last: %d\n", _last);
		i = i-1;	

	}

	//printf("%d %d\n", _first, _last);

	fclose(bwt_file);
	fclose(occ_file);

	res[0] = _first;
	res[1] = _last;


	//printf("%d, %d\n",_first, _last);

}

void search_i(int i, int j, int del_len){

	
	int index = i;

	int length=5000;
	


	FILE *file_bwt = fopen(bwt_source, "r");

	//printf("%s\n", bwt_source);
	//printf("%s\n", occ_path);
	FILE *file_occ = fopen(occ_path, "r");
	char *output = (char *)calloc(sizeof(char), 5001);
	memset(output, '\0', sizeof(char)*5001);

	//printf("%d : %c \n",del, del);

	int times=0;

	while(!(i>j)){

		
		fseek(file_bwt, index-1, SEEK_SET);
		char *ch = (char *)calloc(sizeof(char), 2);
		fread(ch, sizeof(char), 1, file_bwt);

		//printf("%s\n", ch);
		


		if(ch[0] == del ){
			i++;
			//if(i==del_len){
			//	index = 0;
			//}
			//else{
				index = i;
			//}
			//printf("=================\n");
			if(length == 5000){
				printf("\n");
			}else{
				printf("%s\n", &output[length]);
				memset(output, '\0', sizeof(char)*5001);
			}
			continue;
		}
		output[--length] = ch[0];
	
		int occ = get_occ_Table(file_bwt, file_occ, ch[0], index);

		index = c_table[ch[0]]+occ;

		//printf("=======%d==========\n", times++);
		//if(times>6000) break;

	}

	
	fclose(file_bwt);
	fclose(file_occ);

}

void search_m(){
	
	int res[2] = {0, 0};
	backward_search(res);
	
	//printf("%d %d\n", res[0], res[1]);

	if(res[1] < res[0] || !res[0] || !res[0]){
		printf("0\n");
	}
	else{
		//printf("first: %d  last: %d\n", _first, _last);
		printf("%d\n", (res[1] - res[0] + 1));
	}

}

int search_n(int *del_arr){

//int *del_arr = (int *)calloc(sizeof(int), length);

	//printf("===============%d\n", length);
	int res[2] = {0, 0};
	backward_search(res);

	//return 0;

	//printf("%d %d\n", res[0], res[1]);

	if(res[1] < res[0] || !res[0] || !res[0]){
		return 0;
	}
	
	int i = res[0];
	int j = res[1];
	int index=i;
	int cur = 0;

	//printf("================\n");

	
	

	FILE *file_bwt = fopen(bwt_source, "r");
	FILE *file_occ = fopen(occ_path, "r");

	char ch[1];
	
	while(!(i>j)){

		//printf("==========%d==========\n", cur);
	
		
		fseek(file_bwt, index-1, SEEK_SET);
		fread(ch, sizeof(char), 1, file_bwt);

		

		if(ch[0] == del ){
			//printf("%d\n", index);
			//printf("%d\n", index);
			int flag = test(index, del_arr);
			//printf("flag %d\n", flag);
			if(!flag){
				set(index, del_arr);
				cur++;
			}
			i++;
			index = i;
			
			
			continue;
		}
		
	
		int occ = get_occ_Table(file_bwt, file_occ, ch[0], index);

		index = c_table[ch[0]]+occ;


	}
	


	

	fclose(file_bwt);
	fclose(file_occ);

	
	return cur;
}

void search_a(int *del_arr){


	search_n(del_arr);


	int i;
	//printf("length :%d\n", length);
	int *order_arr = (int *)calloc(sizeof(int), (1+filesize/32));
	
	for(i=0; i<=filesize; i++){
        clr(i, order_arr);
        
    }

	FILE *bwt_file = fopen(bwt_source, "r");
	FILE *occ_file = fopen(occ_path, "r");
	FILE *aux_file = fopen(aux_path, "r");

	for(i=0; i<=filesize; i++){
		if(!test(i, del_arr) ) continue;
		//printf("flag: %d\n", i);
		//clr(i, del_arr);

		int s = get_occ_Table(bwt_file, occ_file, (char)del, i);
		//printf("%d\n", del_arr[i]);
		int aux_i[1];
		fseek(aux_file, sizeof(int)*(s-1), SEEK_SET);
		fread(aux_i, sizeof(int), 1, aux_file);
		//printf("%d\n", aux_i[0]);
		//printf("flag: %d\n", test(aux_i[0], order_arr));
		if(!test(aux_i[0], order_arr)){
			set(aux_i[0], order_arr);
		}

		//printf("%d\n", del_arr[i]);
	}


	fclose(bwt_file);
	fclose(occ_file);
	fclose(aux_file);
	

	//printf("%d", del_arr[0]);

	// for(i=0; i<length; i++){
	// 	if(del_arr[i] == 0) continue;
	// 	else{
	// 		printf("%d\n", del_arr[i]);
	// 	}
	// }

	for(i=0; i<=filesize; i++)
    {
        
        if(test(i, order_arr)){
            printf("%d\n", i);
        }
    }


	//printf("================\n");
	/*

	int *del_pos = (int *)calloc(sizeof(int), length);
	
	FILE *pos_file = fopen(position_path, "r");

	fread(del_pos, sizeof(int), length, pos_file);

	fclose(pos_file);*/


}



#include "search.h"




void is_table_exist(){

    FILE *occ_file = fopen(occ_path, "r");
    FILE *c_file = fopen(c_path, "r");
    
    if(occ_file == NULL || c_file == NULL){

        if(!(occ_file == NULL)) fclose(occ_file);
        if(!(c_file == NULL)) fclose(c_file);
        
        occ_Table();


    }
    else{
        //get_c_Table(c_file);
        
        //printtest(occ_file, c_file);

        fclose(occ_file);
        fclose(c_file);

    }
}


int main( int argc, char *argv[] ){

    char *deli = argv[1];

    if(strcmp(deli, "\\n")==0){
        del = 10;
    }
    else if(strcmp(deli, "\\t")==0){
        del = 9;
    }
    else{
        del=deli[0];
    }

    

    bwt_source = argv[2];

	index_path = argv[3];

	option = argv[4];

	search_code = argv[5];

    int i, j;

    for(i=strlen(bwt_source); i>0; i--){
        if(bwt_source[i-1] == '.'){
            aux_path = (char *)calloc(sizeof(char),(i+9));
            for (j = 0; j < i; j++)
            {
               aux_path[j] = bwt_source[j];
            }
            break;
        }
    }

	occ_path = (char *)calloc(sizeof(char), (strlen(index_path)+10));
	c_path = (char *)calloc(sizeof(char), (strlen(index_path)+10));


    FILE *bwt_file = fopen(bwt_source, "r");
    fseek(bwt_file, 0, SEEK_END);
    filesize = ftell(bwt_file);
    fclose(bwt_file);

	strcpy(occ_path, index_path);
	strcpy(c_path, index_path);
    //strcpy(position_path, bwt_source);
	
	strcat(occ_path, "/occ_table");
	strcat(c_path, "/c_table");
    strcat(aux_path, "aux");

    //printf("%s\n", aux_path);

    //printf("%s\n", position_path);
    
    //printf("======1=========\n");

	//printf("%s\n", occ_path);


	if(strcmp(option, "-a")==0){

        is_table_exist();

        get_c_Table();

        //printf("=====================\n");

        int *del_arr = (int *)calloc(sizeof(int), (1+filesize/32));
        //int order_arr[1+N/32]
        //printf("%d\n", filesize);

        int i;

        for(i=0; i<filesize; i++){
            clr(i, del_arr);
            
        }


        search_n(del_arr);

        //printf("=====================\n");
        
        search_a(del_arr);
        


        
    }
    else if(strcmp(option, "-i")==0){

        is_table_exist();
        
        char *num_1 = (char *)calloc(sizeof(char), strlen(search_code));
        char *num_2 = (char *)calloc(sizeof(char), strlen(search_code));

        int i, j;
        for(i=0; i<strlen(search_code); i++){
        	if(search_code[i] == '\t' ||  search_code[i] == ' '){
        		break;
        	}
        	num_1[i] = search_code[i] ;
        }

        for(i=i, j=0; i<strlen(search_code); i++){
        	if(search_code[i] == '\t' ||  search_code[i] == ' '){
        		continue;
        	}
        	num_2[j++] = search_code[i]; 
        }

        int dol_left = atoi(num_1);
        int dol_right = atoi(num_2);

        free(num_1);
        free(num_2);

        get_c_Table();

        int length;

        for(i=0; i<130; i++){
            if(c_table[i]!=0){
                length = c_table[i]-c_table[del];
                break;
            }
        }

        search_i(dol_left, dol_right, length);
        //printf("%d %d\n",dol_left, dol_right );

    }
    else if(strcmp(option, "-m")==0){

        is_table_exist();

        get_c_Table();

        search_m();

    }
    else if(strcmp(option, "-n")==0){

        
        is_table_exist();

        get_c_Table();

        int length = 0;

        /*
        for(i=0; i<130; i++){
            if(c_table[i]!=0){
                length = c_table[i]-c_table[del];
                break;
            }
        }*/

        

        //printf("%d\n", length);
        //int *del_arr = (int *)calloc(sizeof(int), length);
        int *del_arr = (int *)calloc(sizeof(int), (1+filesize/32));

        //printf("%d\n", filesize);

        int i;

        for(i=0; i<filesize; i++){
            clr(i, del_arr);
        }


        i = search_n(del_arr);
        

        
       
        
        printf("%d\n", i);
        

    }
    else{

        return 0;
    }

    //printf("================\n");
    
    return 1;


}
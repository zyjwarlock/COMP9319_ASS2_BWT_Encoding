
#include "encode.h"


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

    tmp_path = argv[2];
    source_path = argv[3];
    bwt_path = argv[4];


    int bwt_folder_length = 0;

    int i;
    int j;
    
    for(i=strlen(bwt_path); i>0; i--){
        if(bwt_path[i-1] == '.'){
            position_path = (char *)calloc(sizeof(char), (i+4));
            path_aux = (char *)calloc(sizeof(char), (i+4));
            for (j = 0; j < i; j++)
            {
               position_path[j] = bwt_path[j];
               path_aux[j] = bwt_path[j];
            }
            break;
        }
    }
    
    if(position_path == NULL){
        position_path = (char *)calloc(sizeof(char), (5));
    }
    if(path_aux == NULL){
        path_aux = (char *)calloc(sizeof(char), (5));
    }

    path_bwt_1 = (char *)calloc(sizeof(char), (strlen(tmp_path)+8));
    path_bwt_2 = (char *)calloc(sizeof(char), (strlen(tmp_path)+8));
    path_aux_tmp = (char *)calloc(sizeof(char), (strlen(tmp_path)+11));

    
    strcpy(path_bwt_1, tmp_path);
    strcpy(path_bwt_2, tmp_path);

    strcpy(path_aux_tmp, tmp_path);


    strcat(path_bwt_1, "/bwt_1");
    strcat(path_bwt_2, "/bwt_2");
    strcat(position_path, "pos");
    strcat(path_aux_tmp, "/aux");
    strcat(path_aux, "aux");




    
    
    FILE *file = fopen(source_path, "r");




    if(file == NULL){
        printf("Cannot open file: %s\n", source_path);
    }
    
   
    fseek(file, 0, SEEK_END);

    filesize = ftell(file);
    
    fseek(file, 0, SEEK_SET);




    file_source = (char *)calloc(sizeof(char), (filesize+1));
    
    fread(file_source, sizeof(char), filesize, file);

    fclose(file);



    if(filesize > _MID_LENGTH){

        encode_large(0, _MID_LENGTH-1, 0);
        encode_large(_MID_LENGTH, filesize-1, 1);
        merge();

    }else{

        encode_small();
    
    }

    
    
    free(bwt_save);

    free(del_arr);
    

    free(file_source);
    free(path_bwt_1);
    free(path_bwt_2);
    free(position_path);

    read_Aux();


    //printf("merge\n");

    //printtest();

    return 1;
}
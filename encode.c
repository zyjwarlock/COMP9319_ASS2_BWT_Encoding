#include "encode.h"



int Cmp(char *source, int left, int right){
    

    while(source[left]==source[right] && source[left]!= del){
        left+=1;
        right+=1;
    }

    if(source[left]==del && source[right]!=del){
        return 1;
    }
    else if(source[left]==del && source[right]==del){
        if(left<right)
            return 1;
    }
    else if(source[left]!=del && source[right]!=del){
        if(source[left]<source[right])
            return 1;
    }

    return 0;
    
}


int hoare_partition(char *str, int *source, int head, int tail) {
    
    int x = source[head];
    int i = head - 1;
    int j = tail + 1;
    while (1) {
        do {
            j--;
            //printf("x:%d sourcej:%d\n", x, source[j]);
        } while (Cmp(str, x,  source[j]));//小的在前
        do {
            i++;
            //printf("x:%d sourcei:%d\n", x, source[j]);
        } while (Cmp(str, source[i], x));
        if (i < j) {
            int tmp = source[i];
            source[i] = source[j];
            source[j] = tmp;
        } else return j;
    }

}

int hoare_partition_aux(int *arr, int *index, int head, int tail) {
    
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
        } while (x > arr[i]);
        if (i < j) {
            int tmp_arr = arr[i];
            int tmp_index = index[i];
            
            arr[i] = arr[j];
            index[i] = index[j];

            arr[j] = tmp_arr;
            index[j] = tmp_index;
        } else return j;
    }

}

void Qsort(char *str, int *source, int head, int tail){

    if (head >= tail)
        return;

    int mid = hoare_partition(str, source, head, tail);


    Qsort(str, source, head, mid);
    Qsort(str, source, mid + 1, tail);

}

void sort_Aux(int *arr, int *index, int head, int tail){

    if (head >= tail)
        return;

    int mid = hoare_partition_aux(arr, index, head, tail);


    sort_Aux(arr, index, head, mid);
    sort_Aux(arr, index, mid + 1, tail);

}



void read_Aux(){

    FILE *rfile = fopen(path_aux_tmp, "r");

    int i;

    int *aux_arr = (int *)calloc(sizeof(int), del_qua);

    fread(aux_arr, sizeof(int), del_qua, rfile);

    fclose(rfile);

    int *aux_index = (int *)calloc(sizeof(int), del_qua);

    aux_index[0] = 1;

    for(i=1; i<del_qua; i++){
        aux_index[i] = aux_index[i-1]+1;
    }


    sort_Aux(aux_arr, aux_index, 0, del_qua-1);

    

    for(i=0; i<del_qua; i++){

        aux_arr[i] = i+1;
        
    }
   

    sort_Aux(aux_index, aux_arr, 0, del_qua-1);
    //printf("%d\n", del_qua);

    for(i=0; i<del_qua; i++){
        //printf("%d\n", aux_arr[i]);
        if(aux_arr[i]+1 > del_qua){
            aux_arr[i] = 1;
        }
        else{
            aux_arr[i] += 1;
        }
        
        //printf("%d\n", aux_arr[i]);
    }




    FILE *wfile = fopen(path_aux, "w");

    fwrite(aux_arr, sizeof(int), del_qua, wfile);

    fclose(wfile);

    unlink(path_aux_tmp);


}

int merge_sort(int *arrA, int *arrB, int i, int j, int lengthA, int lengthB, FILE *wfile, FILE *afile){

    

    while(i<lengthA && j<lengthB){

        if(Cmp(file_source, arrA[i], arrB[j])){
            
            int index = arrA[i]-1 < 0 ? filesize - 1 :  arrA[i]-1;
            bwt_save[bwt_cur_index++] = file_source[index];
            
            if(file_source[index] == del){
                del_arr[del_qua%_read_AUX] = index;
                del_qua++;
                if(del_qua%_read_AUX==0){
                    fwrite(del_arr, sizeof(int), _read_AUX, afile);
                    memset(del_arr, 0, sizeof(int));
                }
            }

            i++;
        }
        else{
            
            int index = arrB[j]-1 < 0 ? filesize - 1 :  arrB[j]-1;
            bwt_save[bwt_cur_index++] = file_source[index];
            
            if(file_source[index] == del){
                del_arr[del_qua%_read_AUX] = index;
                del_qua++;
                if(del_qua%_read_AUX==0){
                    fwrite(del_arr, sizeof(int), _read_AUX, afile);
                    memset(del_arr, 0, sizeof(int));
                }
            }

            j++;
        }
    }


    while(j<lengthB){

        
        int index = arrB[j]-1 < 0 ? filesize - 1 :  arrB[j]-1;
        bwt_save[bwt_cur_index++] = file_source[index];
        

        if(file_source[index] == del){
            del_arr[del_qua%_read_AUX] = index;
            del_qua++;
            if(del_qua%_read_AUX==0){
                fwrite(del_arr, sizeof(int), _read_AUX, afile);
                memset(del_arr, 0, sizeof(int));
            }
        }

        j++;
    }

    return i;

}


void merge(){

    bwt_save = (char *)calloc(sizeof(char), (filesize+1));
    
    FILE *fileA = fopen(path_bwt_1, "r");

    FILE *fileB = fopen(path_bwt_2, "r");

    del_arr = (int *)calloc(sizeof(int),_read_AUX);

    bwt_cur_index=0;


    if(fileA == NULL || fileB == NULL){
        printf("Something goes wrong\n");
    }
    else{

        fseek(fileA, 0, SEEK_END);

        size_t filesizeA = ftell(fileA);
        
        int filelengthA = filesizeA/(sizeof(int)/sizeof(char));

        fseek(fileA, 0, SEEK_SET);

        int *bwt_1 = (int *)calloc(sizeof(int), (filelengthA+1));
        
        fread(bwt_1, sizeof(int), filelengthA, fileA);
        
        fclose(fileA);


        //int length = strlen(file_source);
        FILE *bwt = fopen(bwt_path, "w");

        FILE *aux = fopen(path_aux_tmp, "w");

       

        fseek(fileB, 0, SEEK_END);

        size_t filesizeB = ftell(fileB);

        size_t readlength = filesizeB > _BWT_LENGTH ? _BWT_LENGTH : filesizeB;
        
        fseek(fileB, 0, SEEK_SET);

        size_t totalBytesRead = 0;

        int position_A = 0;

        int i;

        

        while(_BWT_LENGTH < filesizeB){

            int filelengthB = _BWT_LENGTH/(sizeof(int)/sizeof(char));

            int *bwt_2 = (int *)calloc(sizeof(int), (filelengthB));
            
            fread(bwt_2, sizeof(int), filelengthB, fileB);

            filesizeB -= _BWT_LENGTH;


            position_A = merge_sort(bwt_1, bwt_2, position_A, 0, filelengthA, filelengthB, bwt, aux);

            free(bwt_2);



        }

        if(filesizeB>0){

            int filelengthB = filesizeB/(sizeof(int)/sizeof(char));

            int *bwt_2 = (int *)calloc(sizeof(int), (filelengthB));

            fread(bwt_2, sizeof(int), filelengthB, fileB);

           

            position_A = merge_sort(bwt_1, bwt_2, position_A, 0, filelengthA, filelengthB, bwt, aux);

            free(bwt_2);

        }

        fclose(fileB);

        unlink(path_bwt_2);
        
        for(i=position_A; i<filelengthA; i++){
            

            int index = bwt_1[i]-1 < 0 ? filesize - 1 :  bwt_1[i]-1;

            bwt_save[bwt_cur_index++] = file_source[index];
            

            if(file_source[index] == del){

                del_arr[del_qua%_read_AUX] = index;
                del_qua++;
                if(del_qua%_read_AUX==0){
                    fwrite(del_arr, sizeof(int), _read_AUX, aux);
                    memset(del_arr, 0, sizeof(int));
                }
            }
        }

        if(del_qua%_read_AUX>0){
            fwrite(del_arr, sizeof(int), del_qua%_read_AUX, aux);
        }

        
        fwrite(bwt_save, sizeof(char), bwt_cur_index, bwt);

        fclose(aux);

        fclose(bwt);

        unlink(path_bwt_1);

        free(bwt_1);
        
    }

}

void encode_large(int start, int end, int times){




    Bucket *arrBuck = (Bucket *)calloc(sizeof(Bucket), 130);

    int i = 0;
    int j = 0;

    del_qua = 0;



    for (i=0; i<130; i++){
        arrBuck[i].key = i;
        arrBuck[i].quantity = 0;
        arrBuck[i].currSize = 0;
    }



    
    for (i=start; i<=end; i++){
        
        arrBuck[file_source[i]].quantity++;       
    }

    
    for (i=0; i<130; i++){
        arrBuck[i].index = (int *)calloc(sizeof(int), arrBuck[i].quantity);
    }
    
    

    //printf("%d\n", length);

    for (i=start; i<=end; i++){

        //printf("%d\n", i);
        arrBuck[file_source[i]].index[arrBuck[file_source[i]].currSize++] = i;
       
    }



   

    for (i=0; i<130; i++){
        if(i==del){
            //sortDel(arrBuck[i].index, arrBuck[i].currSize);
            continue;
        }
        if(arrBuck[i].currSize>0){

            Qsort(file_source, arrBuck[i].index, 0, arrBuck[i].currSize-1);
        }
    }




    char *path = (char *)calloc(sizeof(char), (strlen(tmp_path)+7));


    if(times == 0) {

        strcpy(path, path_bwt_1);

    }
    else{

        strcpy(path, path_bwt_2);
      
    }


    //printf("%s\n", path);



    FILE *file = fopen(path, "w");

    if(file == NULL){
        printf("no such file\n");
    }


    fwrite(arrBuck[del].index, sizeof(int), arrBuck[del].currSize, file);

    

    for (i=0; i<130; i++){

        if(i!=del && arrBuck[i].currSize>0){

            fwrite(arrBuck[i].index, sizeof(int), arrBuck[i].currSize, file);

        }
    }



    //printf("====================\n");

    fclose(file);

    for (i=0; i<130; i++){
        free(arrBuck[i].index);
    }

    free(arrBuck);


}

void encode_small(){

    Bucket *arrBuck = (Bucket *)calloc(sizeof(Bucket), 130);

    bwt_save = (char *)calloc(sizeof(char), (filesize+1));

    del_arr = (int *)calloc(sizeof(int),_read_AUX);

    int i = 0;
    int j = 0;

    del_qua = 0;

    bwt_cur_index=0;



    for (i=0; i<130; i++){

        arrBuck[i].key = i;
        arrBuck[i].quantity = 0;
        arrBuck[i].currSize = 0;

    }



    
    for (i=0; i<filesize; i++){
        
        arrBuck[file_source[i]].quantity++;       
    }

    
    for (i=0; i<130; i++){
        arrBuck[i].index = (int *)calloc(sizeof(int), arrBuck[i].quantity);
    }
    
    
    //printf("==========put in Bucket============\n");
    //printf("%d\n", length);

    for (i=0; i<filesize; i++){

        //printf("%d\n", i);
        arrBuck[file_source[i]].index[arrBuck[file_source[i]].currSize++] = i;
       
    }



   //printf("==========ready for sort============\n");

    for (i=0; i<130; i++){
        if(i==del){
            //sortDel(arrBuck[i].index, arrBuck[i].currSize);
            continue;
        }
        if(arrBuck[i].currSize>0){
            //printf("===============sort %c===================\n", i);
            Qsort(file_source, arrBuck[i].index, 0, arrBuck[i].currSize-1);
        }
    }

    //printf("===============sort completed===================\n");


    char *path = (char *)calloc(sizeof(char), (strlen(tmp_path)+7));


    //printf("%s\n", path);

    //printf("====================\n");



    FILE *bwt = fopen(bwt_path, "w");

    FILE *aux = fopen(path_aux_tmp, "w");

    if(aux==NULL){
        printf("%s\n", path_aux_tmp);
        return;
    }

    
    //printf("===============writing file===================\n");
    for(i=0; i<arrBuck[del].currSize; i++){

        if(arrBuck[del].index[i] == 0){
            bwt_save[bwt_cur_index] = file_source[filesize-1];
        }
        else{
            bwt_save[bwt_cur_index] = file_source[arrBuck[del].index[i]-1];
        }
        
        if(bwt_save[bwt_cur_index] == del){
            del_arr[del_qua%_read_AUX] = (arrBuck[del].index[i]-1 < 0) ? (filesize-1) : arrBuck[del].index[i]-1;
            del_qua++;
            if(del_qua%_read_AUX==0){
                fwrite(del_arr, sizeof(int), _read_AUX, aux);
                memset(del_arr, 0, sizeof(int));
            }
        }
        bwt_cur_index++;

    }
    //printf("===============%c writed===================\n", del);

    for (i=0; i<130; i++){

        if(i!=del && arrBuck[i].currSize>0){

            //printf("===============%c writing===================\n", i);

            for(j=0; j<arrBuck[i].currSize; j++){

                if(arrBuck[i].index[j] == 0){
                    bwt_save[bwt_cur_index] = file_source[filesize-1];
                    
                }
                else{
                    bwt_save[bwt_cur_index] = file_source[arrBuck[i].index[j]-1];
                }
                
                if(bwt_save[bwt_cur_index] == del){
                    del_arr[del_qua%_read_AUX] = (arrBuck[i].index[j]-1 < 0) ? (filesize-1) : arrBuck[i].index[j]-1;
                    del_qua++;
                    if(del_qua%_read_AUX==0){
                        fwrite(del_arr, sizeof(int), _read_AUX, aux);
                        memset(del_arr, 0, sizeof(int));
                    }
                }
                bwt_cur_index++;

                //printf("%d\n", arrBuck[i].index[j]);
            }
            //printf("===============%c writed===================\n", i);
        }
        
    }

    //printf("=================writing completed=================\n");

    //printf("===============del_arr writing===================\n");
    
    if(del_qua%_read_AUX>0){
        /*
        int i;
        for (i = 0; i < del_qua%_read_AUX; ++i)
        {
            printf("%d\n", del_arr[i]);
        }
        //printf("=====%d\n", del_qua);*/
        fwrite(del_arr, sizeof(int), del_qua%_read_AUX, aux);
    }

    //printf("===============del_arr writed===================\n");


    //printf("=========1===========\n");
    
    fwrite(bwt_save, sizeof(char), filesize, bwt);
    //printf("===============bwt writed===================\n");

    //free(arrBuck);

    for (i=0; i<130; i++){
        free(arrBuck[i].index);
    }

    free(arrBuck);



    fclose(bwt);
    fclose(aux);
}













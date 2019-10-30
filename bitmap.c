#include "bitmap.h"

//int a[1 + N/BITSPERWORD];

void set(int i, int *a) {        	
	
	a[i>>SHIFT] |=  (1<<(i & MASK)); 

}//clr 初始化所有的bit位为0

void clr(int i, int *a) {        	
	a[i>>SHIFT] &= ~(1<<(i & MASK)); 

}
//test 测试所在的bit为是否为1

int  test(int i, int *a){ 	
	return a[i>>SHIFT] &   (1<<(i & MASK)); 
}



// #include <iostream>
#include <stdio.h>
#include <string.h>
#include<assert.h>
// using namespace std;

// void print(int (*parr)[5], int row, int col)
// {
//     for(int i = 0; i < row; ++i)
//     {
//         for(int j = 0; j < col; ++j)
//         {
//             cout << parr[i][j] << " ";
//         }
//         cout << endl;
//     }
// }
// int main()
// {
//     // //一维数组
//     // int a[] = {1,2,3,4}; 
//     // printf("sizeof(a) = %d\n",sizeof(a));  
//     // //16，sizeof数组名代表整个数组的大小
//     // printf("sizeof(a+0) = %d\n",sizeof(a+0)); 
//     // //4，sizeof加数组中的一个元素等于sizeof(int)
//     // printf("sizeof(*a) = %d\n",sizeof(*a)); 
//     // //4，既然不在两个例外，a就代表首元素，*a代表首元素地址解引用，就是首元素的大小，4
//     // printf("sizeof(a+1) = %d\n",sizeof(a+1)); 
//     // //4/8(32位/64位), sizeof(pointer)
//     // printf("sizeof(a[1]) = %d\n",sizeof(a[1])); 
//     // //4，sizeof加数组中的一个元素等于sizeof(int)
//     // printf("sizeof(&a) = %d\n",sizeof(&a)); 
//     // //a取地址，指针大小，4/8
//     // printf("sizeof(*&a) = %d\n",sizeof(*&a));
//     // //&a代表的是又解引用，就是首元素的地址，指针大小，4/8，类型int*
//     // printf("sizeof(&a+1) = %d\n",sizeof(&a+1)); 
//     // //&a代表
//     // printf("sizeof(&a[0]) = %d\n",sizeof(&a[0])); 
//     // printf("sizeof(&a[0]+1) = %d\n",sizeof(&a[0]+1));


//     // //字符数组
//     // char arr[] = {'a','b','c','d','e','f'}; 
//     // printf("sizeof(arr) = %d\n", sizeof(arr)); 
//     // //7，整个数组的大小
//     // printf("sizeof(arr+0) = %d\n", sizeof(arr+0)); 
//     // //4/8，指针大小，指向第0个元素
//     // printf("sizeof(*arr) = %d\n", sizeof(*arr)); 
//     // //1，首元素的大小
//     // printf("sizeof(arr[1]) = %d\n", sizeof(arr[1])); 
//     // //1，第二个元素arr[1]的大小
//     // printf("sizeof(&arr) = %d\n", sizeof(&arr)); 
//     // //4/8，指针大小，代表整个数组的地址
//     // printf("sizeof(&arr+1) = %d\n", sizeof(&arr+1)); 
//     // //4/8指针大小，代表这个数组后面的地址
//     // printf("sizeof(&arr[0]+1) = %d\n", sizeof(&arr[0]+1));
//     // //4/8指针大小，指向第0个元素之后
//     // printf("strlen(arr) = %d\n", strlen(arr));
//     // //随机值，没有'\0'
//     // printf("strlen(arr+0) = %d\n", strlen(arr+0));
//     // //
//     // printf("strlen(*arr) = %d\n", strlen(*arr));
//     // printf("strlen(arr[1]) = %d\n", strlen(arr[1]));
//     // printf("strlen(&arr) = %d\n", strlen(&arr));
//     // printf("strlen(&arr+1) = %d\n", strlen(&arr+1));
//     // printf("strlen(&arr[0]+1) = %d\n", strlen(&arr[0]+1));

//     //随机值


//     // char arr[] = "abcdef";
//     // printf("%d\n", sizeof(arr));
//     // //6，sizeof数组名代表整个数组的大小
//     // printf("%d\n", sizeof(arr+0));
//     // //arr+0 代表首元素的地址，指针大小4/8
//     // printf("%d\n", sizeof(*arr));
//     // //*arr代表首元素地址解引用，就是a的大小，1
//     // printf("%d\n", sizeof(arr[1]));
//     // //b的大小，1
//     // printf("%d\n", sizeof(&arr));
//     // //指针大小，4/8
//     // printf("%d\n", sizeof(&arr+1));
//     // //指针大小，4/8
//     // printf("%d\n", sizeof(&arr[0]+1));
//     // //指针大小，4/8
//     // printf("%d\n", strlen(arr));
//     // //6，""末尾有隐藏'\0'
//     // printf("%d\n", strlen(arr+0));
//     // //6
//     // printf("%d\n", strlen(*arr));
//     // //arr是首元素的地址，*arr就是首元素，a是97，会把97当成一个地址向后查找，可能会出现野指针问题。段错误
//     // printf("%d\n", strlen(arr[1]));
//     // //arr[1]代表b，第二个元素，把98当成一个地址向后查找，野指针。
//     // printf("%d\n", strlen(&arr));
//     // //&arr整个数组的地址，随机值，和strlen(arr)相同
//     // printf("%d\n", strlen(&arr+1));
//     // //随机值
//     // printf("%d\n", strlen(&arr[0]+1));
//     // //随机值
//     // char *p = "abcdef";
//     // printf("%d\n", sizeof(p));
//     // //p是一个字符指针，指针大小4/8
//     // printf("%d\n", sizeof(p+1));
//     // //p类型是char*,p+1指针大小，4/8
//     // printf("%d\n", sizeof(*p));
//     // //*p是char类型，7, +'\0'
//     // printf("%d\n", sizeof(p[0]));
//     // //从p[0]开始数，7，因为还有一个'\0'
//     // printf("%d\n", sizeof(&p));
//     // //p是char*，&p是char**，指针大小4/8
//     // printf("%d\n", sizeof(&p+1));
//     // //&p+1，指针大小
//     // printf("%d\n", sizeof(&p[0]+1));
//     // //&p[0]就是char*类型，+1后，指针大小4/8
//     // printf("%d\n", strlen(p));
//     // //6
//     // printf("%d\n", strlen(p+1));
//     // //5
//     // printf("%d\n", strlen(*p));
//     // //经典野指针
//     // printf("%d\n", strlen(p[0]));
//     // //野指针，随机值
//     // printf("%d\n", strlen(&p));
//     // //随机值
//     // printf("%d\n", strlen(&p+1));
//     // //随机值
//     // printf("%d\n", strlen(&p[0]+1));
//     // //&p[0]是char*类型，等于5
    
//     // char *p = "abcdef";
//     // printf("%d\n", sizeof(p));
//     // //p是一个字符指针，指针大小4/8
//     // printf("%d\n", sizeof(p+1));
//     // //p类型是char*,p+1指针大小，4/8
//     // printf("%d\n", sizeof(*p));
//     // //*p是char类型，7, +'\0'
//     // printf("%d\n", sizeof(p[0]));
//     // //从p[0]开始数，7，因为还有一个'\0'
//     // printf("%d\n", sizeof(&p));
//     // //p是char*，&p是char**，指针大小4/8
//     // printf("%d\n", sizeof(&p+1));
//     // //&p+1，指针大小
//     // printf("%d\n", sizeof(&p[0]+1));
//     // //&p[0]就是char*类型，+1后，指针大小4/8
//     // printf("%d\n", strlen(p));
//     // //6
//     // printf("%d\n", strlen(p+1));
//     // //5
    
//     int a[3][4] = {0}; 
//     printf("%d\n",sizeof(a)); 
//     //sizeof数组名为 整个数组的大小，是12*4 = 48
//     printf("%d\n",sizeof(a[0][0])); 
//     //一个int的大小，4
//     printf("%d\n",sizeof(a[0])); 
//     // a[0]单独存在，就是代表整个数组，在二维数组中，a[0]代表第一行，4*4 = 16
//     printf("%d\n",sizeof(a[0]+1)); 
//     // a[0]并非单独存在，代表首元素的地址，就是a[0][0]的地址，指针大小
//     printf("%d\n",sizeof(*(a[0]+1))); 
//     //a[0]+1，就是a[0][1]的地址，解引用后就是sizeof(int) = 4
//     printf("%d\n",sizeof(a+1)); 
//     //a并非单独存在，代表首元素地址，这里指首行的地址，加1仍为指针大小。
//     printf("%d\n",sizeof(*(a+1))); 
//     //a并非单独存在，代表首行的地址，+1代表第二行的地址，*后为16
//     printf("%d\n",sizeof(&a[0]+1)); 
//     //&a[0]，代表第1行的地址，+1仍为指针大小
//     printf("%d\n",sizeof(*(&a[0]+1))); 
//     //代表第一行的地址，解引用后变为了16
//     printf("%d\n",sizeof(*a)); 
//     //a并非单独存在，代表首行地址，*后，16
//     printf("%d\n",sizeof(a[3]));
//     //a[3]是代表第4行的大小，总共四个元素，16
//     return 0;
// }

// int main()
// {
// 		int a[5] = {1,2,3,4,5};
// 		int *ptr = (int*)(&a + 1);
// 		printf("%d, %d", *(a + 1),*(ptr - 1));
// 		return 0;
// }

// struct Test{
// 		int Num;
// 		char* pcName;
// 		short sDate;
// 		char cha[2];
// 		short sBa[4];
// }*p;

// int main()
// {
//     printf("%p\n", p);
//     printf("%p\n", p + 0x1);
//     printf("%p\n", (unsigned long)p + 0x1);
//     printf("%p\n", (unsigned int*)p + 1);
//     return 0;
// }
// int main()
// {
// 		int a[4] = {1,2,3,4};
// 		// int* ptr1 = (int*)(&a + 1);
// 		int* ptr2 = (int*)((int)a + 1);
// 	//	printf("%x, %x", ptr1[-1], *ptr2);
// 		// printf("%x", *ptr2);
// 		return 0;
// }

// int main()
// {
// 		int a[3][2] = { (0,1), (2,3), (4,5) };
// 		int* p = a[0];
// 		printf("%d", p[0]);
// 		printf("%d",p[1]);
// 		printf("%d",p[2]);
// 		return 0;
// }

// int main()
// {
// 	int a[5][5];
// 	int(*p)[4];
// 	p = (int(*)[4])a;
// 	printf("%p, %d\n", &p[4][2] - &a[4][2], &p[4][2] - &a[4][2]);
// 	return 0;
// }

// int main()
// {
// 	char* a[] = {"work", "at", "home"};
// 	char** pa = a;
// 	pa++;
// 	printf("%s\n", *pa);
// 	return 0;
// }

// int main()
// {
// 		char* c[] = {"ENTER", "NEW", "POINT", "FIRST"};
// 		char** cp[] = { c+3, c+2, c+1, c};
// 		char*** cpp = cp;
// 		printf("%s\n", **++cpp);
//     printf("%s\n", *--*++cpp+3);
//     printf("%s\n", *cpp[-2]+3);
//     printf("%s\n", cpp[-1][-1]+1);
// 		return 0;
// }
// int myStrlen(const char* s)
// {
//   char* p = s;
//   while(*p != '\0')
//   {
//     p++;
//   }
//   return p - s;
// }
// char* myStrcpy(char* dest, const char* source)
// char* myStrcpy(char* dest, char* source)
// {
// 	char* ret = dest;
// 	assert(dest);
// 	assert(source);
// 	while((*dest++ = *source++));
// 	return ret;
// }
// char* myStrcat(char* dest, const char* source){
// 	char* ret = dest;
//   while(*dest != '\0')
//   {
//     dest++;
//   }
//   while((*dest++ = *source++ ));
// 	return ret;
// }

// int myStrcmp(const char* str1, const char* str2)
// {
// 	assert(str1 && str2);
// 	// while(*str1++ == *str2++ && *str1 != '\0');
// 	while(*str1 == *str2 && *str1 != '\0'){
// 		str1++;
// 		str2++;
// 	}
// 	if(*str1 == *str2 && *str1 == '\0'){
// 		return 0;
// 	}
// 	return (int)(*str1 - *str2);
// }

// char* myStrncpy(char* dest, const char* source, size_t num,char ch)
// {
// 	char* ret = dest;
// 	int count = 0;
// 	while(count < num && *source != '\0')
// 	{
// 		*dest = *source;
// 		dest++ && source++;
// 		count ++;
// 	}
// 	if(*source == '\0' && count < num){
//     *dest = '\0';
//     count++;
// 		while(count < num)
// 		{
// 			*dest = ch;
// 			*dest++;
// 			count++;
// 		}
// 	}
// 	return ret;
// }

// int main()
// {
// 	const char* c1 = "abcdef";
// 	const char* c2 = "abcdeg";
// 	char* c3[30] = {};
// 	// printf("%d\n",myStrcmp(c1,c2));
// 	// printf("%d\n",strcmp(c1,c2));
// 	printf("%s\n",myStrncpy(c3,c1,30,'x'));

// 	return 0;
// }


int main()
{
	char str[] = "Please! Give age to civilization, not civilization to age.";
	char* p;
	printf("%s\n", str);
	p = strtok(str, ",.!");
	while(p != NULL)
	{
		printf("%s\n",p);
		p = strtok(NULL,",.!");
	}
	return 0;
}
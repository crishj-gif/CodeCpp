#include<stdio.h>
#include<malloc.h>
#include <stdlib.h>
int comp(const void *a,const void *b){
    int c=*(const int*)a;
    int d=*(const int*)b;
    if(c>d) return -1;
    if(c<d) return 1;
    return 0;
}
bool containsDuplicate(int* nums, int numsSize) { 
    qsort(nums,numsSize,sizeof(int),comp);
    for (int i=0;i<numsSize-1;i++){
            if(nums[i]==nums[i+1]){
                return true;
            }
    }
    return false;

}
int main(){
    int *nums;
    int numsSize;printf("Nhap so phan tu");
    scanf("%d",&numsSize);
    for(int i=0;i<numsSize;i++){
        printf("Nhap phan tu [%d]",i+1);
        scanf("%d",&nums[i]);
    }
    if(containsDuplicate(nums,numsSize)){
        printf("%d",1);
    }
    else{
        printf("%d",0);
    }
}
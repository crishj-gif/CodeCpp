#include<stdio.h>
#include<malloc.h>
int maxSubArray(int* nums, int numsSize) {
    int max=nums[0];
    int max_current=0;
    for(int i=0;i<numsSize;i++){
        max_current+=nums[i];
        if(max_current>max){
            max=max_current;
        }
        if(max_current < 0){
            max_current = 0;
        }
    }
    return max;
}
int main(){
    int *nums;
    int numsSize;
    printf("Nhap so phan tu:");
    scanf("%d",&numsSize);
    nums=(int*)malloc(numsSize*sizeof(int));
    for(int i=0;i<numsSize;i++){
        printf("Nhap phan tu[%d]\n",i+1);
        scanf("%d",&nums[i]);

    }
    int max=maxSubArray(nums,numsSize);
    printf("Gia tri mang con lon nhat:%d",max);
    return 0;
}

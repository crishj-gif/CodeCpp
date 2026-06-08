#include <iostream>
void bubbleSort(int a[],int n){
    for(int i=0;i<n-1;i++){
        for(int j=0;j<n-i-1;j++){
            if (a[j] > a[j + 1]) {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}
int longestConsecutive(int a[],int n) {
    int numsSize;
    bubbleSort(a,n);
    int current=1;
    int maxst=1;
    if (n==0) return 0;
    else if(n==1) return 1;
    else{



        for(int i=1;i<n;i++){
            if(a[i]!=a[i+1]){
                if(a[i]==a[i+1]-1){
                    current++;                
                }
            else{
                 if(maxst < current){
                    maxst=current;
                 }
            }

            }
            }
        }
    numsSize = (maxst > current) ? maxst : current;
    return numsSize;
}
int main(){
    int a[100];
    int n;
    printf("Nhap so phan tu mang a: ");
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        printf("Nhap phan tu a[%d]:",i);
        scanf("%d",&a[i]);
    }
    int numsSize=longestConsecutive(a,n);
    printf("Chuoi dai nhat trong mang la:%d \n",numsSize);
    return 0;

}

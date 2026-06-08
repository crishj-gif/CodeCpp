#include <stdio.h>
#include<string.h>
#include<malloc.h>
int kiemtra(char*s,int start,int end){
    int a[256]={0};
    for(int i=start;i<end;i++){
        char c=s[i];
        a[(int )c]++;
        if(a[(int)c] > 1) return 1;
    } 
    return 0;  
}
int lengthOfLongestSubstring(char* s) {
    int max=0;
    int current=0;
    for (int i=0;i<strlen(s);i++){
        for(int j=i+1;j<=strlen(s);j++){
            if(kiemtra(s,i,j)==0){
                current=j-i;
                if(current>max){
                    max=current;
                }
            }
        }
    }
    return max;
}

int main(){
    int n;
    char*s=(char*)malloc(1000*sizeof(char));
    printf("Nhap chuoi can kiem tra: ");
    scanf("%[^\n]", s);
    n=lengthOfLongestSubstring(s);
    printf("Ket qua la: %d \n",n);
    return 0;
}

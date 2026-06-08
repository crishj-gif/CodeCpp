#include<stdio.h>
#include<malloc.h>
#include<string.h>
int main(){
    char*s;
    s=(char*)malloc(sizeof(char));
    printf("Nhap xau:");
    fgets(s,8,stdin);
    int a[256]={0};
    int i=0;
    while(i<strlen(s)){
        if(s[i]){
            a[int(s[i])]++;
        }
        i++;
    }
    for(int j=0;j<256;j++){
        if(a[j]<=1) printf("1");
        else printf("-1");

    }

}
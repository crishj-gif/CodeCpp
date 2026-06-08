#include<stdio.h>
#include<string.h>
int main(){
    char c[1000];
    char s[1000];
    printf("Nhap phep tinh:");
    fgets(s, sizeof(s), stdin);
    int trangthai=1;
    int sohang1=0;
    int sohang2=0;
    char toantu;

    for (int i=0;i<strlen(s);i++){
        char c=s[i];
        if(c>='0'&&c<='9'){
            if(trangthai==1){
                sohang1=sohang1*10+int(c-'0');
            }
            else if(trangthai==2){
                sohang2=sohang2*10+int(c-'0');
            }
        }
        else if(c=='+'||c=='-'||c=='*'||c=='/')
        {
            toantu=c;
            trangthai=2;
        }
    }
    switch (toantu)
    {
    case '+':
        printf("\n Ket qua:%d",sohang1+sohang2);
        break;
    case '-':
        printf("\n Ket qua:%d",sohang1-sohang2);
        break;
    case '*':
        printf("\n Ket qua:%d",sohang1*sohang2);
        break;
    case '/':
        if(sohang2!=0){
            printf("\n Ket qua:%d",sohang1/sohang2);
        }
        break;
    }
    return 0;
} 
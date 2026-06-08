#include <stdio.h>
#include <math.h>
int phuongtrinhbac2(int a,int b,int c,int *x1,int*x2){
    int delta=b*b-4*a*c;

        if(delta>0){
            *x1=(-b-sqrt(delta))/(2*a);
            *x2=(-b+sqrt(delta))/(2*a);
            return 2;
        }
        else if(delta==0){
            *x1=-b/(2*a);
            return 1;
        }
        else{
            return 0;

       }
}


int main(){
    int a,b,c;
    printf("Nhap he so a,b,c:");
    scanf("%d%d%d",&a,&b,&c);
    int x1,x2;
    int songhiem=phuongtrinhbac2(a,b,c,&x1,&x2);
    if(songhiem==1){
        printf("Pt co 1 nghiem duy nhat x=%d",x1);

    }
    else if(songhiem==2){
        printf("Pt co 2 nghiem phan biet x1=%d,x2=%d",x1,x2);

    }
    else if(songhiem==0){
        printf("Phuong trinh vo nghiem");
    }
    return 0;
}
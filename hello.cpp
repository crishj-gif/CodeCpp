
#include<stdio.h>

int main(){
    int a,b,c,d,e,f;
    int D, Dx, Dy, x, y;
    printf("\nNhap a: "); scanf("%d", &a);
    printf("\nNhap b: "); scanf("%d", &b);
    printf("\nNhap c: "); scanf("%d", &c);
    printf("\nNhap d: "); scanf("%d", &d);
    printf("\nNhap e: "); scanf("%d", &e);
    printf("\nNhap f: "); scanf("%d", &f);
    D = a * e - b* d;
    Dx = c* e- f * b;
    Dy = a* f - c * d;
    if (D == 0) {
        if (Dx + Dy == 0)
            printf("He phuong trinh co vo so nghiem");
        else
            printf("He phuong trinh vo nghiem");
    }
    else {
        x = Dx / D;
        y = Dy / D;
        printf("He phuong trinh co nghiem (x, y) = (%d, %d)", x, y);
    }
    return 0;
}
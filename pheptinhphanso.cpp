#include<stdio.h>
typedef struct 
{
	int tu,mau;
}phanso;
int main(){
	phanso p,q,r;
	char c;
	printf("Nhap phan so p:\n");
	scanf("%d",&p.tu);
	printf("_\n\n");
	scanf("%d",&p.mau);
	printf("Nhap phan so q:\n");
	scanf("%d",&q.tu);
	printf("_\n\n");
	scanf("%d",&q.mau);
	printf("Nhap phep tinh: ");
	scanf("%s",&c);
	if(c=='+'){

		r.tu=p.tu*q.mau+p.mau*q.tu;
		r.mau=p.mau*q.mau;
	}
	else if(c=='-'){
	
		r.tu=p.tu*q.mau-p.mau*q.tu;
		r.mau=p.mau*q.mau;
	}
	else if(c=='*'){
		r.tu=p.tu*q.tu;
		r.mau=p.mau*q.mau;

	}
	else if(c=='/'){
		if(q.tu!=0){
			r.tu=p.tu*q.mau;
			r.mau=p.mau*q.tu;

		}

	}
	printf("Ket qua phan so la:%d\n                   _\n\n                   %d",r.tu,r.mau);
	return 0;

}
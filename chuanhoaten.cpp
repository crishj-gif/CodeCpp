#include<stdio.h>
#include<string.h>
char *(char*hoten){
	int len=strlen(hoten);
	for (int i<=len;i>=0;i++){
		if(hoten[i]=" "){
			hoten[i]='/0';
			return &hoten[i+1];
		}
	}
	return hoten;
}
void (char*ten,char*hoten){
	strcat(ten," ");
	strcat(ten,hoten);
}
int main(){
	
}
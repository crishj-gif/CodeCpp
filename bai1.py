s = input("Nhap phep tinh ")
trangthai=1
sohang1=sohang2=0
toantu=""
for c in s:
    if '0'<=c<='9':
        chuso=int (c)
        if trangthai==1:
            sohang1=sohang1*10+chuso
        elif trangthai==2:
            sohang2=sohang2*10+chuso
    elif c in '+-*/':
        toantu=c
        trangthai=2
if toantu=='+':
    print('Ket qua phep tinh la',sohang1+sohang2)
elif toantu=='-':
    print('Ket qua phep tinh la',sohang1-sohang2)
elif toantu=='*':
    print('Ket qua phep tinh la',sohang1*sohang2)
elif toantu=='/':
    if(sohang2!=0):
        print('Ket qua phep tinh la',sohang1//sohang2)
    else:
        print('ERROR')
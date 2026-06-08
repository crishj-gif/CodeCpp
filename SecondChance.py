def secondchance(frame,string):
    pagefault=0
    memory=[]
    pointer=0
    bit=[]
    for page in string:
        if page in memory:  
            bit[memory.index(page)]=1
        else:
            pagefault+=1
            if len(memory)<frame:
                memory.append(page)
                bit.append(1)
            else:
                while True:
                    if bit[pointer]==0:
                        memory[pointer]=page
                        bit[pointer]=1
                        pointer=(pointer+1)%frame
                        break
                    else:
                        bit[pointer]=0
                        pointer=(pointer+1)%frame
    return pagefault
sokhung=4
chuoi = [1, 2, 3, 4, 5, 7, 1, 4, 2, 3, 4, 7, 1, 2, 3, 1, 4, 3, 5, 6, 7, 1]
sotrangloi=secondchance(sokhung,chuoi)
print("So trang loi:",sotrangloi)

    




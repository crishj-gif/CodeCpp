def opt(frame,string):
    pagefault=0
    memory=[]
    n = len(string)
    for i in range(n):
        page=string[i]
        if page not in memory:
            pagefault+=1
            
            if len(memory)==frame:
                vitrixa=-1
                victim=-1
                for mem in memory:
                    try:
                        next=string[i+1:].index(mem)
                    except ValueError:
                        next=float('inf')
                    if next>vitrixa:
                        vitrixa=next
                        victim=mem
                memory.remove(victim)
            memory.append(page)    


    
    return pagefault
sokhung=4
chuoi = [1, 2, 3, 4, 5, 7, 1, 4, 2, 3, 4, 7, 1, 2, 3, 1, 4, 3, 5, 6, 7, 1]
sotrangloi=opt(sokhung,chuoi)
print("So trang loi:",sotrangloi)
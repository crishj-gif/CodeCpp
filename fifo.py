def fifo(frame,string):
    pagefault=0
    memory=[]
    for page in string:
        if page not in memory:
            pagefault+=1
            if len(memory)==frame:
                memory.pop(0)
            memory.append(page)
    return pagefault
sokhung=4
chuoi = [1, 2, 3, 4, 5, 7, 1, 4, 2, 3, 4, 7, 1, 2, 3, 1, 4, 3, 5, 6, 7, 1]
sotrangloi=fifo(sokhung,chuoi)
print("So trang loi",sotrangloi)


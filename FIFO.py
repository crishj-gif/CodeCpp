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
sokhung=3
chuoi = [1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5]
sotrangloi=fifo(sokhung,chuoi)
print("So trang loi:",sotrangloi)
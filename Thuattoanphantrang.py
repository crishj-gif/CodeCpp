#FIFO
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
#LRU
def lru(frame,string):
    pagefault=0
    memory=[]
    for page in string:
        if page not in memory:
            pagefault+=1
            if len(memory)==frame:
                memory.pop(0)
            memory.append(page)
        else:
            memory.remove(page)
            memory.append(page)
    return pagefault
#OPT
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
#SecondChance
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
#MFU
def mfu(frames_count, ref_string):
    memory = []
    counters = [] 
    page_faults = 0

    for page in ref_string:
        if page in memory:
            idx = memory.index(page)
            counters[idx] += 1
        else:
            page_faults += 1
            if len(memory) < frames_count:
                memory.append(page)
                counters.append(1)
            else:
                max_score = max(counters)  
                victim_idx = counters.index(max_score)
                memory.pop(victim_idx)
                counters.pop(victim_idx)
                memory.append(page)
                counters.append(1)

    return page_faults

#LFU
def lfu(frames_count, ref_string):
    memory = []
    counters = [] 
    page_faults = 0 

    for page in ref_string:
        if page in memory:
            idx = memory.index(page)
            counters[idx] += 1
        else:
            page_faults += 1
            if len(memory) < frames_count:
                memory.append(page)
                counters.append(1)
            else:
               
                min_score = min(counters) 
                victim_idx = counters.index(min_score)
                memory.pop(victim_idx)
                counters.pop(victim_idx)
                memory.append(page)
                counters.append(1)

    return page_faults
sokhung=3
chuoi = [7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1]
sotrangloi1=fifo(sokhung,chuoi)
sotrangloi2=lru(sokhung,chuoi)
sotrangloi3=opt(sokhung,chuoi)
sotrangloi4=secondchance(sokhung,chuoi)
sotrangloi5=mfu(sokhung,chuoi)
sotrangloi6=lfu(sokhung,chuoi)
print("So loi FIFO:",sotrangloi1)
print("So loi LRU:",sotrangloi2)
print("So loi OPT:",sotrangloi3)
print("So loi SecondChance:",sotrangloi4)
print("So loi MFU:",sotrangloi5)
print("So loi LFU:",sotrangloi6)

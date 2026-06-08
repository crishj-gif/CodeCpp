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




#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int timTrang(int frames[], int soKhung, int page) {
    for (int i = 0; i < soKhung; i++) {
        if (frames[i] == page) return i;
    }
    return -1;
}

int fifoPageFault(int pages[], int n, int soKhung) {
    int* frames = (int*)malloc(soKhung * sizeof(int));
    if (frames == NULL) {
        printf("Loi cap phat bo nho cho FIFO.\n");
        return -1;
    }

    for (int i = 0; i < soKhung; i++) {
        frames[i] = -1;
    }

    int pageFault = 0;
    int front = 0;      
    int daDung = 0;     

    for (int i = 0; i < n; i++) {
        if (timTrang(frames, soKhung, pages[i]) == -1) {
            pageFault++;

            if (daDung < soKhung) {
                frames[daDung] = pages[i];
                daDung++;
            }
            else {
                frames[front] = pages[i];
                front = (front + 1) % soKhung;
            }
        }
    }

    free(frames);
    return pageFault;
}


int lruPageFault(int pages[], int n, int soKhung) {
    int* frames = (int*)malloc(soKhung * sizeof(int));
    int* lastUsed = (int*)malloc(soKhung * sizeof(int));

    if (frames == NULL || lastUsed == NULL) {
        printf("Loi cap phat bo nho cho LRU.\n");
        free(frames);
        free(lastUsed);
        return -1;
    }

    for (int i = 0; i < soKhung; i++) {
        frames[i] = -1;
        lastUsed[i] = -1;
    }

    int pageFault = 0;
    int daDung = 0;

    for (int i = 0; i < n; i++) {
        int pos = timTrang(frames, soKhung, pages[i]);

        if (pos != -1) {
            
            lastUsed[pos] = i;
        }
        else {
            pageFault++;

            if (daDung < soKhung) {
                frames[daDung] = pages[i];
                lastUsed[daDung] = i;
                daDung++;
            }
            else {
               
                int lruIndex = 0;
                for (int j = 1; j < soKhung; j++) {
                    if (lastUsed[j] < lastUsed[lruIndex]) {
                        lruIndex = j;
                    }
                }

                frames[lruIndex] = pages[i];
                lastUsed[lruIndex] = i;
            }
        }
    }

    free(frames);
    free(lastUsed);
    return pageFault;
}


int lanDungTiepTheo(int pages[], int n, int start, int page) {
    for (int i = start; i < n; i++) {
        if (pages[i] == page) return i;
    }
    return INT_MAX; 
}

int optPageFault(int pages[], int n, int soKhung) {
    int* frames = (int*)malloc(soKhung * sizeof(int));
    if (frames == NULL) {
        printf("Loi cap phat bo nho cho OPT.\n");
        return -1;
    }

    for (int i = 0; i < soKhung; i++) {
        frames[i] = -1;
    }

    int pageFault = 0;
    int daDung = 0;

    for (int i = 0; i < n; i++) {
        if (timTrang(frames, soKhung, pages[i]) != -1) {
            continue; 
        }

        pageFault++;

        if (daDung < soKhung) {
            frames[daDung] = pages[i];
            daDung++;
        }
        else {
            int viTriThay = 0;
            int xaNhat = -1;

            for (int j = 0; j < soKhung; j++) {
                int next = lanDungTiepTheo(pages, n, i + 1, frames[j]);
                if (next > xaNhat) {
                    xaNhat = next;
                    viTriThay = j;
                }
            }

            frames[viTriThay] = pages[i];
        }
    }

    free(frames);
    return pageFault;
}


int main() {
    int n, soKhung;

    printf("Nhap so luong tham chieu trang n: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("n phai > 0\n");
        return 1;
    }

    int* pages = (int*)malloc(n * sizeof(int));
    if (pages == NULL) {
        printf("Loi cap phat bo nho cho chuoi trang.\n");
        return 1;
    }

    printf("Nhap chuoi tham chieu trang:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &pages[i]);
    }

    printf("Nhap so khung trang: ");
    scanf("%d", &soKhung);

    if (soKhung <= 0) {
        printf("So khung phai > 0\n");
        free(pages);
        return 1;
    }

    int fifo = fifoPageFault(pages, n, soKhung);
    int lru = lruPageFault(pages, n, soKhung);
    int opt = optPageFault(pages, n, soKhung);

    printf("\nKet qua:\n");
    printf("FIFO = %d page faults\n", fifo);
    printf("LRU  = %d page faults\n", lru);
    printf("OPT  = %d page faults\n", opt);

    free(pages);
    return 0;
}

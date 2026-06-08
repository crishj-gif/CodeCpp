#include <stdio.h>

void twosum(int a[], int target, int n) {
    int i = 0;
    int j = 0;
    int found = 0; // Biến đánh dấu để dừng khi tìm thấy

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) { // j bắt đầu từ i + 1
            if (a[i] + a[j] == target) {
                printf("[%d,%d]\n", i, j); // In ngay khi tìm thấy
                found = 1;
                break; 
            }
        }
        if (found) break; // Thoát vòng lặp ngoài
    }
    
    if (!found) {
        printf("Khong tim thay cap so nao.\n");
    }
}

int main() {
    int arr[100];
    int target;
    int n = sizeof(arr) / sizeof(arr[0]);
    twosum(arr, target, n);
    return 0;
}
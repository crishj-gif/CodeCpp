#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
  int ngay, thang, nam;
} NgaySinh;

typedef struct {
  char maLop[15];
  char maSV[15];
  char hoTen[50];
  NgaySinh ngaySinh;
  float dtbtl;
} HocVien;

int main() {
    FILE *f = fopen("sinhvien.bin", "wb");
    if (!f) {
        printf("Loi tao file sinhvien.bin\n");
        return 1;
    }
    
    srand(time(NULL));
    
    // Khai báo danh sách tên người nổi tiếng (ca sĩ, diễn viên, cầu thủ, tỷ phú...)
    const char* tenNoiTieng[] = {
        "Nguyen Thanh Tung", "Tran Thanh", "Ho Ngoc Ha", "Nguyen Thuc Thuy Tien",
        "Doan Van Hau", "Nguyen Quang Hai", "Que Ngoc Hai", "Phan Thi My Tam",
        "Nguyen Duc Cuong", "Hoang Thuy Linh", "Le Thanh Tung", "Ninh Duong Lan Ngoc",
        "Vo Vu Truong Giang", "Vuong Dinh Hue", "Huynh Minh Hung", "Vu Cat Tuong",
        "Nguyen Phuoc Thinh", "Nguyen Khoa Toc Tien", "Pham Nhat Vuong", "Nguyen Thi Phuong Thao",
        "Nguyen Van Toan", "Nguyen Tien Linh", "Bui Tien Dung", "Phan Van Duc",
        "Vu Van Thanh", "Nguyen Hoang Duc", "Nguyen Cong Phuong", "Luong Xuan Truong",
        "Nguyen Tuan Anh", "Mai Hong Ngoc", "Le Quyen", "Bich Phuong",
        "Chi Pu", "Nguyen Thuy Lam", "Mai Phuong Thuy", "Do My Linh",
        "Tran Tieu Vy", "Nguyen Tran Khanh Van", "Ly Hai", "Hoai Linh",
        "Xuan Bac", "Cong Ly", "Tu Long", "Kieu Minh Tuan",
        "Isaac", "Soobin Hoang Son", "Erik", "Duc Phuc",
        "Hoa Minzy", "Thieu Bao Tram", "Karik", "Binz"
    };
    int soLuongTen = sizeof(tenNoiTieng) / sizeof(tenNoiTieng[0]);
    
    const char* lop[] = {"CNTT1", "CNTT2", "BDATTT"};
    
    // Sinh 100 người
    for (int i = 1; i <= 100; i++) {
        HocVien hv;
        sprintf(hv.maSV, "SV%03d", i);
        
        // Xét 2 trường hợp đặc biệt bắt buộc có
        if (i == 1) {
            strcpy(hv.maLop, "CNTT2");
            strcpy(hv.hoTen, "Nguyen Huu Truong Son");
            hv.ngaySinh.ngay = 30;
            hv.ngaySinh.thang = 12;
            hv.ngaySinh.nam = 2006;
            hv.dtbtl = 8.5;
        } 
        else if (i == 2) {
            strcpy(hv.maLop, "BDATTT");
            strcpy(hv.hoTen, "Bui Thi Kim Ngan");
            hv.ngaySinh.ngay = 24;
            hv.ngaySinh.thang = 8;
            hv.ngaySinh.nam = 2006;
            hv.dtbtl = 9.0;
        } 
        else {
            // Sinh ngẫu nhiên cho các người còn lại
            strcpy(hv.maLop, lop[rand() % 3]);
            strcpy(hv.hoTen, tenNoiTieng[rand() % soLuongTen]);
            hv.ngaySinh.ngay = rand() % 28 + 1; // 1-28 để khỏi dính năm nhuận/tháng thiếu
            hv.ngaySinh.thang = rand() % 12 + 1;
            hv.ngaySinh.nam = 2000 + (rand() % 7); // Từ năm 2000 đến 2006
            hv.dtbtl = (float)(rand() % 51 + 50) / 10.0; // Điểm từ 5.0 đến 10.0
        }
        
        fwrite(&hv, sizeof(HocVien), 1, f);
    }
    
    fclose(f);
    printf("Da tao thanh cong file sinhvien.bin gom 100 hoc vien!\n");
    return 0;
}

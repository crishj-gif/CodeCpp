#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

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

typedef struct Node {
  HocVien data;
  struct Node *next;
} Node;

// --- Khai báo nguyên hàm  ---
typedef int (*CompareFunc)(HocVien, HocVien);

int my_getch();
void xoaManHinh();
void tamDung();
void inDuongNgang(int kieu, int chieuRongTrong);
void inDongKhung(const char *noiDung, int chieuRongTrong, int canGiua,
                 const char *colorPrefix, const char *colorSuffix);
void inThongBao(const char *thongBao, int kieuLoai);
int hienThiMenu(const char *title, const char *menuItems[], int numItems,
                int defaultIdx);
void chuanHoaTen(char *name);
void themHocVien(const char *filename);
void inDanhSach(const char *filename);

Node *docDanhSachLienKet(const char *filename);
void giaiPhongLienKet(Node *head);
int containsIgnoreCase(const char *haystack, const char *needle);
void inDanhSachLienKet(Node *head, const char *title);

void menuSapXep(const char *filename);
void menuTimKiem(const char *filename);
void menuThongKe(const char *filename);

// Bắt ngay phím lập tức mà không cần nhấn Enter
int my_getch() {
  struct termios oldt, newt;
  int ch;
  // Lấy thông tin cấu hình hiện tại của terminal
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  // Tắt chế độ canonical và echo
  newt.c_lflag &= ~(ICANON | ECHO);
  // Áp dụng cấu hình mới
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}
// Xóa màn hình
void xoaManHinh() { system("clear"); }
// Tạm dừng chương trình
void tamDung() {
  printf("\n\033[1;36mBam phim bat ky de tiep tuc...\033[0m");
  fflush(stdout);
  my_getch();
}
// Vẽ đường ngang
void inDuongNgang(int kieu, int chieuRongTrong) {
  if (kieu == 0) {
    printf("┌");
    for (int i = 0; i < chieuRongTrong; i++)
      printf("─");
    printf("┐\n");
  } else if (kieu == 1) {
    printf("├");
    for (int i = 0; i < chieuRongTrong; i++)
      printf("─");
    printf("┤\n");
  } else {
    printf("└");
    for (int i = 0; i < chieuRongTrong; i++)
      printf("─");
    printf("┘\n");
  }
}
// Vẽ dòng nội dung trong khung
void inDongKhung(const char *noiDung, int chieuRongTrong, int canGiua,
                 const char *colorPrefix, const char *colorSuffix) {
  printf("│");
  if (colorPrefix)
    printf("%s", colorPrefix);

  int len = strlen(noiDung);
  int khoangTrong = chieuRongTrong - len;
  if (khoangTrong < 0)
    khoangTrong = 0;

  if (canGiua) {
    int trai = khoangTrong / 2;
    int phai = khoangTrong - trai;
    for (int i = 0; i < trai; i++)
      printf(" ");
    printf("%s", noiDung);
    if (colorPrefix)
      printf("%s", colorSuffix);
    for (int i = 0; i < phai; i++)
      printf(" ");
  } else {
    printf("%s", noiDung);
    if (colorPrefix)
      printf("%s", colorSuffix);
    for (int i = 0; i < khoangTrong; i++)
      printf(" ");
  }

  printf("│\n");
}
// In thông báo với màu sắc khác nhau
void inThongBao(const char *thongBao, int kieuLoai) {
  int w = 58;
  inDuongNgang(0, w);
  if (kieuLoai == 1) {
    inDongKhung("[ THANH CONG ]", w, 1, "\033[1;32m", "\033[0m"); // màu xanh lá
  } else if (kieuLoai == 2) {
    inDongKhung("[ CANH BAO ]", w, 1, "\033[1;33m", "\033[0m"); // màu vàng
  } else if (kieuLoai == 3) {
    inDongKhung("[ LOI ]", w, 1, "\033[1;31m", "\033[0m"); // màu đỏ
  } else {
    inDongKhung("[ THONG BAO ]", w, 1, "\033[1;36m", "\033[0m"); // màu xanh lam
  }
  inDuongNgang(1, w);
  inDongKhung("", w, 0, NULL, NULL);       // in dong trong
  inDongKhung(thongBao, w, 1, NULL, NULL); // in thông báo
  inDongKhung("", w, 0, NULL, NULL);       // in dòng trong
  inDuongNgang(2, w);
}

int hienThiMenu(const char *title, const char *menuItems[], int numItems,
                int defaultIdx) {
  int currentIdx = defaultIdx;
  int w = 58;
  while (1) {
    xoaManHinh();
    inDuongNgang(0, w);
    inDongKhung(title, w, 1, "\033[1;36m", "\033[0m");
    inDuongNgang(1, w);
    inDongKhung("", w, 0, NULL, NULL);
    for (int i = 0; i < numItems; i++) {
      char itemStr[100];
      if (i == currentIdx) {
        sprintf(itemStr, "  -> %d. %s", i + 1, menuItems[i]);
        inDongKhung(itemStr, w, 0, "\033[1;32m", "\033[0m");
      } else {
        sprintf(itemStr, "     %d. %s", i + 1, menuItems[i]);
        inDongKhung(itemStr, w, 0, NULL, NULL);
      }
    }
    inDongKhung("", w, 0, NULL, NULL);
    inDuongNgang(1, w);
    inDongKhung("(Dung phim mui ten Len/Xuong de di chuyen, Enter de chon)", w,
                1, "\033[1;30m", "\033[0m");
    inDuongNgang(2, w);

    int ch1 = my_getch();
    if (ch1 == '\n' || ch1 == '\r') {
      return currentIdx + 1; // Trả về số thứ tự (1-based)
    }
    if (ch1 == 27) { // Ký tự mũi tên
      int ch2 = my_getch();
      if (ch2 == 91) {
        int ch3 = my_getch();
        if (ch3 == 65) { // Len
          currentIdx = (currentIdx > 0) ? currentIdx - 1 : numItems - 1;
        } else if (ch3 == 66) { // Xuong
          currentIdx = (currentIdx < numItems - 1) ? currentIdx + 1 : 0;
        }
      }
    } else if (ch1 >= '1' && ch1 <= '0' + numItems) {
      return ch1 - '0';
    }
  }
}

void chuanHoaTen(char *name) {
  int i = 0, j = 0;
  int len = strlen(name);
  char temp[100];
  int space = 1;

  while (i < len) {
    if (name[i] == ' ') {
      if (j > 0 && temp[j - 1] != ' ') {
        temp[j++] = ' ';
      }
      space = 1;
    } else {
      if (space) {
        temp[j++] =
            toupper((unsigned char)name[i]); // chuyển chữ đầu thành chữ hoa
        space = 0;
      } else {
        temp[j++] =
            tolower((unsigned char)name[i]); // chuyển chữ sau thành chữ thường
      }
    }
    i++;
  }
  if (j > 0 && temp[j - 1] == ' ') { // xóa khoảng trắng ở cuối chuỗi
    j--;
  }
  temp[j] = '\0';
  strcpy(name, temp);
}

// Hàm nhập chuỗi có bắt lỗi và cho nhập lại trên cùng 1 dòng
void nhapChuoiCoKiemTra(const char *prompt, char *output, int maxLen,
                        const char *errorMsg) {
  char buffer[100];
  while (1) {
    printf("%s", prompt);
    if (fgets(buffer, sizeof(buffer), stdin)) {
      buffer[strcspn(buffer, "\n")] = 0;
      if (strlen(buffer) > 0) {
        strncpy(output, buffer, maxLen - 1);
        output[maxLen - 1] = '\0';
        break;
      }
    }
    printf("\033[31mLoi: %s\033[0m", errorMsg);
    fflush(stdout);
    usleep(1000000);                  // dung 1 giay
    printf("\r\033[2K\033[A\033[2K"); // Xoa loi, lui len 1 dong, xoa prompt
  }
}

// --- CHƯƠNG TRÌNH CHÍNH ---
int main() {
  int luaChon;
  int firstTimeExit = 1;
  const char *dataFile = "sinhvien.bin";
  // định nghĩa menu chính
  const char *menuMain[] = {"Them moi ho so",    "In danh sach hoc vien",
                            "Sap xep danh sach", "Tim kiem hoc vien",
                            "Thong ke bao cao",  "Thoat chuong trinh"};

  do {
    // "Khi vào chức năng nào thì mục đang chọn vẫn đang là số 3" => mặc định
    // chọn index 2
    luaChon =
        hienThiMenu("CHUONG TRINH QUAN LY HOC VIEN (QLHV)", menuMain, 6, 0);

    switch (luaChon) {
    case 1:
      themHocVien(dataFile);
      break;
    case 2:
      inDanhSach(dataFile);
      break;
    case 3:
      menuSapXep(dataFile);
      break;
    case 4:
      menuTimKiem(dataFile);
      break;
    case 5:
      menuThongKe(dataFile);
      break;
    case 6:
      if (firstTimeExit) {
        xoaManHinh();
        inDuongNgang(0, 58);
        inDongKhung("XAC NHAN THOAT CHUONG TRINH", 58, 1, "\033[1;33m",
                    "\033[0m");
        inDuongNgang(1, 58);
        inDongKhung("", 58, 0, NULL, NULL);
        inDongKhung("Ban co chac muon thoat khong? (y/n)", 58, 1, NULL, NULL);
        inDongKhung("", 58, 0, NULL, NULL);
        inDuongNgang(2, 58);

        char confirm = my_getch();
        if (confirm == 'y' || confirm == 'Y') {
          xoaManHinh();
          inDuongNgang(0, 58);
          inDongKhung("TAM BIET", 58, 1, "\033[1;32m", "\033[0m");
          inDuongNgang(1, 58);
          inDongKhung("", 58, 0, NULL, NULL);
          inDongKhung("Cam on ban da su dung phan mem!", 58, 1, NULL, NULL);
          inDongKhung("", 58, 0, NULL, NULL);
          inDuongNgang(2, 58);
          return 0;
        } else {
          firstTimeExit = 0; // Đánh dấu đã hỏi lần đầu
          luaChon = 0;       // Hủy lệnh thoát
        }
      } else {
        xoaManHinh();
        inDuongNgang(0, 58);
        inDongKhung("TAM BIET", 58, 1, "\033[1;32m", "\033[0m");
        inDuongNgang(1, 58);
        inDongKhung("", 58, 0, NULL, NULL);
        inDongKhung("Cam on ban da su dung phan mem!", 58, 1, NULL, NULL);
        inDongKhung("", 58, 0, NULL, NULL);
        inDuongNgang(2, 58);
        return 0;
      }
      break;
    }
  } while (luaChon != 6);

  return 0;
}

// --- CÁC HÀM SẮP XẾP VÀ HỖ TRỢ ---
const char *getTen(const char *hoTen) {
  const char *lastSpace = strrchr(hoTen, ' ');
  if (lastSpace) {
    return lastSpace + 1;
  }
  return hoTen;
}

int cmpMaLop(HocVien a, HocVien b) { return strcmp(a.maLop, b.maLop); }
int cmpMaSV(HocVien a, HocVien b) { return strcmp(a.maSV, b.maSV); }
int cmpHoTen(HocVien a, HocVien b) {
  int cmp = strcmp(getTen(a.hoTen), getTen(b.hoTen));
  if (cmp == 0)
    return strcmp(a.hoTen, b.hoTen);
  return cmp;
}
int cmpNgaySinh(HocVien a, HocVien b) {
  if (a.ngaySinh.nam != b.ngaySinh.nam)
    return a.ngaySinh.nam - b.ngaySinh.nam;
  if (a.ngaySinh.thang != b.ngaySinh.thang)
    return a.ngaySinh.thang - b.ngaySinh.thang;
  return a.ngaySinh.ngay - b.ngaySinh.ngay;
}
int cmpDTBTL(HocVien a, HocVien b) {
  if (a.dtbtl < b.dtbtl)
    return -1;
  if (a.dtbtl > b.dtbtl)
    return 1;
  return 0;
}

void hoanVi(HocVien *a, HocVien *b) {
  HocVien temp = *a;
  *a = *b;
  *b = temp;
}

void selectionSort(HocVien arr[], int n, CompareFunc cmp) {
  for (int i = 0; i < n - 1; i++) {
    int min_idx = i;
    for (int j = i + 1; j < n; j++) {
      if (cmp(arr[j], arr[min_idx]) < 0)
        min_idx = j;
    }
    hoanVi(&arr[min_idx], &arr[i]);
  }
}

void insertionSort(HocVien arr[], int n, CompareFunc cmp) {
  for (int i = 1; i < n; i++) {
    HocVien key = arr[i];
    int j = i - 1;
    while (j >= 0 && cmp(arr[j], key) > 0) {
      arr[j + 1] = arr[j];
      j = j - 1;
    }
    arr[j + 1] = key;
  }
}

int partition(HocVien arr[], int low, int high, CompareFunc cmp) {
  HocVien pivot = arr[high];
  int i = (low - 1);
  for (int j = low; j <= high - 1; j++) {
    if (cmp(arr[j], pivot) < 0) {
      i++;
      hoanVi(&arr[i], &arr[j]);
    }
  }
  hoanVi(&arr[i + 1], &arr[high]);
  return (i + 1);
}

void quickSort(HocVien arr[], int low, int high, CompareFunc cmp) {
  if (low < high) {
    int pi = partition(arr, low, high, cmp);
    quickSort(arr, low, pi - 1, cmp);
    quickSort(arr, pi + 1, high, cmp);
  }
}

void merge(HocVien arr[], int l, int m, int r, CompareFunc cmp) {
  int n1 = m - l + 1;
  int n2 = r - m;
  HocVien *L = (HocVien *)malloc(n1 * sizeof(HocVien));
  HocVien *R = (HocVien *)malloc(n2 * sizeof(HocVien));

  for (int i = 0; i < n1; i++)
    L[i] = arr[l + i];
  for (int j = 0; j < n2; j++)
    R[j] = arr[m + 1 + j];

  int i = 0, j = 0, k = l;
  while (i < n1 && j < n2) {
    if (cmp(L[i], R[j]) <= 0) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
  free(L);
  free(R);
}

void mergeSort(HocVien arr[], int l, int r, CompareFunc cmp) {
  if (l < r) {
    int m = l + (r - l) / 2;
    mergeSort(arr, l, m, cmp);
    mergeSort(arr, m + 1, r, cmp);
    merge(arr, l, m, r, cmp);
  }
}

int docDanhSachDuLieu(const char *filename, HocVien **arr) {
  FILE *f = fopen(filename, "rb");
  if (!f)
    return 0;

  fseek(f, 0, SEEK_END);
  long fileSize = ftell(f);
  rewind(f);

  int n = fileSize / sizeof(HocVien);
  if (n > 0) {
    *arr = (HocVien *)malloc(n * sizeof(HocVien));
    fread(*arr, sizeof(HocVien), n, f);
  }
  fclose(f);
  return n;
}

void ghiDanhSachDuLieu(const char *filename, HocVien *arr, int n) {
  FILE *f = fopen(filename, "wb");
  if (f) {
    fwrite(arr, sizeof(HocVien), n, f);
    fclose(f);
  }
}

void inDanhSachMang(HocVien *arr, int n) {
  xoaManHinh();
  inDuongNgang(0, 76);
  inDongKhung("DANH SACH HOC VIEN (MANG DA SAP XEP)", 76, 1, "\033[1;34m",
              "\033[0m");
  inDuongNgang(1, 76);

  if (n == 0) {
    inDongKhung("(Danh sach rong)", 76, 1, NULL, NULL);
    inDuongNgang(2, 76);
    return;
  }
  printf("│ %-10s │ %-10s │ %-25s │ %-12s │ %-5s │\n", "Ma Lop", "Ma SV",
         "Ho Ten", "Ngay Sinh", "DTBTL");
  printf("├────────────┼────────────┼───────────────────────────┼──────────────"
         "┼───────┤\n");
  for (int i = 0; i < n; i++) {
    printf("│ %-10s │ %-10s │ %-25s │ %02d/%02d/%04d   │ %-5.2f │\n",
           arr[i].maLop, arr[i].maSV, arr[i].hoTen, arr[i].ngaySinh.ngay,
           arr[i].ngaySinh.thang, arr[i].ngaySinh.nam, arr[i].dtbtl);
  }
  printf("└────────────┴────────────┴───────────────────────────┴──────────────"
         "┴───────┘\n");
  char summaryStr[100];
  sprintf(summaryStr, "Tong cong: %d sinh vien.", n);
  inDuongNgang(0, 76);
  inDongKhung(summaryStr, 76, 1, "\033[1;32m", "\033[0m");
  inDuongNgang(2, 76);
}

// --- CHI TIẾT CÁC MENU CẤP 2 ---
void menuSapXep(const char *filename) {
  int lc_algo, lc_key;
  const char *algoItems[] = {"Sap xep Chon (Selection Sort)",
                             "Sap xep Chen (Insertion Sort)", "Quick Sort",
                             "Merge Sort", "Quay lai menu chinh"};
  const char *keyItems[] = {"Sap xep theo Ma Lop",    "Sap xep theo Ma SV",
                            "Sap xep theo Ho Ten",    "Sap xep theo Ngay Sinh",
                            "Sap xep theo Diem TBTL", "Quay lai"};

  do {
    lc_algo = hienThiMenu("MENU SAP XEP (M3)", algoItems, 5, 0);
    if (lc_algo >= 1 && lc_algo <= 4) {
      lc_key = hienThiMenu("CHON KHOA SAP XEP", keyItems, 6, 0);
      if (lc_key >= 1 && lc_key <= 5) {
        HocVien *arr = NULL;
        int n = docDanhSachDuLieu(filename, &arr);
        if (n == 0) {
          xoaManHinh();
          inThongBao("File chua ton tai hoac khong co du lieu!", 3);
          tamDung();
          continue;
        }

        CompareFunc cmp = NULL;
        switch (lc_key) {
        case 1:
          cmp = cmpMaLop;
          break;
        case 2:
          cmp = cmpMaSV;
          break;
        case 3:
          cmp = cmpHoTen;
          break;
        case 4:
          cmp = cmpNgaySinh;
          break;
        case 5:
          cmp = cmpDTBTL;
          break;
        }

        if (lc_algo == 1)
          selectionSort(arr, n, cmp);
        else if (lc_algo == 2)
          insertionSort(arr, n, cmp);
        else if (lc_algo == 3)
          quickSort(arr, 0, n - 1, cmp);
        else if (lc_algo == 4)
          mergeSort(arr, 0, n - 1, cmp);

        inDanhSachMang(arr, n);

        printf("\n\033[1;33mBan co muon luu danh sach da sap xep vao file "
               "khong? (y/n): \033[0m");
        char confirm = my_getch();
        if (confirm == 'y' || confirm == 'Y') {
          ghiDanhSachDuLieu(filename, arr, n);
          printf("\n");
          inThongBao("Da luu danh sach vao file thanh cong!", 1);
        }

        free(arr);
        tamDung();
      }
    }
  } while (lc_algo != 5);
}

// --- CÁC HÀM XỬ LÝ DANH SÁCH LIÊN KẾT ---
Node *docDanhSachLienKet(const char *filename) {
  FILE *f = fopen(filename, "rb");
  if (!f)
    return NULL;
  Node *head = NULL, *tail = NULL;
  HocVien hv;
  while (fread(&hv, sizeof(HocVien), 1, f) == 1) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = hv;
    newNode->next = NULL;
    if (!head) {
      head = newNode;
    } else {
      tail->next = newNode;
    }
    tail = newNode;
  }
  fclose(f);
  return head;
}

void giaiPhongLienKet(Node *head) {
  Node *temp;
  while (head) {
    temp = head;
    head = head->next;
    free(temp);
  }
}
//Hàm lõi tìm kiếm tên gần đúng
int containsIgnoreCase(const char *haystack, const char *needle) {
  if (!haystack || !needle)
    return 0;
  int hLen = strlen(haystack);
  int nLen = strlen(needle);
  if (nLen > hLen)
    return 0;

  for (int i = 0; i <= hLen - nLen; i++) {
    int match = 1;
    for (int j = 0; j < nLen; j++) {
      if (tolower((unsigned char)haystack[i + j]) !=
          tolower((unsigned char)needle[j])) {
        match = 0;
        break;
      }
    }
    if (match)
      return 1;
  }
  return 0;
}

void inDanhSachLienKet(Node *head, const char *title) {
  xoaManHinh();
  inDuongNgang(0, 76);
  inDongKhung(title, 76, 1, "\033[1;34m", "\033[0m");
  inDuongNgang(1, 76);

  if (!head) {
    inDongKhung("(Danh sach trong hoac khong co ket qua phu hop)", 76, 1, NULL,
                NULL);
    inDuongNgang(2, 76);
    return;
  }
  printf("│ %-10s │ %-10s │ %-25s │ %-12s │ %-5s │\n", "Ma Lop", "Ma SV",
         "Ho Ten", "Ngay Sinh", "DTBTL");
  printf("├────────────┼────────────┼───────────────────────────┼──────────────"
         "┼───────┤\n");
  int count = 0;
  Node *curr = head;
  while (curr) {
    printf("│ %-10s │ %-10s │ %-25s │ %02d/%02d/%04d   │ %-5.2f │\n",
           curr->data.maLop, curr->data.maSV, curr->data.hoTen,
           curr->data.ngaySinh.ngay, curr->data.ngaySinh.thang,
           curr->data.ngaySinh.nam, curr->data.dtbtl);
    count++;
    curr = curr->next;
  }
  printf("└────────────┴────────────┴───────────────────────────┴──────────────"
         "┴───────┘\n");
  char summaryStr[100];
  sprintf(summaryStr, "Tong cong: %d sinh vien.", count);
  inDuongNgang(0, 76);
  inDongKhung(summaryStr, 76, 1, "\033[1;32m", "\033[0m");
  inDuongNgang(2, 76);
}

void menuTimKiem(const char *filename) {
  int lc;
  const char *menuItems[] = {"Tim kiem theo Ma SV (Chinh xac)",
                             "Tim kiem theo Ho ten (Gan dung)",
                             "Tim kiem theo Ma lop",
                             "Tim kiem theo Khoang diem TBTL",
                             "Tim kiem theo Nam sinh",
                             "Quay lai menu chinh"};

  do {
    lc = hienThiMenu("MENU TIM KIEM (M4)", menuItems, 6, 0);
    if (lc >= 1 && lc <= 5) {
      Node *head = docDanhSachLienKet(filename);
      if (!head) {
        xoaManHinh();
        inThongBao("File chua ton tai hoac khong co du lieu!", 3);
        tamDung();
        continue;
      }

      Node *filteredHead = NULL, *filteredTail = NULL;
      char query[100];
      float minVal = 0, maxVal = 10;
      int yearQuery = 2000;

      if (lc == 1) {
        nhapChuoiCoKiemTra("Nhap Ma SV can tim: ", query, sizeof(query),
                           "Khong duoc de trong!");
      } else if (lc == 2) {
        nhapChuoiCoKiemTra("Nhap Ho ten can tim (gan dung): ", query,
                           sizeof(query), "Khong duoc de trong!");
      } else if (lc == 3) {
        nhapChuoiCoKiemTra("Nhap Ma lop can tim: ", query, sizeof(query),
                           "Khong duoc de trong!");
      } else if (lc == 4) {
        char scoreBuf[100];
        while (1) {
          printf("Nhap Khoang diem TBTL (min max, vi du: 8.0 9.5): ");
          if (fgets(scoreBuf, sizeof(scoreBuf), stdin)) {
            if (sscanf(scoreBuf, "%f %f", &minVal, &maxVal) == 2) {
              if (minVal >= 0 && minVal <= 10 && maxVal >= 0 && maxVal <= 10 &&
                  minVal <= maxVal) {
                break;
              }
            }
          }
          printf("\033[31mLoi: Diem phai tu 0-10 va min <= max!\033[0m");
          fflush(stdout);
          usleep(1000000);
          printf("\r\033[2K\033[A\033[2K");
        }
      } else if (lc == 5) {
        char yearBuf[100];
        while (1) {
          printf("Nhap Nam sinh can tim: ");
          if (fgets(yearBuf, sizeof(yearBuf), stdin)) {
            if (sscanf(yearBuf, "%d", &yearQuery) == 1) {
              if (yearQuery >= 1900 && yearQuery <= 2100) {
                break;
              }
            }
          }
          printf("\033[31mLoi: Nam sinh khong hop le (1900-2100)!\033[0m");
          fflush(stdout);
          usleep(1000000);
          printf("\r\033[2K\033[A\033[2K");
        }
      }

      // Duyệt danh sách liên kết để lọc
      Node *curr = head;
      while (curr) {
        int match = 0;
        switch (lc) {
        case 1:
          if (strcmp(curr->data.maSV, query) == 0)//so sánh từng kí tự mã sinh viên với query
            match = 1;
          break;
        case 2:
          if (containsIgnoreCase(curr->data.hoTen, query))//so sánh từng kí tự tên sinh viên với query không phân biệt hoa thường
            match = 1;
          break;
        case 3:
          if (containsIgnoreCase(curr->data.maLop, query))//so sánh từng kí tự mã lớp với query không phân biệt hoa thường
            match = 1;
          break;
        case 4:
          if (curr->data.dtbtl >= minVal && curr->data.dtbtl <= maxVal)//so sánh điểm trung bình với minVal và maxVal
            match = 1;
          break;
        case 5:
          if (curr->data.ngaySinh.nam == yearQuery)//so sánh năm sinh với yearQuery
            match = 1;
          break;
        }

        if (match) {
          Node *newNode = (Node *)malloc(sizeof(Node));
          newNode->data = curr->data;
          newNode->next = NULL;
          if (!filteredHead) {
            filteredHead = newNode;
          } else {
            filteredTail->next = newNode;
          }
          filteredTail = newNode;
        }
        curr = curr->next;
      }

      // In kết quả
      char titleStr[150];
      if (lc == 1)
        sprintf(titleStr, "KET QUA TIM KIEM MA SV: %s", query);
      else if (lc == 2)
        sprintf(titleStr, "KET QUA TIM KIEM HO TEN CHUA: %s", query);
      else if (lc == 3)
        sprintf(titleStr, "KET QUA TIM KIEM MA LOP CHUA: %s", query);
      else if (lc == 4)
        sprintf(titleStr, "KET QUA TIM KIEM DIEM TRONG KHOANG: %.2f - %.2f",
                minVal, maxVal);
      else if (lc == 5)
        sprintf(titleStr, "KET QUA TIM KIEM NAM SINH: %d", yearQuery);

      inDanhSachLienKet(filteredHead, titleStr);

      // Giải phóng bộ nhớ của cả 2 danh sách liên kết
      giaiPhongLienKet(filteredHead);
      giaiPhongLienKet(head);
      tamDung();
    }
  } while (lc != 6);
}

void menuThongKe(const char *filename) {
  int lc;
  const char *menuItems[] = {"Thong ke so luong theo lop",
                             "Thong ke ty le hoc tap", "Quay lai menu chinh"};

  do {
    lc = hienThiMenu("MENU THONG KE (M5)", menuItems, 3, 0);
    if (lc >= 1 && lc <= 2) {
      Node *head = docDanhSachLienKet(filename);
      if (!head) {
        xoaManHinh();
        inThongBao("File chua ton tai hoac khong co du lieu!", 3);
        tamDung();
        continue;
      }

      if (lc == 1) {
        // Thống kê số lượng theo lớp
        xoaManHinh();
        inDuongNgang(0, 58);
        inDongKhung("THONG KE SO LUONG SINH VIEN THEO LOP", 58, 1, "\033[1;34m",
                    "\033[0m");
        inDuongNgang(1, 58);
        printf("│ %-25s │ %-26s │\n", "Ten Lop", "So Luong Sinh Vien");
        printf("├───────────────────────────┼────────────────────────────┤\n");

        typedef struct {
          char maLop[15];
          int count;
        } ThongKeLop;

        ThongKeLop tk[100];
        int nLop = 0;

        Node *curr = head;
        while (curr) {
          int foundLop = -1;
          for (int i = 0; i < nLop; i++) {
            if (strcmp(tk[i].maLop, curr->data.maLop) == 0) {
              foundLop = i;
              break;
            }
          }

          if (foundLop != -1) {
            tk[foundLop].count++;
          } else {
            strcpy(tk[nLop].maLop, curr->data.maLop);
            tk[nLop].count = 1;
            nLop++;
          }
          curr = curr->next;
        }

        int totalCount = 0;
        for (int i = 0; i < nLop; i++) {
          printf("│ %-25s │ %-26d │\n", tk[i].maLop, tk[i].count);
          totalCount += tk[i].count;
        }
        printf("└───────────────────────────┴────────────────────────────┘\n");

        char summary[100];
        sprintf(summary, "Tong so lop: %d. Tong sinh vien: %d", nLop,
                totalCount);
        inDuongNgang(0, 58);
        inDongKhung(summary, 58, 1, "\033[1;32m", "\033[0m");
        inDuongNgang(2, 58);

      } else if (lc == 2) {
        // Thống kê tỷ lệ học tập
        int xuatSac = 0;   // >= 9.0
        int gioi = 0;      // >= 8.0 && < 9.0
        int kha = 0;       // >= 6.5 && < 8.0
        int trungBinh = 0; // >= 5.0 && < 6.5
        int yeu = 0;       // < 5.0
        int total = 0;

        Node *curr = head;
        while (curr) {
          float d = curr->data.dtbtl;
          if (d >= 9.0)
            xuatSac++;
          else if (d >= 8.0)
            gioi++;
          else if (d >= 6.5)
            kha++;
          else if (d >= 5.0)
            trungBinh++;
          else
            yeu++;

          total++;
          curr = curr->next;
        }

        xoaManHinh();
        inDuongNgang(0, 58);
        inDongKhung("THONG KE TY LE HOC TAP", 58, 1, "\033[1;34m", "\033[0m");
        inDuongNgang(1, 58);
        printf("│ %-15s │ %-15s │ %-18s │\n", "Phan Loai", "So Luong",
               "Ty Le (%)");
        printf("├─────────────────┼─────────────────┼────────────────────┤\n");

        if (total > 0) {
          printf("│ %-15s │ %-15d │ %-17.2f%% │\n", "Xuat sac", xuatSac,
                 (float)xuatSac / total * 100);
          printf("│ %-15s │ %-15d │ %-17.2f%% │\n", "Gioi", gioi,
                 (float)gioi / total * 100);
          printf("│ %-15s │ %-15d │ %-17.2f%% │\n", "Kha", kha,
                 (float)kha / total * 100);
          printf("│ %-15s │ %-15d │ %-17.2f%% │\n", "Trung binh", trungBinh,
                 (float)trungBinh / total * 100);
          printf("│ %-15s │ %-15d │ %-17.2f%% │\n", "Yeu", yeu,
                 (float)yeu / total * 100);
        } else {
          printf("│ (Danh sach rong)                                     │\n");
        }
        printf("└─────────────────┴─────────────────┴────────────────────┘\n");

        char summary[100];
        sprintf(summary, "Tong so hoc vien da thong ke: %d", total);
        inDuongNgang(0, 58);
        inDongKhung(summary, 58, 1, "\033[1;32m", "\033[0m");
        inDuongNgang(2, 58);
      }

      giaiPhongLienKet(head);
      tamDung();
    }
  } while (lc != 3);
}

// --- DINH NGHIA CAC HAM CHUC NANG CO BAN ---
int kiemTraTrungMaSV(const char *filename, const char *maSV) {
  FILE *f = fopen(filename, "rb");
  if (!f)
    return 0; // File chua ton tai, khong bi trung

  HocVien hv;
  while (fread(&hv, sizeof(HocVien), 1, f) == 1) {
    if (strcmp(hv.maSV, maSV) == 0) {
      fclose(f);
      return 1; // Trung ma SV
    }
  }
  fclose(f);
  return 0; // Khong trung
}

int laNamNhuan(int nam) {
  return (nam % 4 == 0 && nam % 100 != 0) || (nam % 400 == 0);
}

int kiemTraNgayHopLe(int ngay, int thang, int nam) {
  if (nam < 1900 || nam > 2100)
    return 0;
  if (thang < 1 || thang > 12)
    return 0;
  if (ngay < 1)
    return 0;

  int ngayTrongThang[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if (laNamNhuan(nam)) {
    ngayTrongThang[2] = 29;
  }

  return ngay <= ngayTrongThang[thang];
}

void themHocVien(const char *filename) {
  HocVien hv;
  strcpy(hv.maLop, "");
  strcpy(hv.maSV, "");
  strcpy(hv.hoTen, "");
  hv.ngaySinh.ngay = 0; hv.ngaySinh.thang = 0; hv.ngaySinh.nam = 0;
  hv.dtbtl = -1.0;

  int currentIdx = 0;
  int w = 65;

  while(1) {
    xoaManHinh();
    inDuongNgang(0, w);
    inDongKhung("THEM MOI HO SO HOC VIEN", w, 1, "\033[1;35m", "\033[0m");
    inDuongNgang(1, w);
    inDongKhung("", w, 0, NULL, NULL);
    
    char items[7][150];
    sprintf(items[0], "Nhap Ma Lop:    [ %s ]", hv.maLop[0] ? hv.maLop : "...");
    sprintf(items[1], "Nhap Ma SV:     [ %s ]", hv.maSV[0] ? hv.maSV : "...");
    sprintf(items[2], "Nhap Ho Ten:    [ %s ]", hv.hoTen[0] ? hv.hoTen : "...");
    if (hv.ngaySinh.ngay != 0)
        sprintf(items[3], "Nhap Ngay Sinh: [ %02d/%02d/%04d ]", hv.ngaySinh.ngay, hv.ngaySinh.thang, hv.ngaySinh.nam);
    else
        sprintf(items[3], "Nhap Ngay Sinh: [ ... ]");
        
    if (hv.dtbtl >= 0)
        sprintf(items[4], "Nhap Diem:      [ %.2f ]", hv.dtbtl);
    else
        sprintf(items[4], "Nhap Diem:      [ ... ]");
        
    strcpy(items[5], "[ LUU HO SO ]");
    strcpy(items[6], "[ HUY BO VA QUAY LAI ]");
    
    for (int i = 0; i < 7; i++) {
        char line[200];
        if (i == currentIdx) {
            sprintf(line, "  -> %d. %s", i + 1, items[i]);
            inDongKhung(line, w, 0, "\033[1;32m", "\033[0m");
        } else {
            sprintf(line, "     %d. %s", i + 1, items[i]);
            inDongKhung(line, w, 0, NULL, NULL);
        }
    }
    
    inDongKhung("", w, 0, NULL, NULL);
    inDuongNgang(1, w);
    inDongKhung("(Dung phim mui ten Len/Xuong de chon, Enter de nhap)", w, 1, "\033[1;30m", "\033[0m");
    inDuongNgang(2, w);
    
    int ch1 = my_getch();
    if (ch1 == '\n' || ch1 == '\r') {
        printf("\n"); 
        if (currentIdx == 0) {
            nhapChuoiCoKiemTra("Nhap Ma lop: ", hv.maLop, sizeof(hv.maLop), "Khong duoc de trong!");
        } else if (currentIdx == 1) {
            while (1) {
                nhapChuoiCoKiemTra("Nhap Ma SV (Bat dau bang 'SV'): ", hv.maSV, sizeof(hv.maSV), "Khong duoc de trong!");
                if (strncmp(hv.maSV, "SV", 2) != 0 && strncmp(hv.maSV, "sv", 2) != 0 && strncmp(hv.maSV, "sV", 2) != 0 && strncmp(hv.maSV, "Sv", 2) != 0) {
                    printf("\033[31mLoi: Ma SV phai bat dau bang chu 'SV'!\033[0m"); fflush(stdout);
                    usleep(1500000);
                    printf("\r\033[2K\033[A\033[2K");
                    continue;
                }
                hv.maSV[0] = 'S'; hv.maSV[1] = 'V'; 
                
                if (kiemTraTrungMaSV(filename, hv.maSV)) {
                    printf("\033[31mLoi: Ma SV nay da ton tai!\033[0m"); fflush(stdout);
                    usleep(1500000);
                    printf("\r\033[2K\033[A\033[2K");
                } else {
                    break;
                }
            }
        } else if (currentIdx == 2) {
            nhapChuoiCoKiemTra("Nhap Ho va ten: ", hv.hoTen, sizeof(hv.hoTen), "Khong duoc de trong!");
            chuanHoaTen(hv.hoTen);
        } else if (currentIdx == 3) {
            char buffer[100];
            while(1) {
                printf("Nhap Ngay sinh (dd mm yyyy): ");
                if (fgets(buffer, sizeof(buffer), stdin)) {
                    if (sscanf(buffer, "%d %d %d", &hv.ngaySinh.ngay, &hv.ngaySinh.thang, &hv.ngaySinh.nam) == 3) {
                        if (kiemTraNgayHopLe(hv.ngaySinh.ngay, hv.ngaySinh.thang, hv.ngaySinh.nam)) {
                            break;
                        }
                    }
                }
                printf("\033[31mLoi: Ngay sinh khong hop le!\033[0m"); fflush(stdout);
                usleep(1000000);
                printf("\r\033[2K\033[A\033[2K");
            }
        } else if (currentIdx == 4) {
            char buffer[100];
            while(1) {
                printf("Nhap Diem TBTL (0-10): ");
                if (fgets(buffer, sizeof(buffer), stdin)) {
                    if (sscanf(buffer, "%f", &hv.dtbtl) == 1) {
                        if (hv.dtbtl >= 0.0 && hv.dtbtl <= 10.0) {
                            break;
                        }
                    }
                }
                printf("\033[31mLoi: Diem phai tu 0 den 10!\033[0m"); fflush(stdout);
                usleep(1000000);
                printf("\r\033[2K\033[A\033[2K");
            }
        } else if (currentIdx == 5) {
            if (strlen(hv.maLop) == 0 || strlen(hv.maSV) == 0 || strlen(hv.hoTen) == 0 || hv.ngaySinh.ngay == 0 || hv.dtbtl < 0) {
                xoaManHinh();
                inThongBao("Vui long nhap day du cac truong truoc khi Luu!", 2);
                tamDung();
            } else {
                FILE *f = fopen(filename, "ab");
                if (!f) {
                    xoaManHinh();
                    inThongBao("Khong the mo file de ghi!", 3);
                } else {
                    fwrite(&hv, sizeof(HocVien), 1, f);
                    fclose(f);
                    xoaManHinh();
                    inThongBao("Da them ho so hoc vien thanh cong!", 1);
                }
                tamDung();
                return;
            }
        } else if (currentIdx == 6) {
            return;
        }
    } else if (ch1 == 27) {
        int ch2 = my_getch();
        if (ch2 == 91) {
            int ch3 = my_getch();
            if (ch3 == 65) currentIdx = (currentIdx > 0) ? currentIdx - 1 : 6;
            else if (ch3 == 66) currentIdx = (currentIdx < 6) ? currentIdx + 1 : 0;
        }
    }
  }
}

void inDanhSach(const char *filename) {
  xoaManHinh();
  inDuongNgang(0, 76);
  inDongKhung("DANH SACH HOC VIEN (TOAN BO)", 76, 1, "\033[1;34m", "\033[0m");
  inDuongNgang(1, 76);

  FILE *f = fopen(filename, "rb");
  if (!f) {
    inDongKhung("File chua ton tai hoac khong co du lieu!", 76, 1, "\033[1;31m",
                "\033[0m");
    inDuongNgang(2, 76);
    tamDung();
    return;
  }

  printf("│ %-10s │ %-10s │ %-25s │ %-12s │ %-5s │\n", "Ma Lop", "Ma SV",
         "Ho Ten", "Ngay Sinh", "DTBTL");
  printf("├────────────┼────────────┼───────────────────────────┼──────────────"
         "┼───────┤\n");

  HocVien hv;
  int count = 0;
  while (fread(&hv, sizeof(HocVien), 1, f) == 1) {
    printf("│ %-10s │ %-10s │ %-25s │ %02d/%02d/%04d   │ %-5.2f │\n", hv.maLop,
           hv.maSV, hv.hoTen, hv.ngaySinh.ngay, hv.ngaySinh.thang,
           hv.ngaySinh.nam, hv.dtbtl);
    count++;
  }
  fclose(f);

  if (count == 0) {
    inDongKhung("(Danh sach rong)", 76, 1, NULL, NULL);
    inDuongNgang(2, 76);
  } else {
    printf("└────────────┴────────────┴───────────────────────────┴────────────"
           "──┴───────┘\n");
    char summaryStr[100];
    sprintf(summaryStr, "Tong cong: %d sinh vien.", count);
    inDuongNgang(0, 76);
    inDongKhung(summaryStr, 76, 1, "\033[1;32m", "\033[0m");
    inDuongNgang(2, 76);
  }

  tamDung();
}

/*
 =====================================================================
 HƯỚNG DẪN BIÊN DỊCH VÀ CHẠY CHƯƠNG TRÌNH (DÀNH CHO TERMINAL LINUX)
 =====================================================================

 1. Cài đặt GCC (nếu chưa có):
    sudo apt install gcc

 2. Sinh dữ liệu giả lập (100 người) trước để chạy thử chức năng in (Mục 2):
    Mở terminal tại thư mục chứa file, chạy lệnh:
    gcc tao_du_lieu_mau.c -o tao_du_lieu_mau && ./tao_du_lieu_mau
    (File sinhvien.bin sẽ được tự động tạo ra với dữ liệu theo yêu cầu).

 3. Biên dịch chương trình chính:
    gcc quanlysinhviendemo.c -o qlsv

 4. Chạy chương trình:
    ./qlsv

 5. Gỡ lỗi (Debug với GDB):
    - Biên dịch có thông tin debug: gcc -g quanlysinhviendemo.c -o qlsv
    - Mở bằng gdb: gdb ./qlsv
    - Gõ lệnh "run" trong gdb để bắt đầu chạy.
*/
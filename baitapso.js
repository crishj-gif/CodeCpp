// Hiển thị hộp thoại để người dùng nhập phép tính (chạy trên trình duyệt)
let s = prompt("Nhập phép tính: ");

let trangthai = 1;
let sohang1 = 0;
let sohang2 = 0;
let toantu = "";

// Vòng lặp for duyệt qua từng ký tự giống hệt C
for (let i = 0; i < s.length; i++) {
    let c = s[i];

    // Kiểm tra ký tự từ '0' đến '9'
    if (c >= '0' && c <= '9') {
        let chuso = parseInt(c); // Hàm parseInt() tương đương int() trong Python
        
        // Dùng === để so sánh bằng (chuẩn thực hành tốt trong JS)
        if (trangthai === 1) {
            sohang1 = sohang1 * 10 + chuso;
        } else if (trangthai === 2) {
            sohang2 = sohang2 * 10 + chuso;
        }
    } 
    // Kiểm tra toán tử
    else if (c === '+' || c === '-' || c === '*' || c === '/') {
        toantu = c;
        trangthai = 2;
    }
}

// Xử lý tính toán và in kết quả ra Console
if (toantu === '+') {
    console.log("Kết quả phép tính là:", sohang1 + sohang2);
} else if (toantu === '-') {
    console.log("Kết quả phép tính là:", sohang1 - sohang2);
} else if (toantu === '*') {
    console.log("Kết quả phép tính là:", sohang1 * sohang2);
} else if (toantu === '/') {
    if (sohang2 !== 0) {
        console.log("Kết quả phép tính là:", sohang1 / sohang2);
    } else {
        console.log("Lỗi: Không thể chia cho 0");
    }
} else if (toantu === "") {
    console.log("Bạn chưa nhập toán tử phép tính!");
}
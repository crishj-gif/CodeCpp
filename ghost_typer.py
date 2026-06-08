import difflib
from pynput import keyboard
import time

# Đường dẫn file thật và file backup (Giữ nguyên như của bạn)
MAIN_FILE = "/home/soncris/Documents/CodeCpp/quanlysinhviendemo.c"
BACKUP_FILE = "/home/soncris/Documents/CodeCpp/backup.c"

controller = keyboard.Controller()
idx = 0
mode = "NORMAL"
last_type_time = 0
text_to_type = ""

def get_missing_code():
    """Thuật toán thông minh: Chỉ lấy khối code bị xóa lớn nhất, bỏ qua lỗi format linh tinh"""
    try:
        with open(BACKUP_FILE, 'r', encoding='utf-8') as f:
            backup = f.read()
        with open(MAIN_FILE, 'r', encoding='utf-8') as f:
            current = f.read()
            
        matcher = difflib.SequenceMatcher(None, current, backup)
        missing_chunks = []
        
        for tag, i1, i2, j1, j2 in matcher.get_opcodes():
            if tag in ('insert', 'replace'):
                chunk = backup[j1:j2]
                if len(chunk.strip()) > 0:
                    missing_chunks.append(chunk)
        
        # Lọc ra đoạn code bị xóa dài nhất để tránh nhiễu do VS Code tự format
        if missing_chunks:
            return max(missing_chunks, key=len)
        return ""
    except Exception as e:
        print(f"[!] Lỗi đọc file: {e}")
        return ""

def on_press(key):
    global idx, mode, last_type_time, text_to_type

    if time.time() - last_type_time < 0.05:
        return

    # F9: LÊN NÒNG
    if key == keyboard.Key.f9:
        if mode == "NORMAL":
            text_to_type = get_missing_code()
            if not text_to_type:
                print("\n[!] Không tìm thấy thay đổi. Chắc chắn bạn đã LƯU (Ctrl+S) file chưa?")
                return
            idx = 0
            mode = "HACK"
            print(f"\n[+] ĐÃ BẬT HACK. Sẵn sàng nã {len(text_to_type)} ký tự!")
        else:
            mode = "NORMAL"
            print("\n[-] ĐÃ TẮT HACK.")
        return

    # F12: THOÁT
    elif key == keyboard.Key.f12:
        print("\n[!] Thoát Tool.")
        return False

    if mode == "HACK":
        # FIX LỖI 1: Chấp nhận cả phím chữ, số và PHÍM CÁCH (Spacebar)
        valid_key = False
        if hasattr(key, 'char') and key.char is not None:
            valid_key = True
        elif key == keyboard.Key.space:
            valid_key = True

        if valid_key:
            if idx < len(text_to_type):
                last_type_time = time.time()
                
                # Xóa phím rác bạn vừa gõ
                controller.press(keyboard.Key.backspace)
                controller.release(keyboard.Key.backspace)

                # Gõ ký tự chuẩn
                char = text_to_type[idx]
                controller.type(char)
                idx += 1

                # FIX LỖI 2 (CHÍ MẠNG): Trị Auto-Indent của VS Code
                # Nếu tool vừa gõ phím Enter (\n), VS Code sẽ tự thụt lùi lề.
                # Do đó, Tool phải tự động bỏ qua toàn bộ dấu cách/tab ở đầu dòng tiếp theo trong bản backup!
                if char == '\n':
                    while idx < len(text_to_type) and text_to_type[idx] in (' ', '\t'):
                        idx += 1
            else:
                mode = "NORMAL"
                print("\n[-] Đã khôi phục 100% vẹn nguyên. Tự động tắt Hack.")

print(">>> V5.0 ULTIMATE (Anti-Format Edition) Đã sẵn sàng!")
print(">>> [F9] Bật/Tắt | [F12] Thoát hoàn toàn")

with keyboard.Listener(on_press=on_press) as listener:
    listener.join()
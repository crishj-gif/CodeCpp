#include <gtk/gtk.h>

// Hàm này sẽ được gọi khi bạn nhấn vào nút (Callback function)
static void print_hello(GtkWidget *widget, gpointer data) {
    g_print("Xin chào! Nút đã được nhấn.\n");
}

// Hàm kích hoạt ứng dụng, dùng để dựng giao diện
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button;

    // 1. Tạo một cửa sổ mới
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Cửa sổ GTK đầu tiên");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

    // 2. Tạo một nút bấm
    button = gtk_button_new_with_label("Nhấn vào đây!");

    // 3. Bắt sự kiện: Khi nút được nhấn ("clicked"), gọi hàm print_hello
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);

    // 4. Đặt nút bấm vào trong cửa sổ
    gtk_container_add(GTK_CONTAINER(window), button);

    // 5. Hiển thị tất cả các thành phần (widget) bên trong cửa sổ
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Khởi tạo ứng dụng GTK
    app = gtk_application_new("com.example.GtkApp", G_APPLICATION_DEFAULT_FLAGS);
    
    // Kết nối sự kiện "activate" với hàm dựng giao diện ở trên
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Chạy vòng lặp sự kiện chính của ứng dụng
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Dọn dẹp bộ nhớ khi tắt app
    g_object_unref(app);

    return status;
}
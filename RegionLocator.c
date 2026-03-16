// #include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

// #ifdef _MSC_VER
// #pragma execution_character_set("utf-8")
// #endif

static int is_vk_pressed(int vk_code) {
  return (GetAsyncKeyState(vk_code) & 0x8000) != 0;
}

static void get_cursor_position(LONG *x, LONG *y) {
  POINT point;
  GetCursorPos(&point);
  *x = point.x;
  *y = point.y;
}

static void wait_f12(const char *record_prompt, LONG *x, LONG *y) {
  int was_pressed;
  int is_pressed;

  printf("%s\n", record_prompt);
  was_pressed = is_vk_pressed(VK_F12);

  for (;;) {
    get_cursor_position(x, y);
    printf("当前坐标: X=%ld, Y=%ld\r", *x, *y);
    fflush(stdout);

    is_pressed = is_vk_pressed(VK_F12);
    if (is_pressed && !was_pressed) {
      printf("\n已记录坐标: X=%ld, Y=%ld\n", *x, *y);
      return;
    }

    was_pressed = is_pressed;
    Sleep(30);
  }
}

int main(void) {
  LONG left_top_x, left_top_y;
  LONG right_bottom_x, right_bottom_y;
  LONG left, top, width, height;

  SetConsoleOutputCP(CP_UTF8);
  // SetConsoleCP(CP_UTF8);
  // setlocale(LC_ALL, ".UTF8");

  printf("GTAOL CEO Helper - 识别区域定位工具\n");
  printf("--------------------------------\n");

  wait_f12("请将鼠标移动到矩形识别区域左上角后按 F12...", &left_top_x,
           &left_top_y);
  wait_f12("\n请将鼠标移动到矩形识别区域右下角后按 F12...", &right_bottom_x,
           &right_bottom_y);

  left = (left_top_x < right_bottom_x) ? left_top_x : right_bottom_x;
  top = (left_top_y < right_bottom_y) ? left_top_y : right_bottom_y;
  width = labs(right_bottom_x - left_top_x);
  height = labs(right_bottom_y - left_top_y);

  printf("--------------------------------\n");
  printf("识别区域左上角坐标: X=%ld, Y=%ld\n", left, top);
  printf("识别区域长宽: Width=%ld, Height=%ld\n", width, height);
  printf("请将以上值填入 config.yaml\n");
  printf("\n按回车键退出...");
  fflush(stdout);
  getchar();

  return 0;
}

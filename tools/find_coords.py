import pyautogui
import time
import keyboard


def wait_f12(record_prompt):
    print(record_prompt)
    was_pressed = keyboard.is_pressed("f12")

    while True:
        x, y = pyautogui.position()
        print(f"当前坐标: X={x}, Y={y}", end="\r")

        is_pressed = keyboard.is_pressed("f12")
        if is_pressed and not was_pressed:
            print(f"\n已记录坐标: X={x}, Y={y}")
            return x, y

        was_pressed = is_pressed
        time.sleep(0.03)


def main():
    print("GTAOL CEO Helper - 识别区域定位工具")
    print("--------------------------------")

    try:
        left_top = wait_f12("请将鼠标移动到矩形识别区域左上角后按 F12...")
        right_bottom = wait_f12("\n请将鼠标移动到矩形识别区域右下角后按 F12...")

        left = min(left_top[0], right_bottom[0])
        top = min(left_top[1], right_bottom[1])
        width = abs(right_bottom[0] - left_top[0])
        height = abs(right_bottom[1] - left_top[1])

        print("--------------------------------")
        print(f"识别区域左上角坐标: X={left}, Y={top}")
        print(f"识别区域长宽: Width={width}, Height={height}")
        print("请将以上值填入 config.yaml")
        input("\n按回车键退出...")

    except KeyboardInterrupt:
        print("\n已退出。")


if __name__ == "__main__":
    main()

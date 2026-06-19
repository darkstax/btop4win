"""Capture btop4win window screenshot for automated CJK verification."""
import mss
import sys
import ctypes
from ctypes import wintypes
from pathlib import Path

OUTPUT_DIR = Path(__file__).parent.parent / "screenshots"
OUTPUT_DIR.mkdir(exist_ok=True)

# Win32 API
user32 = ctypes.windll.user32
kernel32 = ctypes.windll.kernel32

def find_btop_window():
    """Find the btop console window by enumerating top-level windows."""
    btop_hwnd = None
    WNDENUMPROC = ctypes.WINFUNCTYPE(ctypes.c_bool, wintypes.HWND, wintypes.LPARAM)

    def enum_proc(hwnd, lparam):
        nonlocal btop_hwnd
        if not user32.IsWindowVisible(hwnd):
            return True
        length = user32.GetWindowTextLengthW(hwnd)
        if length == 0:
            return True
        buf = ctypes.create_unicode_buffer(length + 1)
        user32.GetWindowTextW(hwnd, buf, length + 1)
        title = buf.value
        # btop console window title contains "btop" or "btop4win"
        if title and ("btop" in title.lower() or "btop4win" in title.lower()):
            btop_hwnd = hwnd
            print(f"Found btop window: '{title}' (hwnd={hwnd})")
            return False  # stop enumeration
        return True

    user32.EnumWindows(WNDENUMPROC(enum_proc), 0)
    return btop_hwnd

def capture_window(hwnd, output_path):
    """Capture a specific window by its HWND."""
    # Get window rect
    rect = wintypes.RECT()
    user32.GetWindowRect(hwnd, ctypes.byref(rect))

    left, top = rect.left, rect.top
    width = rect.right - rect.left
    height = rect.bottom - rect.top

    if width <= 0 or height <= 0:
        print(f"Window rect invalid: {width}x{height}")
        return False

    print(f"Window rect: left={left} top={top} width={width} height={height}")

    with mss.mss() as sct:
        monitor = {"left": left, "top": top, "width": width, "height": height}
        sct_img = sct.grab(monitor)
        mss.tools.to_png(sct_img.rgb, sct_img.size, output=str(output_path))

    print(f"Screenshot saved: {output_path}")
    return True

def capture(output_name="btop_latest.png"):
    """Find btop window and capture it."""
    output_path = OUTPUT_DIR / output_name

    hwnd = find_btop_window()
    if hwnd:
        return capture_window(hwnd, output_path)

    # Fallback: full screen
    print("btop window not found, capturing full screen...")
    with mss.mss() as sct:
        sct.shot(output=str(output_path))
    print(f"Full screenshot saved: {output_path}")
    return True

if __name__ == "__main__":
    name = sys.argv[1] if len(sys.argv) > 1 else "btop_latest.png"
    capture(name)

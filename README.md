# C PICKER


# C PICKER Color Picker

**C PICKER** is a system tray-based color picker tool written in C using SDL2 and WinAPI. It enables quick RGB/HEX/HSV/HSL color selection, preview, and clipboard copy from anywhere on the screen. Ideal for UI/UX designers, developers, and digital artists.

---

## ✨ Features

- 🖱️ System-wide color picking via cursor
- 🎨 RGB, HEX, HSV, and HSL readouts
- 📋 One-click copy to clipboard
- 🔲 Smooth, rounded SDL2 UI
- 🕒 Color history with timestamps
- 🧰 Always-on-top shaped window
- 📦 Tray icon toggle to show/hide window

---

## 📸 Preview

![app](https://github.com/user-attachments/assets/0085c2f4-56c1-4674-a2b3-6fef196e5a2f)
---

## 🛠 Dependencies

- [SDL2](https://libsdl.org/)
- [SDL2_ttf](https://github.com/libsdl-org/SDL_ttf)
- Windows API (Win32)
- OpenMP 

> ⚠️ This project is designed for **Windows** only.

---

## 🔧 Build Instructions (Windows)

### Prerequisites:
- `gcc` or `clang` (MinGW recommended)
- SDL2 development libraries
- `SDL2_ttf` development libraries

### Build example (MinGW):

```bash
gcc main.c -o main.exe -lSDL2 -lSDL2_ttf -lgdi32 -lole32 -lcomdlg32 -loleaut32 -luser32 -lShell32 -mwindows -fopenmp

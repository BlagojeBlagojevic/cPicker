

---

# ColorPicker

A lightweight and always-on-top color picking utility for Windows, built with SDL2. This application allows you to instantly grab the color under your mouse cursor, display its values in various formats (RGB, HEX, HSV, HSL), and copy them to your clipboard. It also features a minimalist system tray icon for quick access and a color history.



---

## Features

* **System-wide Color Picking**: Easily pick colors from any part of your screen using a simple keyboard shortcut.
* **Multiple Color Formats**: View the picked color in **RGB**, **HEX**, **HSV**, and **HSL** formats.
* **Clipboard Integration**: Copy the color values directly to your clipboard for quick use in other applications.
* **Always-on-Top Window**: The main application window and a small follow-up window (for precise picking) stay visible above other applications.
* **System Tray Icon**: Minimize the application to the system tray for unobtrusive background operation and quick access.
* **Color History**: (Inferred from `Date_Color_DA` and `create_date_color`) Likely stores a history of picked colors along with the time they were picked.

---

## Installation & Building

This project is written in C and uses **SDL2**, **SDL2_ttf**, and a custom `tray.h` library.

### Prerequisites

* **MinGW-w64** (or a compatible C compiler like MSVC)
* **SDL2 Development Libraries**: You'll need the `SDL2.dll`, `SDL2.lib`, and the `include` folder.
* **SDL2_ttf Development Libraries**: You'll need `SDL2_ttf.dll`, `SDL2_ttf.lib`, and its `include` folder.
* A TrueType Font (`.ttf`) file. The code specifies `"assets/fonts/LB.ttf"`. Make sure this path is correct or update `fontLoc` in the source.

### Build Instructions 
```bash
gcc main.c -o stagod.exe -lSDL2 -lSDL2_ttf -lgdi32 -lole32 -lcomdlg32 -loleaut32 -luser32 -lShell32 -mwindows -fopenmp

```
---

## Usage

1.  **Run the Application**: Execute `ColorPicker.exe`. It will likely appear in your system tray by default.
2.  **Access Main Window**: Click on the system tray icon and select "CPICK" to bring up the main application window.
3.  **Pick a Color**:
    * Click the "Select Color" button in the main window.
    * Move your mouse cursor over the desired color on your screen.
    * Press `A + Space` (as inferred from `IsKeyPressedFollow`) to activate the small follow-up window and `S + Space` (as inferred from `IsKeyPressedExtendedWindow`) to capture the color and display it in the main window.
    * (Alternatively, a single click with the left mouse button might also pick the color if `program_state.isMouseClicked` is handled on an element click after activating selection).
4.  **Copy Color Values**: Click on the displayed HEX or RGB values to copy them to your clipboard.

---


## Contributing

No specific contributing guidelines are provided, but feel free to fork the repository and submit pull requests.

---

## License

MIT
---

## Preview

![app](https://github.com/user-attachments/assets/0085c2f4-56c1-4674-a2b3-6fef196e5a2f)

---



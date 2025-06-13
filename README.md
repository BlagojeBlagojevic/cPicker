

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

### Build Instructions (Inferred for MinGW)

1.  **Place Libraries**:
    * Create a `lib` folder in your project root and place `SDL2.lib` and `SDL2_ttf.lib` inside it.
    * Create an `include` folder in your project root. Inside `include`, create an `SDL2` folder and copy all `.h` files from SDL2's `include` directory into it. Do the same for SDL2_ttf.
    * Ensure the `tray.h` file is in the same directory as your main source file, or within your compiler's include path.
2.  **Compile**:
    Open a terminal or command prompt in your project directory and use a command similar to this (adjust paths as necessary):

    ```bash
    gcc -o ColorPicker.exe main.c -I./include -L./lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lopengl32 -lwinmm -static-libgcc -static-libstdc++ -Wl,--no-undefined -mwindows
    ```
    * `main.c`: Your primary source file.
    * `-I./include`: Specifies the directory for additional include files.
    * `-L./lib`: Specifies the directory for additional library files.
    * `-lSDL2main -lSDL2 -lSDL2_ttf`: Links the SDL2, SDL2_ttf, and SDL2main libraries.
    * `-lopengl32 -lwinmm -static-libgcc -static-libstdc++`: Common libraries needed for SDL2 on Windows and static linking for portability.
    * `-mwindows`: Prevents a console window from opening.

3.  **Runtime Dependencies**:
    After compilation, ensure `SDL2.dll` and `SDL2_ttf.dll` (and your chosen font file in the `assets/fonts/` directory relative to the executable) are in the same directory as your compiled `ColorPicker.exe`.

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

## Known Issues / Limitations (Guessed)

* **Windows-Specific**: The code uses `Windows.h`, `GetPixel`, `GetCursorPos`, and `GetAsyncKeyState`, making it a Windows-only application.
* **Font Dependency**: Requires a `LB.ttf` font file in `assets/fonts/`.
* **Limited Error Handling**: While there are `SDL_ERR` and `ERROR_BREAK` macros, extensive error handling for all possible scenarios might not be present.
* **Hardcoded UI Layout**: The UI elements are positioned with hardcoded coordinates.

---

## Contributing

No specific contributing guidelines are provided, but feel free to fork the repository and submit pull requests.

---

## License

(Please specify your desired license here, e.g., MIT, GPL, etc. If not specified, the code is under implied copyright.)

---

## Preview

![app](https://github.com/user-attachments/assets/0085c2f4-56c1-4674-a2b3-6fef196e5a2f)

---



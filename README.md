<div align="center">

# 🏙️ Cox's Bazar - Full City View
**A Dynamic 2D OpenGL Experience in C++**

[![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](#)
[![OpenGL](https://img.shields.io/badge/OpenGL-FFFFFF?style=for-the-badge&logo=opengl)](#)
[![Code::Blocks](https://img.shields.io/badge/Code::Blocks-4A90E2?style=for-the-badge&logo=codeblocks&logoColor=white)](#)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](https://opensource.org/licenses/MIT)

*Experience the scenic beauty of Cox's Bazar with a dynamic day/night cycle, realistic traffic, ocean views, and weather effects.*

> This project was developed as part of the **Computer Graphics course at AIUB** (American International University-Bangladesh). 

[Explore Features](#-features) • [Installation](#-installation) • [Controls](#-controls) • [Contributing](#-contributing)

</div>

---

## 📸 Gallery

<p align="center">
  <!-- 
  TODO: Replace 'screenshot_day.png' and 'screenshot_night.png' with your actual screenshot filenames.
  Make sure to place the image files in the 'assets/images' folder.
  -->
  <img src="assets/images/screenshot_day.png" width="48%" alt="Day Mode View" />
  <img src="assets/images/screenshot_night.png" width="48%" alt="Night Mode View" />
</p>

## ✨ Features

- 🌤️ **Dynamic Day/Night Cycle** – Seamlessly switch between a bright day and a starry night.
- 🔊 **Immersive Audio** – Integrated sound effects for passing cars, ship horns, and falling rain.
- 🌧️ **Weather System** – Toggle rain with realistic visual drops and ambient sound.
- 🚗 **Dynamic Traffic** – Animated vehicles traveling in distinct lanes with speed adjustments.
- ⛴️ **Ocean Scenery** – Moving ships along the shoreline with distinct models for varied distances.
- ☁️ **Procedural Sky** – Clouds passing during the day and softly glowing stars lighting up the night sky.

---

## 📂 Project Structure

```text
/
├── assets/
│   ├── car-passing.wav      # 🎵 Sound effects
│   ├── light-rain.wav
│   └── ship.wav
├── main.cpp                 # 🧠 Main OpenGL rendering logic
├── test3.cbp                # ⚙️ Code::Blocks project file
├── README.md                # 📖 Project documentation
└── .gitignore               # 🚫 Git ignore rules
```

## 🎮 Controls

Take control of the environment using your keyboard:

| Key | Action |
| :---: | :--- |
| <kbd>n</kbd> | Switch to **Night Mode** 🌙 |
| <kbd>d</kbd> | Switch to **Day Mode** ☀️ |
| <kbd>c</kbd> | Play **Car Passing** sound 🚗 |
| <kbd>s</kbd> | Play **Ship Horn** sound 🚢 |
| <kbd>r</kbd> | Toggle **Rain & Audio** 🌧️ |
| <kbd>ESC</kbd> | Exit the application ❌ |

---

## 🚀 Installation & Usage

### 🛠️ Prerequisites
- **C++ Compiler** (e.g., MinGW)
- **OpenGL & GLUT** libraries configured
- **Windows OS** (required for `<mmsystem.h>` audio playback)

### 🖥️ Running via Code::Blocks
1. Open the `test3.cbp` project file.
2. Ensure GLUT is correctly linked in your build settings.
3. Press **F9** to **Build and Run**.
> ⚠️ **Note:** To hear sound effects when running directly from your IDE's bin folder, ensure the `.wav` files are in the same directory as the executable.

### 💻 Manual Compilation (CLI)
Using MinGW, run the following command to link the required libraries:
```bash
g++ main.cpp -o app.exe -lglut32 -lopengl32 -lglu32 -lwinmm -lgdi32
```
After building, ensure your `assets/` audio files sit alongside the compiled `app.exe` before executing.

---

## 🤝 Contributing

Contributions, issues, and feature requests are highly appreciated! 

1. **Fork** the repository
2. **Create** your feature branch (`git checkout -b feature/AmazingFeature`)
3. **Commit** your changes (`git commit -m 'Add some AmazingFeature'`)
4. **Push** to the branch (`git push origin feature/AmazingFeature`)
5. **Open a Pull Request**

## 📜 License

Distributed under the [MIT License](LICENSE). See `LICENSE` for more information.

<div align="center">
  <sub>Built with ❤️ using C++ and OpenGL.</sub>
</div>

# 🕹️ Game Engine – Setup & Run Guide

This guide explains how to clone, build, and run the Game Engine project locally.

---

## 📦 Prerequisites

Before starting, make sure you have the following installed:

- **Git**
- **CMake (3.29 or higher)**
  - Can also use Visual Studio 2022 (recommended on Windows) along with a C++20 compatible compiler
- **SDL3**
- **SDL3_image**
- **SDL3_ttf (fonts)**

Make sure SDL3 and SDL3_image are properly installed and their `include`, `lib`, and `bin` directories are available.

---

## 📥 Clone the Repository

Open a terminal and run:

```bash
git clone https://github.com/Nav-Codes/game-engine
cd your-repo-name

```

## ⚙️ Configure SDL Paths

Before building, you need to update the SDL library paths in `CMakeLists.txt` to match where SDL is installed on your system.

### 🔧 Locate the SDL path section

Open `CMakeLists.txt` and find:

```cmake
if(WIN32)
    set(SDL3_PATH "C:/path/to/your/sdl3")
    set(SDL3_IMAGE_PATH "C:/path/to/your/sdl3_image")
    set(SDL3_TTF_PATH "C:/path/to/your/sdl3_ttf")
```    
Update the paths to each of the libraries to wherever you downloaded the libraries.    

### ⚠️ Common build/run issue

The SDL3 .dll's should come with the repo when you clone it.
If you are facing issues running the game because certain .dll's are not available.

1. Locate your SDL3 installation directory.
2. Go to:

```
SDL3-devel-3.2.20-mingw\SDL3-3.2.20\x86_64-w64-mingw32\bin
```

Copy:

```
SDL3.dll
```

3. Then go to your SDL3_image directory:

```
SDL3_image-devel-3.2.4-mingw\SDL3_image-3.2.4\x86_64-w64-mingw32\bin
```

Copy:

```
SDL3_image.dll
```

4. Then go to your SDL3_ttf directory:

```
SDL3_ttf-devel-3.2.2-mingw\SDL3_ttf-3.2.2\x86_64-w64-mingw32\bin
```

Copy:

```
SDL3_ttf.dll
```

5. Paste all .dll files into:

```
cmake-build-debug/
```

Place them in the **same folder as the generated `.exe` file**.

If these DLLs are missing, the program will not start and may exit with error code `0xC0000135`.

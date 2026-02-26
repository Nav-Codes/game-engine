# 🕹️ Game Engine – Setup & Run Guide

This guide explains how to clone, build, and run the Game Engine project locally.

---

## 📦 Prerequisites

Before starting, make sure you have the following installed:

- **Git**
- **CMake (3.29 or higher)**
- **C++20 compatible compiler**
    - Visual Studio 2022 (recommended on Windows)
- **SDL3**
- **SDL3_image**

Make sure SDL3 and SDL3_image are properly installed and their `include`, `lib`, and `bin` directories are available.

---

## 📥 Clone the Repository

Open a terminal and run:

```bash
git clone https://github.com/Nav-Codes/game-engine
cd your-repo-name

```
### ⚠️ IMPORTANT – Copy Required SDL DLLs (Windows Only)

If you are on Windows, you must manually copy the required SDL runtime DLL files into the build folder.

1. Locate your SDL3 installation directory.
2. Go to:

```
SDL3/bin/
```

Copy:

```
SDL3.dll
```

3. Then go to your SDL3_image directory:

```
SDL3_image/bin/
```

Copy:

```
SDL3_image.dll
```

4. Paste BOTH files into:

```
cmake-build-debug/
```

Place them in the **same folder as the generated `.exe` file**.

If these DLLs are missing, the program will not start and may exit with error code `0xC0000135`.

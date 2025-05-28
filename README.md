# 2D Drawing Program in C++
A Windows desktop application written in C++ for interactive 2D drawing and graphics algorithm visualization.

## Features

- **Customizable UI:**
  - White or Dark background window
  - Custom mouse cursor shape
  - Mouse-only interaction (no keyboard input)
  - Dropdown menus for options and commands

- **Drawing:**
  - Select shape color before drawing from menu
  - Draw points, lines, circles, ellipses, polygons, and curves

- **Algorithms Implemented:**
  - **Line Drawing:**
    - DDA (Digital Differential Analyzer)
    - Midpoint line algorithm
    - Parametric line algorithm
  - **Circle Drawing:**
    - Direct method
    - Polar coordinates method
    - Iterative polar method
    - Midpoint circle algorithm
    - Modified midpoint circle algorithm
  - **Ellipse Drawing:**
    - Direct
    - Polar
    - Midpoint
  - **Curve Drawing:**
    - Hermite curves 
    - Bezier curves 
    - Cardinal spline curves
  - **Filling Algorithms:**
    - Circle filling with lines and smaller circles (with user-selected quarter)
    - Filling square using Hermite curve (Vertical)
    - Filling rectangle using Bezier curve (Horizontal)
    - Convex and Non-Convex polygon filling
    - Recursive and Non-recursive flood fill algorithms
  - **Clipping Algorithms:**
    - Rectangle clipping window for points, lines, polygons
    - Square clipping window for points and lines

- **Additional Functionalities:**
  - Clear screen option to erase all shapes
  - Save current drawing data to file
  - Load drawing data from file

## How to Build

1. Requirements:
   
   * Windows OS
   * C++ compiler supporting Windows API (e.g., MSVC / Visual Studio)
  
2. Steps:

```bash
# Clone repo
git clone https://github.com/alaaashraf19/2D-Drawing-Program.git
cd 2D-Drawing-Program

# Build using your method (Visual Studio project - Windows Desktop Application C++ / CMake etc.)
# For example, open the `.sln` file in Visual Studio and build

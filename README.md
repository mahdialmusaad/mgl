## Overview
A simple yet powerful OpenGL abstractor to heavily simplify creating graphical applications.
## Features
- Simple and intuitive API
- Low overhead abstraction
- Extensive error checking
- Compiles in standard C99
- Supports OpenGL 3.3+ contexts
The test file demonstrates most features available (frame capture, indirect and instanced drawing, uniforms, textures, etc).
## Building
The provided CMake file can be used to create a static library and build the test program. You can alternatively compile the source file alongside your own program and include the header.
```bash
git clone https://github.com/mahdialmusaad/mgl
cd mgl
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build --config=Release
```

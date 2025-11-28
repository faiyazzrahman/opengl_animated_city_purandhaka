# OpenGL Project with FreeGLUT

## Overview
This is an OpenGL project developed using **Code::Blocks** and **FreeGLUT**. It demonstrates basic OpenGL graphics and can be built and run on Windows using Code::Blocks.

---

## Prerequisites
Before running the project, you need:

1. **Code::Blocks IDE** (with GCC/MinGW compiler)  
   Download: [http://www.codeblocks.org/downloads](http://www.codeblocks.org/downloads)

2. **FreeGLUT** (OpenGL Utility Toolkit replacement)  
   Download development package: [https://www.transmissionzero.co.uk/software/freeglut-devel/](https://www.transmissionzero.co.uk/software/freeglut-devel/)

---

## How to Open and Run the Project

1. **Download or Clone the Repository**  
   ```bash
   git clone https://github.com/username/repo.git
Or download the ZIP and extract it.

Open in Code::Blocks

Open Code::Blocks.

Go to File → Open… and select the .cbp project file.

Set Up FreeGLUT in Code::Blocks

Go to Project → Build Options → Search directories:

Compiler tab: add the path to the include folder from FreeGLUT.

Linker tab: add the path to the lib folder from FreeGLUT.

Go to Project → Build Options → Linker settings:

Add the library freeglut (or libfreeglut.a for MinGW).

Copy freeglut.dll from FreeGLUT bin folder to the same folder as the .exe file.

Build and Run

Build the project: Build → Build (or press F9).

Run the project: Build → Run (or press Ctrl+F10).

Project Structure
rust
Copy code
ProjectRoot/
│
├─ src/            -> Source files (.c/.cpp)
├─ include/        -> Header files (.h)
├─ resources/      -> Textures, models, or other assets
├─ .gitignore      -> Files to ignore in version control
└─ project.cbp     -> Code::Blocks project file
Notes
Make sure freeglut.dll is in the same folder as the executable.

Avoid modifying library paths unless you know what you are doing.

This project is for Windows; Linux or Mac users may need different setup steps.

License
This project is released under [Specify Your License, e.g., MIT License].

# Password Manager
A fully offline password manager which will use a file as password database which is unlocked by a master key. 

> This is an attempt to make a cross-platform password manager in C++. I don't know when this will get completed or reach a usable state because this is just a **hobby project under no deadline.**

## 🚧 Status
**Currently, in development. It is not in a usable state.**  

I am just writing APIs for the core functionality to make an GUI wrapper around it.

## Dependencies
1. clang, gcc or MSVC
2. cmake & make
3. catch2
4. crypto++
5. nlohmann-json

## 🔨 Building
### Linux/MacOS
To build the project just run **build.sh** Bash script after getting the dependencies.  
Make sure to edit it and replace *'make -j 8'* with *'make'* if you don't know about your system or make. 

    $ git clone https://github.com/CowGivesMilk/password_manager
    $ cd password_manager
    $ ./build.sh

Executable will be: **password_manager/build/password_manager**  
Note: _I don't have an Apple Computer. So, I can not make sure it will work or not. I hope it will work since, I won't be using any OS Specific code._
### Windows
I don't use Windows. So, I haven't made a build script for it. I'll do it later.  
You can just compile it yourself. It's not hard.

## Contributions
I want this as my personal solo project. So, I won't add any people as contributors. But, if you have a suggestion or critique please tell me. 
## 📜 License
Licensed under the MIT License  
© 2025 Nimesh Poudel aka CowGivesMilk
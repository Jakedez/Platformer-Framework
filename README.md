# Overview


This project includes a simple 2D platformer game with simple obstacles such as platforms with different properties, such as Ice, Sand, or fake platforms. Additionally, this Project includes a level editor which can be used to create custom levels for the platformer game.

This is a project I have been working on that was designed to help me practice programming with C and C++, as well as to help me learn how to overcome some of the challenges of video game development.

{Provide a link to your YouTube demonstration.  It should be a 4-5 minute demo of the game being played and a walkthrough of the code.}

[Software Demo Video](https://youtu.be/XB803NogXBo)

# Development Environment

To develop this software, I utilized both Visual Studio, and Visual Studio Code. For minor graphical elements such as the splash screen, and Desktop Icon, I used Adobe Photoshop Elements, as well as a 64-bit implementation of the Apophysis Fractal Generator (for the splash screen).

The actual platformer was written in C++, and utilizes the Raylib Library as well as the Standard Library.

The Editor that is included was written in C, and also uses the Raylib Library, and C standard library (Including stdio.h, stdlib.h, stdbool.h, math.h, and string.h), the minGW package library "Lib Loader API" is also used. 

I also incorporated a small library I wrote myself that I include by using a header file called "splashscreen.h", this library uses raylib, the c standard library, and the "Pthreads" library.

Another Library I created called "Easy Dialog" was used to incorporate some WinAPI elements from the win32 API library (windows.h).


# Useful Websites

* [Raylib | a simple and easy-to-use library to enjoy videogames programming](https://www.raylib.com/)
* [Raylib | Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
* [MessageBox function (winuser.h)](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox)
* [GetOpenFileName Function](http://www.jasinskionline.com/windowsapi/ref/g/getopenfilename.html)
* [GetSaveFileName Function](http://www.jasinskionline.com/windowsapi/ref/g/getsavefilename.html)

# Future Work


* Add suport for complex graphics (Animated Sprites, Backgrounds, Etc.).
* Add Undo/Redo feature to Level Editor utilizing stack concepts
* Reformat .level file format to include Signature, version number, and other features
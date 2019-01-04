# Game-of-Life-GLSL
Can we use GLSL Shaders to compute Conways Game of Life? Yes, we can! Also we use SFML for very easy access to shaders and OpenGL

Just open the provided *.pro file with Qtcreator and compile or use gcc. The only external Dependency is SFML.
https://www.sfml-dev.org/
Make sure, that the SFML Headers and Binaries are in your path! (Windows users might change the *pro file for their needs)
Maybe you need to copy the glsl shaders to the Build folder, so the program can find them.

On Unix-Like systems it also should compile with

g++ -c -O3 -s -ffast-math -std=gnu++11 -o main.o main.cpp
g++ -o GameOfLife_Shaders main.o -lsfml-graphics -lsfml-system -lsfml-window

Also you can find a x86_64 comaptible Linux build in this Repo. Just run
./GameOfLife_Shaders

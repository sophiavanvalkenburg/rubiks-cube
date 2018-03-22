# rubiks-cube

![](https://github.com/sophiavanvalkenburg/rubiks-cube/blob/master/demo.gif)

## Instructions for running
Mac OSX: `$clang++ CubeModel.cpp RubiksCube.cpp State.cpp Util.cpp main.cpp -std=c++11 -stdlib=libc++ -lGLEW -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo`

## MVP features
* User can rotate the cube to view in different angles by dragging the mouse
* User can rotate individual faces by holding down the A (x-axis), S (y-axis), or D (z-axis) keys while dragging the mouse vertically

## Bonus features
* To rotate individual faces, the user drags the mouse in the direction they want to rotate the face instead of holding down a key
* Realistic textures and/or shading on the rubiks cube
* Click a button to generate a random cube orientation (with animation?)

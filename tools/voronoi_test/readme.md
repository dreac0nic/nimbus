# Voronoi Test
## What is this?
The voronoi test is a simple Boost/GL application that tests the functionality 
of mapping simple entities to generated voronoi cells from the Boost library's 
voronoi generator.

## Why?
There are issues of the coupling between cells and these entities. This test is 
merely to work with the Voronoi generation and talk about referencing these 
with simple entities, mostly as a proof of concept.

## Requirements
The requirements for this application are relatively minimal, however, the 
following libraries are necessary.

 - GL/GLEW
 - GLFW
 - Boost

Compiled under MINGW 4.8.1

Link against: glfw3, glew32s, opengl32, gdi32 (windows)
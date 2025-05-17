Authors
Tau Morapeli  202100001
Lerotholi Koai 202101693
-------------------------------------------------------------------------------------------------------------------------------------------
CS5431 - Assignment 3
Bi-cubic Bezier Patch

Program: CG_hw3
Language: C++
Compiler: g++
OS: Linux 
---------------------------------------------------------------------------------------------------------------------------------------------
Description:
The program evaluates a 3D bi-cubic Bezier patch and approximates it with triangles. It supports both flat-shaded and smooth-shaded polygonal approximations. The program reads control points from a file, evaluates the Bezier patch, and writes the output in Open Inventor format.
---------------------------------------------------------------------------------------------------------------------------------------------
Usage:
run make
then
./CG_hw3 [-f filename] [-u num_u] [-v num_v] [-r radius] [-F | -S] > output.iv

Arguments:
-f filename: Specify the input file containing control points (default: patchPoints.txt).
-u num_u: Number of point samples in the u direction (default: 11).
-v num_v: Number of point samples in the v direction (default: 11).
-r radius: Radius of the control point spheres (default: 0.1).
-F: Output a flat-shaded mesh (default).
-S: Output a smooth-shaded mesh with exact normals.

Example:
make
./CG_hw3 -f controlPoints.txt -u 15 -v 9 -r 0.2 -S > output.iv

Main File:
CG_hw3.cpp

Authors
Tau Morapeli  202100001
Lerotholi Koai 202101693
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
1.Bezier Curve Evaluation Program

Program Overview:
Program reads a set of control points from a file, computes the Bezier curve using the 
Bernstein polynomial, and outputs the result in Open Inventor (.iv) format which is visualized in FreeCAD.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
2.Features:

-Reads control points from a file (-f filename).
-Generates an arbitrary-degree Bezier curve.
-Supports adjustable curve resolution (-n segments).
-Displays control points as spheres (-r radius).
-Outputs the curve in Open Inventor format (.iv file).
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
3. Requirements:

   PrograRmming Language: C++
   Operating System: Ubuntu Linux
   Compiler: g++ Compiler
   FreeCAD for visualization
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
4. Execute the program:

   -execute make(Makefile):
     1. make  
     2. output on cosole----> g++ CG_hw1.cpp -o CG_hw1 (which great object file "CG_hw1").
     		              chmod +x CG_hw1 (which make CG_hw1 executable).
     3. then run ----> ./CG_hw1 -f filename -n n -r radius > output.iv (with the appropriate filename, number of control points and radius).
     4. Open .iv file using FreeCAD
     
   OR
 
   1. Compile the program: 
      	g++ CG_hw1.cpp -o CG_hw1
   2. Make CG_hw1 executable:
      	chmod +x CG_hw1
   3. Run the program:
      ./CG_hw1 -f filename -n n -r radius > output.iv
   4. Open .iv file using FreeCAD




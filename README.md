
# CS406-Parallel-Computing
### Course Project for CS406
Ilgın Şara Hacipoğlu & Buse Ak

## Problem Description
![Screen-Shot-2018-08-21-at-7 17 58-PM](https://user-images.githubusercontent.com/48694043/120443243-1c737380-c38f-11eb-9b17-3ffb39c7d869.png)

The problem is finding the number of length k cycles containing the vertex u for each u in vertex set V, given an undirected graph G=(V,E), and a positive integer 2<k<6. If the length k is taken as an input and not known because of it, the problem becomes NP-complete but in this project, the cycle length k is given, so the problem can be solved in either O(VE) time. 

For example with the graph example given above, when k=4 input is given the program will find the cycle 3-4-5-6-3 and increment the 4 length-cycle count for each of the vertices u in the given path, similarly when k=3 is given as input the found cycle will be 11-12-13-11 and program will increment cycle count for each of the respective vertices present in the path. 

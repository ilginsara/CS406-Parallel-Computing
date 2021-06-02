
# CS406-Parallel-Computing
### Course Project for CS406
Ilgın Şara Hacipoğlu & Buse Ak

## 1.Problem Description
![Screen-Shot-2018-08-21-at-7 17 58-PM](https://user-images.githubusercontent.com/48694043/120443243-1c737380-c38f-11eb-9b17-3ffb39c7d869.png)

The problem is finding the number of length k cycles containing the vertex u for each u in vertex set V, given an undirected graph G=(V,E), and a positive integer 2<k<6. If the length k is taken as an input and not known because of it, the problem becomes NP-complete but in this project, the cycle length k is given, so the problem can be solved in either O(VE) time. 

For example with the graph example given above, when k=4 input is given the program will find the cycle 3-4-5-6-3 and increment the 4 length-cycle count for each of the vertices u in the given path, similarly when k=3 is given as input the found cycle will be 11-12-13-11 and program will increment cycle count for each of the respective vertices present in the path. 

## 2.Data Preprocessing
The program will be run with two inputs from the user, with the inputs file path and the k respectively. The input file structure is such that for each ui<vi the edge source and destination node is given. Therefore, the reverse orientation (for any ui and vi pair, vi and ui creation) is also added before creating the CRS structure from the adjacency list. 
### 2.1 Compressed Row Storage (CRS)
CRS uses to arrays; first array known as the adj array holds all the contents of the adjacency list appended row by row, the second array known as the xadj array corresponds to index in adj array where vertices have an edge from the given vertex start. <img width="302" alt="Screen Shot 2021-06-02 at 11 08 59" src="https://user-images.githubusercontent.com/48694043/120447160-fa7bf000-c392-11eb-92ea-5942722840fc.png">

If xadj[i]= k and xadj[i+1]=k+3 for instance, it implies that there is an edge from vertex i to all vertices stored in adj array starting from adj[k] to adj[k+2].

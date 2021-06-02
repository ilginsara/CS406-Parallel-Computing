
# CS406-Parallel-Computing
### Course Project for CS406
Ilgın Şara Hacipoğlu & Buse Ak

## 1.Introduction and Problem Description
![Screen-Shot-2018-08-21-at-7 17 58-PM](https://user-images.githubusercontent.com/48694043/120443243-1c737380-c38f-11eb-9b17-3ffb39c7d869.png)

The problem is finding the number of length k cycles containing the vertex u for each u in vertex set V, given an undirected graph G=(V,E), and a positive integer 2<k<6. If the length k is taken as an input and not known because of it, the problem becomes NP-complete but in this project, the cycle length k is given, so the problem can be solved in O(VE) time. 

For example with the graph example given above, when k=4 input is given the program will find the cycle 3-4-5-6-3 and increment the 4 length-cycle count for each of the vertices u in the given path, similarly when k=3 is given as input the found cycle will be 11-12-13-11 and program will increment cycle count for each of the respective vertices present in the path. 

In this project, a recursive DFS algorithm and Depth Limited Search idea has been utilized to find simple small cycles of given length. Depth-Limited Search algorithm is in fact very similar to DFS with a predetermined limit. With this algorithm, the node at the depth limit will be treated as if it has no further successors so that traversing further down the graph is avoided if the cycle length threshold is reached. As this will be pruning the adjacency search, we anticipate a significant reduction in runtime. Upon this approach, a CPU paralleization with OpenMP has been performed in an attempt to increase throughput.
## 2.Data Preprocessing
The program will be run with two inputs from the user, with the inputs file path and the k respectively. The input file structure is such that for each ui<vi the edge source and destination node is given. Therefore, the reverse orientation (for any ui and vi pair, vi and ui creation) is also added before creating the CRS structure from the adjacency list. 
### 2.1 Compressed Row Storage (CRS)
CRS uses to arrays; first array known as the adj array holds all the contents of the adjacency list appended row by row, the second array known as the xadj array corresponds to index in adj array where vertices have an edge from the given vertex start. <img width="302" alt="Screen Shot 2021-06-02 at 11 08 59" src="https://user-images.githubusercontent.com/48694043/120447160-fa7bf000-c392-11eb-92ea-5942722840fc.png">

If xadj[i]= k and xadj[i+1]=k+3 for instance, it implies that there is an edge from vertex i to all vertices stored in adj array starting from adj[k] to adj[k+2].

## 3. Other Attempted Solutions and Solution Description 
### 3.1 General BFS Approach (Breadth-First Search)
There are 3 main BFS approaches known as Top-down, Bottom-up and Hybrid heuristic, where Bottom-Up and Top-down approaches are combined based on vertex count in a specific frontier. Nodes in the same level are pushed into a frontier and upon iteration over those present in the frontier, next frontier is created for the upcoming levels. One drawback of BFS algorithm is that each frontier of the tree needs to be saved into memory to expand into the next levels. This will require significant time, especially if cycles are located farther down the graph as the algorithm will need to proceed into much lower levels and might even need to traverse the entire graph. Although literature search suggests that a general BFS algorithm is more suitable for parallelization than a general DFS, given the nature of the problem and graph sizes, it did not appear feasible to continue with this approach. 

### 3.2 Graph Coloring
The general approach with this algorithm is to use a DFS algorithm to mark all vertices with unique colors(or numbers) and push similarly marked vertices into an adjaceny list once the graph traversal is complete to find out unique cycle paths. For this there will need to be 3 groups of coloring: partially visited, visited and non-visited. Whenever a partially visited vertex is encountered along the adjacency trace to find a cycle, algorithm needs to backtrack until the start vertex mark all the vertices along the process with cycle number and increment cycle count for each of these vertices. This algorithm worked will during our trials with smaller sample graphs however managing backtracks, coloring and cycle numbers became overly complicated with very large graphs and parallelized methods. 

### 3.3 Powers of Adjacency Matrix 

<img width="1022" alt="Screen Shot 2021-06-02 at 12 27 42" src="https://user-images.githubusercontent.com/48694043/120456211-0e791f00-c39e-11eb-9ce0-fd0e48a1c700.png">
Given an adjacency matrix A, A^k matrix provides count of number of length k walks between vertices i->j in directed graphs. We tried to utilize this idea such that diagonals of A^k give walks of length k each vertex i is included in (starts in i ends in i). This approach seemed very viable initially, as we thought we could directly compute a matrix-matrix multiplication k times in parallel using the CRS structure efficiently. Matrix multiplications are in fact easier to parallelize than recursive algorithms like DFS however could also be very memory intensive, creating yet again a bottleneck. 

All things considered, this approach was problematic, as the definition of "walk" of k length from same vertex i to i does not necessarily imply a cycle as a walk can backtrack to a previously visited node via an already used edge. Some trial efforts with smaller graphs yielded wrong results so we moved away from this approach as well. 


### 3.4 Our Approach: Depth-Limited Search
For each vertex, the adjacent nodes are traversed until a certain depth(k) to check cycle existence.
With this algorithm, the node at the depth limit will be treated as if it has no further successors so that traversing further down the graph is avoided if the cycle length threshold is reached. As this will be pruning the adjacency search, we anticipate a significant reduction in runtime.


** algorithm returning vector of cycle paths	**								
recursive_paths(start_node, caller_node, length,max_length))

//recursion check
If(length == max_length) // no cycle found
  If(start_node!=caller_node)
    return;
   else //cycle found
     path_list.push_back(path);
     return;
  
 path.push_back(caller_node);
 for adj vertices of each vertex //traverse adj of each vertex
  in new_caller=adj[i]
  if(new_caller not found in path: 
    call recursive_paths //pass length+1 inside
              
   





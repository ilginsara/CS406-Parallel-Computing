#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <math.h> /* fabs */
#include <string.h>
#include <stdlib.h>
#include <limits.h>
using namespace std;


void recursive_paths(int start_node, int caller_node, vector<int> path, vector<vector<int>> &path_list, int length, int max_length,int *adj, int *xadj){
        

        if(length==max_length){//check if the desired length is reached

            if(start_node!=caller_node)//no cycle found
                return;

            else //cycle found
            {
                path_list.push_back(path);//add current path to path list
                return;
            }    
        }
        
        
        path.push_back(caller_node);//add current vertex to the path
        
       
        for(int i= xadj[caller_node]; i<xadj[caller_node+1]; i++){//check the adjacents of current vertex
                
                
                int new_caller= adj[i];
                //check if the neighboring vertex is in the path i.e visited before
                //if the vertex in the path but it is the starting vertex
                if(std::find(path.begin(), path.end(), new_caller)==path.end() || (new_caller==start_node && length==max_length-1)){
                    //call the function again with new parameters
                    //inrement the length by one to keep track of the current depth
                    recursive_paths(start_node, new_caller, path, path_list, length+1 ,max_length, adj, xadj);
                }
                
                
                

        }

}


int main(int argc, char *argv[])
{
    int cycle_len = 0;

    string filename = argv[1];
    cycle_len = stoi(std::string(argv[2]));
    int nt;
    cout<<"Please enter thread number: ";
    cin>>nt;

    //Reading the Input File
     std::ifstream fin;
    fin.open(filename);

    int v1, v2;
    std::vector<std::pair<int, int>> intermediate;
    int num_vert = 0;
    int min = INT_MAX;
    while (!fin.eof())
    {
        fin >> v1 >> v2;
        
        if (v1 != v2)//avoiding self loops
        {
            //updating the both endpoints of an edge
            intermediate.push_back(std::pair<int, int>(v1, v2));
            intermediate.push_back(std::pair<int, int>(v2, v1));
        }
        min = (v1 < min) ? v1 : min;
        min = (v2 < min) ? v2 : min;
        num_vert = (v1 > num_vert) ? v1 : num_vert;
        num_vert = (v2 > num_vert) ? v2 : num_vert;
    }
    
    if (min == 0)//checking if the vertices are 0-based
    {
        num_vert++;
    }
    else
    {
        num_vert = num_vert - min + 1;
    }
    
    std::stable_sort(intermediate.begin(), intermediate.end());
   

    //      ****CREATE CSR*****
    int *adj = new int[intermediate.size()]; //num of edges
    int *xadj = new int[num_vert + 1];       //number of vertex
                                          

    xadj[num_vert + 1] = num_vert - 1;

    memset(xadj, 0, sizeof(int) * (num_vert + 1));
    
    for (int i = 0; i < intermediate.size(); i++)
    {
        adj[i] = intermediate[i].second;
        xadj[(intermediate[i].first) + 1]++; //That's the trick
    }
    for (int i = 1; i <= num_vert; i++)
    {
        xadj[i] += xadj[i - 1];
    }

    
   
    std::vector<double> length_cyles(num_vert, 0.0);
    
    double start=omp_get_wtime();          
#pragma omp parallel for schedule(dynamic) num_threads(nt)
    for(int i=0; i<num_vert+1; i++){//calling the function for each vertex
        if(xadj[i]!=xadj[i+1]){//check if the vertex has edges

            vector<vector<int>> path_list;
            vector<int> path;
            recursive_paths(i, i, path, path_list, 0, cycle_len, adj, xadj);
	   

            #pragma omp critical
            for(int i=0 ; i<path_list.size(); i++){
                for(int j=0 ; j<path_list[i].size(); j++){
		  length_cyles[path_list[i][j]]+=1.0/cycle_len;//increment the cycle count the contributing vertices
                }

            }


        }
    }
    double end=omp_get_wtime();
    //TO CHECK OUTPUT
    //please replace 100 with num_vertices
        /*
     for(int a=0; a<=100;a++){
      cout<<a<<" "<<length_cyles[a]<<endl;
	}
	*/
    
    cout<<"No threads: "<<nt<<" Time: "<<end-start<<endl;
    return 0;
}

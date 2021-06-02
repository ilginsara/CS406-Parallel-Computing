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
        
        //cout<<"HERE1 "<<start_node<<"  "<<caller_node<<"  "<<length<<endl;
  // for(int i=0; i<path.size();i++){
	  //            cout<<path[i]<<" ";
  //    }
	// cout<<"CALLER: "<<caller_node<<endl;
        if(length==max_length){

            if(start_node!=caller_node)

                return;
            else
            {
	      //            cout<<"Cycle"<<endl;
                path_list.push_back(path);
                return;
            }    
        }
        
        
        path.push_back(caller_node);
        
       
        for(int i= xadj[caller_node]; i<xadj[caller_node+1]; i++){
                
                
                int new_caller= adj[i];
                if(std::find(path.begin(), path.end(), new_caller)==path.end() || (new_caller==start_node && length==max_length-1)){
                    //cout<<"HERE2 "<<start_node<<"  "<<new_caller<<endl;
                    recursive_paths(start_node, new_caller, path, path_list, length+1 ,max_length, adj, xadj);
                }
                /*
                else if(new_caller==start_node){
                    cout<<"HERE3 "<<start_node<<"  "<<new_caller<<endl;
                    recursive_paths(start_node, new_caller, path, path_list, length+1,max_length, adj, xadj);
                }
                */
                
                

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
     std::ifstream fin;
    fin.open(filename);

    int v1, v2;
    std::vector<std::pair<int, int>> intermediate;
    int num_vert = 0;
    int min = INT_MAX;
    while (!fin.eof())
    {
        fin >> v1 >> v2;
        if (v1 != v2)
        {
            intermediate.push_back(std::pair<int, int>(v1, v2));
            intermediate.push_back(std::pair<int, int>(v2, v1));
        }
        min = (v1 < min) ? v1 : min;
        min = (v2 < min) ? v2 : min;
        num_vert = (v1 > num_vert) ? v1 : num_vert;
        num_vert = (v2 > num_vert) ? v2 : num_vert;
    }
    if (min == 0)
    {
        num_vert++;
    }
    else
    {
        num_vert = num_vert - min + 1;
    }
    //    std::cout << "numberOfVertices = " << num_vert << "\n";
    //std::cout << "min = " << min << "\n";
    std::stable_sort(intermediate.begin(), intermediate.end());
    //see pairs
    /*
    for (int i = 0; i < intermediate.size(); i++)
    {
        cout << intermediate[i].first << ":" << intermediate[i].second << endl;
    }*/

    //      ****CREATE CSR*****
    int *adj = new int[intermediate.size()]; //num of edges
    int *xadj = new int[num_vert + 1];       //vertex sayısı
                                             //std::vector<int> xadj(num_vert + 1, 0);
                                             //std::vector<int> adj;

    xadj[num_vert + 1] = num_vert - 1;

    memset(xadj, 0, sizeof(int) * (num_vert + 1));
    //    memset(adj, 0, sizeof(int) * (intermediate.size()));
    for (int i = 0; i < intermediate.size(); i++)
    {
        adj[i] = intermediate[i].second;
        xadj[(intermediate[i].first) + 1]++; //That's the trick
    }
    for (int i = 1; i <= num_vert; i++)
    {
        xadj[i] += xadj[i - 1];
    }

    //for printing xadj and adj
    /*    
    for (int i = 0; i < intermediate.size(); i++)
    {
        std::cout << "i: " << i << "  adj[i]: " << adj[i] << std::endl;
    }

    for (int i = 0; i < num_vert + 1; i++)
    {
        std::cout << "i: " << i << "  xadj[i]: " << xadj[i] << std::endl;
	}*/
   
    std::vector<int> length_cyles(num_vert, 0);
    
    double start=omp_get_wtime();          
#pragma omp parallel for schedule(dynamic) num_threads(nt)
    for(int i=0; i<num_vert+1; i++){
        if(xadj[i]!=xadj[i+1]){

            vector<vector<int>> path_list;
            vector<int> path;
            recursive_paths(i, i, path, path_list, 0, cycle_len, adj, xadj);
	    /* for(int i=0;i<path_list.size();i++){
	      for(int j=0;j<path_list[i].size();j++){

		cout<<path_list[i][j]<<" ";

}
	      cout<<endl;

	      }*/

            #pragma omp critical
            for(int i=0 ; i<path_list.size(); i++){
                for(int j=0 ; j<path_list[i].size(); j++){
                    length_cyles[path_list[i][j]]++;
                }

            }


        }
    }
    double end=omp_get_wtime();
    //TO CHECK OUTPUT
    //please replace 100 with num_vertices
    /*    
     for(int a=0; a<=100;a++){
      cout<<a<<" "<<length_cyles[a]/cycle_len<<endl;
	}
    */
    cout<<"No threads: "<<nt<<" Time: "<<end-start<<endl;
    return 0;
}

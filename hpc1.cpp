#include <iostream>
#include <omp.h>
#include <vector>
#include <queue>
#include <list>

using namespace std;

class Graph{
    private:
        int V;
        vector<list<int>> adj;

    public:
        Graph(int V){
            this -> V = V;
            adj.resize(V);
        } 

        void addEdge(int u, int v){
            adj[u].push_back(v);
            adj[v].push_back(u);
        }

        void printGraph(){
            for (int i = 0; i < V; i++){
                cout << i << " Connected to ";
                for (int neighbor : adj[i]){
                    cout << neighbor << " ";
                }
                cout << endl;
            }
        }

        void parallelBFS(int start){
            vector<bool> visited(V, false);
            queue<int> q;

            visited[start] = true;
            q.push(start);

            cout << "Parallel BFS starting from the node " << start << ":\n";

            while (!q.empty()){
                int size = q.size();
                vector<int> res;

                while (size--){
                    int node = q.front();
                    q.pop();
                    cout << node << " ";
                    res.push_back(node);
                }

                #pragma omp parallel for
                for(int i = 0; i < res.size(); i++){
                    int node = res[i];
                    
                    for (int neighbor : adj[node]){
                        #pragma omp critical
                        {
                            if (!visited[neighbor]){
                                visited[neighbor] = true;
                                q.push(neighbor);
                            }
                        }
                    }

                }
            }
            cout << endl;
        }
        
        void parallelDFSUtil(int node, vector<bool> &visited){
            bool nodeVisited;

            #pragma omp critical
            {
                nodeVisited = visited[node];
                if (!nodeVisited){
                    visited[node] = true;
                    cout << node << " ";
                } 
            }

            if(nodeVisited) return;
 
            #pragma omp parallel for
            for (int i=0; i < adj[node].size(); i++){
                int neighbor = *next(adj[node].begin(), i);

                if (!visited[neighbor]){
                    #pragma omp task
                    parallelDFSUtil (neighbor, visited);
                }
            }

            #pragma omp taskwait
        }

        void parallelDFS(int start){
            vector<bool> visited(V, false);
            cout << "Parallel DFS starting from node " << start << ": \n";

            #pragma omp parallel
            {
                #pragma omp single
                {
                    parallelDFSUtil(start, visited);
                }
            }
            cout << endl;
        }
};

int main(){
    int V, E;

    cout << "Enter number of Vertices and Edges: ";
    cin >> V >> E;

    Graph g(V);

    cout << "Enter edges (u v): \n";
    for (int i = 0; i < E; ++i){
        int u, v;
        cin >> u >> v;
        g.addEdge(u,v);
    }
    g.printGraph();

    int startNode;

    cout << "Enter Start node: ";
    cin >> startNode;

    g.parallelBFS(startNode);
    g.parallelDFS(startNode);
}



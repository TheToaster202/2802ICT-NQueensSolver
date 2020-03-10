#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <array>
#include <queue>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

class Problem{
    public:
    //Constructors
    Problem(int const & nP): n{nP}, board{0}{}

    //Destructor
    ~Problem(){}

    int stateBfs(){
        
    }

    int getN(){return n;}

    int getNXN(){return n*n;}

    array<array<int, 20>, 20> getGraph(){return board;}

    void printBoard(){
        for (int i=0; i<n; i++){
            for (int j=0; j<n; j++){
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
    }

    private:
    int n;
    array<array<int, 20>, 20> board;  //Assumes maximum possible number of N, 20
};

int bfs(Problem & problem){ //Breadth First Search Algorithm
    
    queue<pair<int, int>> frontier;     //Queue of nodes
    vector<pair<int, int>> visited;     //List of visited Nodes

    pair<int, int> node{0,0};
    frontier.push(node);

    //Note depth needs to be n-1
    int depth=0;
    while (!frontier.empty()){
        node = frontier.front();
        frontier.pop();

        visited.push_back(node);

        for(int i=0; )
    }
    
}


int main(int const argc, char const ** argv){
    if (argc < 2){
        cerr << "Program needs an N value of 1-20 to run" << endl;
        return EXIT_FAILURE;
    }

    if (atoi(argv[1]) < 1 || atoi(argv[1]) > 20){
        cerr << "Program needs an N value of 1-20 to run" << endl;
    }

    Problem queens(atoi(argv[1]));

    bfs(queens);

}
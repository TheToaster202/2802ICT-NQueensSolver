#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <array>
#include <queue>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

class Problem{//Holds information regarding the given problem
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

class State{//Holds information about a current state, positions of queens the depth within a tree
    public:

    //Constructors
    State():qPos{0}, sDepth{0}{}    //For the intial state

    State(pair<int, int> const & pos):qPos{pos}, sDepth{0} {sDepth = qPos.size();} //First node push
    
    State(const State & that){//Copy Constructor
        qPos = that.qPos;
        sDepth = qPos.size();
    }

    //Getters
    vector<pair<int, int>> getPos(){
        return qPos;
    }

    int getDepth(){
        return sDepth;
    }

    void newDepth(int const & i, int const & j){//Pushes a new queen pos into the positions list
        qPos.push_back(pair<int, int>(i,j));
        sDepth = qPos.size();
    }
    void remove(){
        qPos.erase(qPos.begin());
        sDepth = qPos.size();
    }

    void printPos(){
        cout << "[ ";
        for(int i=0; i<qPos.size(); i++){
            cout << "(" << qPos[i].first << " " << qPos[i].second << ")";
        }
        cout << " ] \n";
    }

    private:
    vector<pair<int, int>> qPos;    //A vector of queen positions
    int sDepth;                     //Current Depth of the search tree
};

bool isUnique(State & state, int const & depth, int const & width){//Tests if the current selection is unqiue
    for (int i=0; i<state.getDepth(); i++){
        if(depth == state.getPos()[i].first && width == state.getPos()[i].second){
            return false;
        }
    }
    return true;
}

bool goalState(Problem problem, State & node){//Function that will test if the current solution is a goal state
    
    for (int i=0; i<node.getPos().size(); i++){//Probably redundant
		problem.getGraph()[node.getPos()[i].first][node.getPos()[i].second];
	}

	
	for (int i=0; i<node.getPos().size()-1; i++){//Loops through all queen positions and determines whether they are in a goal state
		for (int j=i; j<node.getPos().size(); j++){
			if (node.getPos()[i].first == node.getPos()[j].first){return false;}
			if (node.getPos()[i].second == node.getPos()[j].second){return false;}
			
			int xHold = node.getPos()[i].first - node.getPos()[j].first;
			xHold *= xHold;
			
			int yHold = node.getPos()[i].second - node.getPos()[j].second;
			yHold *= yHold;
			
			if(yHold == xHold){return false;}
		}
	}
	
    return true;
}

void printFrontier(queue<State> frontier){
    while(!frontier.empty()){
        frontier.front().printPos();
        frontier.pop();
    }

    cout << endl;
}

/*void printFront(queue<pair<int, int>> frontier, vector<pair<int, int>> const & visited){
    cout << "Queue [ ";
    while (!frontier.empty()){
        cout << "(" <<frontier.front().first << " " << frontier.front().second << ")";
        frontier.pop();
    }
    cout << " ]" << endl;

    /*cout << "Visited [ ";
    for (int i=0; i<visited.size(); i++){
        cout << "(" << visited[i].first << " " << visited[i].second << ")";
    }
    cout << " ]" << endl;
}
*/

bool notExplored(pair<int, int> const & node, vector<State> & list, queue<State> frontier, int const & n){
    
    while (!frontier.empty()){
        for(int i=0; i<frontier.front().getPos().size(); i++){
            if(node.first == frontier.front().getPos()[i].first && node.second == frontier.front().getPos()[i].second){
                return false;
            }
        }
        frontier.pop();
    }
    
    for (int i=0; i<list.size(); i++){
        for (int j=0; j<list[i].getPos().size(); j++){
            if (node.first == list[i].getPos()[j].first && node.second == list[i].getPos()[j].second){
				return false;
			}
        }
    }

    return true;
}

int bfs(Problem & problem){ //Breadth First Search Algorithm

    queue<State> frontier;
    vector<State> visited;
    vector<State> success;

    State node;

    frontier.push(State(pair<int, int>(0, 0)));
    int depth = 0, solutions = 0;
    

    while(!frontier.empty()){
        
        node = frontier.front();
        frontier.pop();

        printFrontier(frontier);

        visited.push_back(node);

        for (int i=0; i<problem.getN(); i++){
            
            State stateHold = node;
            pair<int, int> child(depth, i);

            if(notExplored(child, visited, frontier, problem.getN())){
                stateHold.newDepth(child.first, child.second);

                //node.printPos();        
                
                if(goalState(problem, stateHold) && depth == problem.getN()-1){
                    if(success.size() < 7){
                        success.push_back(frontier.front());
                    }
                    solutions++;
                }

                frontier.push(stateHold);   
            }
        }

        if(depth < problem.getN()-1){
            depth++;
        }

    }


    cout << endl << endl;
    for(int i=0; i<success.size(); i++){
        success[i].printPos();
    }
    return solutions;
    
}

void ideal(){
    State state(pair<int, int>(0,0));

    queue<State> something;
    something.push(state);
    something.push(State(pair<int, int>(0,1)));
    something.push(State(pair<int, int>(0,2)));

    state = something.front();
    something.pop();
    State stateHold = state;
    
    stateHold.newDepth(1,0);
    something.push(stateHold);

    stateHold = state;
    stateHold.newDepth(1,1);
    something.push(stateHold);

    stateHold = state;
    stateHold.newDepth(1,2);
    something.push(stateHold);

    printFrontier(something);

    state = something.front();
    something.pop();
    stateHold = state;

    stateHold.newDepth(1,0);
    something.push(stateHold);

    stateHold = state;
    stateHold.newDepth(1,1);
    something.push(stateHold);

    stateHold = state;
    stateHold.newDepth(1,2);
    something.push(stateHold);

    printFrontier(something);
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

    ideal();

    queue<State> qq;
    qq.push(State());
    State nodeHold;
    int depth = 0;

    while (!qq.empty()){                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
        
        cout << "DEPTH: " << depth << endl;
        nodeHold = qq.front();
        qq.pop();
        depth = nodeHold.getPos().size();

        if(depth > 2){
            break;
        }

        for(int i=0; i < 3; i++){
            State node = nodeHold;
            node.newDepth(depth, i);
            qq.push(node);
        }
        printFrontier(qq);
        
    }


    cout << "PROBLEM:\n";
    //cout << bfs(queens) << endl;
    

}
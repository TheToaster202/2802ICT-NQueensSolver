#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <array>
#include <queue>
#include <stack>
#include <utility>
#include <vector>
#include <chrono>

using namespace std;

//Note this program needs to be complied with -O3 optimization to be relatively quick

class Problem{//Holds information regarding the given problem
    public:
    //Constructors
    Problem(int const & nP): n{nP}{}

    //Destructor
    ~Problem(){}

    int stateBfs(){
        
    }

    int getN(){return n;}

    int getNXN(){return n*n;}

    private:
    int n;
};

class State{//Holds information about a current state, ie the positions of the queens current "on the board"
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

int goalState(Problem & problem, queue<State> frontier){//Function that will test if the current solution is a goal state
    int solutions = 0;

    cout << "HERE" << endl;
    
    while (!frontier.empty()){//Loops until the queue is empty
        bool isSolved = true;
        //frontier.front().printPos();

        for (int i=0; i<problem.getN()-1; i++){

            for (int j=i+1; j<problem.getN(); j++){

                if (frontier.front().getPos()[i].first == frontier.front().getPos()[j].first){
                    isSolved = false;
                    break;
                }
                
                if (frontier.front().getPos()[i].second == frontier.front().getPos()[j].second){
                    isSolved = false;
                    break;
                }

                
                //Removes the subtracts two positions from each other, and if they produce the same value then they are diagonal to each other
                //(0,1) - (2,3) = -2, 2 * itself == 4,4 therefore they are diagonal

                int xHold=0, yHold=0;
                xHold = frontier.front().getPos()[i].first - frontier.front().getPos()[j].first;
                yHold = frontier.front().getPos()[i].second - frontier.front().getPos()[j].second;

                xHold*=xHold, yHold*=yHold;

                if (xHold == yHold){
                    /*cout << "HOLD: (" << xHold << " " << yHold << ") " << endl;
                    cout << "I: (" << frontier.front().getPos()[i].first << " " << frontier.front().getPos()[i].second << ") J: (" 
                    << frontier.front().getPos()[j].first << " " << frontier.front().getPos()[j].second << ")" << endl;*/
                    isSolved = false;
                    break;
                }
            }
        }

        if (isSolved == true){
            
            solutions ++;
            if(problem.getN() < 7){
                frontier.front().printPos();
            }
            
        }
        frontier.pop();
    }
	
    return solutions;
}

bool isValid(State & node, int & width, int & depth, Problem & prob){//Tests whether the new positions is directly diagonal to the prior positions
    if (node.getDepth() == 0){
        return true;
    }
    
    for (int i=0; i<node.getDepth(); i++){

        if(width == node.getPos()[i].second){
            return false;
        }

        int xHold = node.getPos()[i].first - depth;
        int yHold = node.getPos()[i].second - width;

        yHold *= yHold, xHold *= xHold;

        if (xHold == yHold){
            return false;
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

int bfs(Problem & problem){ //Breadth First Search Algorithm
    //Currently finds all paths to the lowest depth

    queue<State> qq;    //queue of queen's positionsd
    qq.push(State());   //Pushes empty "State"
    State nodeHold;     //Independent sate holder, does not change within the for loop
    int depth = 0;
    unsigned int solutions = 0;

    while (!qq.empty()){//Loops until either the queue is empty or the depth of N is reached                                                                                                                                                                                                                                                                                                                                                                                                                                                  
        
        nodeHold = qq.front();
        
        depth = nodeHold.getPos().size();

        if(depth > problem.getN()-1){
            solutions = goalState(problem, qq);
            
            break;
        }
        qq.pop();

        for(int i=0; i < problem.getN(); i++){
            State node = nodeHold;

            if (isValid(nodeHold, i, depth, problem)){
                node.newDepth(depth, i);
                qq.push(node);
            }
        
            
        }
        //printFrontier(qq);
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


int main(int const argc, char const ** argv){// Main Code Driver
    
    if (argc < 2){
        cerr << "Program needs an N value of 1-20 to run" << endl;
        return EXIT_FAILURE;
    }

    if (atoi(argv[1]) < 1 || atoi(argv[1]) > 20){
        cerr << "Program needs an N value of 1-20 to run" << endl;
    }

    Problem queens(atoi(argv[1]));

    //ideal();

    /*queue<State> qq;
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
        
    }*/
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    cout << "PROBLEM:\n";
    cout << bfs(queens) << endl;

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<seconds>(stop - start);
    auto duration2 = duration_cast<milliseconds>(stop - start);
    cout << "TIME TAKEN" << endl << "Seconds: " << duration.count() << " Milliseconds: " << duration2.count() << endl;
    

}
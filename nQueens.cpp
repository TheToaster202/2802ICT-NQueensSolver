#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <queue>
#include <utility>
#include <vector>
#include <chrono>
#include <random>

using namespace std;

//Note this program needs to be complied with -O3 optimization to be relatively quick

class Problem{//Holds information regarding the given problem
    public:
    //Constructors
    Problem(int const & nP): n{nP}{}

    //Destructor
    ~Problem(){}

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
    long unsigned int sDepth;                     //Current Depth of the search tree
};

int goalState(Problem & problem, State frontier, bool pruned = false){//Function that will test if the current solution is a goal state
    int solutions = 0;
    
    bool isSolved = true;

    for (int i=0; i<problem.getN()-1; i++){

        for (int j=i+1; j<problem.getN(); j++){

            if (frontier.getPos()[i].first == frontier.getPos()[j].first){
                isSolved = false;
                break;
            }
                
            if (frontier.getPos()[i].second == frontier.getPos()[j].second){
                isSolved = false;
                break;
            }

                
            //Removes the subtracts two positions from each other, and if they produce the same value then they are diagonal to each other
            //(0,1) - (2,3) = -2, 2 * itself == 4,4 therefore they are diagonal
            
            int xHold=0, yHold=0;
            xHold = frontier.getPos()[i].first - frontier.getPos()[j].first;
            yHold = frontier.getPos()[i].second - frontier.getPos()[j].second;

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

        if (pruned){
            frontier.printPos();
            return 1;
        }
            
        solutions ++;
        if(problem.getN() < 7){
            frontier.printPos();
        }
            
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

int bfs(Problem & problem, bool oneAnswer = false){ //Breadth First Search Algorithm
    //POTENTIALLY: Re write this so that instead of using a vector of pairs you just use the index as the depth indicator...
    
    //Currently finds all paths to the lowest depth

    queue<State> qq;    //queue of queen's positionsd
    qq.push(State());   //Pushes empty "State"
    State nodeHold;     //Independent sate holder, does not change within the for loop

    int depth = 0;      //The depth of the tree
    long unsigned int solutions = 0; //Number of found, non unique solutions

    while (!qq.empty()){//Loops until either the queue is empty or the depth of N is reached                                                                                                                                                                                                                                                                                                                                                                                                                                                  
        
        nodeHold = qq.front();
        
        depth = nodeHold.getPos().size();

        if(depth > problem.getN()-1){
            solutions += goalState(problem, qq.front(), oneAnswer);
            
            if (oneAnswer && solutions > 0){
                return solutions;
            }
        }

        qq.pop();

        for(int i=0; i < problem.getN(); i++){
            State node = nodeHold;

            if (isValid(nodeHold, i, depth, problem)){
                node.newDepth(depth, i);
                qq.push(node);
            }
        
            
        }
    }
    return solutions;
    
}

bool isVisited(vector<pair<int, int>> const & nodes, int const & depth, int const & width){
    
    if(nodes.empty()){
        return false;
    }

    for (auto i=nodes.begin(); i!=nodes.end(); i++){
        if ((*i).first == depth && (*i).second == width){
            return true;
        }
    }
    

    return false;
}

int bfsP(Problem & problem, bool oneAnswer = false){   //BFS But with pruned searches
    //Currently finds all paths to the lowest depth

    queue<State> qq;    //queue of queen's positionsd
    qq.push(State());   //Pushes empty "State"
    State nodeHold;     //Independent sate holder, does not change within the for loop

    vector<pair<int, int>> visited;

    int depth = 0;      //The depth of the tree
    long unsigned int solutions = 0; //Number of found, non unique solutions

    while (!qq.empty()){//Loops until either the queue is empty or the depth of N is reached                                                                                                                                                                                                                                                                                                                                                                                                                                                  
        
        nodeHold = qq.front();
        
        depth = nodeHold.getPos().size();

        if(depth > problem.getN()-1){
            solutions += goalState(problem, qq.front(), oneAnswer);  
            break;
        }
        qq.pop();


        for(int i=0; i < problem.getN(); i++){
            State node = nodeHold;

            if(!isVisited(visited, depth, i)){
                if (isValid(nodeHold, i, depth, problem)){
                    node.newDepth(depth, i);
                    visited.push_back(pair<int, int>(depth, i));

                    if (visited.size() >= depth){
                        visited.erase(visited.begin());
                    }
                    
                    qq.push(node);
                }
            }     
        }
        
        
    }
    return solutions;
}

vector<int> genRandomState(Problem & prob){    //Generates a random state where all queens are on the board
    
    vector<int> randState;
    vector<int> usedNumbers;

    for(int i=0; i<prob.getN(); i++){
        usedNumbers.push_back(0);
    }

    int i=0;
    while (i<prob.getN()){
        int randNumber = rand()%prob.getN();
        
        if (usedNumbers[randNumber]==0){
            randState.push_back(randNumber);
            i++;
            usedNumbers[randNumber] = 1;
        }
        
    }

    return randState;


}

int stateEval(vector<int> & positions, Problem & prob){
    int level = 0;
    
    for (int i=0; i<prob.getN()-1; i++){
        for (int j=i+1; j<prob.getN(); j++){
            if (positions[i] == positions[j]){
                level ++;
            }

            int xH = positions[i] - positions[j];
            xH *= xH;

            int yH = i - j;
            yH *= yH;

            if (xH == yH){
                level++;
            }
        }
    }

    return level;
}

void hillClimbing(Problem & prob){

    srand((int)time(NULL)); //Seeding rand() for later

    vector<int> initState = genRandomState(prob);   //Note: Index is breadth
    
    int confidence = stateEval(initState, prob);
    cout << "CONFIDENCE " << confidence << endl;

    cout << "INITIAL STATE" << endl;
    for (int i=0; i<prob.getN(); i++){
        for (int j=0; j<prob.getN(); j++){
            if(j == initState[i]){
                cout << "Q\t";
            }else{
                cout << "-\t";
            }
                
        }

        cout << endl << endl;
    }

   while (confidence != 0){        //Shotgun Hill Climbing

        for (int i=0; i<prob.getN(); i++){
            for (int j=0; j<prob.getN(); j++){
                if (i != initState[j]){
                    vector<int> newState = initState;
                    newState[j] = i;

                    int newConfidence = stateEval(newState, prob);
                    if (newConfidence < confidence){
                        initState = newState;
                        confidence = newConfidence;
                    }    
                }
            }
        }

        if (confidence != 0){
            //cout << "CONFIDENCE: " << confidence << " SHOUTGUN FIRE!" << endl << endl;
            initState = genRandomState(prob);
            confidence = stateEval(initState, prob);
        }
    }

    cout << "GOAL STATE" << endl;

    for (int i=0; i<prob.getN(); i++){
            for (int j=0; j<prob.getN(); j++){
                if(j == initState[i]){
                    cout << "Q\t";
                }else{
                    cout << "-\t";
                }
                
            }

            cout << endl << endl;
        }

    return;

}

int main(int const argc, char const ** argv){// Main Code Driver
    
    if (argc < 2){
        cerr << "Program needs an N value of 1-20 to run" << endl;
        return EXIT_FAILURE;
    }

    if (atoi(argv[1]) < 1 || atoi(argv[1]) > 20){
        cerr << "Program needs an N value of 1-20 to run" << endl;
        return EXIT_FAILURE;
    }

    if (atoi(argv[1]) == 1){
        cout << "[(0,0)]" << endl << "1" << endl;
        return EXIT_SUCCESS;
    }

    Problem queens(atoi(argv[1]));

    using namespace std::chrono;
    /*auto start = high_resolution_clock::now(); //Timing how long the bfs function takes to run and complete

    cout << "PROBLEM:\n";
    cout << bfs(queens, false) << endl;

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<seconds>(stop - start);
    auto duration2 = duration_cast<milliseconds>(stop - start);

    cout << "BFS TIME TAKEN" << endl << "Seconds: " << duration.count() << " Milliseconds: " << duration2.count() << endl;

    //BFS Pruned Call
    start = high_resolution_clock::now(); //Timing how long the bfs function takes to run and complete
    cout << bfsP(queens, true) << endl;

    stop = high_resolution_clock::now();

    duration = duration_cast<seconds>(stop - start);
    duration2 = duration_cast<milliseconds>(stop - start);

    cout << "BFS ONE SOLUTION, TIME TAKEN" << endl << "Seconds: " << duration.count() << " Milliseconds: " << duration2.count() << endl;*/

    hillClimbing(queens);

    return EXIT_SUCCESS;

}
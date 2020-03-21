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

    State(int const & pos):qPos{pos}, sDepth{0} {sDepth = qPos.size();} //First node push
    
    State(const State & that){//Copy Constructor
        qPos = that.qPos;
        sDepth = qPos.size();
    }

    //Getters
    vector<int> & getPos(){
        return qPos;
    }

    int getDepth(){
        return sDepth;
    }

    void newDepth(int const & i){//Pushes a new queen pos into the positions list
        qPos.push_back(i);
        sDepth = qPos.size();
    }
    void remove(){
        qPos.erase(qPos.begin());
        sDepth = qPos.size();
    }

    void printPos(){
        cout << "[ ";
        for(int i=0; i<qPos.size()-1; i++){
            cout << "(" << i << " " << qPos[i] << ")";
        }
        cout << " ] \n";
    }

    private:
    vector<int> qPos;    //A vector of queen positions
    long unsigned int sDepth;                     //Current Depth of the search tree
};

void printFrontier(queue<State> frontier){
    while(!frontier.empty()){
        frontier.front().printPos();
        frontier.pop();
    }

    cout << endl;
}

int goalState(Problem & problem, queue<State> frontier, bool pruned = false){//Function that will test if the current solution is a goal state
    //Alter this to take the whole frontier
    int solutions = 0;
    
    bool isSolved = true;

    while (!frontier.empty()){
        isSolved = true;

        for (int i=0; i<problem.getN()-1; i++){

            for (int j=i+1; j<problem.getN(); j++){

                if (frontier.front().getPos()[i] == frontier.front().getPos()[j]){
                    isSolved = false;
                    break;
                }

                        
                //Removes the subtracts two positions from each other, and if they produce the same value then they are diagonal to each other
                //(0,1) - (2,3) = -2, 2 * itself == 4,4 therefore they are diagonal
                    
                int xHold=0, yHold=0;
                xHold = frontier.front().getPos()[i] - frontier.front().getPos()[j];
                yHold = i - j;

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
                frontier.front().printPos();
                return 1;
            }
                    
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
    
    for (int i=0; i<node.getDepth()-1; i++){

        if(width == node.getPos()[i]){
            return false;
        }

        int xHold = node.getPos()[i] - width;
        int yHold = i - depth;

        yHold *= yHold, xHold *= xHold;

        if (xHold == yHold){
            return false;
        }
    }
    
    return true;
}  

int bfs(Problem & problem, bool oneAnswer = false){ //Breadth First Search Algorithm
    //POTENTIALLY: Re write this so that instead of using a vector of pairs you just use the index as the depth indicator...
    //Rewrite this to remove the need for the isVaild check
    
    //Currently finds all paths to the lowest depth

    queue<State> qq;    //queue of queen's positionsd
    qq.push(State());   //Pushes empty "State"
    State nodeHold;     //Independent sate holder, does not change within the for loop

    int depth = 0;      //The depth of the tree
    long unsigned int solutions = 0; //Number of found, non unique solutions

    while (!qq.empty()){//Loops until either the queue is empty or the depth of N is reached                                                                                                                                                                                                                                                                                                                                                                                                                                                  
        
        nodeHold = qq.front();

        depth = nodeHold.getPos().size()-1;

        if(depth > problem.getN()-1){
            solutions += goalState(problem, qq, oneAnswer);
            
            if (oneAnswer && solutions > 0){
                return solutions;
            }
            break;
        }

        qq.pop();

        for(int i=0; i < problem.getN(); i++){
            State node = nodeHold;

            node.getPos()[depth] = i;
            node.newDepth(0);
            qq.push(node);
        
            
        }

    }
    return solutions;
    
}

int bfsP(Problem & problem, bool oneAnswer = false){   //BFS But with pruned searches
    //Currently finds all paths to the lowest depth
    //Rewrite this to be as the original bfs function was

    queue<State> qq;    //queue of queen's positionsd
    qq.push(State());   //Pushes empty "State"
    State nodeHold;     //Independent sate holder, does not change within the for loop

    //vector<pair<int, int>> visited;

    int depth = 0;      //The depth of the tree
    long unsigned int solutions = 0; //Number of found, non unique solutions

    while (!qq.empty()){//Loops until either the queue is empty or the depth of N is reached                                                                                                                                                                                                                                                                                                                                                                                                                                                  
        
        nodeHold = qq.front();

        depth = nodeHold.getPos().size()-1;

        if(depth > problem.getN()-1){
            solutions += goalState(problem, qq, oneAnswer);
            
            if (oneAnswer && solutions > 0){
                return solutions;
            }
            break;
        }

        qq.pop();

        for(int i=0; i < problem.getN(); i++){
            State node = nodeHold;

            if(isValid(node, i, depth, problem)){
                node.getPos()[depth] = i;
                node.newDepth(0);
                qq.push(node);
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

void hillClimbing(Problem & prob, vector<int> initState, bool shotgun = false){

    //Note INDEX is BREADTH on the vector of positions
    
    int confidence = stateEval(initState, prob);

    cout << "INITIAL STATE" << endl;
    cout << "EVAL: " << confidence << endl;
    for (int i=0; i<prob.getN(); i++){
        for (int j=0; j<prob.getN(); j++){
            if(i == initState[j]){
                cout << "Q\t";
            }else{
                cout << "-\t";
            }
                
        }

        cout << endl << endl;
    }

    if(shotgun){
        cout << "Shotgun Mode" << endl;
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

        if (shotgun && confidence != 0){
            initState = genRandomState(prob);
            confidence = stateEval(initState, prob);
        }else{
            break;
        }
    }

    cout << "FINAL STATE" << endl;
    cout << "EVAL: " << confidence << endl;

    for (int i=0; i<prob.getN(); i++){
        for (int j=0; j<prob.getN(); j++){
            if(i == initState[j]){
                cout << "Q\t";
            }else{
                cout << "-\t";
            }
                
        }

         cout << endl << endl;
    }


    return;

}

void simulatedAnnealing(Problem & prob, vector<int> initState, double const decayRate = 0.5, int const constantMultiplyer = 10){    //SA, similar to HC and SHC but as the evaluation gets lower the probability for it to pick an answer anyway increases
    
    //Note: Index is breadth
    
    int confidence = stateEval(initState, prob);
    double temperature = prob.getN() * constantMultiplyer; //Adjusts the adjustment to be relative to the size of the problem
    int tempMax = temperature;
    int iterTemp = 0;

    cout << "INITIAL STATE" << endl;
    cout << "EVAL: " << confidence << endl;
    for (int i=0; i<prob.getN(); i++){
        for (int j=0; j<prob.getN(); j++){
            if(i == initState[j]){
                cout << "Q\t";
            }else{
                cout << "-\t";
            }
                
        }

        cout << endl << endl;
    }

    while (confidence != 0){
        for (int i=0; i<prob.getN(); i++){
            for (int j=0; j<prob.getN(); j++){
                if (i != initState[j]){
                    vector<int> newState = initState;
                    newState[j] = i;

                    int newConfidence = stateEval(newState, prob);

                    if (newConfidence == 0){
                        initState = newState;
                        confidence = newConfidence;
                        break;
                    }

                    int skipP = rand() % tempMax;

                    if (newConfidence < confidence || (skipP * newConfidence) < temperature){//IF the random value - new confidence is greater than the adjusted value /2 then it will skip
                        initState = newState;
                        confidence = newConfidence;
                    }    
                }
            }
        }

        temperature = tempMax * pow(exp(1), -(decayRate * iterTemp));
        iterTemp++;
    }
    

    cout << "FINAL STATE" << endl;
    cout << "EVAL: " << confidence << endl;

    for (int i=0; i<prob.getN(); i++){
        for (int j=0; j<prob.getN(); j++){
            if(i == initState[j]){
                cout << "Q\t";
            }else{
                cout << "-\t";
            }
                
        }

         cout << endl << endl;
    }
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

    srand((int)time(NULL)); //Seeding rand() for later

    Problem queens(atoi(argv[1]));

    using namespace std::chrono;
    auto start = high_resolution_clock::now(); //Timing how long the bfs function takes to run and complete

    cout << "PROBLEM:\n";
    cout << bfs(queens, false) << endl;

    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<seconds>(stop - start);
    auto duration2 = duration_cast<milliseconds>(stop - start);

    cout << "BFS TIME TAKEN" << endl << "Seconds: " << duration.count() << " Milliseconds: " << duration2.count() << endl;

    //BFS Pruned Call
    start = high_resolution_clock::now(); //Timing how long the bfs function takes to run and complete
    cout << bfsP(queens, false) << endl;

    stop = high_resolution_clock::now();

    duration = duration_cast<seconds>(stop - start);
    duration2 = duration_cast<milliseconds>(stop - start);
    cout << "BFS ONE SOLUTION, TIME TAKEN" << endl << "Seconds: " << duration.count() << " Milliseconds: " << duration2.count() << endl;

    /*vector<int> initState = genRandomState(queens);
    cout << "HILL CLIMBING" << endl;
    start = high_resolution_clock::now();

    hillClimbing(queens, initState, true);

    stop = high_resolution_clock::now();
    duration = duration_cast<seconds>(stop - start);
    duration2 = duration_cast<milliseconds>(stop - start);
    cout << "TIME TAKEN:" << endl << "Seconds: " << duration.count() << " Milliseconds: " << duration2.count() << endl;

    cout << "SIMULATED ANNEALING" << endl;
    start = high_resolution_clock::now();

    simulatedAnnealing(queens, initState);

    stop = high_resolution_clock::now();
    duration = duration_cast<seconds>(stop - start);
    duration2 = duration_cast<milliseconds>(stop - start);
    cout << "TIME TAKEN:" << endl << "Seconds: " << duration.count() << " Milliseconds: " << duration2.count() << endl;*/

    return EXIT_SUCCESS;

}
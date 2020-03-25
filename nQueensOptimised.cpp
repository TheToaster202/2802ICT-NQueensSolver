#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <queue>
#include <utility>
#include <vector>
#include <chrono>
#include <random>
#include <fstream>

using namespace std;

//s5132278, Mitchell Roles

//Note this Program uses command line arguments to alter how the program runs
// Arguments: 1=N, 2=Range, 3=Run BFS, 4=Run Local Searches, 5=Uses Shotgun Hill Climbing Search

class Problem{//Holds information regarding the given problem
    public:
    //Constructors
    Problem(int const & nP, int const & kP): n{nP}, k{kP}{}

    //Destructor
    ~Problem(){}

    int getN(){return n;}
    int getK(){return k;}

    void incN(){n+=1;}

    private:
    int n;  // NxN Board and number of queens
    int k;  //  N-K range for local search algorithms
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

    int getDepth(){ //returns the depth of the tree
        return sDepth;
    }

    void newDepth(int const & i){//Pushes a new queen pos into the positions list
        qPos.push_back(i);
        sDepth = qPos.size();
    }

    void printPos(){    //Prints the nodes in the path
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

void printFrontier(queue<State> frontier){  //Prints the contents of the frontier (For testing purposes)
    while(!frontier.empty()){
        frontier.front().printPos();
        frontier.pop();
    }

    cout << endl;
}

int goalState(Problem & problem, queue<State> frontier, bool pruned = false){//Function that will test if the current solution is a goal state
    //Problem Class, Queue of States, exit early bool

    int solutions = 0;  //Solutions found

    bool isSolved = true;   //Confition to break early if 1 solution has been found

    while (!frontier.empty()){  //Loops until frontier is empty
        isSolved = true;

        for (int i=0; i<problem.getN()-1; i++){

            for (int j=i+1; j<problem.getN(); j++){

                if (frontier.front().getPos()[i] == frontier.front().getPos()[j]){  //Checks if the current node and another node are on the same level
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
                    isSolved = false;
                    break;
                }
            }
        }

        if (isSolved == true){

            if (pruned){    //Returns one solution
                frontier.front().printPos();
                return 1;
            }
                    
            solutions ++;
            if(problem.getN() < 7){ //Prints path of solutions up to 6
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

int bfs(Problem & problem, bool oneAnswer = false, bool toFile = false){ //Breadth First Search Algorithm
    
    //Currently finds all paths to the lowest depth

    queue<State> qq;    //queue of queen's positionsd
    qq.push(State());   //Pushes empty "State"
    State nodeHold;     //Independent sate holder, does not change within the for loop
    
    ofstream queueOut;
    ifstream queueIn;

    int depth = 0;      //The depth of the tree
    long unsigned int solutions = 0; //Number of found, non unique solutions

    while (!qq.empty()){//Loops until either the queue is empty or the depth of N is reached                                                                                                                                                                                                                                                                                                                                                                                                                                                  
        
        nodeHold = qq.front();

        depth = nodeHold.getPos().size()-1;

        if(depth > problem.getN()-1){   //If the number of nodes on a path is greater than N test goal states
            //No point testing any other state

            solutions += goalState(problem, qq, oneAnswer);
            
            if (oneAnswer && solutions > 0){
                return solutions;
            }
            break;
        }

        qq.pop();   //Pop placed under the goal state check to ensure all Depth == N paths are checked

        for(int i=0; i < problem.getN(); i++){
            State node = nodeHold;      //Assigns a path that will have all of the previous information of the prior, NodeHold = {1, 2, 3} : Node = {1, 2, 3, i} --> Node = {1, 2, 3, i+1}

            node.getPos()[depth] = i;
            node.newDepth(0);
            qq.push(node);  // pushes the new node onto the queue
        }

    }
    return solutions;
    
}

int bfsP(Problem & problem, bool oneAnswer = false){   //BFS But with pruned searches
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

            if(isValid(node, i, depth, problem)){   //Checks if the node is on the same vertical level or is diagonal to another node on the same path
                //Doesn't need to check if they are on the same breadth, as it is impossible with the current implementation
                node.getPos()[depth] = i;
                node.newDepth(0);
                qq.push(node);
            }
            
        }

    }
    return solutions;
}

vector<int> genRandomState(Problem & prob){    //Generates a random state where all queens are on the board
    
    vector<int> randState;      //Holds the initial state for the algorithms
    vector<int> usedNumbers;    //Holds the numbers that have already been generated by the program

    for(int i=0; i<prob.getN(); i++){
        usedNumbers.push_back(0);
    }

    int i=0;

    while (i<prob.getN()){
        int randNumber = rand()%prob.getN();
        
        if (usedNumbers[randNumber]==0){        //Won't allow two queen positions to be on the same vertical level
            randState.push_back(randNumber);
            i++;                                //I will only increment when a unique number has been found
            usedNumbers[randNumber] = 1;
        }
        
    }

    return randState;


}

int stateEval(vector<int> & positions, Problem & prob){ //Evaluates the current state and counts the number of queens that are currently attacking each other
    int level = 0;
    
    //Checks if queens are on the same vertical level or on the same diagonal line
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

void hillClimbing(Problem & prob, vector<int> initState, bool shotgun){     //Hill Climbing Algorithm with a Shotgun Search mode

    //Note INDEX is BREADTH on the vector of positions
    
    int confidence = stateEval(initState, prob);    //Recieves an evaulation of the initial state

    cout << "INITIAL STATE" << endl;
    cout << "EVAL: " << confidence << endl;

   for (int i=0; i<prob.getN(); i++){
        cout << "( " << i << " " << initState[i] << " ) ";
    }

    cout << endl;
    //Prints the board
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
                    if (newConfidence < confidence){        //Compares the evaluation of the old state against the new one
                        initState = newState;
                        confidence = newConfidence;
                    }    
                }
            }
        }

        if (shotgun && confidence != 0){                //if Shotgun Search is true it will feed the program another random state and allows it to loop, else it will break
            initState = genRandomState(prob);
            confidence = stateEval(initState, prob);
        }else{
            break;
        }

    }

    cout << "FINAL STATE" << endl;
    cout << "EVAL: " << confidence << endl;

    for (int i=0; i<prob.getN(); i++){
        cout << "( " << i << " " << initState[i] << " ) ";
    }

    cout << endl;

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
    //Decay rate and the constant multiplier are for the exponetial decay equation, are set up by default to work well enough but can be altered

    //Note: Index is breadth
    
    int confidence = stateEval(initState, prob);
    double temperature = prob.getN() * constantMultiplyer; //Adjusts the adjustment to be relative to the size of the problem
    int tempMax = temperature;                             //Hold the maximum temperature
    int iterTemp = 0;                                      //Iteration in the decay of the temperature

    cout << "INITIAL STATE" << endl;
    cout << "EVAL: " << confidence << endl;
   for (int i=0; i<prob.getN(); i++){
        cout << "( " << i << " " << initState[i] << " ) ";
    }

    cout << endl;
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

                    if (newConfidence < confidence || (skipP * newConfidence) < temperature){ //If random value * new evaluation is less than the current temperature than the program will allow the change to happen
                        initState = newState;
                        confidence = newConfidence;
                    }    
                }
            }
        }

        temperature = tempMax * pow(exp(1), -(decayRate * iterTemp));   //Expnential Decay equation
        iterTemp++;
    }
    

    cout << "FINAL STATE" << endl;
    cout << "EVAL: " << confidence << endl;

    for (int i=0; i<prob.getN(); i++){
        cout << "( " << i << " " << initState[i] << " ) ";
    }

    cout << endl;

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
    
    bool bfsMode = false, lsMode = false;
    bool hcMode = false;

    cout.clear();

    if (argc < 3){
        cerr << "Program needs an N value of 1-20 and a K range value to run" << endl;
        return EXIT_FAILURE;
    }
    
    if(argc >= 4){
        if (atoi(argv[3]) > 0){
            bfsMode = true;
        }
    }
    
    if(argc >= 5){
        if (atoi(argv[4]) > 0){
            lsMode = true;
        }
    }

    if (argc == 6){
        if (atoi(argv[5]) > 0){
            hcMode = true;
        }
    }


    if (atoi(argv[1]) < 1 || atoi(argv[1]) > 20){
        cerr << "Program needs an N value of 1-20 to run" << endl;
        return EXIT_FAILURE;
    }
    
    if (atoi(argv[2]) < atoi(argv[1]) && atoi(argv[2]) != 0){// k == 0 is the opt out of the range statement
            cerr << "K value must be greater than N value" << endl;
            return EXIT_FAILURE;
    }
    
    srand((int)time(NULL)); //Seeding rand() for later

    Problem queens(atoi(argv[1]), atoi(argv[2]));

    using namespace std::chrono;
    
    auto start = high_resolution_clock::now(); //Timing how long the bfs function takes to run and complete
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    auto duration2 = duration_cast<milliseconds>(stop - start);

    if (bfsMode){
        cout << "BFS" << endl;

        start = high_resolution_clock::now();
        cout << bfs(queens, false) << endl;

        stop = high_resolution_clock::now();
        duration = duration_cast<seconds>(stop - start);
        duration2 = duration_cast<milliseconds>(stop - start);
            

         cout << "BFS TIME TAKEN" << endl << "Seconds: " << duration.count() << " Milliseconds: " << duration2.count() << endl;
        

        //BFS Pruned Call
        start = high_resolution_clock::now(); //Timing how long the bfs function takes to run and complete
        
        cout << bfsP(queens, false) << endl;

        stop = high_resolution_clock::now();

        duration = duration_cast<seconds>(stop - start);
        duration2 = duration_cast<milliseconds>(stop - start);
        cout << "BFS PRUNED, TIME TAKEN" << endl << "Seconds: " << duration.count() << " Milliseconds: " << duration2.count() << endl;
    }

    
    if (lsMode){
        //Local Searches
        cout << endl << "LOCAL SEARCHES" << endl;

        while (queens.getN() <= queens.getK() || queens.getK() == 0){//Will loop until n == k, one iteration if k == 0
            
            cout << endl << "N: " << queens.getN() << endl << endl;

            if (queens.getN() != 2 && queens.getN() != 3){
                vector<int> initState = genRandomState(queens);
                cout << "HILL CLIMBING" << endl;
                start = high_resolution_clock::now();

                hillClimbing(queens, initState, hcMode);

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
                cout << "TIME TAKEN:" << endl << "Seconds: " << duration.count() << " Milliseconds: " << duration2.count() << endl;
                
                if (queens.getK() == 0){
                    break;
                }
            }
            
            queens.incN();
        }
    }

    return EXIT_SUCCESS;

}
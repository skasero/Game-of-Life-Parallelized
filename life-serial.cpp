/**
 * 
 * Conway's Game of Life Serial Implementation
 * Authors: Jonathan Lee & Spencer Kase Rohlfing
 * 
 **/

/**
 * ================ HEADERS ================
 **/
#include <iostream>
#include <string>
#include "omp.h"

#include "Timer.h"
#include "common.h"

using namespace std;


/**
 * ================ Life Class ================
 **/
class Life {
    public:
        Life ();
        Life (const int xsize, const int ysize);
        Life (const Life &other);
        ~Life();

        void randomize(float threshold);

        void setCell(bool alive, int row, int col);
        bool getCell(int row, int col) const;
        void toggleCell(int row, int col);

        int getNeighbors (int x, int y);
        bool getNextState (int x, int y);

        void step (const int generations);
        bool** getNextGeneration ();

        void printBoard();

        int getXSize() const;
        int getYSize() const;

        int getGeneration() const;
        void resetGeneration();

    private:
        bool** cells;
        int xsize;
        int ysize;
        int generation;
};

/**
 * Life()
 * Default Constructor and should probably never be used.
 **/
Life::Life(){
    xsize = 0;
    ysize = 0;
    cells = nullptr;
    generation = 0;
}

/**
 * Life()
 * Parameterized Constructor.
 * Sets the grid size of the board along with allocating memory for the entire grid size.
 **/
Life::Life(const int xsize, const int ysize){
    this->xsize = xsize;
    this->ysize = ysize;
    generation = 0;
    
    cells = new bool*[xsize];
    for (int i = 0; i < xsize; i++){
        cells[i] = new bool[ysize];
        for (int j = 0; j < ysize; j++){
            cells[i][j] = false;
        }
    }
}

/**
 * Life()
 * Copy Constructor and probably will never be used.
 **/
Life::Life(const Life &other){
    this->~Life();
    this->xsize = other.xsize;
    this->ysize = other.ysize;
    this->generation = other.generation;

    cells = new bool*[xsize];
    for (int i = 0; i < xsize; i++){
        cells[i] = new bool[ysize];
        for (int j = 0; j < ysize; j++){
            cells[i][j] = other.getCell(i, j);
        }
    }
}

/**
 * ~Life()
 * Destructor to delete all allocated memory in the grid
 **/
Life::~Life(){
    for (int i = 0; i < xsize; i++){
        delete cells[i];
    }
    delete [] cells;
    
    generation = 0;
    xsize = 0;
    ysize = 0;
    cells = nullptr;
}

/**
 * setCell()
 * Sets the current cell the specified status. 
 **/
void Life::setCell(bool status, int row, int col){
    cells[row][col] = status;
}

/**
 * getCell()
 * returned the value of the cell.
 **/
bool Life::getCell(int row, int col) const{
    return cells[row][col];
}

/**
 * toggleCell()
 * Switches the value of the cell.
 **/
void Life::toggleCell(int row, int col){
    cells[row][col] = !cells[row][col];
}

/**
 * randomize()
 * Used to randomly assign values to the cell on a board with default threshold of 0.5. 
 * Or about 50% of the cells will be alive.
 **/
void Life::randomize(float threshold = 0.5){
    for (int i = 0; i < xsize; i++){
        for (int j = 0; j < ysize; j++){
            cells[i][j] = ((float) rand() / (RAND_MAX)) < threshold ? true : false;
        }
    }
}

/**
 * step()
 * Used to iterate through the number of generations specified updating the board.
 * Creates a copy of the grid from getNextGeneration() then deletes the old grid. 
 **/
void Life::step(const int generations = 1){
    for (int step = 0; step < generations; step++){
        bool** prevGeneration = cells;
        cells = getNextGeneration();

        for (int i = 0; i < xsize; i++){
            delete [] prevGeneration[i];
        }
        delete [] prevGeneration;
    }
    generation++;
}

/**
 * getNextGeneration()
 * Used to calculate the next generation of the board. Returns a copy of the new grid. 
 **/
bool** Life::getNextGeneration(){
    bool** nextGeneration = new bool*[xsize];
    for (int i = 0; i < xsize; i++){   
        nextGeneration[i] = new bool[ysize];
    }

    for (int i = 0; i < xsize; i++){
        for (int j = 0; j < ysize; j++){
            nextGeneration[i][j] = getNextState(i, j);
        }
    }
    return nextGeneration;
}

/**
 * getNeighbors()
 * Checks the number of neighboring cells around each cell. Returns that number of alive neighboring cells.
 **/
int Life::getNeighbors(int x, int y){
    int neighbors = getCell(x, y) ? -1 : 0; // -1 to negate self if alive, otherwise 0
    // cout << "Focusing on cell (" << x << "," << y << ")" << endl;

    int iStart = (x == 0 ? 0 : -1);
    int jStart = (y == 0 ? 0 : -1);
    int ii = (x == xsize - 1 ? 0 : 1);
    int jj = (y == ysize - 1 ? 0 : 1);

    for (int i = iStart; i <= ii; i++){
        for (int j = jStart; j <= jj; j++){
            // cout << "   Getting state of cell (" << x + i << "," << y + j << ")" << endl;
            if (cells[x + i][y + j] == true){
                neighbors++;
            }
        }
    }
    return neighbors;
}

/**
 * getNextState()
 * Calculates the next state of each cell. It determines if bases on the a set of rules if a cell is to continue to live, 
 * or if the cell dies out due to to few or to many neighboring cells are around it.
 **/
bool Life::getNextState(int x, int y){
    int neighbors = getNeighbors(x, y);
    // cout << "       Cell at (" << x << "," << y << ") has " << neighbors << " neighbors." << endl;
    if (cells[x][y] == true && (neighbors == 2 || neighbors == 3)){
        // cout << "           Cell is alive so returning TRUE." << endl;
        return true;
    } else if (cells[x][y] == false && neighbors == 3){
        // cout << "           Cell is dead so returning TRUE." << endl;
        return true;
    } else {
        // cout << "           Returning FALSE." << endl;
        return false;
    }
}

/**
 * printBoard()
 * Visualize the board.
 **/
void Life::printBoard(){
    for (int y = 0; y < ysize; y++){
        for (int x = 0; x < xsize; x++){
            if (cells[x][y]){
                cout << "[]";
            } else {
                cout << "  ";
            }
        }
        cout << endl;
    }
}

/**
 * getXSize()
 * Returns the X size of the board.
 */
int Life::getXSize() const{
    return xsize;
}

/**
 * getYSize()
 * Returns the Y size of the board.
 */
int Life::getYSize() const{
    return ysize;
}

/**
 * getGeneration()
 * Returns the number of generations.
 **/
int Life::getGeneration() const{
    return generation;
}

/**
 * resetGeneration()
 * Resets the generation count.
 **/
void Life::resetGeneration(){
    generation = 0;
}


/**
 * ================ Main ================
 **/

int main(int argc, char **argv){
    if(find_option(argc, argv, "-h") >= 0){
        cout << "Usage:" << endl
            << "-h to see usage" << endl
            << "-n <int> for the number of generations. Default is 10"
            << "-x <int> for X grid size. Default is 25" << endl
            << "-y <int> for Y grid size. Default is 25" << endl
            << "-p <int> (0-100) for percent of alive cells. Default is 50%" << endl
            << "-d debug mode allows the user to iterate over each generation manually. This inherents -v"
            << "-v verbose mode that print the board after each generation. Default this is disabled" << endl;
        return 0;
    }
    
    int nGens = read_int(argc, argv, "-n", 10);
    int xsize = read_int(argc, argv, "-x", 25);
    int ysize = read_int(argc, argv, "-y", 25);
    float percent = read_int(argc, argv, "-p", 50) / 100;
    
    Life* newLife = new Life(xsize, ysize);
    newLife -> randomize(percent);
    string input = "";

    Timer timer;
    timer.start();
    // while (input != "x"){
    while (newLife -> getGeneration() < nGens){
        // cout << "Generation #" << newLife -> getGeneration() << endl;
        // for (int lines = 0; lines < newLife -> getXSize(); lines++ )
        //     cout << "--";
        // cout << endl;
        // newLife -> printBoard();
        // for (int lines = 0; lines < newLife -> getXSize(); lines++ )
        //     cout << "--";
        // cout << endl;
        // cout << "(Enter to continue, 'r' to randomize, 'x' to exit): ";
        // getline(cin, input);
        // cout << endl;
        // if(newLife -> getGeneration() % 100 == 0)
        //     cout << newLife -> getGeneration() << endl;
        // if (input == "r"){
        //     newLife -> randomize();
        //     newLife -> resetGeneration();
        // } else {
            newLife -> step();
        // }
    }
    timer.stop();

    cout << "This took: " << timer.getElapsedTime() << endl;
    return 0;
}
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

        void randomize(double threshold);

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
Life::Life() {
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
Life::Life(const int xsize, const int ysize) {
    this->xsize = xsize;
    this->ysize = ysize;
    generation = 0;
    
    cells = new bool*[xsize];
    for (int i = 0; i < xsize; i++) {
        cells[i] = new bool[ysize];
        for (int j = 0; j < ysize; j++) {
            cells[i][j] = false;
        }
    }
}

/**
 * Life()
 * Copy Constructor and probably will never be used.
 **/
Life::Life(const Life &other) {
    this->~Life();
    this->xsize = other.xsize;
    this->ysize = other.ysize;
    this->generation = other.generation;

    cells = new bool*[xsize];
    for (int i = 0; i < xsize; i++) {
        cells[i] = new bool[ysize];
        for (int j = 0; j < ysize; j++) {
            cells[i][j] = other.getCell(i, j);
        }
    }
}

/**
 * ~Life()
 * Destructor to delete all allocated memory in the grid
 **/
Life::~Life() {
    for (int i = 0; i < xsize; i++) {
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
void Life::setCell(bool status, int row, int col) {
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
void Life::toggleCell(int row, int col) {
    cells[row][col] = !cells[row][col];
}

/**
 * randomize()
 * Used to randomly assign values to the cell on a board with default threshold of 0.5. 
 * Or about 50% of the cells will be alive.
 **/
void Life::randomize(double threshold = 0.5) {
    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            cells[i][j] = ((double) rand() / (RAND_MAX)) < threshold ? true : false;
        }
    }
}

/**
 * step()
 * Used to calculate the number of generations specified updating the board.
 **/
void Life::step(const int generations = 1) {
    for (int step = 0; step < generations; step++) {
        bool** prevGeneration = cells;
        cells = getNextGeneration();

        for (int i = 0; i < xsize; i++) {
            delete [] prevGeneration[i];
        }
        delete [] prevGeneration;
    }
    generation++;
}

bool** Life::getNextGeneration () {
    bool** nextGeneration = new bool*[xsize];
    for (int i = 0; i < xsize; i++) {   
        nextGeneration[i] = new bool[ysize];
    }

    for (int i = 0; i < xsize; i++) {
        for (int j = 0; j < ysize; j++) {
            nextGeneration[i][j] = getNextState(i, j);
        }
    }
    return nextGeneration;
}

int Life::getNeighbors (int x, int y) {
    int neighbors = getCell(x, y) ? -1 : 0; // -1 to negate self if alive, otherwise 0
    // cout << "Focusing on cell (" << x << "," << y << ")" << endl;

    int iStart = (x == 0 ? 0 : -1);
    int jStart = (y == 0 ? 0 : -1);
    int ii = (x == xsize - 1 ? 0 : 1);
    int jj = (y == ysize - 1 ? 0 : 1);

    for (int i = iStart; i <= ii; i++) {
        for (int j = jStart; j <= jj; j++) {
            // cout << "   Getting state of cell (" << x + i << "," << y + j << ")" << endl;
            if (cells[x + i][y + j] == true) {
                neighbors++;
            }
        }
    }
    return neighbors;
}

bool Life::getNextState (int x, int y) {
    int neighbors = getNeighbors(x, y);
    // cout << "       Cell at (" << x << "," << y << ") has " << neighbors << " neighbors." << endl;
    if (cells[x][y] == true && (neighbors == 2 || neighbors == 3)) {
        // cout << "           Cell is alive so returning TRUE." << endl;
        return true;
    } else if (cells[x][y] == false && neighbors == 3) {
        // cout << "           Cell is dead so returning TRUE." << endl;
        return true;
    } else {
        // cout << "           Returning FALSE." << endl;
        return false;
    }
}

void Life::printBoard (){
    for (int y = 0; y < ysize; y++) {
        for (int x = 0; x < xsize; x++) {
            if (cells[x][y]) {
                cout << "[]";
            } else {
                cout << "  ";
            }
        }
        cout << endl;
    }
}

int Life::getXSize() const{
    return xsize;
}

int Life::getYSize() const{
    return ysize;
}

int Life::getGeneration() const{
    return generation;
}

void Life::resetGeneration() {
    generation = 0;
}

int main(int argc, char **argv) {
    int size;
    if(argc == 1){
        size = 1000;
    }else{
        size = atoi(argv[1]);
    }
    Life* newLife = new Life(size, size);
    newLife -> randomize();
    string input = "";

    Timer timer;
    timer.start();
    // while (input != "x") {
    while (newLife -> getGeneration() < 200) {
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
        // if (input == "r") {
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
#include "Map.h"
#include "MapSection.h"

using namespace std;

// -----------------------------------------

Map::~Map(){
    delete mapArea;
}

// ------------------------------------------

// ------------------------------------------

MapSection* Map::getPos(int x, int y){
    return mapArea[x][y];
}

// ---------------- getColAsChar ------------

char* Map::getColAsChar(int col)
{
	// Char sent to computer
	char* output = new char[25];
    
	for (int it = 0; it < 25; it++){
        // Type of the block we are looking at
        output[it] = this->getPos(col,it);
	}
    
    return output;
}

// --------- SET COL ------------

void Map::setColAsChar(char* inCol){
    
    int col = inCol[2];
    
    for (int row = 0; x < 25; row++) {
        setSection(col,row,inCol[row]);
    }
}

// --------- SET POS ------------

void Map::setSection(int xPos, int yPos, MapSection* inSection){
    mapArea[xPos][yPos] = inSection;
}

// --------- DEBUG --------------

#if DEBUG == 1
void Map::printMap(){    
    for (int it = 0; it < 17; it++) {
        for (int i = 0; i < 32; i++) {
            cout << this->getPos(i,it) << " ";
        }
        cout << endl;
    }
}
#endif
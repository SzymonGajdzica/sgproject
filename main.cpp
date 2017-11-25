#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int ** createNewTable(const unsigned int size){
    auto** table = new int*[size];
    for(int i = 0; i < size; ++i)
        table[i] = new int[size];
    return table;
}

void saveTableToFile(const unsigned int size, int** table, const string &fileName) {
    ofstream file;
    file.open (fileName);
    for(unsigned int i = 0; i < size; i++) {
        for(unsigned int j = 0; j < size; j++) {
            if(j != 0)
                file << ",";
            file << table[j][i];
        }
        file << "\n";
    }
    file.close();
}

int ** getTableFromFile(const unsigned int size, const string &fileName){
    string line;
    ifstream file (fileName);
    if (file.is_open()) {
        int** table = createNewTable(size);
        int k = 0;
        while ( getline (file, line) ){
            vector<int> vector;
            stringstream ss(line);
            int i;
            while (ss >> i) {
                vector.push_back(i);
                if (ss.peek() == ',')
                    ss.ignore();
            }
            for (unsigned n = 0; n < vector.size(); n++) {
                table[n][k] = vector.at(n);
            }
            k++;
        }
        file.close();
        return table;
    }
    else{
        cout << "Unable to open file: " << fileName << endl;
        return nullptr;
    }
}

const unsigned int getSize(const string &fileName){
    string line;
    unsigned int size = 0;
    ifstream file (fileName);
    if (file.is_open()) {
        int counterX = 0;
        while ( getline (file, line) ){
            vector<int> vector;
            int i;
            stringstream ss(line);
            while (ss >> i) {
                vector.push_back(i);
                if (ss.peek() == ',')
                    ss.ignore();
            }
            counterX ++;
            if(size == 0)
                size = vector.size();
            else if (size != vector.size()) {
                cout << "Size of the rows are not equal in file: " << fileName << endl;
                file.close();
                return 0;
            }
        }
        if(counterX != size){
            cout << "Number of the rows and columns are not equal in file: " << fileName << endl;
            file.close();
            return 0;
        }
    } else {
        cout << "Unable to open file: " << fileName << endl;
        return 0;
    }
    file.close();
    return size;
}

int ** codeTable1(const unsigned int size, int** table){
    int **newTable = createNewTable(size);
    unsigned int counter = 0;
    for(unsigned int i = 0; i < size; i++){
        if(counter == 0)
            for(unsigned int j = 0; j < size; j++){
                newTable[j][i] = table[i][counter];
                counter++;
            }
        else{
            counter = size - 1;
            for(unsigned int j = 0; j < size; j++){
                newTable[j][i] = table[i][counter];
                counter--;
            }
            counter = 0;
        }
    }
    return newTable;
}

bool checkCoord(int i, int j, const unsigned int size) {
    return i >= 0 && j >= 0 && i < size && j < size;
}

int ** codeTable2(const unsigned int size, int** table){
    auto* tableHelper = new int[size * 2];
    int **newTable = createNewTable(size);
    unsigned int indexX = 0;
    unsigned int indexY = 0;
    unsigned int counter = 0;
    bool check = false;
    for (unsigned int k = 0; k < size; k++) {
        tableHelper[counter] = table[k][0];
        counter++;
        int i = k - 1;
        int j = 1;
        while (checkCoord(i, j, size)) {
            tableHelper[counter] = table[i][j];
            counter++;
            i--;
            j++;
        }
        if(check)
            for(unsigned int p = 0; p < counter; p++){
                if(indexX > size - 1){
                    indexX = 0;
                    indexY++;
                }
                newTable[indexX][indexY] = tableHelper[p];
                indexX ++;
            }
        else
            for(int p = counter - 1; p >= 0 ; p--){
                if(indexX > size - 1){
                    indexX = 0;
                    indexY++;
                }
                newTable[indexX][indexY] = tableHelper[p];
                indexX ++;
            }
        counter = 0;
        check = !check;
    }
    counter = 0;
    check = false;
    for (unsigned int k = 1; k < size; k++) {
        tableHelper[counter] = table[size-1][k];
        counter++;
        int i = size - 2;
        int j = k + 1;
        while (checkCoord(i, j, size)) {
            tableHelper[counter] = table[i][j];
            counter++;
            i--;
            j++;
        }
        if(check)
            for(unsigned int p = 0; p < counter; p++){
                if(indexX > size - 1){
                    indexX = 0;
                    indexY++;
                }
                newTable[indexX][indexY] = tableHelper[p];
                indexX ++;
            }
        else
            for(int p = counter - 1; p >= 0 ; p--){
                if(indexX > size - 1){
                    indexX = 0;
                    indexY++;
                }
                newTable[indexX][indexY] = tableHelper[p];
                indexX ++;
            }
        counter = 0;
        check = !check;
    }
    return newTable;
}

int ** codeTable3(const unsigned int size, int** table){
    int **newTable = createNewTable(size);
    unsigned int counter = 0;
    for(unsigned int i = 0; i < size; i++){
        if(counter == 0)
            for(unsigned int j = 0; j < size; j++){
                newTable[i][j] = table[i][counter];
                counter++;
            }
        else{
            counter = size - 1;
            for(unsigned int j = 0; j < size; j++){
                newTable[i][j] = table[i][counter];
                counter--;
            }
            counter = 0;
        }
    }
    return newTable;
}

bool executeMatrix(const string &fileName) {
    const unsigned int size = getSize(fileName);
    if(size == 0){
        cout << "Problem with reading file: " << fileName << endl;
        return false;
    }
    int **table = getTableFromFile(size,fileName);
    saveTableToFile(size, codeTable1(size,table), "Encrypted1_" + fileName);
    saveTableToFile(size, codeTable2(size,table), "Encrypted2_" + fileName);
    saveTableToFile(size, codeTable3(size,table), "Encrypted3_" + fileName);
    return true;
}

void displayHelp() {
    cout << "help me pls" << endl;
}

int main(int argc, char *argv[]) {

    if(argc < 2){
        displayHelp();
        return 0;
    }

    auto *fileNames = new string[argc];
    for(unsigned int i = 1; i < argc; i++){
        string parameter = argv[i];
        if(parameter == "h"){
            displayHelp();
            return 0;
        }
        fileNames[i - 1] = parameter;
    }

    for(unsigned int i = 0; i < argc - 1; i++){
        string fileName = fileNames[i];
        if(executeMatrix(fileName))
            cout << "Encryption of file: " + fileName + " SUCCESS";
        else
            cout << "Encryption of file: " + fileName + " FAIL";
        cout << endl << endl;
    }
    return 0;
}

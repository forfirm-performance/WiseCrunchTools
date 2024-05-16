#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
    //Controllo il numero dei parametri
    //Verify parameters' number
    if(argc==1){
        cout << endl;
        cout << "Usage:" << endl;
        cout << "      count_line <filename>" << endl;
        return 0;
	}
	//Il secondo parametro è il nome file
	//Second parameter = file name
    std::string file_name="";
    if(argc==2){
        file_name = argv[1];
    }

    // Apro il file se esiste; se non esiste ritorno -1
    //Open file to test the line number; if it does not exist return -1
    std::ifstream f(file_name.c_str());

        //Verify if file is good
    if(f.fail()) {
        cout << "001 - Error reading file " << file_name << endl;
        return 0;
    }

    if(f.good()){
        std::string line;
        int i;
        for (i = 0; std::getline(f, line); ++i);
        f.close();
        cout << i << endl;
    } else {
        return -1;
    }
    return 0;
}

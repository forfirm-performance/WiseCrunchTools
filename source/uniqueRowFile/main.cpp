#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

using namespace std;

int main(int argc, char* argv[])
{

    //Controllo il numero dei parametri
    //Verify parameters' number
    if(argc==1){
        cout << endl;
        cout << "Usage:" << endl;
        cout << "      uniqueRowFile <filename>" << endl;
        return 0;
	}

	//Il secondo parametro è il nome file
	//Second parameter = file name
    std::string FILE_NAME="";
    if(argc==2){
        FILE_NAME = argv[1];
    }

    std::vector<std::string> data;
    ifstream myfile (FILE_NAME.c_str());

    // Verify if file exixst
    if(myfile.fail()){
        cout << "001-Error on reading file: " << FILE_NAME << endl;
        return 0;
    }


    string line;
    //if not required to use array
    while (getline(myfile, line))
    {
      data.push_back(line);
    }
    myfile.close();

    std::sort( data.begin(), data.end() );
    data.erase( std::unique( data.begin(), data.end()), data.end() );
    //now print vector out:
    std::copy(data.begin(), data.end(), ostream_iterator<string>(cout, "\n"));
    return 0;
}


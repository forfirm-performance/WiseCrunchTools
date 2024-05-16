#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

int main(int argc, char* argv[])
{

    std::string INPUT_FILE = ""; //argv[1];
    std::string  m_stringa="";
    std::string line;

    bool found=false;
    try{
        for(int n=1;n<=argc-1;++n){
            string option(argv[n]);
            // Input filename
            if(option.substr(0,3)=="-i:"){
                if(argc>=3){
                    string valore(argv[n+1]);
                    INPUT_FILE = valore; //option.substr(3,string::npos).c_str();
                    found=true;
                }
            }
        }
    } catch (exception& e){
            cout << endl;
            cout << "Error passing input parameters !" << endl;
            cout << endl;
            cout << "Usage:" << endl;
            cout << "      creaStringa -i: <INPUT_filename>" << endl;
            cout << endl;
            cout << "MANDATORY -i:    INPUT file name" << endl;
            cout << endl;
            cout << "More details on README file" << endl;
            cout << endl;
            return 0;
    }

    //Controllo il numero dei parametri
    //Verify parameters' number
    if(!found){
        cout << endl;
        cout << "Usage:" << endl;
        cout << "      creaStringa -i: <INPUT_filename>" << endl;
        cout << endl;
        cout << "MANDATORY -i:    INPUT file name" << endl;
        cout << endl;
        cout << "More details on README file" << endl;
        cout << endl;
        return 0;
    }

	//Read INPUT_FILE
        ifstream myfile(INPUT_FILE.c_str());
        if(myfile.fail()){
            cout << "001-Error on reading file: " << INPUT_FILE << endl;
            return 0;
        }

        if (myfile.is_open())
        {
              while ( myfile.good() )
              {
                    getline (myfile,line);
                    if(!line.empty()){
                        m_stringa+=line.substr(0,1);
                    } //Close if line not empty on INPUT_FILE
              } // Close while file good on INPUT_FILE
              // Close INPUT_FILE
              myfile.close();
        } // Close file open on INPUT_FILE

    cout << m_stringa << endl;

    return 0;
} // End main


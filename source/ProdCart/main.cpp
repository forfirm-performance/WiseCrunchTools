// ProdCart.cpp : Defines the entry point for the console application.
//

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <deque>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{

    std::string FILE_NAME = "";
    std::string SEP = ";";
    bool FLAG_COMMENTS = false;

    //Read passed values
    try{
        for(int n=1;n<=argc-1;++n){
                string option(argv[n]);
                // Input file name
                if(option.substr(0,3)=="-i:"){
                        string valore(argv[n+1]);
                        FILE_NAME = valore;
                }
                // Input separator
                if(option.substr(0,3)=="-s:"){
                        string valore(argv[n+1]);
                        SEP=valore;
                }
                // Input flag comments
                if(option.substr(0,2)=="-c"){
                        FLAG_COMMENTS=true;
                }

        } // Close for on argc
	} catch (exception& e){
            cout << endl;
            cout << "Error passing input parameters !" << endl;
            cout << endl;
            cout << "Usage:" << endl;
            cout << "      ProdCart -i: <filename> -s: (SEPARATOR) -c" << endl;
            cout << endl;
            cout << "MANDATORY -i:    INPUT file name" << endl;
            cout << "OPTIONAL  -s:    SEPARATOR fields file. The default is ;" << endl;
            cout << "OPTIONAL  -c:    Print comments" << endl;
            cout << endl;
            cout << "More details on README file" << endl;
            cout << endl;
              return 0;
    }

    //Controllo il numero dei parametri
    //Verify parameters' number
    if(FILE_NAME==""){
        cout << endl;
        cout << "Usage:" << endl;
        cout << "      ProdCart -i: <filename> -s: (SEPARATOR) -c" << endl;
        cout << endl;
        cout << "MANDATORY -i:    INPUT file name" << endl;
        cout << "OPTIONAL  -s:    SEPARATOR fields file. The default is ;" << endl;
        cout << "OPTIONAL  -c:    Print comments" << endl;
        cout << endl;
        cout << "More details on README file" << endl;
        cout << endl;
        return 0;
	}

    //Imposto i parametri
    //Variable definition
    string line,item,strTemp;
    deque< vector<string> > deq;
    vector<string> rowMix;
    long int count = 0;

    //Definizione funzioni ausiliarie
    //Auxiliary function definition
    bool verifyAvailableMemory();

    //Input file
    ifstream myfile(FILE_NAME.c_str());

    //Verify if file exist
    if(myfile.fail()){
        cout << "001-Error on reading file: " << FILE_NAME << endl;
        return 0;
    }

    if (myfile.is_open())
    {
      while ( myfile.good() )
      {

          // Creo una nuova riga
          // Create new row
          vector<string> row;

          // Leggo la nuova linea dal file e la carico nello stringstream
          // Read new line and load into the stringstream
          getline (myfile,line);

          // Se la linea non è vuota...gli spazi vengono considerati validi
          // If line not empty... (spaces are considered valid)
          if(!line.empty()){

            stringstream ss(line);

            // La linea del file deve terminare con un enter...
            // File line must finish with enter...
            try{
                while(getline(ss,item,SEP.c_str()[0])){
                    if(verifyAvailableMemory()){
                        row.push_back(item);
                    } else {
                        cout << "002-Error allocating memory while reading file. File too big or not correctly formatted !" << endl;
                        return 0;
                    }
                }
            }catch (exception& e){
              cout << "003-The following error occurred: " << e.what() << endl;
              return 0;
            }

            // Inserisco la nuova riga nella deque
            // Insert new row in deque
            try{
                if(verifyAvailableMemory()){
                    deq.push_back(row);
                } else {
                    cout << "004-Error allocating memory while creating deque. Too many combinations !" << endl;
                    return 0;
                }
            } catch (exception& e){
                cout << "005-The following error occurred: " << e.what() << endl;
                return 0;
            }

          } // Close empty line - Chiusura line vuota

      } // Close while - Chiudo while

      // Close file - Chiusura file
      myfile.close();


      // Provo a mischiare i record...Quando trovo che il size è uno, allora ho finito.
      // Try to mix the records...When size = 1 --> finish!
      try{

          //Stampo solo se FLAG_COMMENTS = true
          //Print just if FLAG_COMMENTS is true
          if (FLAG_COMMENTS){
              cout << endl;
              cout << "Started! Wait... when the counter is 1 the combinations start print out. Depending n the numbers " ;
              cout << "of combinations, this operation should take long time! It's possible to interrupt the program with Control+C";
              cout << endl << endl;
          }
          while(deq.size()>1){
              strTemp.clear();
              rowMix.clear();
              for(unsigned long long i=0; i<deq[0].size(); ++i){
                  for (unsigned long long j=0; j<deq[1].size(); ++j){
                      strTemp = deq[0][i] + SEP[0] + deq[1][j];
                      try{
                          if(verifyAvailableMemory()){
                              rowMix.push_back(strTemp);
                          } else {
                              cout << "006-Error allocating memory while mixing records. Too many combinations !" << endl;
                              return 0;
                          }
                      } catch(exception& e){
                          cout << "007-The following error occurred: " << e.what() << endl;
                          return 0;
                      }

                  } // Close 2-nd for - Chiudo 2-do for

              } // Close 1-st for - Chiudo 1-mo for

           // Devo eliminare i due record di testa da deq e sostituirli con quello calcolato deqMix
           // Remove 2 records in the head from deque and replace with the deqMix just calculated
           // .... remove - elimino
           deq.pop_front();
           deq.pop_front();

           // .... insert - inserisco
           try{
                if(verifyAvailableMemory()){
                 deq.push_front(rowMix);
                } else {
                    cout << "008-Error allocating memory while creating record. Too many combinations !" << endl;
                    return 0;
                }

           } catch (exception& e){
                 cout << "009-The following error occurred: " << e.what() << endl;
                 return 0;
           }

           //Stampo solo se FLAG_COMMENTS = true
           //Print just if FLAG_COMMENTS is true
           if (FLAG_COMMENTS){
            cout << "Counter = " << deq.size() << endl;
           }

          } // End while cycle - Fine ciclo while

      } catch (exception& e) {
           cout << "010-The following error occurred: " << e.what() << endl;
           return 0;
      }

      // Stampo il vettore mischiato
      // Print the mixed record
      try{
          for(unsigned long long i=0; i<deq.size(); ++i){
              for (unsigned long long j=0; j<deq[0].size(); ++j){
                  // Incremento il contatore delle combinazioni...
                  // Update combinations counter
                  count++;
                  cout  << deq[0][j] << endl;
              }
           }
      } catch (exception& e){
          cout << "011-The following error occurred: " << e.what() << endl;
          return 0;
      }

      //Stampo il numero di combinazioni totali
      //Print the total combinations number
      //Stampo solo se FLAG_COMMENTS = true
      //Print just if FLAG_COMMENTS is true
      if (FLAG_COMMENTS){
        cout << "# of combinations: " << count <<endl;
      }

      //Controllo se il numero di combinazioni e zero, il file è vuoto
      //If combinations number is 0 the file could be empty
      if(count==0){
          cout << "Mhhh.. strange! # of combinations = 0; should the input file be empty ?" << endl;
      }


    } // End file open control - Fine controllo apertura file

    else cout << "012-Unable to open file: " << FILE_NAME << endl;
    return 0;

}

//*****************************************
// Auxiliary functions
//*****************************************

// Verify if available memory exist...
bool verifyAvailableMemory(){
    int *pi = new (nothrow) int[16384];
    if(pi == NULL)
    {
       cout << "V1-Could not allocate memory" << endl;
       return false;
    }
    delete pi;
    return true;
}

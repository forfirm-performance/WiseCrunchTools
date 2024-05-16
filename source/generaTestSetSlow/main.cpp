#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include <iomanip>      // std::setprecision
#include <map>

using namespace std;

int main(int argc, char* argv[])
{

	//Controllo il parametri
    //Verify parameters' number
    string FORBIDDEN_nple_filename="";
	string WISE_filename=""; // = argv[1];
	string TESTSET_filename=""; // = argv[2];
	unsigned int WISE=0; // = atoi(argv[3]);
    string SEP = ";";

    try{
        for(int n=1;n<=argc-1;++n){

            string option(argv[n]);
            // WISE filename
            if(option.substr(0,3)=="-w:"){
                string valore(argv[n+1]);
                WISE_filename = valore; //option.substr(3,string::npos).c_str();
            }
            // TestSet filename
            if(option.substr(0,3)=="-t:"){
                string valore(argv[n+1]);
                TESTSET_filename = valore; //option.substr(3,string::npos).c_str();
            }
            // WISE number
            if(option.substr(0,3)=="-o:"){
                string valore(argv[n+1]);
                WISE =  atoi(valore.c_str());  //atoi(option.substr(3,string::npos).c_str());
            }
            // Separator
            if(option.substr(0,3)=="-s:"){
                string valore(argv[n+1]);
                SEP = valore;  //option.substr(3,1).c_str();
            }
            // Forbidden nple file
            if(option.substr(0,3)=="-f:"){
                string valore(argv[n+1]);
                FORBIDDEN_nple_filename = valore; //option.substr(3,string::npos).c_str();
            }
        }
    } catch (exception& e){
            cout << endl;
            cout << "Error passing input parameters !" << endl;
            cout << endl;
            cout << "Usage:" << endl;
            cout << "      generaTestSet -w: <WISE_filename> -t: <TESTSET_filename> -o: <WISE_coverage> -s: (SEP) -f: (FROM_TESTSET_filename)" << endl;
            cout << endl;
            cout << "MANDATORY -w:    WISE file name" << endl;
            cout << "MANDATORY -t:    TEST SET file name" << endl;
            cout << "MANDATORY -o:    WISE number test coverage" << endl;
            cout << "OPTIONAL  -s:    SEPARATOR fields file. The default is ;" << endl;
            cout << "OPTIONAL  -f:    FROM Test Set file name. Test set which nple will be excluded from the test set generated" << endl;
            cout << endl;
            cout << "More details on README file" << endl;
            cout << endl;
              return 0;
    }


    //Controllo il numero dei parametri
    //Verify parameters' number
    if(WISE_filename=="" || TESTSET_filename=="" || WISE==0){
        cout << endl;
        cout << "Usage:" << endl;
        cout << "      generaTestSet -w: <WISE_filename> -t: <TESTSET_filename> -o: <WISE_coverage> -s: (SEP) -f: (FROM_TESTSET_filename)" << endl;
        cout << endl;
        cout << "MANDATORY -w:    WISE file name" << endl;
        cout << "MANDATORY -t:    TEST SET file name" << endl;
        cout << "MANDATORY -o:    WISE number test coverage" << endl;
        cout << "OPTIONAL  -s:    SEPARATOR fields file. The default is ;" << endl;
        cout << "OPTIONAL  -f:    FROM Test Set file name. Test set which nple will be excluded from the test set generated" << endl;
        cout << endl;
        cout << "More details on README file" << endl;
        cout << endl;
        return 0;
	}

    // Definizione delle variabili
    long num_riga_wise = 0;                 // Numero di riga in lettura del file WISE
    long num_riga_ts = 0;                   // Numero di riga in lettura del file TESTSET
    std::string line,readTSLine,item;


    std::vector<string> rWise;          // record in lettura file Wise
    std::vector<string> rTestSet;       // record in lettura file TestSet

    map <long, std::string> contenutoRigaTS;        //Vettore con riga del TestSet (tutte le righe)
    map <long, std::string> contenutoRigaTSPrint;   //Vettore con riga del TestSet da stampare
    map <long,std::vector<long> > npleTrovate;      //Vettore con tutte le nple trovate (numero riga del file del WISE)
    std::vector<long> npleVietate;                  // Nple vietate
    long curr_max=0;                                // Utilizzate per il calcolo del massimo numero di nple trovate
    long massimo=0;
    long ts_row_max=0;

    //Auxiliary function definition
    bool verifyAvailableMemory();


	//Load npleVietate se ho il file con le n-ple vietate
    if(FORBIDDEN_nple_filename!="") {
        ifstream myfileF(FORBIDDEN_nple_filename.c_str());

        if(myfileF.fail()) {
            cout << "001-Error reading file " << FORBIDDEN_nple_filename << endl;
            return 0;
        }

        npleVietate.clear();
        if (myfileF.is_open())
        {
              while ( myfileF.good() )
              {
                    getline (myfileF,line);
                    if(!line.empty()){
                        try{
                                if(verifyAvailableMemory()){
                                    //Preload npleVietate
                                    npleVietate.push_back(atol(line.c_str()));
                                } else {
                                    cout << "002-Error allocating memory while loding npleVietate!" << endl;
                                    return 0;
                                }

                        }catch (exception& e){
                          cout << "003-The following error occurred: " << e.what() << endl;
                          return 0;
                        }

                    } //Close if line not empty on FORBIDDEN_nple_filename
              } // Close while file good on FORBIDDEN_nple_filename
              // Close FORBIDDEN_nple_filename
              myfileF.close();
        } // Close file open on FORBIDDEN_nple_filename
    } // Close if argc=5


   //Input file del TESTSET
    ifstream myfileT(TESTSET_filename.c_str());

    // Verify if file is valid
    if(myfileT.fail()) {
        cout << "004-Error reading file " << TESTSET_filename << endl;
        return 0;
    }

    if (myfileT.is_open())
    {
      while ( myfileT.good() )
      {
          // Leggo la nuova linea dal file e la carico nello stringstream
          // Read new line and load into the stringstream
          getline (myfileT,line);

          // Se la linea non è vuota...gli spazi vengono considerati validi
          // If line not empty... (spaces are considered valid)
          if(!line.empty()){

            //Salvo la linea letta per poi ristamparla alla fine con il numero di occorrenze trovate
            readTSLine = line;

            //Ripulisco il contatore delle n-ple trovate sulla riga
            //cont_nple_trovate=0;

            //Incremento il contatore di riga TESTSET
            num_riga_ts++;

            stringstream ss(line);

            // La linea del file deve terminare con un enter...
            // File line must finish with enter...
            rTestSet.clear();
            try{
                while(getline(ss,item,SEP.c_str()[0])){
                    if(verifyAvailableMemory()){
                        //Create rTestSet
                        rTestSet.push_back(item);
                    } else {
                        cout << "005-Error allocating memory while reading file. File too big or not correctly formatted !" << endl;
                        return 0;
                    }
                }
            }catch (exception& e){
              cout << "006-The following error occurred: " << e.what() << endl;
              return 0;
            }


            // ***********************************************************
            // DI QUI IN GIU' APRO IL FILE WISE PER I CONFRONTI
            // ***********************************************************

                //Input file del WISE
                ifstream myfileW(WISE_filename.c_str());

                // Verify if file is valid
                if(myfileW.fail()) {
                    cout << "007-Error reading file " << WISE_filename << endl;
                    return 0;
                }

                if (myfileW.is_open())
                {

                  //Numero della riga del file WISE che contiene la npla trovata
                  num_riga_wise=0;

                  while ( myfileW.good() )
                  {
                      // Leggo la nuova linea dal file e la carico nello stringstream
                      // Read new line and load into the stringstream
                      getline (myfileW,line);

                      // Se la linea non è vuota...gli spazi vengono considerati validi
                      // If line not empty... (spaces are considered valid)
                      if(!line.empty()){

                        stringstream ss(line);

                        //Incremento il contatore delle righe lette
                        num_riga_wise++;

                        // La linea del file deve terminare con un enter...
                        // File line must finish with enter...
                        rWise.clear();
                        try{
                            while(getline(ss,item,SEP.c_str()[0])){
                                if(verifyAvailableMemory()){
                                    //Create rWise
                                    rWise.push_back(item);
                                } else {
                                    cout << "008-Error allocating memory while reading file. File too big or not correctly formatted !" << endl;
                                    return 0;
                                }
                            }
                        }catch (exception& e){
                          cout << "009-The following error occurred: " << e.what() << endl;
                          return 0;
                        }

                        // *********************************************************
                        // Inizio dei confronti di copertura
                        // *********************************************************
                        // Gli array wise e testset hanno lo stesso numero di elementi (pr definizione)
                        // confronto gli elementi uno ad uno e se li trovo uguali, aumento il contatore
                        int i = -1;
                        unsigned int cont=0;
                        for( std::vector<string>::const_iterator j = rWise.begin(); j != rWise.end(); ++j){
                            i++;
                            if(rWise[i]==rTestSet[i] && rTestSet[i]!="*"){
                                cont++;
                            }
                        } // Fine ciclo for sul controllo campo a campo di rTestSet e rWise

                        // Se cont = WISE allora ho trovato la n-pla giusta ed inserisco in npleTrovate e contenutoRigaTS
                        if(cont==WISE){
                            //Inserisco in npleTrovate
                            try{
                                if(verifyAvailableMemory()){
                                    //Inserisco il dettaglio delle nple trovate e nella contenutoRigaTS (riga del file di TestSet)
                                    contenutoRigaTS[num_riga_ts]=readTSLine;
                                    npleTrovate[num_riga_ts].push_back(num_riga_wise);
                                } else {
                                    cout << "010-Error allocating memory while creatig npleTrovateDett !" << endl;
                                    return 0;
                                }

                            } catch (exception& e){
                                    cout << "011-The following error occurred: " << e.what() << endl;
                                    return 0;
                            }
                            //cont_nple_trovate++;
                        }

                      } // Close empty line WISE- Chiusura line vuota


                  } // Close while WISE - Chiudo while

                  // Close file  WISE- Chiusura file
                  myfileW.close();

                } // Close if WISE file is open - Chiudo if su file open

          } // Close empty line TESTSET- Chiusura line vuota

      } // Close while TESTSET - Chiudo while

      // Close file  TESTSET- Chiusura file
      myfileT.close();


    } // Close if TESTSET file is open - Chiudo if su file open

    // **********************************************************************
    // Elaborazione e calcolo delle npleTrovate e vietate
    // **********************************************************************
    bool bolEsci=false;
    std::vector<long>::iterator it;
    std::vector<long> v;
    try{
    while(!bolEsci){
       massimo=0;
       curr_max=0;
       ts_row_max=0;
       for( std::map<long,std::vector<long> >::const_iterator k = npleTrovate.begin(); k != npleTrovate.end(); ++k){
                //k->first: riga del testset
                //k->second: vettore delle nple

                //Elimino da npleTrovate[ts_row_max] quelle gia trovate
                v.clear();
                std::sort(npleTrovate[k->first].begin(),npleTrovate[k->first].end());
                std::set_difference (npleTrovate[k->first].begin(), npleTrovate[k->first].end(), npleVietate.begin(),npleVietate.end(), std::inserter(v, v.begin()));
                //v.resize(it-v.begin());
                npleTrovate[k->first]=v;

                // Calcolo del valore massimo
                curr_max = npleTrovate[k->first].size();
                if(massimo<curr_max){
                    massimo=curr_max;
                    ts_row_max = k->first;
                }

        }


        // Carico il contatore delle npleVietate con le nple trovate e le sorto
        try{
            if(verifyAvailableMemory()){
                npleVietate.insert( npleVietate.end(), npleTrovate[ts_row_max].begin(), npleTrovate[ts_row_max].end() );
            } else {
                cout << "012-Error allocating memory while creatig npleTrovateDett !" << endl;
                return 0;
            }
        } catch (exception& e){
            cout << "013-The following error occurred: " << e.what() << endl;
            return 0;
        }

        //Sorto e rendo unique gli elelenti di npleVietate
        std::sort(npleVietate.begin(),npleVietate.end());
        set<long> temp_s( npleVietate.begin(), npleVietate.end() );
        npleVietate.assign( temp_s.begin(), temp_s.end() );

        // Metto il tutto nella mappa finale che poi stampo, ottenendo una serie ordinata sulla riga
        // La mappa contenutoRigaTSPrint contiene solo le righe da stampare, mentre contenutoRigaTS contiene
        // tutte le righe
        if(massimo!=0){
            contenutoRigaTSPrint[ts_row_max]=contenutoRigaTS[ts_row_max];
        } else {
            bolEsci=true;
        }

    } // Fine ciclo while

    //Stampo le righe di contenutoRigaTSPrint
    // COUT: stampo riga + valore
    // CERR: stampo solo valore, da passare a runCC per il calcolo copertuta (da BAT DOS)
    for( std::map<long,std::string>::const_iterator k = contenutoRigaTSPrint.begin(); k != contenutoRigaTSPrint.end(); ++k){
            cout << k->first << "\t" << k->second << endl;
            cerr << k->second  << endl;
    }

    }  catch (exception& e){
            cout << "014-The following error occurred: " << e.what() << endl;
            return 0;
    }

    //Termine programma
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

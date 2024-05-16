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

    std::string WISE_filename = ""; //argv[1];
    std::string  TESTSET_filename = ""; //argv[2];
	unsigned int WISE = 0; //atoi(argv[3]);
	std::string  SEP = ";";
	// Flag for printout
	bool bolOnlyNple = false;
	bool bolOnlyDetails = false;
	bool bolOnlyFrequency = false;
	bool bolOnlyStatistica = false;
	bool bolOnlyRecordWithOccurrenceFound = false;


    try{
        for(int n=1;n<=argc-1;++n){

            string option(argv[n]);
            //cout << "argv[" << n << "]" << argv[n] << endl;

            // WISE filename
            if(option.substr(0,3)=="-w:"){ //OK
                string valore(argv[n+1]);
                WISE_filename = valore;
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
            // Print MATCHED NPLE
            if(option.substr(0,2)=="-n"){
                bolOnlyNple=true;
            }
            // Print DETAILS
            if(option.substr(0,2)=="-d"){
                bolOnlyDetails=true;
            }
            // Print FREQUENCY MAP
            if(option.substr(0,2)=="-f"){
                bolOnlyFrequency=true;
            }
            // Print STATISTIC
            if(option.substr(0,2)=="-r"){
                bolOnlyStatistica=true;
            }
            // Print RECORD + OCCURENCE FOUND for each record
            if(option.substr(0,2)=="-z"){
                bolOnlyRecordWithOccurrenceFound=true;
            }
        }
    } catch (exception& e){
            cout << endl;
            cout << "Error passing input parameters !" << endl;
            cout << endl;
            cout << "Usage:" << endl;
            cout << "      calcolaCopertura -w: <WISE_filename> -t: <TESTSET_filename> -o: <WISE_coverage> -s: (SEP) -n" << endl;
            cout << endl;
            cout << "MANDATORY -w:    WISE file name" << endl;
            cout << "MANDATORY -t:    TEST SET file name" << endl;
            cout << "MANDATORY -o:    WISE number test coverage" << endl;
            cout << "OPTIONAL  -s:    SEPARATOR fields file. The default is ;" << endl;
            cout << "OPTIONAL  -n     Print out matched nple" << endl;
            cout << "OPTIONAL  -d     Print out details (#TestSet, #Wise)" << endl;
            cout << "OPTIONAL  -f     Print out Frequency Map" << endl;
            cout << "OPTIONAL  -r     Print out Statistic" << endl;
            cout << "OPTIONAL  -z     Print out TestSet record with number of matched nple" << endl;
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
            cout << "      calcolaCopertura -w: <WISE_filename> -t: <TESTSET_filename> -o: <WISE_coverage> -s:(SEP) -n" << endl;
            cout << endl;
            cout << "MANDATORY -w:    WISE file name" << endl;
            cout << "MANDATORY -t:    TEST SET file name" << endl;
            cout << "MANDATORY -o:    WISE number test coverage" << endl;
            cout << "OPTIONAL  -s:    SEPARATOR fields file. The default is ;" << endl;
            cout << "OPTIONAL  -n     Print out matched nple" << endl;
            cout << "OPTIONAL  -d     Print out details (#TestSet, #Wise)" << endl;
            cout << "OPTIONAL  -f     Print out Frequency Map" << endl;
            cout << "OPTIONAL  -r     Print out Statistic" << endl;
            cout << "OPTIONAL  -z     Print out TestSet record with number of matched nple" << endl;
            cout << endl;
            cout << "More details on README file" << endl;
            cout << endl;
            return 0;
	}

    // record lettura file
    std::vector<string> rWise;
    std::vector<string> rTestSet;

    //Vettore con tutte le nple trovate (numero riga del file del WISE)
    long num_riga_wise = 0;                 // Numero di riga in lettura del file WISE
    long num_riga_ts = 0;                   // Numero di riga in lettura del file TESTSET
    std::vector<string> npleTrovateDett;    // Elenco delle nple trovate+riga TestSet
    //std::vector<long> npleTrovateRigaWise;      // Elenco delle nple trovate (solo riga file WISE)
    map<long,int> npleTrovateRigaWise;
    long max_wise_lines = 0;                // Massimo numero di linee del file WISE


    std::string line,readTSLine,item;
    int cont_nple_trovate = 0;      // Cotatore delle n-ple trovate per la singola riga

    //Auxiliary function definition
    bool verifyAvailableMemory();

   //Input file del TESTSET
    ifstream myfileT(TESTSET_filename.c_str());

    //Verify if file is good
    if(myfileT.fail()) {
        cout << "001-Error reading file " << TESTSET_filename << endl;
        return 0;
    }

    if (myfileT.is_open())
    {

      while ( myfileT.good() )
      {
          // Leggo la nuova linea dal file e la carico nello stringstream
          // Read new line and load into the stringstream
          getline (myfileT,line);
          //Salvo la linea letta per poi ristamparla alla fine con il numero di occorrenze trovate
          readTSLine = line;

          // Se la linea non è vuota...gli spazi vengono considerati validi
          // If line not empty... (spaces are considered valid)
          if(!line.empty()){

            //Ripulisco il contatore delle n-ple trovate sulla riga
            cont_nple_trovate=0;

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
                        cout << "002-Error allocating memory while reading file. File too big or not correctly formatted !" << endl;
                        return 0;
                    }
                }
            }catch (exception& e){
              cout << "003-The following error occurred: " << e.what() << endl;
              return 0;
            }


            // ***********************************************************
            // DI QUI IN GIU' APRO IL FILE WISE PER I CONFRONTI
            // ***********************************************************

                //Input file del WISE
                ifstream myfileW(WISE_filename.c_str());

                if(myfileW.fail()) {
                    cout << "004-Error reading file " << WISE_filename << endl;
                    return 0;
                }

                if (myfileW.is_open())
                {

                  //Numero della riga del file WISE che contiene la npla trovata
                  num_riga_wise=0;

                  //Verify if file is good
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
                                    cout << "005-Error allocating memory while reading file. File too big or not correctly formatted !" << endl;
                                    return 0;
                                }
                            }
                        }catch (exception& e){
                          cout << "006-The following error occurred: " << e.what() << endl;
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

                        // Se cont = WISE allora ho trovato la n-pla giusta ed incremento il contatore cont_nple_trovate
                        if(cont==WISE){
                            //Inserisco la riga letta dal file WISE in npleTrovate
                            string temp;
                            //Converto num_riga_ts
                            std::ostringstream sst;
                            sst << num_riga_ts;
                            temp = sst.str();
                            //Pulisco lo stringstream
                            sst.str(std::string());
                            //Converto num_riga_wise
                            sst << num_riga_wise;
                            //Concateno i due valori
                            temp = temp + SEP.c_str()[0] + sst.str();

                            //Inserisco in npleTrovate
                            try{
                                if(verifyAvailableMemory()){
                                    //Inserisco il dettaglio delle nple trovate
                                    npleTrovateDett.push_back(temp);
                                } else {
                                    cout << "007-Error allocating memory while creatig npleTrovateDett !" << endl;
                                    return 0;
                                }
                                if(verifyAvailableMemory()){
                                    //Inserisco la riga wise
                                    npleTrovateRigaWise[num_riga_wise] += 1;
                                } else {
                                    cout << "008-Error allocating memory while creatig npleTrovateRigaWise !" << endl;
                                    return 0;
                                }
                            } catch (exception& e){
                                    cout << "009-The following error occurred: " << e.what() << endl;
                                    return 0;
                            }
                            cont_nple_trovate++;
                        }

                      } // Close empty line WISE- Chiusura line vuota


                  } // Close while WISE - Chiudo while

                  // Close file  WISE- Chiusura file
                  myfileW.close();
                  //Stampo il record elaborato di TestSet, aggiungendo il numero  di occorrenze trovate
                  // solo se bolOnlyRecordWithOccurrenceFound=true
                  if(bolOnlyRecordWithOccurrenceFound){
                    cout << readTSLine << SEP.c_str()[0] << cont_nple_trovate << endl;
                  }


                } // Close if WISE file is open - Chiudo if su file open

                //Salvo il numero max di WISE line
                max_wise_lines = num_riga_wise;

            // ********************************
            // Fine file WISE
            // ********************************


          } // Close empty line TESTSET- Chiusura line vuota

      } // Close while TESTSET - Chiudo while

      // Close file  TESTSET- Chiusura file
      myfileT.close();


    } // Close if TESTSET file is open - Chiudo if su file open


     //Solo nel caso in cui ho il parametro bolOnlyNple stampo solo le n-ple trovate ed esco
     //"##################################################"
     //"                S O L O  L E  NPLE                "
     // "##################################################"
    if(bolOnlyNple){
        for( std::map<long,int>::const_iterator k = npleTrovateRigaWise.begin(); k != npleTrovateRigaWise.end(); ++k){
                cout << k->first << endl;
        }
    } // End if bolOnlyNple

    //Stampo il vettore npleTrovate con tutti i dettagli
    if(bolOnlyDetails){
        cout << "##################################################" << endl;
        cout << "                D E T A I L S                     " << endl;
        cout << "##################################################" << endl;
        cout << "#row TestSet" << SEP.c_str()[0] <<  "#row WISE" << endl;
        for( std::vector<string>::const_iterator k = npleTrovateDett.begin(); k != npleTrovateDett.end(); ++k){
                cout << *k << endl;
        }
    } // Close if bolOnlyDetails


    //Frequency map
    if(bolOnlyFrequency){
        cout << "##################################################" << endl;
        cout << "          F R E Q U E N C Y   M A P              " << endl;
        cout << "##################################################" << endl;
        cout << "#row WISE" << SEP.c_str()[0] <<  "Frequency" << endl;

        //Stampo anche le righe con copertura zero
        long cop_zero=1;
        for( std::map<long,int>::const_iterator k = npleTrovateRigaWise.begin(); k != npleTrovateRigaWise.end(); ++k){
                // iterator->first = key
                // iterator->second = value
                for (long kk=cop_zero;kk<k->first;++kk){
                    cout << cop_zero << SEP.c_str()[0] << 0 << endl;
                    cop_zero++;
                }

                cout << k->first << SEP.c_str()[0] << k->second << endl;
                cop_zero++;
        }
        //Stampo anche le ultime n-ple del wise (che se non sono state stampate prima sono a zero)
        for (long kk=cop_zero;kk<=max_wise_lines;++kk){
            cout << kk << SEP.c_str()[0] << 0 << endl;
            cop_zero++;
        }
    } // Close if bolOnlyFrequency


    if(bolOnlyStatistica){
        // Calcolo la percentuale totale coperta
        cout << "##################################################" << endl;
        cout << "                C O V E R A G E                  " << endl;
        cout << "##################################################" << endl;
        if(max_wise_lines!=0){
            cout << "Wise level                  : " << WISE << endl;
            cout << "Total number of nple        : " << max_wise_lines << endl;
            cout << "Total number of covered nple: " << npleTrovateRigaWise.size() << endl;
            cout << "Coverage percentage (%):      " << std::setprecision(5) << (double) (npleTrovateRigaWise.size()*100)/max_wise_lines << "%" <<endl;
        }
    } // Close bolOnlyStatistica

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

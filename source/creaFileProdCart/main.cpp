#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

int main( int argc, char* argv[])
{

    //Controllo il numero dei parametri
    //Verify parameters' number

	std::string var_file_name = "";
	std::string comb_file_name = "";
	std::string prefix_out_file_name = "";

	try{
        for(int n=1;n<=argc-1;++n){

            string option(argv[n]);

            // FILENAME_var filename
            if(option.substr(0,3)=="-i:"){
                string valore(argv[n+1]);
                var_file_name = valore;
            }
            // FILENAME_comb filename
            if(option.substr(0,3)=="-c:"){
                string valore(argv[n+1]);
                comb_file_name = valore;
            }
            // PREFIX_out_file string
            if(option.substr(0,3)=="-p:"){
                string valore(argv[n+1]);
                prefix_out_file_name =  valore.c_str();
            }
        }
    } catch (exception& e){
            cout << endl;
            cout << "Error passing input parameters !" << endl;
            cout << endl;
            cout << "Usage:" << endl;
            cout << "      creaFileProdCart -i: <FILENAME_var> -c: <FILENAME_comb> -p: <PREFIX_out_file>" << endl;
            cout << endl;
            cout << "MANDATORY -i:    Input variables/values file name" << endl;
            cout << "MANDATORY -c:    Combination file name" << endl;
            cout << "MANDATORY -p:    The path+prefix of the output file" << endl;
            cout << endl;
            cout << "More details on README file" << endl;
            cout << endl;
            return 0;
    }



    if(var_file_name=="" || comb_file_name=="" || prefix_out_file_name==""){
            cout << endl;
            cout << "Usage:" << endl;
            cout << "      creaFileProdCart -i: <FILENAME_var> -c: <FILENAME_comb> -p: <PREFIX_out_file>" << endl;
            cout << endl;
            cout << "MANDATORY -i:    Input variables/values file name" << endl;
            cout << "MANDATORY -c:    Combination file name" << endl;
            cout << "MANDATORY -p:    The path+prefix of the output file" << endl;
            cout << endl;
            cout << "More details on README file" << endl;
            cout << endl;
            return 0;
        return 0;
	}

	//Read files name
	//char* var_file_name = argv[1];
	//char* comb_file_name = argv[2];
	//std::string prefix_out_file_name = argv[3];

    //User variables - variabili usate
    string line,item;
    //Riga e header sul file variabili - header and row on variable file
    vector<string> header;
    vector<string> row;
    vector<string> comb_var;
    stringstream stmp;      //create a stringstream di appoggio

    //Separatore nome/valori delle variabili sul file Variabili - separator variables name/values on Variable file
    char SEP = ':';
    char SEP_C = ' ';

   //Definizione funzioni ausiliarie
    //Auxiliary function definition
    bool verifyAvailableMemory();
    bool WriteOutFile(std::string, std::string);

    //Input file delle VARIABILI (file di input principale, scritto a mano)
    ifstream myfile(var_file_name.c_str());
    if (myfile.is_open())
    {
      while ( myfile.good() )
      {

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
                int i = 1; //Counter used to decide to write on header or row; if i=0 --> header; i=1 --> row
                while(getline(ss,item,SEP)){
                    if(verifyAvailableMemory()){
                        //Creare header and row - Creo header e row
                        if(i==1){
                            header.push_back(item);
                        }
                        if(i==2){
                            row.push_back(item);
                        }
                    } else {
                        cout << "001-Error allocating memory while reading file. File too big or not correctly formatted !" << endl;
                        return 0;
                    }
                    i++;
                }
            }catch (exception& e){
              cout << "002-The following error occurred: " << e.what() << endl;
              return 0;
            }

          } // Close empty line - Chiusura line vuota

      } // Close while - Chiudo while

      // Close file - Chiusura file
      myfile.close();


    } // Close if file is open - Chiudo if su file open


    string cont_file; // Contenuto del file di output di ProdCart
    int conta_numero_file;
    //Input file delle COMBINAZIONI (file generato da Combinazioni_n_k)
    myfile.open(comb_file_name.c_str(),std::ifstream::in);
    conta_numero_file = 0;

    if (myfile.is_open())
    {
      while ( myfile.good() )
      {

          // Leggo la nuova linea dal file e la carico nello stringstream
          // Read new line and load into the stringstream
          getline (myfile,line);

          // Se la linea non è vuota...gli spazi vengono considerati validi
          // If line not empty... (spaces are considered valid)
          if(!line.empty()){

            conta_numero_file++;
            stringstream ss(line);

            // La linea del file deve terminare con un enter...
            // File line must finish with enter...
            try{
                while(getline(ss,item,SEP_C)){
                    if(verifyAvailableMemory()){
                        //Creare header and row - Creo header e row
                            comb_var.push_back(item);
                    } else {
                        cout << "003-Error allocating memory while reading file. File too big or not correctly formatted !" << endl;
                        return 0;
                    }

                }
            }catch (exception& e){
              cout << "004-The following error occurred: " << e.what() << endl;
              return 0;
            }



            // Dopo aver registrato i valori della singola riga sul vettore comb_var, lo confronto con header;
            // Se trovo i valori li stampo... (uso > per mandarlo su file)
            int k;
            bool found;
            cont_file="";
            k=0;
            for( std::vector<string>::const_iterator i = header.begin(); i != header.end(); ++i){
                //Inizializzo le due variabili
                found = false;
                for( std::vector<string>::const_iterator j = comb_var.begin(); j != comb_var.end(); ++j){
                    //Trovo la corrispondenza, stampo la riga row dei valori
                    if (*i == *j){
                        cont_file = cont_file + row[k] + "\n";
                        found = true;
                        break;
                    }

                } //Chiude for su combinazioni
                if(found==false){
                    cont_file = cont_file + "*" + "\n";
                }
                //Incremento il contatore di riga su header
                k++;
            } //Chiude for su header


            //Pulisco comb_var
            comb_var.clear();


          } // Close empty line - Chiusura line vuota

            stmp.clear();  //Pulisco lo stream e stato di errore (http://stackoverflow.com/questions/20731/in-c-how-do-you-clear-a-stringstream-variable)
            stmp.str(std::string());
            stmp << conta_numero_file;         //add number to the stream

            //Stampo sul file
            if(!WriteOutFile(cont_file, prefix_out_file_name + stmp.str() + ".txt")){
                return 0;
            };

      } // Close while - Chiudo while

      // Close file - Chiusura file
      myfile.close();
      return 0;

    } // Close if file is open - Chiudo if su file open


} //Close main - chiudo main

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

// Scrive i file di output
bool WriteOutFile(string strIn, string outNomeFile){
    try{
    std::ofstream out(outNomeFile.c_str());
    out << strIn;
    out.close();
    } catch (exception& e){
        cout << "W1-The following error occurred: " << e.what() << endl;
        return false;
    }
    return true;

}


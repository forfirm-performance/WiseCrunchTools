#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include <iomanip>      // std::setprecision
#include <map>
#include <iterator>

using namespace std;

int main(int argc, char* argv[])
{

	//Controllo il parametri
    //Verify parameters' number
    string NPLE_filename="";
    string SEP = ";";

    try{
        for(int n=1;n<=argc-1;++n){

            string option(argv[n]);
            // NPLE filename
            if(option.substr(0,3)=="-n:"){
                string valore(argv[n+1]);
                NPLE_filename = valore; //option.substr(3,string::npos).c_str();
            }

            // Separator
            if(option.substr(0,3)=="-s:"){
                string valore(argv[n+1]);
                SEP = valore;  //option.substr(3,1).c_str();
            }
        }
    } catch (exception& e){
            cout << endl;
            cout << "Error passing input parameters !" << endl;
            cout << endl;
            cout << "Usage:" << endl;
            cout << "      generaTestSetFast -n: <NPLE_filename> -s: (SEP)" << endl;
            cout << endl;
            cout << "MANDATORY -n:    NPLE file name" << endl;
            cout << "OPTIONAL  -s:    SEPARATOR fields file. The default is ;" << endl;
            cout << endl;
            cout << "More details on README file" << endl;
            cout << endl;
            return 0;
    }


    //Controllo il numero dei parametri
    //Verify parameters' number
    if(NPLE_filename==""){
        cout << endl;
        cout << "Usage:" << endl;
        cout << "      generaTestSetFast -n: <NPLE_filename> -s: (SEP)" << endl;
        cout << endl;
        cout << "MANDATORY -n:    NPLE file name" << endl;
        cout << "OPTIONAL  -s:    SEPARATOR fields file. The default is ;" << endl;
        cout << endl;
        cout << "More details on README file" << endl;
        cout << endl;
        return 0;
	}

    // Definizione delle variabili
    long num_riga_nple = 0;         // Numero di riga in lettura del file NPLE
    std::string line, item;         //Usate nella lettura del file

    // Record in lettura file NPLE
    map <long,std::vector<string> > npleTotali; //NPLE Totali
    vector<string> rastrello;                   //Il rastrella-nple
    vector<string> fieno;                       //Le nple da rastrellare (u fen)
    vector<long> nple_elaborate;              //Le nple già elaborate


    //Auxiliary function definition
    bool verifyAvailableMemory();
    void concatena(std::vector<string>&, std::string&, std::string&);


	//Load npleVietate se ho il file con le n-ple totali
    ifstream myfileN(NPLE_filename.c_str());

    if(myfileN.fail()) {
        cout << "001-Error reading file " << NPLE_filename << endl;
        return 0;
    }

    npleTotali.clear();
    if (myfileN.is_open())
    {
          while ( myfileN.good() )
          {
                // Leggo la linea
                getline (myfileN,line);

                if(!line.empty()){

                        // String stream sulla linea del file
                        stringstream ss(line);

                        // La linea del file deve terminare con un enter...
                        // File line must finish with enter...
                        try{
                            while(getline(ss,item,SEP.c_str()[0])){
                                if(verifyAvailableMemory()){
                                    //Create rTestSet
                                    npleTotali[num_riga_nple].push_back(item);
                                } else {
                                    cout << "005-Error allocating memory while reading file. File too big or not correctly formatted !" << endl;
                                    return 0;
                                }
                            }
                        }catch (exception& e){
                          cout << "006-The following error occurred: " << e.what() << endl;
                          return 0;
                        }

                    //Incremento il contatore num_riga_nple
                    num_riga_nple++;

                } //Close if line not empty on NPLE_filename

          } // Close while file good on NPLE_filename
          // Close NPLE_filename
          myfileN.close();

    } // Close file open on NPLE_filename


       //Definizioni di partenza
       long cont_ester = 0;      //Contatore ciclo esterno  - NON LI USO
       long cont_inter = 0;      //Contatore ciclo interno  - NON LI USO
       rastrello.clear();        // Pulisco il rastrello
       nple_elaborate.clear();   // Pulisco nple elaborate

        //Primo ciclo sulla mappa
       for( std::map<long, std::vector<string> >::const_iterator ester = npleTotali.begin(); ester != npleTotali.end(); ++ester){

            //Imposto il rastrello (DA VEDERE SE CLEAR PRIMA)
            rastrello = npleTotali[ester->first];
            cont_ester++;

           // Verifico che non sia tra le n-ple gia elaborate
           if(std::find(nple_elaborate.begin(), nple_elaborate.end(), ester->first) == nple_elaborate.end()) {

                   cont_inter=0;
                    // Secondo ciclo sulla mappa
                   for( std::map<long, std::vector<string> >::const_iterator inter = npleTotali.begin(); inter != npleTotali.end(); ++inter){

                        // Rastrellamento dei valori: definisco il fieno
                        fieno = npleTotali[inter->first];
                        cont_inter++;

                        // Verifico che non sia tra le n-ple gia elaborate
                        if(std::find(nple_elaborate.begin(), nple_elaborate.end(), inter->first) == nple_elaborate.end()) {


                                // Verifico la compatibilità tra rastello e fieno
                                int i=-1;
                                bool ok = true;
                                for( std::vector<string>::const_iterator ras = rastrello.begin(); ras != rastrello.end(); ++ras){

                                    //Incremento il contatore. Rastrello e fieno hanno lo stesso size.
                                    i++;

                                    //A. Verifico il KO
                                    if (rastrello[i]!="*" &&  fieno[i]!="*" && rastrello[i]!=fieno[i] ){
                                       ok=false;
                                       break;
                                    }

                                    //B. Verifico l'OK
                                    if ((rastrello[i]=="*" &&  fieno[i]!="*") || (rastrello[i]!="*" &&  fieno[i]=="*") || rastrello[i]==fieno[i] ){
                                       ok=true;
                                    }

                                } // Fine ciclo for di controllo di compatibilità


                                // Mergio gli elementi, se OK. Rastrello e fieno hanno lo stesso size.
                                i=-1;
                                vector<string> temp_v;
                                if(ok) {
                                        for( std::vector<string>::const_iterator ras = rastrello.begin(); ras != rastrello.end(); ++ras){

                                            //Incremento il contatore
                                            i++;

                                            // Effettuo il merge
                                            if(rastrello[i]!="*"){
                                                 temp_v.push_back(rastrello[i]);
                                            } else if (fieno[i]!="*"){
                                                 temp_v.push_back(fieno[i]);
                                            } else {
                                                 temp_v.push_back("*");
                                            }


                                            //Logica di rastrellamento
                                            // Rastrello: A * C D *
                                            // Fieno    : A B * E *
                                            // Essendo D diverso da E, non rastrella; negli altri casi si

//                                            //1. Elementi uguali (A=A o *=*)
//                                            if(rastrello[i]==fieno[i]){
//                                               temp_v.push_back(rastrello[i]);
//                                            }
//
//                                            //2. Uno dei due = * e l'altro diverso da *
//                                            if(rastrello[i]=="*" && fieno[i]!="*"){
//                                               temp_v.push_back(fieno[i]);
//                                            }
//                                            if(rastrello[i]!="*" && fieno[i]=="*"){
//                                               temp_v.push_back(rastrello[i]);
//                                            }
//
//                                            //3. Diversi tra di loro e diversi entrambi da * - non rastrellano, cancello il vettore ed esco dal for
//                                            if(rastrello[i]!="*" && fieno[i]!="*" && rastrello[i]!=fieno[i]){
//                                                //cout << "rastrello[i]=" << rastrello[i] << " fieno[i]=" << fieno[i] << endl;
//                                                temp_v.clear();
//                                                break;
//                                            }

                                        } //Fine ciclo merge for sul vettore rastrello e fieno

                                } // Fine if su ok e cont

                                //cout << "size=" << temp_v.size() << endl;

                                // Se il temp_v non è vuoto (quindi ho rastrellato), aggiungo il fieno al rastrello
                                // ed elimino le nple rastrellate
                                if(temp_v.size()!=0){
                                    rastrello = temp_v;
                                    //cout <<  "Interno = " << inter->first <<  " size = " <<  temp_v.size() << endl;
                                    nple_elaborate.push_back(inter->first);

                                }

                        } // Fine ciclo di controllo sul secondo ciclo (interno) mappa delle nple_elaborate

                    } //Fine secondo (interno) ciclo sulla mappa


                    string s_npla="";
                    concatena(rastrello,SEP,s_npla);
                    //cout <<  ester->first  << " " <<  s_npla << endl;
                    cout <<  s_npla << endl;

                    // INSERIRE UNIQUE SU nple_elaborate

               } // Fine ciclo di controllo sul primo ciclo mappa delle nple_elaborate

           } // Fine del primo ciclo (esterno) sulla mappa

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

// Concatena gli elementi di un vettore
void concatena(std::vector<string> &vect, std::string &sep, std::string &s){
   std::ostringstream oss;
   for( std::vector<string>::const_iterator n = vect.begin(); n != vect.end(); ++n){
            oss << *n  << sep ;
   }
   // Modifico la stringa s ed elimino il SEP finale
   s = oss.str().substr(0,oss.str().size()-1);
}

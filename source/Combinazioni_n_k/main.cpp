#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

// Trova le combinazioni
template <typename Iterator>
bool next_combination(const Iterator first, Iterator k, const Iterator last)
{
   /* Credits: Mark Nelson http://marknelson.us */
   if ((first == last) || (first == k) || (last == k))
      return false;
   Iterator i1 = first;
   Iterator i2 = last;
   ++i1;
   if (last == i1)
      return false;
   i1 = last;
   --i1;
   i1 = k;
   --i2;
   while (first != i1)
   {
      if (*--i1 < *i2)
      {
         Iterator j = k;
         while (!(*i1 < *j)) ++j;
         std::iter_swap(i1,j);
         ++i1;
         ++j;
         i2 = k;
         std::rotate(i1,j,last);
         while (last != j)
         {
            ++j;
            ++i2;
         }
         std::rotate(k,i2,last);
         return true;
      }
   }
   std::rotate(first,k,last);
   return false;
}

// Aggiunge il carattere 'cart' tra le parole
std::string addChar(std::string ss_in, char cart){
    std::string ss_out = "";
    std::vector<char> ss_vec;
    for(std::string::iterator i=ss_in.begin();i!=ss_in.end();++i){
        ss_vec.push_back(*i);
        ss_vec.push_back(cart);
    }
    ss_out = std::string(ss_vec.data(), ss_vec.size());
    return ss_out;

}

// Verifica se il numero in input è un intero
inline bool isInteger(const std::string & s)
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

// Verifica se ci sono caratteri duplicati nella stringa
bool IsDuplicatesCharPresent(std::string stringToVerify)
{
//Stringa vuota
if(stringToVerify.size() == 0 ){
    return false;
}
// Sorto la stringa
std::sort(stringToVerify.begin(), stringToVerify.end());

    // I primi 2 chr che trovo, se uguali ritorno false
    for(std::string::iterator i=stringToVerify.begin();i!=stringToVerify.end();++i){
        // Se non sono alla fine..
        if(i!=stringToVerify.end()){
            if(*i == *(i+1)){
                return true;
                std::cout << *i << std::endl;
                std::cout << *i+1 << std::endl;
            }
        }
    }
    return false;
}

// Funzione main. Accetta in input la stringa (primo parametro) ed il numero di combinazioni (secondo parametro)
int main(int argc, char* argv[])
{


    std::string s = ""; //argv[1];
    std::string str_comb_size="";
    std::size_t comb_size = 0;

    //Read passed values
    try{
            for(int n=1;n<=argc-1;++n){

                string option(argv[n]);

                // Input string
                if(option.substr(0,3)=="-s:"){
                        string valore(argv[n+1]);
                        s = valore;
                }
                // Input k value
                if(option.substr(0,3)=="-k:"){
                        string valore(argv[n+1]);
                        str_comb_size=valore;
                } // Close if option.substr = -k:

            } // Close for on argc
    } catch (exception& e){
            cout << endl;
            cout << "Error passing input parameters !" << endl;
            cout << endl;
            cout << "Usage:" << endl;
            cout << "      Combinazioni_n_k -s: <STRING> -k: <K value>" << endl;
            cout << "MANDATORY -s:    String" << endl;
            cout << "MANDATORY -k:    Number of k-combinazions (combinations k at k)" << endl;
            cout << endl;
            cout << "More details on README file" << endl;
            cout << endl;
            return 0;
    }

    // General error message
    if (s=="" || str_comb_size=="") {
        cout << endl;
        cout << "Usage:" << endl;
        cout << "      Combinazioni_n_k -s: <STRING> -k: <K value>" << endl;
        cout << "MANDATORY -s:    String" << endl;
        cout << "MANDATORY -k:    Number of k-combinazions (combinations k at k)" << endl;
        cout << endl;
        cout << "More details on README file" << endl;
        cout << endl;
        return 0;
    }

    // Verifico se nella stringa argv[n+1] ho un intero
    if(isInteger(str_comb_size)){
        comb_size = atoi(str_comb_size.c_str());
        //Controllo il numero di combinazioni
        if(comb_size<0 || (unsigned int) comb_size> s.size()){
            std::cout << std::endl;
            std::cout << "Second parameter range not valid; it must be a integer number between 0 and the Length of the input string" << std::endl;
            std::cout << std::endl;
            return 0;
        }
    } else {
        std::cout << std::endl;
        std::cout << "Second parameter must be a integer number between 0 and the Length of the input string. For example: Combinazioni_nk ABCDEFG 3" << std::endl;
        std::cout << std::endl;
        return 0;
    }

    // Verifico se nella stringa s non ci sono caratteri duplicati
    if(IsDuplicatesCharPresent(s)){
        std::cout << std::endl;
        std::cout << "First parameter must be a string without duplicated chars. For example, ABCDEFG is a valid string, while AABBCC is invalid" << std::endl;
        std::cout << std::endl;
        return 1;
    }





    // Ciclo di stampa dei valori
    do
    {
        std::cout << addChar(std::string(s.begin(),s.begin() + comb_size),' ') << std::endl;
    }
    while(next_combination(s.begin(),s.begin() + comb_size,s.end()));
    return 0;
}

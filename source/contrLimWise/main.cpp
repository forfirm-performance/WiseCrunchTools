#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[])
{
    int wise = 0;
    int wise_max = 0;
    if(argc>1){
        wise = atoi(argv[1]);
        wise_max = atoi(argv[2]);
        if(wise>0 && wise<=wise_max){
            cout << "ok" << endl;
        } else {
            cout << "ko" << endl;
        }
    } else {
        cout << "ko" << endl;
    }
    return 0;
}

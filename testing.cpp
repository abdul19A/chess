#include "states.h"

int main(){
    std::string s = "rnbq1bn1/ppp1pk1r/7p/3pPppP/8/5N2/PPPPKPP1/RNBQ1B1R w - g6 0 7";
    ChessState cs(s);
    std::cout << sizeof(cs) << std::endl;
    cs.print();
    return 0;
}
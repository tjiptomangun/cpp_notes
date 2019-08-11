/**
 * g++ -Wall -ggdb3 play_ref.cpp -o play_ref 
 */
#include <iostream>
class play_ref{
public:
    play_ref();

    int v1;
   
    /**
     * functions that returns reference
     */ 
    play_ref& operator = (const int& i1);
    play_ref& operator += (const int& i1);
    /**
     * functions doing the same but returns void
     */ 
    void operator - (const int& i1);
    void operator / (const int& i1);

    /**
     * returning void or returning reference does not 
     * have any side effect to the object.
     * the different is that by returning by referfence
     * returned value can be passed to a variable and
     * and that variable has now the reference to this object
     */

    friend std::ostream& operator<<(std::ostream &out, const play_ref& the_ref);
};
play_ref::play_ref(){
    v1 = 0;
}

play_ref& play_ref::operator= (const int& i1){
    v1 = i1;
    return *this;
}

play_ref& play_ref::operator+= (const int& i1){
    v1 = v1 + i1;
    return *this;
}

void play_ref::operator -(const int& i1) {
    v1 = i1; 
}

void play_ref::operator / (const int& i1) {
    v1 = v1 + i1; 
}

std::ostream& operator<<(std::ostream &out, const play_ref& the_ref){
    out << the_ref.v1 << '\n';
    return out;
}


extern std::ostream cout;
int main (int argc, char **argv){
    play_ref* pr = new play_ref();
    play_ref* kr = new play_ref();
    std::cout << *pr;
    *pr = 10; 
    std::cout << *pr;
    *pr+= 21; 
    std::cout << *pr;


    std::cout << *kr;
    *kr - 10;
    std::cout << *kr;
    *kr / 21; 
    std::cout << *kr;
    
    
    //*pr+= 4 = 5;
    //*pr=4 += 5;

    return 0;
}

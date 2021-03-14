#include <iostream>
#include "md5.h"

using std::cout;

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        cout << "No arguments were provided\n";
        return 1;
    }

    string input(argv[1]);
    cout << md5::create_hash(input) << '\n';
    return 0;
}

#include <iostream>
#include <string>

using namespace std;

int main()
{

    string a = "a|A|B|C|D|E|F|G|H|I|J|K|L|M|N|O|P|Q|R|S|T|U|V|W|X|Y|Z|";
    string z = "z|";
    string e = a.substr(0,10);
    cout<<e<<z<<endl;


   for(int i = 0; i< 10; i++)
    {
        e = a.substr(i,i+2);
        cout<<e<<endl;

        for(int j= 0; j< 10; j++ )
        {

         cout << 4 ;

        }
        cout << "\n"<< endl;
    }

    return 0;
}


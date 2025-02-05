#include"game.h"
#include<string>
#include<iostream>
using namespace std;

int main(int argc, char* argv[])
{
    Game game;
    string path = "map.dat";
    if(argc>1) path = argv[1];
    if( game.Init(path) )
    {
        do{
            cout << game;
            game.Action();
        }while( !game.End() );
        cout << "Thanks for your playing, have a nice day!\n";
        return 0;
    }
    else
    {
        cout << "An error occurs. Please check for input or map.dat file.\n";
        return -1;
    }
}

#include<iostream>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include"player.h"
#include"location.h"
#include"game.h"
using namespace std;

int Game::Init(const string &s)
{
    cout.flags(ios::right);
    ifstream ifile;
    ifile.open(s);
    if(!ifile.is_open())return 0;

    cout << "How many players want to play the game?(2-4)...>" ;
    string num;
    getline(cin,num);
    if(num[0]>= '2' && num[0] <= '4')num_players_ = num[0]-'0';
    else return 0;
    for(int i=0; i<num_players_; ++i)
    {
        string name;
        cout << "Please input player " << i+1 << " 's name(default: "
            << DEFAULT_NAME[i] << " )...>";
        getline(cin,name);
        if(name=="")name = DEFAULT_NAME[i];
        players_.AddPlayer(new Player(i,name));
    }
    LocationBase::SetNumOfPlyers(num_players_);

    //manipulate map location
    char type;
    while( ifile >> type )
    {
        string name;
        ifile >> name;
        LocationBase* p = nullptr;
        if( type=='U' )
        {
            int price, upgradePrice, unit_fine[Upgradeable::MAX_LEVEL];
            ifile >> price >> upgradePrice;
            for(int i=0; i<Upgradeable::MAX_LEVEL; ++i)ifile >> unit_fine[i];
            p = new Upgradeable(num_locations_,name,price,upgradePrice,unit_fine);
        }
        else if( type=='C' || type =='R' )
        {
            int price, unit_fine;
            ifile >> price >> unit_fine;
            if(type=='C')p = new Collectable(num_locations_,name,price,unit_fine);
            else p = new RandomCost(num_locations_,name,price,unit_fine);
        }
        else if( type=='J' )
        {
            p = new Jail(num_locations_,name);
        }
        locations_.AddLocation(p);
        num_locations_ += 1;
    }
    ifile.close();
    Player::SetNumOfLocations(num_locations_);
    srand(time(0));
    return 1;
}
void Game::Action()
{
    string action;
    Player& p = players_[players_.GetCurrentPlayer()];
    locations_[p.GetCurrentLocation()]->Leave(p.GetId());

    cout << p.GetName()
        << ", your action? (1:Dice [default] / 2:Exit)...>";
    getline(cin,action);
    if( action=="2" )players_.RemovePlayer();
    else
    {
        int steps = rand()%6+1;
        p.Move(steps);
        locations_[p.GetCurrentLocation()]->Come(p.GetId());
        cout << *this ;
        locations_[p.GetCurrentLocation()]->Interact(p);

        if(p.IsBankrupt())
        {
            locations_[p.GetCurrentLocation()]->Leave(p.GetId());
            players_.RemovePlayer();
        }
    }
    players_.NextPlayer();
    system("pause");
}
bool Game::End() const
{
    bool rValue = false;
    if( players_.Winner() != -1 )
    {
        rValue = true;
        cout << "The Winner Is " << players_[0].GetName() << " !\n";
    }
    return rValue;
}
ostream& operator << (ostream& os, const Game& g)
{
    system("cls");
    os << g.locations_ << '\n' << g.players_ << '\n';
    return os;
}

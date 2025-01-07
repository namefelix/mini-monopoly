#include<string>
#include<iostream>
#include<iomanip>
#include<cstdlib>
#include"player.h"
#include"location.h"
using namespace std;

int LocationBase::num_players_;
LocationBase::LocationBase(int id, const string &name):id_(id),name_(name)
{
    if( id==0 )
    {
        for(int i=0; i<num_players_; ++i)
            player_is_here_[i] = 1;
    }
}
Upgradeable::Upgradeable(int id, const string &name, int price, int upgradePrice, int* fine):
    Buyable(id,name,price),level_up_cost_(upgradePrice),level_(0)
{
    for(int i=0;i<MAX_LEVEL;++i)unit_fine_[i]=fine[i];
}

void LocationBase::Display(ostream& os)const
{
    os << "=";
    for(int i=0; i<LocationBase::num_players_; ++i)
    {
        if(player_is_here_[i])os << i;
        else os << ' ';
    }
    os << "=" ;
    os << " [" << setw(2) << id_ << "] "  << setw(10) << name_;
}
void Jail::Display(ostream& os)const
{
    LocationBase::Display(os);
    os << setw(18) << "";
}
void Buyable::Display(ostream& os)const
{
    LocationBase::Display(os);
    os << ' ';
    if(host_!=nullptr) os << '{' << host_->GetId() << '}';
    else os << "   ";
}
void Upgradeable::Display(ostream& os) const
{
    Buyable::Display(os);
    if(host_==nullptr) os << " B$ " << setw(5) << price_ << setw(5) << "";
    else if(level_==MAX_LEVEL-1) os << " L" << MAX_LEVEL << setw(11) << "";
    else os << " U$ " << setw(5) << level_up_cost_ << " L" << level_+1 << setw(2) << "";
}
void RandomCost::Display(ostream& os) const
{
    Buyable::Display(os);
    if(host_==nullptr) os << " B$ " << setw(5) << price_ << setw(5) << "";
    else os << " ?" << setw(12) << "";
}
int Collectable::collected_data_[4];
void Collectable::Display(ostream& os) const
{
    Buyable::Display(os);
    if(host_==nullptr) os << " B$ " << setw(5) << price_ << setw(5) << "";
    else os << " x" << collected_data_[host_->GetId()] << setw(11) << "";
}

void Jail::Interact(Player& p){ p.Jail(); }
void Buyable::Interact(Player& p)
{
    cout << p.GetName() << ", do you want to buy " << name_ << " ? (1:Yes[default] / 2:No)...>";
    string reply;
    getline(cin,reply);
    if(reply[0]!='2') p.Buy(this);
}
void Upgradeable::Interact(Player& p)
{
    if(host_==nullptr)Buyable::Interact(p);
    else if(host_ == &p && level_!=MAX_LEVEL-1)
    {
        cout << p.GetName() << ", do you want to upgrade " << name_ << " ? (1:Yes[default] / 2:No)...>";
        string reply;
        getline(cin,reply);
        if(reply[0]!='2') p.Upgrade(this);
    }
    else if(host_ != &p)host_->RecieveMoney(p, unit_fine_[level_]);
}
void RandomCost::Interact(Player& p)
{
    if(host_==nullptr)Buyable::Interact(p);
    else if(host_ != &p)host_->RecieveMoney(p, (rand()%6+1)*unit_fine_);
}
void Collectable::Interact(Player& p)
{
    if(host_==nullptr)
    {
        Buyable::Interact(p);
        if(host_!=nullptr)collected_data_[p.GetId()]+=1;
    }
    else if(host_ != &p)host_->RecieveMoney(p, collected_data_[host_->GetId()]*unit_fine_);
}

void Upgradeable::Free()
{
    Buyable::Free();
    level_ = 0;
}

ostream& operator << (std::ostream& os, const Locations& locations)
{
    int num = locations.num_locations_;
    for(int i=0; i<num/2; ++i)
    {
        locations[i]->Display(os);
        locations[num-1-i]->Display(os);
        os << endl;
    }
    if(num%2==1)locations[num/2]->Display(os);
    os << endl;
    return os;
}

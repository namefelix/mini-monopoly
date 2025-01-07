#include"player.h"
#include"location.h"
#include<iostream>
#include<iomanip>
#include<cstdlib>
using namespace std;

int Player::num_location_;
void Player::Move(int step)
{
    if(in_jail)
    {
        cout << name_ << ", you can't move because of imprisoning.\n";
        system("pause");
    }
    else
    {
        cout << "You rolled " << step << "!\n";
        for(int i=0; i<step; ++i)
        {
            current_location_ += 1;
            if( current_location_ == num_location_ )
            {
                current_location_ = 0;
                *this += reward_;
                cout << name_ << ", you passed the starting location and earned $" << reward_ << "!\n";
            }
        }
        system("pause");
    }
}
void Player::RecieveMoney(Player& p2, int amount)
{
    cout << p2.name_ << ", you have to pay " << name_ << " $" << amount << ".\n";
    *this += amount;
    p2 -= amount;
}
void Player::Buy(Buyable* b)
{
    int cost = b->GetPrice();
    if( money_ < cost )cout << name_ << ", you can't buy this location.\n";
    else
    {
        cout << "You paid $" << cost << " and bought " << b->GetName() << ".\n";
        b->SetHost(this);
        location_owned_.push_back(b);
        num_location_owned_ += 1;
        money_ -= cost;
    }
}
void Player::Upgrade(Upgradeable* u)
{
    int cost = u->GetLevelUpCost();
    if( money_ < cost )cout << name_ << ", you can't upgrade this location.\n";
    else
    {
        cout << "You paid $" << cost << " and upgraded " << u->GetName() << ".\n";
        u->LevelUp();
        money_ -= cost;
    }
}
void Player::Lose()
{
    cout << name_ << " is bankrupt!\n";
    for(int i=0; i<num_location_owned_; ++i)
        location_owned_[i]->Free();
}
ostream& operator << (ostream& os,const Player& p)
{
    os << '[' << p.id_ << ']' << setw(16) << p.name_ << " $"<< p.money_
        << " with " << p.num_location_owned_ << " units\n";
    return os;
}

int Players::AddPlayer(Player* p)
{
    if( num_players_==MAX_PLAYERS )return -1;
    num_players_ += 1;
    game_players_.push_back(p);
    return 0;
}
void Players::RemovePlayer()
{
    game_players_[current_player_]->Lose();
    delete game_players_[current_player_];
    num_players_ -= 1;
    game_players_.erase(game_players_.begin()+current_player_);
    current_player_ -= 1;
}
int Players::Winner()const
{
    if( num_players_!=1 )return -1;
    else return game_players_[0]->GetId();
}
ostream& operator << (ostream& os, const Players& p)
{
    for(int i=0; i<p.num_players_; ++i)
    {
        if (i==p.current_player_ )os << "=>";
        else os << "  ";
        os << p[i];
    }
    return os;
}

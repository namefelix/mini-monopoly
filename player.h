#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include<string>
#include<iosfwd>
#include<vector>

class LocationBase;
class Buyable;
class Upgradeable;

constexpr char DEFAULT_NAME[4][15] = {"A-Tu", "Little-Mei", "King-Baby", "Mrs.Money"};

class Player
{
    friend std::ostream& operator << (std::ostream&, const Player&);

public:
    Player( int id, const std::string &name):id_(id), name_(name), location_owned_(std::vector<LocationBase*>()){}

    void Move(int);
    void RecieveMoney(Player&,int);
    void Buy(Buyable*);
    void Upgrade(Upgradeable*);
    void Lose();

    int GetId()const{ return id_; }
    const std::string& GetName() const { return name_; }
    int GetCurrentLocation() const { return current_location_; }
    bool IsBankrupt() const { return money_ < 0; }
    void Jail(){ in_jail = !in_jail; }
    static void SetNumOfLocations(int i){ num_location_ = i; }

private:
    void operator += (int amount){ money_ += amount; }
    void operator -= (int amount){ money_ -= amount; }

    constexpr static int reward_ = 0;
    static int num_location_ ;
    const int id_ ;
    const std::string name_ = "A-Tu";
    int money_ = 30000;
    int current_location_ = 0;
    int num_location_owned_ = 0;
    std::vector <LocationBase*> location_owned_;
    bool in_jail = false;
};

class Players
{
    friend std::ostream& operator << (std::ostream&, const Players&);

public:
    constexpr static int MAX_PLAYERS = 4;

    Players() = default;
    Players(const Players&) = delete;
    Players& operator = (const Players&) = delete;
    ~Players(){ for(int i=0; i<num_players_; ++i) delete game_players_[i]; }

    int AddPlayer(Player*);
    void RemovePlayer();
    int Winner() const;

    void NextPlayer(){ current_player_+=1; current_player_%=num_players_; }
    const Player& operator [] (int index)const { return *(game_players_[index]); }
    Player& operator[] (int index){ return const_cast<Player &>( ( (static_cast<const Players &>(*this))[index]) ); }
    int GetCurrentPlayer() const { return current_player_; }

private:
    int num_players_ = 0;
    std::vector <Player*> game_players_ = std::vector <Player*>();
    int current_player_ = 0;
};

#endif // PLAYER_H_INCLUDED

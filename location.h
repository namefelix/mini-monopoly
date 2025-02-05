#ifndef LOCATION_H_INCLUDED
#define LOCATION_H_INCLUDED

#include<string>
#include<vector>
#include<iosfwd>
class Player;
constexpr int MAX_PLAYER = 4;

class LocationBase
{
public:
    LocationBase() = default;
    LocationBase(int, const std::string&);
    virtual ~LocationBase(){};
    virtual void Display(std::ostream&)const;
    virtual void Interact(Player&) = 0;
    virtual void Free() = 0;

    const std::string& GetName() const { return name_; }
    void Leave(int index){ player_is_here_[index] = false; }
    void Come(int index){ player_is_here_[index] = true; }
    static void SetNumOfPlyers(int i){ num_players_ = i; }

private:
    int id_ = 0;
    bool player_is_here_[MAX_PLAYER] = {};

protected:
    static int num_players_ ;
    std::string name_ = "";
};

class Jail final : public LocationBase
{
public:
    Jail(int id, std::string& name): LocationBase(id,name){};
    virtual void Display(std::ostream&)const override;
    virtual void Interact(Player& p) override;
    virtual void Free()override{}
};

class Buyable : public LocationBase
{
public:
    Buyable (const Buyable&) = delete;
    Buyable(int id, const std::string &name, int price):LocationBase(id,name),price_(price) {};
    virtual void Display(std::ostream&) const override;
    virtual void Interact(Player&) override;
    virtual int Price() const = 0;
    virtual void Free() override { host_ = nullptr; }

    int GetPrice() const { return price_; }
    void SetHost(Player* p){ host_ = p; }

protected:
    int price_ = 0;
    Player* host_ = nullptr;
};

class Upgradeable final: public Buyable
{
public:
    constexpr static int MAX_LEVEL = 5;
    Upgradeable(int id, const std::string&, int price, int upgradePrice, const int* fine);

    virtual void Display(std::ostream&) const override;
    virtual void Interact(Player&) override;
    virtual int Price() const override
    {
        if(host_==nullptr)return price_;
        else return level_up_cost_;
    }
    virtual void Free() override;

    int GetLevelUpCost() const { return level_up_cost_; }
    void LevelUp(){ level_ += 1; }

private:
    int level_up_cost_ = 0;
    int unit_fine_[MAX_LEVEL] = {};
    int level_;
};

class RandomCost final: public Buyable
{
public:
    RandomCost(int id, const std::string &name, int price, int unit_fine):
        Buyable(id,name,price),unit_fine_(unit_fine){}

    virtual void Display(std::ostream&) const override;
    virtual void Interact(Player&) override;
    virtual int Price() const override { return price_; }

private:
    int unit_fine_ = 0;
};

class Collectable final: public Buyable
{
public:
    Collectable(int id, const std::string &name, int price, int unit_fine):
        Buyable(id,name,price),unit_fine_(unit_fine){}

    virtual void Display(std::ostream&) const override;
    virtual void Interact(Player&) override;
    virtual int Price() const override { return price_; }

private:
    static int collected_data_[MAX_PLAYER];
    int unit_fine_ = 0;
};

class Locations
{
    friend std::ostream& operator << (std::ostream&, const Locations &);

public:
    Loactions(const Locations&) = delete;
    void AddLocation(LocationBase* p) { game_locations_.push_back(p); num_locations_+=1; }
    ~Locations(){ for(int i=0; i<num_locations_; ++i)delete game_locations_[i]; }

    const LocationBase* operator [] (int index) const { return game_locations_[index]; }
    LocationBase* operator[] (int index){ return const_cast< LocationBase* >( ( (static_cast<const Locations &>(*this))[index]) ); }

private:
    int num_locations_ = 0;
    std::vector <LocationBase*> game_locations_ = std::vector <LocationBase*> ();
};


#endif // LOCATION_H_INCLUDED

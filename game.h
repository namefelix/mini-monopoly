#ifndef GANE_H_INCLUDED
#define GANE_H_INCLUDED

#include"player.h"
#include"location.h"
#include<iosfwd>
#include<string>

class Game
{
    friend std::ostream& operator << (std::ostream&, const Game&);

public:
    Game() = default;
    int Init(const std::string&);
    void Action();
    bool End() const;

private:
    Players players_ = Players();
    Locations locations_ = Locations();
    int num_players_ = 1;
    int num_locations_ = 0;
};

#endif // GANE_H_INCLUDED

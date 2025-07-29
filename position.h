#ifndef __POSITION_H__
#define __POSITION_H__
#include<iostream>
#include<string>
#include<vector>
class Position
{
private:
    std::vector<std::string> poss;

    void init(int pn);
    void adjust(int pn, int di);


public:
    Position(int pn = 3, int di = 1);
    int find(const std::string&) const;

    std::string operator[](int pi) const;

    std::ostream& output(std::ostream& out) const;
};

std::ostream &operator<<(std::ostream &out, const Position &p);
#endif
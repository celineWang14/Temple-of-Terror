//
//  weapon.hpp
//  CS32 Project3
//
//  Created by 王楠楠 on 2024/5/11.
//

#ifndef weapon_h
#define weapon_h
#include <string>

class object{
public:
    object(const std::string& name, const int&row, const int&col):m_name(name),mRow(row),mCol(col){};
    std::string getName()const;
    int getRow()const;
    int getCol()const;
private:
    int mRow;
    int mCol;
    std::string m_name;
};


class weapon:public object{
public:
    weapon(const std::string&w,const int&row, const int&col);
    int getBonus()const;
    int getDamage()const;
    std::string getAction()const;
private:
    int dexBonus;
    int damageAmount;
    std::string actionString;
};

class scroll:public object{
public:
    scroll(const std::string&s,const int&row, const int&col):object(s,row,col){}
};

#endif /* weapon_hpp */

//
//  Actor.hpp
//  CS32 Project3
//
//  Created by 王楠楠 on 2024/5/11.
//

#ifndef Actor_h
#define Actor_h
#include <string>
#include "object.h"
#include "utilities.h"
#include <vector>
struct objectIF{
    int row;
    int col;
    char name;
};

class Actor{
public:
    Actor(const int& row,const int& col);
    //attack the enemy
    void attack(Actor&other);
    virtual void setHP(const int& a);
    //make the enemy asleep
    void makeSleep(const int&a);
    void awake();
    std::string getName(){return m_name;}
    bool isAlive()const;
    weapon* getWeapon()const{return m_weapon;}
    bool isAwake()const;
    bool getAttackResult()const;
    bool getSleepSuccess()const;
    void setPosition(const int&a, const int&b);
    int getRow()const{return m_row;}
    int getCol()const{return m_col;}
    int getHP()const{return m_hp;}
    virtual ~Actor();

protected:
    int m_row;int m_col;
    int m_hp;int m_ap;int m_sp;int m_dp;int m_sleept;
    std::string m_name;
    weapon* m_weapon;
    bool attackResult=false;
    bool sleepSuccess=false;
};



class player:public Actor{
public:
    player(const int&row,const int& col);
    virtual void setHP(const int&a);
    std::string getWeapon()const;
    void cheat();
    //set the default values
    void setAP(const int&a);
    void setSP(const int&a);
    void setDP(const int&a);
    void setMaxhp(const int&h);
    //change weapon
    void setWeapon(const std::string&newWeapon);
    void findObject(const std::string&a);
    void eraseObject(const size_t&pos);
    size_t inventorySize()const;
    std::string objectName(const size_t&a)const;
    int getHP()const{return m_hp;};
    int getAP()const{return m_ap;};
    int getSP()const{return m_sp;};
    int getDP()const{return m_dp;};
    
private:
    int m_maxHP;
    //store the objects found
    std::vector<std::string> m_inventory;
};

class monster:public Actor{
public:
    monster(const int&row, const int&col, const std::string& name);
    void setPreobject(const int&row, const int&col, const char&name);
    char getOBname()const;
    int getOBrow()const;
    int getOBcol()const;
    void deleteOB();
    void setPreIF(const bool& s);
    bool getPre()const;
    //ensure there is no memory leak
    virtual ~monster();
    
private:
    objectIF* pre_object=nullptr;
    bool preObject=false;
};



#endif /* Actor_hpp */


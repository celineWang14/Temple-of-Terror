//
//  Actor.cpp
//  CS32 Project3
//
//  Created by 王楠楠 on 2024/5/11.
//

#include "Actor.h"
Actor::Actor(const int& row,const int& col){
    //default weapon is short sword
    m_weapon=new weapon("short sword",0,0);
    m_row=row;
    m_col=col;
    m_hp=0;
    m_ap=0;
    m_sp=0;
    m_dp=0;
    m_sleept=0;
}

Actor::~Actor(){

}

bool Actor::isAlive()const{
    return m_hp>0;
}

bool Actor::getSleepSuccess()const{
    return sleepSuccess;
}

bool Actor::isAwake()const{
    return m_sleept==0;
}

bool Actor::getAttackResult()const{
    return attackResult;
}

void Actor::setPosition(const int&a, const int&b){
    m_col=b;
    m_row=a;
}

void Actor::setHP(const int& a){
    m_hp=a;
}

void Actor::attack(Actor&other){
    //caluculating my attack and defend points
    int attackP=m_dp+m_weapon->getBonus();
    int defendP=other.m_dp+other.m_ap;
    if (randInt(1, attackP) >= randInt(1, defendP)){
        int damageP=randInt(0, m_sp + m_weapon->getDamage() - 1);
        other.m_hp-=damageP;
        attackResult=true;
        //if weilding fangs of sleep, maybe can make the enemy asleep
        if (this->getWeapon()->getName()=="magic fangs of sleep") {
            if (trueWithProbability(0.2)) {
                other.makeSleep(randInt(2, 6));
                sleepSuccess=true;
            }
            else
            sleepSuccess=false;
        }
    }
    else{
        attackResult=false;
    }
}

void Actor::makeSleep(const int &a){
    if (m_sleept<a) {
        m_sleept=a;
    }
    //else if a>=sleept, do nothing
}

//every round, sleept-1
void Actor::awake(){
    if (m_sleept!=0) {
        m_sleept--;
    }
}


player::player(const int&row,const int& col):Actor(row, col){
    m_maxHP=20;
    m_hp=20;
    m_ap=2;
    m_sp=2;
    m_dp=2;
}


void player::setAP(const int&a){
    m_ap+=a;
}

void player::setSP(const int&a){
    m_sp+=a;
}

void player::setDP(const int&a){
    m_dp+=a;
}

std::string player::getWeapon()const{
    return m_weapon->getName();
}

void player::setWeapon(const std::string&newWeapon){
    delete m_weapon;
    m_weapon=new weapon(newWeapon,0,0);
}

void player::setHP(const int &a){
    if (a>=m_maxHP) {
        m_hp=m_maxHP;
    }
    else{
        m_hp=a;
    }
}

void player::setMaxhp(const int&h){
    m_maxHP+=h;
}

void player::cheat(){
    m_hp=50;
    m_maxHP=50;
    m_sp=9;
}

//add the found item to the player's inventory
void player::findObject(const std::string&a){
    m_inventory.push_back(a);
}

size_t player::inventorySize()const{
    return m_inventory.size();
}

std::string player::objectName(const size_t& a)const{
    return m_inventory.at(a);
}

//find the targetted object and erase it
void player::eraseObject(const size_t&pos){
    std::vector<std::string>::iterator it=m_inventory.begin();
    int fpos=0;
    while (it!=m_inventory.end()) {
        if (fpos==pos) {
            m_inventory.erase(it);
            break;
        }
        it++;
        fpos++;
    }
}

monster::monster(const int&row, const int&col, const std::string& name):Actor(row, col){
    m_name=name;
    if(name=="Bogeymen"){
        m_hp=randInt(5, 10);
        m_sp=randInt(2, 3);
        m_dp=randInt(2, 3);
        m_ap=2;
    }
    if(name=="Snakewoman"){
        m_hp=randInt(3, 6);
        m_sp=2;
        m_dp=3;
        m_ap=3;
        delete m_weapon;
        m_weapon=new weapon("magic fangs of sleep",0,0);
    }
    if(name=="Dragon"){
        m_hp=randInt(20, 25);
        m_sp=4;
        m_dp=4;
        m_ap=4;
        delete m_weapon;
        m_weapon=new weapon("long sword",0,0);
    }
    if(name=="Goblins"){
        m_hp=randInt(15, 20);
        m_sp=3;
        m_dp=1;
        m_ap=1;
    }
}

void monster::setPreobject(const int&row, const int&col, const char&name){
    pre_object=new objectIF{row,col,name};
}

void monster::setPreIF(const bool& s){
    preObject=s;
}

monster::~monster(){
    if (pre_object!=nullptr) {
        deleteOB();
    }
}

char monster::getOBname()const{
    return pre_object->name;
}

int monster::getOBrow()const{
    return pre_object->row;
}

int monster::getOBcol()const{
    return pre_object->col;
}

bool monster::getPre()const{
    return preObject;
}

void monster::deleteOB(){
    if(pre_object!=nullptr){
        delete pre_object;
        pre_object=nullptr;
    }
}

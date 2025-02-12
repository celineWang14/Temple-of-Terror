//
//  weapon.cpp
//  CS32 Project3
//
//  Created by 王楠楠 on 2024/5/11.
//

#include "object.h"
std::string object::getName()const{
    return m_name;
}

int object::getRow()const{
    return mRow;
}

int object::getCol()const{
    return mCol;
}

weapon::weapon(const std::string&w,const int&row, const int&col):object(w,row,col){
    if(w=="magic axe"){
        dexBonus=5;
        damageAmount=5;
        actionString=" chops magic axe at the ";
    }
    
    if(w=="long sword"){
        dexBonus=2;
        damageAmount=4;
        actionString=" swings long sword at the ";
    }
    if(w=="short sword"){
        dexBonus=0;
        damageAmount=2;
        actionString=" slashes short sword at the ";
    }
    
    if(w=="magic fangs of sleep"){
        actionString=" strikes magic fangs at the ";
        dexBonus=3;
        damageAmount=2;
    }
    if(w=="mace"){
        dexBonus=0;
        damageAmount=2;
        actionString=" swings mace at the ";
    }
}

std::string weapon::getAction()const{
    return actionString;
}

int weapon::getBonus()const{
    return dexBonus;
}

int weapon::getDamage()const{
    return damageAmount;
}

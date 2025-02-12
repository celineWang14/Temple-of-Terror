// Game.cpp

#include "Game.h"
#include <cmath>
#include <iostream>
using namespace std;

Game::Game(int goblinSmellDistance):m_player(1, 1)
{
    for (int i=0; i<18; i++) {
        for (int j=0; j<70; j++) {
            mTemple[i][j]='#';
        }
    }
    for (int i=1; i<17; i++) {
        for (int j=1; j<69; j++) {
            mTemple[i][j]=' ';
        }
    }
    goblinD=goblinSmellDistance;
    //start on level 0
    currentLevel=-1;
}

void Game::showPlayerInfo()const{
    std::cout<<"Level: "<<currentLevel <<", Hit points: "<<m_player.getHP()<<", Armor: "<<m_player.getAP()<<", Strength: "<<m_player.getSP()<<", Dexterity: "<<m_player.getDP()<<std::endl<<std::endl;
}

void Game::setPlayerMove(const std::string& info){
    m_playerRound=info;
}

void Game::showPlayerMoveInfo()const{
    std::cout<<m_playerRound<<std::endl;
}

void Game::clearMonsterMove(){
    m_monstersMove.clear();
}

void Game::play()
{
//while not end
//dispaly level (randomly generated) 0-4
    newLevel();
    displayTemple();
    showPlayerInfo();
    //when the game is not ended
    while (m_status==false){
        char a=getCharacter();
        playerMove(a);
        monstersMove();
        updateMonster();
        regainHP(m_player);
        displayTemple();
        showPlayerInfo();
        showPlayerMoveInfo();
        showMonstersMove();
        //if the player is dead
        if (!m_player.isAlive()) {
            break;
        }
        m_playerRound=" ";
        clearMonsterMove();
    }
    std::cout<<"Press q to exit game"<<std::endl;
    char b=getCharacter();
    while (b!='q') {
        b=getCharacter();
    }
    exit(0);
//display player information
//player move
//monster move
//move description
    //end if find golden idol or die
//clearscreen

}

void Game::displayTemple()const{
    for (int i = 0; i < 18; ++i) {
        for (int j = 0; j < 70; ++j) {
            std::cout << mTemple[i][j]; // Accessing m_temple in the correct order
        }
        std::cout << std::endl;
    }
}

void Game::addRooms(){
    //add rooms
    //randomly generate start and end points
    int colStart=randInt(1, 5);
    int width=randInt(5, 10);
    int colEnd=colStart+width;
    int rowStart=randInt(1, 8);
    int depth=randInt(5, 8);
    int rowEnd=rowStart+depth;
    
    for (int ii=0; ii<depth; ii++) {
        for (int jj=0; jj<width; jj++) {
            mTemple[ii+rowStart][jj+colStart]=' ';
        }
    }
    //add room information to the vector
    roomInfo newRoom{rowStart,rowEnd,colStart,colEnd};
    m_rooms.push_back(newRoom);

    for (int i=0; i<4; i++) {
        colStart+=11;
        width=randInt(5, 11);
        colEnd=colStart+width;
        rowStart=randInt(1, 8);
        depth=randInt(5, 8);
        rowEnd=rowStart+depth;
        for (int ii=0; ii<depth; ii++) {
            for (int jj=0; jj<width; jj++) {
                mTemple[ii+rowStart][jj+colStart]=' ';
            }
        }
        //add room information to the vector
        roomInfo newRoom{rowStart,rowEnd,colStart,colEnd};
        m_rooms.push_back(newRoom);
    }
}


void Game::newLevel(){
    currentLevel++;
    for (int i=0; i<18; i++) {
        for (int j=0; j<70; j++) {
            mTemple[i][j]='#';
        }
    }
    m_rooms.clear();
    addRooms();
    //add corridors

    for (size_t i=1; i<m_rooms.size(); i++) {
        int startRow=0;
        //set the startrow infomation according to the 2 rooms' sizes
        if(m_rooms[i].endRow<m_rooms[i-1].startRow){
            startRow=m_rooms[i-1].startRow+1;
        }
        else if(m_rooms[i].startRow>m_rooms[i-1].endRow){
            startRow=m_rooms[i].startRow+1;
        }
        else if (m_rooms[i].startRow>m_rooms[i-1].startRow) {
            if(m_rooms[i].endRow<m_rooms[i-1].endRow){
                startRow=randInt(m_rooms[i].startRow, m_rooms[i].endRow-1);
            }
            else
            startRow=randInt(m_rooms[i].startRow, m_rooms[i-1].endRow-1);
        }
        else if (m_rooms[i].startRow==m_rooms[i-1].startRow){
            int small=min(m_rooms[i].endRow,m_rooms[i-1].endRow);
            startRow=randInt(m_rooms[i].startRow, small-1);
        }
        else{
            if(m_rooms[i-1].endRow<m_rooms[i].endRow){
                startRow=randInt(m_rooms[i-1].startRow, m_rooms[i-1].endRow-1);
            }
            else
            startRow=randInt(m_rooms[i-1].startRow, m_rooms[i].endRow-1);
        }
        int startCol=m_rooms[i-1].endCol-1;
        int endCol=m_rooms[i].startCol+1;
        while (startCol!=endCol) {
            mTemple[startRow][startCol]=' ';
            startCol++;
        }
        //here
        //if still not connected after drawing horizontal corridors
        if(m_rooms[i].endRow<m_rooms[i-1].startRow){
            int startCol1=startCol-1;
            while(startRow!=m_rooms[i].endRow-2){
                mTemple[startRow][startCol1]=' ';
                startRow--;
            }
        }
        else if(m_rooms[i-1].endRow<m_rooms[i].startRow){
            int startCol1=m_rooms[i-1].endCol;
            while(startRow!=m_rooms[i-1].endRow-2){
                mTemple[startRow][startCol1]=' ';
                startRow--;
            }
        }
    }
    
        // Set player position
        int playerRow = randInt(1, 16);
        int playerCol = randInt(1, 68);
        while (mTemple[playerRow][playerCol] == '#') {
            playerRow = randInt(1, 16);
            playerCol = randInt(1, 68);
        }
        m_player.setPosition(playerRow, playerCol);
        mTemple[playerRow][playerCol] = '@';
        
        // Set monsters position
        int nMonsters = randInt(2, 5 * (currentLevel + 1) + 1);
        for (int i = 0; i < nMonsters; i++) {
            int monsterRow = randInt(1, 16);
            int monsterCol = randInt(1, 68);
            while (mTemple[monsterRow][monsterCol] == '#' || mTemple[monsterRow][monsterCol] == '@') {
                monsterRow = randInt(1, 16);
                monsterCol = randInt(1, 68);
            }
            //put different monsters according to the level number
            if (currentLevel<=1) {
                if (trueWithProbability(0.5)) {
                    monster newMonster(monsterRow, monsterCol, "Snakewoman");
                    mMonsters.push_back(newMonster);
                    mTemple[monsterRow][monsterCol] = 'S';
                } else {
                    monster newMonster(monsterRow, monsterCol, "Goblins");
                    mMonsters.push_back(newMonster);
                    mTemple[monsterRow][monsterCol] = 'G';
                }
            }
            if (currentLevel==2) {
                if (trueWithProbability(1.0/3.0)) {
                    monster newMonster(monsterRow, monsterCol, "Snakewoman");
                    mMonsters.push_back(newMonster);
                    mTemple[monsterRow][monsterCol] = 'S';
                } else if(trueWithProbability(1.0/2.0)){
                    monster newMonster(monsterRow, monsterCol, "Goblins");
                    mMonsters.push_back(newMonster);
                    mTemple[monsterRow][monsterCol] = 'G';
                }
                else{
                    monster newMonster(monsterRow, monsterCol, "Bogeymen");
                    mMonsters.push_back(newMonster);
                    mTemple[monsterRow][monsterCol] = 'B';
                }
            }
            if(currentLevel>=3){
                if (trueWithProbability(1.0/4.0)) {
                    monster newMonster(monsterRow, monsterCol, "Snakewoman");
                    mMonsters.push_back(newMonster);
                    mTemple[monsterRow][monsterCol] = 'S';
                } else if(trueWithProbability(1.0/3.0)){
                    monster newMonster(monsterRow, monsterCol, "Goblins");
                    mMonsters.push_back(newMonster);
                    mTemple[monsterRow][monsterCol] = 'G';
                }
                else if(trueWithProbability(1.0/2.0)){
                    monster newMonster(monsterRow, monsterCol, "Bogeymen");
                    mMonsters.push_back(newMonster);
                    mTemple[monsterRow][monsterCol] = 'B';
                }
                else{
                    monster newMonster(monsterRow, monsterCol, "Dragon");
                    mMonsters.push_back(newMonster);
                    mTemple[monsterRow][monsterCol] = 'D';
                }
            }
        }
    
    //if last level, also add the golden idol
    if(currentLevel==4){
        int idolRow = randInt(1, 16);
        int idolCol = randInt(1, 68);
        while (mTemple[idolRow][idolCol] == '#'||mTemple[idolRow][idolCol] == '?'||mTemple[idolRow][idolCol] == ')') {
            idolRow = randInt(1, 16);
            idolCol = randInt(1, 68);
        }
        mTemple[idolRow][idolCol]='&';
    }
    
    //add object
    int nobject=randInt(2, 3);
    for (int i = 0; i < nobject; i++) {
        int objectRow = randInt(1, 16);
        int objectCol = randInt(1, 68);
        while (mTemple[objectRow][objectCol]!= ' '&&mTemple[objectRow][objectCol]!= '@') {
            objectRow = randInt(1, 16);
            objectCol = randInt(1, 68);
        }
        if (trueWithProbability(0.5)) {
            if (trueWithProbability(1.0/3.0)) {
                weapon newW("long sword",objectRow,objectCol);
                m_object.push_back(newW);
                mTemple[objectRow][objectCol]=')';
            }
            else if (trueWithProbability(1.0/2.0)) {
                weapon newW("short sword",objectRow,objectCol);
                m_object.push_back(newW);
                mTemple[objectRow][objectCol]=')';
            }
            else{
                weapon newW("mace",objectRow,objectCol);
                m_object.push_back(newW);
                mTemple[objectRow][objectCol]=')';
            }
        }
        else{
            if (trueWithProbability(0.2)) {
                scroll newS("a scroll of raise strength",objectRow,objectCol);
                m_object.push_back(newS);
                mTemple[objectRow][objectCol]='?';
            }
            else if (trueWithProbability(0.25)) {
                scroll newS("a scroll of improve armor",objectRow,objectCol);
                m_object.push_back(newS);
                mTemple[objectRow][objectCol]='?';
            }
            else if (trueWithProbability(1.0/3.0)) {
                scroll newS("a scroll of raise strength",objectRow,objectCol);
                m_object.push_back(newS);
                mTemple[objectRow][objectCol]='?';
            }
            else if (trueWithProbability(1.0/2.0)) {
                scroll newS("a scroll of enhance health",objectRow,objectCol);
                m_object.push_back(newS);
                mTemple[objectRow][objectCol]='?';
            }
            else{
                scroll newS("a scroll of enhance dexterity",objectRow,objectCol);
                m_object.push_back(newS);
                mTemple[objectRow][objectCol]='?';
            }
        }
    }
    
    //add stair
    if(currentLevel!=4){
        int stairRow = randInt(1, 16);
        int stairCol = randInt(1, 68);
        while (mTemple[stairRow][stairCol]!= ' '&&mTemple[stairRow][stairCol]!= '@'&&!isMonster(stairRow, stairCol)) {
            stairRow = randInt(1, 16);
            stairCol = randInt(1, 68);
        }
        mTemple[stairRow][stairCol]='>';
    }

}

bool Game::isMonster(const int&row, const int&col)const{
    if (mTemple[row][col]=='B'||mTemple[row][col]=='S'||mTemple[row][col]=='D'||mTemple[row][col]=='G') {
        return true;
    }
    return false;
}

bool Game::isObject(const int &row, const int &col)const{
    if (mTemple[row][col]=='>'||mTemple[row][col]=='?'||mTemple[row][col]==')'||mTemple[row][col]=='&') {
        return true;
    }
    return false;
}

bool Game::isWeapon(const std::string &name)const{
    return (name=="magic axe"||name=="long sword"||name=="short sword"||name=="magic fangs of sleep"||name=="mace");
}

bool Game::Breachable(const std::string& name,const int &bRow, const int &bcol, const int &pRow, const int &pCol)const{
    //if can smell the player
    if(name=="Bogeymen"){
        int a,b;
        a=abs(bRow-pRow);
        b=abs(bcol-pCol);
        int c=a+b;
        return (c<=5);
    }
    else{
        int a,b;
        a=abs(bRow-pRow);
        b=abs(bcol-pCol);
        int c=a+b;
        return (c<=3);
    }
}


void Game::showMonstersMove(){
    std::vector<std::string>::iterator it=m_monstersMove.begin();
    while (it!=m_monstersMove.end()) {
        std::cout<<*it<<std::endl;
        it++;
    }
}

void Game::monstersMove(){
    vector<monster>::iterator it=mMonsters.begin();
    while (it!=mMonsters.end()) {
        //if the monster is not asleep
        if(it->isAwake()){
            //dragon can regain HP
            if(it->getName()=="Dragon"){
                regainHP(*it);
            }
            //all monsters near the player can attack the player
            if (abs(it->getRow()-m_player.getRow())+abs(it->getCol()-m_player.getCol())==1)
            {
                it->attack(m_player);
                std::string newMonsterMove;
                newMonsterMove=it->getName()+(it->Actor::getWeapon())->getAction();
                newMonsterMove+="player";
                if(it->getAttackResult()){
                    newMonsterMove+=" and hits";
                }
                else{
                    newMonsterMove+=" and misses";
                }
                if(it->getSleepSuccess()){
                    newMonsterMove+=", putting the player to sleep.";
                }
                //add to the infomation vector
                m_monstersMove.push_back(newMonsterMove);
                it++;
                continue;
            }
            //Bogeymen
            else if (it->getName()=="Bogeymen"||it->getName()=="Snakewoman") {
                //if next to the player
                //if 5 stpes to reach the player
                if (Breachable(it->getName(),it->getRow(), it->getCol(), m_player.getRow(), m_player.getCol())) {
                    //move according to the location respectively to the player
                    //restore if previously stepped on a object
                    if (it->getRow()>m_player.getRow()) {
                        //can step on a empty space
                        if (mTemple[it->getRow()-1][it->getCol()]==' ') {
                            if(!it->getPre())
                                mTemple[it->getRow()][it->getCol()]=' ';
                            if(it->getPre()){
                                mTemple[it->getRow()][it->getCol()]=it->getOBname();
                                it->deleteOB();
                                it->setPreIF(false);
                            }
                            it->setPosition(it->getRow()-1, it->getCol());
                            mTemple[it->getRow()][it->getCol()]=it->getName()[0];
                            it++;
                            continue;
                        }
                        //can step on an object, but remember to restore it!!!
                        else if (isObject(it->getRow()-1,it->getCol())) {
                            if(!it->getPre())
                                mTemple[it->getRow()][it->getCol()]=' ';
                            if(it->getPre()){
                                mTemple[it->getRow()][it->getCol()]=it->getOBname();
                                it->deleteOB();
                                it->setPreIF(false);
                            }
                            //store the info in theat monster's memory
                            it->setPreobject(it->getRow()-1, it->getCol(), mTemple[it->getRow()-1][it->getCol()]);
                            it->setPreIF(true);
                            //step on it
                            it->setPosition(it->getRow()-1, it->getCol());
                            mTemple[it->getRow()][it->getCol()]=it->getName()[0];
                            it++;
                            continue;
                        }
                    }
                    else if (it->getRow()<m_player.getRow()) {
                        if (mTemple[it->getRow()+1][it->getCol()]==' ') {
                            if(!it->getPre())
                                mTemple[it->getRow()][it->getCol()]=' ';
                            if(it->getPre()){
                                mTemple[it->getRow()][it->getCol()]=it->getOBname();
                                it->deleteOB();
                                it->setPreIF(false);
                            }
                            it->setPosition(it->getRow()+1, it->getCol());
                            mTemple[it->getRow()][it->getCol()]=it->getName()[0];
                            it++;
                            continue;
                        }
                        else if (isObject(it->getRow()+1,it->getCol())) {
                            if(!it->getPre())
                                mTemple[it->getRow()][it->getCol()]=' ';
                            if(it->getPre()){
                                mTemple[it->getRow()][it->getCol()]=it->getOBname();
                                it->deleteOB();
                                it->setPreIF(false);
                            }
                            //store the info in theat monster's memory
                            it->setPreobject(it->getRow()+1, it->getCol(), mTemple[it->getRow()+1][it->getCol()]);
                            it->setPreIF(true);
                            //step on it
                            it->setPosition(it->getRow()+1, it->getCol());
                            mTemple[it->getRow()][it->getCol()]=it->getName()[0];
                            it++;
                            continue;
                        }
                    }
                    if (it->getCol()>m_player.getCol()) {
                        if (mTemple[it->getRow()][it->getCol()-1]==' ') {
                            if(!it->getPre())
                                mTemple[it->getRow()][it->getCol()]=' ';
                            if(it->getPre()){
                                mTemple[it->getRow()][it->getCol()]=it->getOBname();
                                it->deleteOB();
                                it->setPreIF(false);
                            }
                            it->setPosition(it->getRow(), it->getCol()-1);
                            mTemple[it->getRow()][it->getCol()]=it->getName()[0];
                            it++;
                            continue;
                        }
                        else if (isObject(it->getRow(),it->getCol()-1)) {
                            if(!it->getPre())
                                mTemple[it->getRow()][it->getCol()]=' ';
                            if(it->getPre()){
                                mTemple[it->getRow()][it->getCol()]=it->getOBname();
                                it->deleteOB();
                                it->setPreIF(false);
                            }
                            //store the info in theat monster's memory
                            it->setPreobject(it->getRow(), it->getCol()-1, mTemple[it->getRow()][it->getCol()-1]);
                            it->setPreIF(true);
                            //step on it
                            it->setPosition(it->getRow(), it->getCol()-1);
                            mTemple[it->getRow()][it->getCol()]=it->getName()[0];
                            it++;
                            continue;
                        }
                    }
                    else if (it->getCol()<m_player.getCol()) {
                        if (mTemple[it->getRow()][it->getCol()+1]==' ') {
                            if(!it->getPre())
                                mTemple[it->getRow()][it->getCol()]=' ';
                            if(it->getPre()){
                                mTemple[it->getRow()][it->getCol()]=it->getOBname();
                                it->deleteOB();
                                it->setPreIF(false);
                            }
                            it->setPosition(it->getRow(), it->getCol()+1);
                            mTemple[it->getRow()][it->getCol()]=it->getName()[0];
                            it++;
                            continue;
                        }
                        else if (isObject(it->getRow(),it->getCol()+1)) {
                            if(!it->getPre())
                                mTemple[it->getRow()][it->getCol()]=' ';
                            if(it->getPre()){
                                mTemple[it->getRow()][it->getCol()]=it->getOBname();
                                it->deleteOB();
                                it->setPreIF(false);
                            }
                            //store the info in that monster's memory
                            it->setPreobject(it->getRow(), it->getCol()+1, mTemple[it->getRow()][it->getCol()+1]);
                            it->setPreIF(true);
                            //step on it
                            it->setPosition(it->getRow(), it->getCol()+1);
                            mTemple[it->getRow()][it->getCol()]=it->getName()[0];
                            it++;
                            continue;
                        }
                    }
                }
            }

            else if (it->getName()=="Goblins") {
                if(abs(it->getRow()-m_player.getRow())+abs(it->getCol()-m_player.getCol())<goblinD){
                    //if next to the player
                    //if 15 stpes to reach the player
                    int step=0;
                    int nextstepX=0;
                    int nextstepY=0;
                    vector<vector<bool>> visited;
                    visited.resize(18, vector<bool>(70));
                    vector<int>firststep;
                    int min1=-1,min2=-1,min3=-1,min4=-1;
                    //store each path's shortest steps
                    findShortestPath(mTemple, visited, it->getRow()+1, it->getCol(), m_player.getRow(), m_player.getCol(), min1, step);
                    visited.assign(18, std::vector<bool>(70, false));
                    findShortestPath(mTemple, visited, it->getRow(), it->getCol()+1, m_player.getRow(), m_player.getCol(), min2, step);
                    visited.assign(18, std::vector<bool>(70, false));
                    findShortestPath(mTemple, visited, it->getRow(), it->getCol()-1, m_player.getRow(), m_player.getCol(), min3, step);
                    visited.assign(18, std::vector<bool>(70, false));
                    findShortestPath(mTemple, visited, it->getRow()-1, it->getCol(), m_player.getRow(), m_player.getCol(), min4, step);
                    
                    //compare the mins to find the shortest path
                    if (min1!=-1||min2!=-1||min3!=-1||min4!=-1) {
                        //restore if previously stepped on a object
                        //can step on a empty space
                        int next=20;
                        if(min1<20&&min1!=-1){
                            next=min1;
                        }
                        if(min2<next&&min2!=-1){
                            next=min2;
                        }
                        if(min3<next&&min3!=-1){
                            next=min3;
                        }
                        if(min4<next&&min4!=-1){
                            next=min4;
                        }
                        if(next==min1){
                            nextstepX=it->getRow()+1;
                            nextstepY=it->getCol();
                        }
                        else if(next==min2){
                            nextstepX=it->getRow();
                            nextstepY=it->getCol()+1;
                        }
                        else if(next==min3){
                            nextstepX=it->getRow();
                            nextstepY=it->getCol()-1;
                        }
                        else if(next==min4){
                            nextstepX=it->getRow()-1;
                            nextstepY=it->getCol();
                        }
                        //go to the next position which is the greatest move
                        if (mTemple[nextstepX][nextstepY]==' ') {
                            if(!it->getPre())
                                mTemple[it->getRow()][it->getCol()]=' ';
                            if(it->getPre()){
                                mTemple[it->getRow()][it->getCol()]=it->getOBname();
                                it->deleteOB();
                                it->setPreIF(false);
                            }
                            it->setPosition(nextstepX, nextstepY);
                            mTemple[it->getRow()][it->getCol()]=it->getName()[0];
                            it++;
                            continue;
                        }
                        //can step on an object, but remember to restore it!!!
                        else if (isObject(nextstepX,nextstepY)) {
                            if(!it->getPre())
                                mTemple[it->getRow()][it->getCol()]=' ';
                            if(it->getPre()){
                                mTemple[it->getRow()][it->getCol()]=it->getOBname();
                                it->deleteOB();
                                it->setPreIF(false);
                            }
                            //store the info in theat monster's memory
                            it->setPreobject(nextstepX, nextstepY, mTemple[nextstepX][nextstepY]);
                            it->setPreIF(true);
                            //step on it
                            it->setPosition(nextstepX, nextstepY);
                            mTemple[it->getRow()][it->getCol()]=it->getName()[0];
                            it++;
                            continue;
                        }
                    }
                }
            }
        }
        else{
            it->awake();
        }
        it++;
    }
    
}


void Game::playerMove(const char& a){
    if (m_player.isAwake()) {
        switch (a) {
            case 'c':{
                m_player.cheat();
                
            }
            case '>':{
                if(preObject&&pre_object->name=='>'){
                    newLevel();
                    delete pre_object;
                    preObject=false;
                }
                break;
            }
                //picking up object and add to the inventory
            case 'g':{
                if//do stepping on a object
                    (preObject&&pre_object->row==m_player.getRow()&&pre_object->col==m_player.getCol()) {
                        //find the object information
                        //if find golden idol
                        if (pre_object->name=='&') {
                            m_playerRound="You pick up the golden idol";
                            m_status=true;
                        }
                        if(pre_object->name=='>'){
                            break;
                        }
                        vector<object>::iterator it=m_object.begin();
                        while (it!=m_object.end()) {
                            if
                                //if information matches
                                (it->getRow()==m_player.getRow()&&it->getCol()==m_player.getCol()) {
                                    //add to the inventory
                                    m_player.findObject(it->getName());
                                    if(isWeapon(it->getName())){
                                        m_playerRound="You pick up "+it->getName();
                                    }
                                    else{
                                        m_playerRound="You pick up "+it->getName();
                                    }
                                    //exit the while loop
                                    break;
                                }
                            //keep finding
                            it++;
                        }
                        //delete the item
                        delete pre_object;
                        preObject=false;
                    }
                break;
            }
            //display inventory
            case 'i':{
                clearScreen();
                showInventory();
                getCharacter();
                clearScreen();
                break;
            }
            //switch weapon
            case 'w':{
                clearScreen();
                showInventory();
                char ee=getCharacter();
                //if choose a valid weapon
                if (ee<'b'+m_player.inventorySize()) {
                    int pos=ee-'b';
                    if (ee!='a'&&isWeapon(m_player.objectName(pos))) {
                        //switch player's weapon
                        m_player.setWeapon(m_player.objectName(pos));
                    }
                    m_playerRound="You are wielding "+m_player.getWeapon();
                }
                clearScreen();
                break;
            }
            //read scroll
            case 'r':{
                clearScreen();
                showInventory();
                char ee=getCharacter();
                //if choose a valid scroll
                if (ee<'b'+m_player.inventorySize()&&ee!='a') {
                    int pos=ee-'b';
                    if (!isWeapon(m_player.objectName(pos))) {
                        //read the scroll and change the statistics
                        if (m_player.objectName(pos)=="a scroll of improve armor") {
                            //increase the player's ap
                            m_player.setAP(randInt(1, 3));
                            clearScreen();
                            m_playerRound="You read the scroll called scroll of enhance armor. Your armor glows blue. ";
                        }
                        else if (m_player.objectName(pos)=="a scroll of teleportation") {
                            //randomly choose a place that is not occupied by a monster or a wall
                            int newRow=randInt(1, 16);
                            int newCol=randInt(1, 68);
                            while (mTemple[newRow][newCol]=='#'||isMonster(newRow, newCol)) {
                                newRow=randInt(1, 16);
                                newCol=randInt(1, 68);
                            }
                            mTemple[m_player.getRow()][m_player.getCol()]=' ';
                            m_player.setPosition(newRow, newCol);
                            mTemple[m_player.getRow()][m_player.getCol()]='@';
                            m_playerRound="You read the scroll called scroll of teleportation. You find yourself in a different location. ";
                        }
                        else if (m_player.objectName(pos)=="a scroll of raise strength") {
                            //increase the player's sp
                            m_player.setSP(randInt(1, 3));
                            m_playerRound="You read the scroll called scroll of raise strength. You feel like you can lift the earth.";
                            
                        }
                        else if (m_player.objectName(pos)=="a scroll of enhance health") {
                            //increase the player's maxHP
                            m_player.setMaxhp(randInt(3, 8));
                            m_playerRound="You read the scroll called scroll of enhance health. You feel your heart beating stronger.";
                        }
                        else if (m_player.objectName(pos)=="a scroll of enhance dexterity") {
                            //increase the player's DP
                            m_player.setDP(1);
                            m_playerRound="You read the scroll called scroll of enhance dexterity. You feel like less of a klutz.";
                        }
                        //erase the read scroll
                        m_player.eraseObject(pos);
                    }
                }
                break;
            }
                
            case ARROW_LEFT:{
                //if an empty spot, move
                if (mTemple[m_player.getRow()][m_player.getCol()-1]==' ') {
                    //if previously stepped on an object, restore it
                    if(!preObject)
                        mTemple[m_player.getRow()][m_player.getCol()]=' ';
                    if(preObject){
                        mTemple[m_player.getRow()][m_player.getCol()]=pre_object->name;
                        if(pre_object!=nullptr)
                        delete pre_object;
                        preObject=false;
                    }
                    m_player.setPosition(m_player.getRow(), m_player.getCol()-1);
                    mTemple[m_player.getRow()][m_player.getCol()]='@';
                    break;
                }
                //if a monster, attack
                else if(isMonster(m_player.getRow(), m_player.getCol()-1)){
                    m_player.attack(*findMonster(m_player.getRow(), m_player.getCol()-1));

                        m_playerRound="Player"+(m_player.Actor::getWeapon())->getAction();
                        m_playerRound+=findMonster(m_player.getRow(), m_player.getCol()-1)->getName();
                    if(m_player.getAttackResult()){
                        m_playerRound+=" and hits";
                    }
                    else{
                        m_playerRound+=" and misses";
                    }
                    if(m_player.getSleepSuccess()){
                        m_playerRound+=", putting the ";
                        m_playerRound+=findMonster(m_player.getRow(), m_player.getCol()-1)->getName();
                        m_playerRound+=" to sleep.";
                    }
                    break;
                }
                //if it is an object
                else if(isObject(m_player.getRow(), m_player.getCol()-1)){
                    if(!preObject)
                        mTemple[m_player.getRow()][m_player.getCol()]=' ';
                    if(preObject){
                        mTemple[m_player.getRow()][m_player.getCol()]=pre_object->name;
                        if(pre_object!=nullptr)
                        delete pre_object;
                        preObject=false;
                    }
                    //store the object's info
                    pre_object=new objectInfo{m_player.getRow(), m_player.getCol()-1,mTemple[m_player.getRow()][m_player.getCol()-1]};
                    //step on the object
                    m_player.setPosition(m_player.getRow(), m_player.getCol()-1);
                    mTemple[m_player.getRow()][m_player.getCol()]='@';
                    setPreObject(true);
                    break;
                }
                break;
            }
            case ARROW_RIGHT:{
                //if an empty spot, move
                if (mTemple[m_player.getRow()][m_player.getCol()+1]==' ') {
                    //if previously stepped on an object, restore it
                    if(!preObject)
                        mTemple[m_player.getRow()][m_player.getCol()]=' ';
                    if(preObject){
                        mTemple[m_player.getRow()][m_player.getCol()]=pre_object->name;
                        if(pre_object!=nullptr)
                        delete pre_object;
                        preObject=false;
                    }
                    m_player.setPosition(m_player.getRow(), m_player.getCol()+1);
                    mTemple[m_player.getRow()][m_player.getCol()]='@';
                    break;
                }
                //if a monster, attack
                else if(isMonster(m_player.getRow(), m_player.getCol()+1)){
                    m_player.attack(*findMonster(m_player.getRow(), m_player.getCol()+1));
                    m_playerRound="Player"+(m_player.Actor::getWeapon())->getAction();
                    m_playerRound+=findMonster(m_player.getRow(), m_player.getCol()+1)->getName();
                if(m_player.getAttackResult()){
                    m_playerRound+=" and hits";
                }
                else{
                    m_playerRound+=" and misses";
                }
                if(m_player.getSleepSuccess()){
                        m_playerRound+=", putting the ";
                        m_playerRound+=findMonster(m_player.getRow(), m_player.getCol()+1)->getName();
                        m_playerRound+=" to sleep.";
                }
                    break;
                }
                //if it is an object
                else if(isObject(m_player.getRow(), m_player.getCol()+1)){
                    if(!preObject)
                        mTemple[m_player.getRow()][m_player.getCol()]=' ';
                    if(preObject){
                        mTemple[m_player.getRow()][m_player.getCol()]=pre_object->name;
                        if(pre_object!=nullptr)
                        delete pre_object;
                        preObject=false;
                    }
                    //store the object's info
                    pre_object=new objectInfo{m_player.getRow(), m_player.getCol()+1,mTemple[m_player.getRow()][m_player.getCol()+1]};
                    m_player.setPosition(m_player.getRow(), m_player.getCol()+1);
                    mTemple[m_player.getRow()][m_player.getCol()]='@';
                    setPreObject(true);
                    break;
                }
                break;
            }
            case ARROW_DOWN:{
                //if an empty spot, move
                if (mTemple[m_player.getRow()+1][m_player.getCol()]==' ') {
                    //if previously stepped on an object, restore it
                    if(!preObject)
                        mTemple[m_player.getRow()][m_player.getCol()]=' ';
                    //if stepped on an object
                    if(preObject){
                        //restore it
                        mTemple[m_player.getRow()][m_player.getCol()]=pre_object->name;
                        if(pre_object!=nullptr)
                            delete pre_object;
                        preObject=false;
                    }
                    m_player.setPosition(m_player.getRow()+1, m_player.getCol());
                    mTemple[m_player.getRow()][m_player.getCol()]='@';
                    break;
                }
                //if a monster, attack
                else if(isMonster(m_player.getRow()+1, m_player.getCol())){
                    m_player.attack(*findMonster(m_player.getRow()+1, m_player.getCol()));
                    m_playerRound="Player"+(m_player.Actor::getWeapon())->getAction();
                    m_playerRound+=findMonster(m_player.getRow()+1, m_player.getCol())->getName();
                if(m_player.getAttackResult()){
                    m_playerRound+=" and hits";
                }
                else{
                    m_playerRound+=" and misses";
                }
                if(m_player.getSleepSuccess()){
                        m_playerRound+=", putting the ";
                        m_playerRound+=findMonster(m_player.getRow()+1,m_player.getCol())->getName();
                        m_playerRound+=" to sleep.";
                }
                    break;
                }
                //if it is an object
                else if(isObject(m_player.getRow()+1, m_player.getCol())){
                    if(!preObject)
                        mTemple[m_player.getRow()][m_player.getCol()]=' ';
                    if(preObject){
                        mTemple[m_player.getRow()][m_player.getCol()]=pre_object->name;
                        if(pre_object!=nullptr)
                        delete pre_object;
                        preObject=false;
                    }
                    //store the object's info
                    pre_object=new objectInfo{m_player.getRow()+1, m_player.getCol(),mTemple[m_player.getRow()+1][m_player.getCol()]};
                    m_player.setPosition(m_player.getRow()+1, m_player.getCol());
                    mTemple[m_player.getRow()][m_player.getCol()]='@';
                    setPreObject(true);
                    break;
                }
                break;
            }
            case ARROW_UP:{
                //if an empty spot, move
                if (mTemple[m_player.getRow()-1][m_player.getCol()]==' ') {
                    //if previously stepped on an object, restore it
                    if(!preObject)
                        mTemple[m_player.getRow()][m_player.getCol()]=' ';
                    if(preObject){
                        mTemple[m_player.getRow()][m_player.getCol()]=pre_object->name;
                        if(pre_object!=nullptr)
                        delete pre_object;
                        preObject=false;
                    }
                    m_player.setPosition(m_player.getRow()-1, m_player.getCol());
                    mTemple[m_player.getRow()][m_player.getCol()]='@';
                    break;
                }
                //if a monster, attack
                else if(isMonster(m_player.getRow()-1, m_player.getCol())){
                    m_player.attack(*findMonster(m_player.getRow()+1, m_player.getCol()));
                    m_playerRound="Player"+(m_player.Actor::getWeapon())->getAction();
                    m_playerRound+=findMonster(m_player.getRow()-1, m_player.getCol())->getName();
                if(m_player.getAttackResult()){
                    m_playerRound+=" and hits";
                }
                else{
                    m_playerRound+=" and misses";
                }
                    if(m_player.getSleepSuccess()){
                            m_playerRound+=", putting the ";
                            m_playerRound+=findMonster(m_player.getRow()-1, m_player.getCol())->getName();
                            m_playerRound+=" to sleep.";
                    }
                    break;
                }
                //if it is an object
                else if(isObject(m_player.getRow()-1, m_player.getCol())){
                    if(!preObject)
                        mTemple[m_player.getRow()][m_player.getCol()]=' ';
                    if(preObject){
                        mTemple[m_player.getRow()][m_player.getCol()]=pre_object->name;
                        if(pre_object!=nullptr)
                        delete pre_object;
                        preObject=false;
                    }
                    //store the object's info
                    pre_object=new objectInfo{m_player.getRow()-1, m_player.getCol(),mTemple[m_player.getRow()-1][m_player.getCol()]};
                    m_player.setPosition(m_player.getRow()-1, m_player.getCol());
                    mTemple[m_player.getRow()][m_player.getCol()]='@';
                    setPreObject(true);
                    break;
                }
                break;
            }
            case 'q':
                exit(0);
            default:{
                break;
            }
        }
    }
    else
        m_player.awake();
}

monster* Game::findMonster(const int&row, const int&col){
    vector<monster>::iterator it=mMonsters.begin();
    while (it!=mMonsters.end()) {
        if (it->getRow()==row&&it->getCol()==col) {
            return &(*it);
        }
        it++;
    }
    return &(*it);
}

void Game::showInventory() const{
    clearScreen();
    std::cout<<"Inventory:"<<std::endl;
    std::cout<<'a'<<". "<<"short sword"<<std::endl;
    for (size_t index = 0; index < m_player.inventorySize(); index++) {
        char itemChar = 'b' + index;
        std::cout << itemChar << ". " << m_player.objectName(index) << std::endl;
    }
}

void Game::updateMonster(){
    std::vector<monster>::iterator it=mMonsters.begin();
    while (it!=mMonsters.end()) {
        if (it->isAlive()) {
            it++;
        }
        else{
            //when monsters die, drop its weapon with probability
            if(it->getName()=="Bogeymen"){
                //restore the object that was stepped
                if(it->getPre()){
                    mTemple[it->getRow()][it->getCol()]=it->getOBname();
                }
                //if stepped on golden idol or the stair skip
                if(mTemple[it->getRow()][it->getCol()]!='&'&&mTemple[it->getRow()][it->getCol()]!='>'){
                    if(trueWithProbability(0.1)){
                        mTemple[it->getRow()][it->getCol()]=')';
                        object newObject("magic axe", it->getRow(), it->getCol());
                        m_object.push_back(newObject);
                    }
                    else{
                        mTemple[it->getRow()][it->getCol()]=' ';
                    }
                }
                    it=mMonsters.erase(it);
            }
            if(it->getName()=="Goblins"){
                //restore the previous stepped object info
                if(it->getPre()){
                    mTemple[it->getRow()][it->getCol()]=it->getOBname();
                }
                else{
                    mTemple[it->getRow()][it->getCol()]=' ';
                }
                //if stepped on golden idol or the stair skip
                if(mTemple[it->getRow()][it->getCol()]!='&'&&mTemple[it->getRow()][it->getCol()]!='>'){
                    if(trueWithProbability(1.0/3.0)){
                        mTemple[it->getRow()][it->getCol()]=')';
                        string objectName;
                        if (trueWithProbability(0.5)) {
                            objectName="magic fangs of sleep";
                        }
                        else{
                            objectName="magic axe";
                        }
                        object newObject(objectName, it->getRow(), it->getCol());
                        m_object.push_back(newObject);
                    }
                }
                    it=mMonsters.erase(it);
            }
            if(it->getName()=="Snakewoman"){
                if(it->getPre()){
                    mTemple[it->getRow()][it->getCol()]=it->getOBname();
                }
                else{
                    mTemple[it->getRow()][it->getCol()]=' ';
                }
                if(mTemple[it->getRow()][it->getCol()]!='&'&&mTemple[it->getRow()][it->getCol()]!='>'){
                    if(trueWithProbability(1.0/3.0)){
                        mTemple[it->getRow()][it->getCol()]=')';
                        object newObject("magic fangs of sleep", it->getRow(), it->getCol());
                        m_object.push_back(newObject);
                    }
                }
                    it=mMonsters.erase(it);
            }
            if(it->getName()=="Dragon"){
                if(it->getPre()){
                    mTemple[it->getRow()][it->getCol()]=it->getOBname();
                }
                if(mTemple[it->getRow()][it->getCol()]!='&'&&mTemple[it->getRow()][it->getCol()]!='>'){
                    mTemple[it->getRow()][it->getCol()]='?';
                    string objectName;
                    if (trueWithProbability(0.2)) {
                        objectName="a scroll of improve armor";
                    }
                    else if (trueWithProbability(0.25)) {
                        objectName="a scroll of enhance health";
                    }
                    else if (trueWithProbability(1.0/3.0)) {
                        objectName="a scroll of teleportation";
                    }
                    else if (trueWithProbability(0.5)) {
                        objectName="a scroll of raise strength";
                    }
                    else{
                        objectName="a scroll of enhance dexterity";
                    }
                    object newObject(objectName, it->getRow(), it->getCol());
                    m_object.push_back(newObject);
                }
                it=mMonsters.erase(it);
            }
        }
    }
}

void Game::regainHP(Actor&who){
    if(trueWithProbability(0.1)){
        who.setHP(who.getHP()+1);
    }
}



bool Game::isValid(char maze[18][70], vector<vector<bool>>visited,int x, int y, int dist) {
    return x >= 0 && x < 18 && y >= 0 && y < 70 && maze[x][y] != '#'  && !visited[x][y] && dist<=goblinD && !isMonster(x,y);
}


void Game::findShortestPath(char maze[18][70], std::vector<std::vector<bool>> &visited,
                            int i, int j, int x, int y, int &min_dist, int dist) {
    if (!isValid(maze, visited, i, j, dist)) {
        return;
    }

    std::queue<std::pair<int, int>> q;
    q.push(std::make_pair(i, j));
    visited[i][j] = true;

    bfsRecursive(maze, visited, x, y, min_dist, q, dist);
}

void Game::bfsRecursive(char maze[18][70], std::vector<std::vector<bool>> &visited,
                        int x, int y, int &min_dist, std::queue<std::pair<int, int>> &q, int dist) {
    //if no elements
    if (q.empty()) {
        return;
    }

    std::queue<std::pair<int, int>> nextLevel;

    while (!q.empty()) {
        std::pair<int, int> current = q.front();
        q.pop();

        int currentX = current.first;
        int currentY = current.second;

        if (currentX == x && currentY == y) {
            if (min_dist == -1 || dist < min_dist) {
                min_dist = dist;
            }
            return;
        }
        //search the four direction
        const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        for (int dirIdx = 0; dirIdx < 4; ++dirIdx) {
            int newX = currentX + directions[dirIdx][0];
            int newY = currentY + directions[dirIdx][1];
            if (isValid(maze, visited, newX, newY, dist + 1)) {
                visited[newX][newY] = true;
                nextLevel.push(std::make_pair(newX, newY));
            }
        }
    }

    bfsRecursive(maze, visited, x, y, min_dist, nextLevel, dist + 1);
}




// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED
#include "Actor.h"
#include "utilities.h"
#include <climits>
#include <map>
#include <vector>
#include <queue>
//hold the objects' locations and names
struct objectInfo{
    int row;
    int col;
    char name;
};

//each room's info
struct roomInfo{
    int startRow;
    int endRow;
    int startCol;
    int endCol;
};

class Game
{
public:
    Game(int goblinSmellDistance);
    void play();
    //displayer the current temple
    void displayTemple()const;
    void playerMove(const char& a);
    //update each monster's locationa and datas
    void updateMonster();
    void monstersMove();
    //if S and B can reach the player within some distances
    bool Breachable(const std::string& name,const int& bRow, const int& bcol, const int&pRow, const int&pCol)const;
    //if the current location can be occupied by an actor or not
    bool isValid(char maze[18][70], std::vector<std::vector<bool> >visited,int x, int y, int dist);
    //Goblins, find the shortest path to reach the player
    void findShortestPath(char maze[18][70], std::vector<std::vector<bool> > &visited,
                          int i, int j, int x, int y, int &min_dist, int dist);
    void bfsRecursive(char maze[18][70], std::vector<std::vector<bool> > &visited,
                            int x, int y, int &min_dist, std::queue<std::pair<int, int> > &q, int dist);
    //for restoring objects
    void setPreObject(const bool&a){preObject=a;};
    void showInventory()const;
    
    void newLevel();
    void addRooms();
    //info display
    void showPlayerInfo()const;
    void showPlayerMoveInfo()const;
    void showMonstersMove();
    void clearMonsterMove();
    void setPlayerMove(const std::string& info);
    
    void regainHP(Actor&who);
    
    bool isMonster(const int&row, const int&col)const;
    bool isObject(const int&row, const int&col)const;
    bool isWeapon(const std::string& name)const;
    
    monster* findMonster(const int&row, const int&col);
    
private:
    int goblinD;
    player m_player;
    char mTemple[18][70];
    //store the object that was stepped
    objectInfo* pre_object;
    //keep track of all the monsters and objects
    std::vector<monster> mMonsters;
    std::vector<object> m_object;
    bool m_status;
    bool preObject=false;
    bool inventoryOpen=false;
    std::string m_playerRound;
    std::vector<std::string> m_monstersMove;
    int currentLevel;
    std::vector<roomInfo> m_rooms;
};


#endif // GAME_INCLUDED

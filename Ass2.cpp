/*
Implement a multiplayer game system that uses an AVL tree data structure to organize and
manage player data efficiently. The multiplayer game supports multiple players participating
simultaneously, and the AVL tree is used to store player information such as player_id and
scores (key, value pair). The system should provide following operation:
1. Player Registration
2. Leaderboard Display
3. Remove player from game
*/

#include <iostream>
using namespace std;

class Player {
    public:
    int player_id;
    int score;
    int height;
    Player* left;
    Player* right;
    Player(int player_id,int score){
        this->player_id = player_id;
        this->score = score;
        height = 1;
        left = right = nullptr;
    }
};

class Game_system {
    Player* root;

    int height(Player* node){
        if(node == nullptr) return 0;

        return node->height;
    }

    int getbalance(Player* node){
        if(node == nullptr) return 0;

        return height(node->left) - height(node->right);
    }

    Player* rightrotate(Player* y){
        Player* x = y->left;
        Player* T2 = x->right;

        x->right = y;
        y->left = T2;

        x->height = 1+max(height(x->left),height(x->right));
        y->height = 1+max(height(y->left),height(y->right));

        return x;
    }

    Player* leftrotate(Player* y){
        Player* x = y->right;
        Player* T2 = x->left;

        x->left = y;
        y->right = T2;

        x->height = 1+max(height(x->left),height(x->right));
        y->height = 1+max(height(y->left),height(y->right));


        return x;
    }

    Player* insert(Player* node,int id,int scr){
        if(node == nullptr){
            return new Player(id,scr);
        }

        if(id < node->player_id){
            node->left = insert(node->left,id,scr);
        }
        else if (id > node->player_id){
            node->right = insert(node->right,id,scr);
        }

        node->height = 1 + max(height(node->left),height(node->right));

        int balance = getbalance(node);

        // Right rotation
        if(balance > 1 && node->left->player_id > id){
            return rightrotate(node);
        }

        // Left rotation 
        if(balance < -1 && node->right->player_id < id){
            return leftrotate(node);
        }

        // LR CASE
        if(balance > 1 && node->left->player_id < id ){
            node->left = leftrotate(node->left);
            return rightrotate(node);
        }

        // RL CASE
        if(balance < -1 && node->right->player_id > id ){
            node->right = rightrotate(node->right);
            return leftrotate(node);
        }

        return node;
    }

    void inorder(Player* node){

        if(node == nullptr){
            return;
        }

        inorder(node->left);
        inorder(node->right);
        cout << node->player_id << " : " << node->score;

    }

    public:

    Game_system(){
        root = nullptr;
    }

    void insertplayer(int plyid,int plysr){
        root = insert(root,plyid,plysr);
    }

    void leaderboard(){
        if(root = nullptr){
            cout << "No Players Registered ! " << endl;
        }else{
            inorder(root);
        }
    }
   
    
};

int main(){

    return 0;
}

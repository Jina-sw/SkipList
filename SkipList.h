#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <climits>
#include "Node.h"


#ifndef SKIPLIST_H
#define SKIPLIST_H

using namespace std;

class SkipList {
public:
    // Maximum number of levels allowed for this skip list
    int MAXIMUM_ALLOWED_LEVELS;

    // current maximum level amongst the inserted nodes
    int level;

    // the head node's next links are connected to the first node at every level
    Node* head;
    Node* tail;

    SkipList(int maxLevels) {
        MAXIMUM_ALLOWED_LEVELS = maxLevels;

        // initially we have the bottom-most level only
        level = 0;

        // create the header node, value is irrelevant (as long as it doesn't match an inserted value - NO REPEATS), number of next links is important (this node is connected to the first node at every level)
        head = new Node(INT_MIN, MAXIMUM_ALLOWED_LEVELS);
        tail = new Node(INT_MAX, MAXIMUM_ALLOWED_LEVELS);
    }

    int RandomLevel() {
        float probablity = (float)rand() / RAND_MAX; // flip a coin
        int lvl = 0;
        while (probablity < 0.5 && lvl < MAXIMUM_ALLOWED_LEVELS) {
            lvl++; // landed heads so increase level by 1
            probablity = (float)rand() / RAND_MAX; // flip a coin again
        }
        return lvl;
    }

    Node* CreateNode(int value, int level) {
        // create a new node with next links for every level that this node will be part of
        // it will use these links to connect to the next node at each level  
        return new Node(value, level);
    }

    void InsertElement(int value) {

        Node* current = head;
        Node* currentNext = head; 
        
        vector<Node*> forwards(MAXIMUM_ALLOWED_LEVELS + 1, nullptr); 
        vector<Node*> backwards(MAXIMUM_ALLOWED_LEVELS + 1, nullptr); //backwards save location info of a node next to the

        for (int i = level; i >= 0; i--) {
            while (current->next[i] != nullptr && current->next[i]->value < value) {
                current = current->next[i];
            }
            forwards[i] = current;
        }

        currentNext = current->next[0];
        current = tail;

        for (int i = level; i >= 0; i--) {
            while (current->prev[i] != nullptr && current->prev[i]->value > value) {
                current = current->prev[i];
            }
            backwards[i] = current;
        }

        if (currentNext == nullptr || currentNext->value != value) {
            int ranLevel = RandomLevel();
            if (ranLevel > level) {
              
                for (int i = level + 1; i <= ranLevel; i++) {
                    forwards[i] = head;
                    backwards[i] = tail;
                }

                
                level = ranLevel;
            }

            Node* n = CreateNode(value, ranLevel);


            // placing new node in the correct place at every level
            for (int i = 0; i <= ranLevel; i++) {
                n->next[i] = forwards[i]->next[i];
                n->prev[i] = backwards[i]->prev[i];
                forwards[i]->next[i] = n;
                backwards[i]->prev[i] = n;
            }
        }

    }


    void Delete(int value) {
        // TO BE COMPLETED
        // remove value from skip list (all levels) if it exists (output a message if it does not exist)
        if (Search(value)) {
            Node* current = head;
            Node* currentNext = head;
            vector<Node*> forwards(MAXIMUM_ALLOWED_LEVELS + 1, nullptr);
            vector<Node*> backwards(MAXIMUM_ALLOWED_LEVELS + 1, nullptr);
            
            for (int i = level; i >= 0; i--) {
                while (current->next[i] != nullptr && current->next[i]->value < value) {
                    current = current->next[i];
                }
                forwards[i] = current;

            }
            currentNext = current->next[0];
            current = tail;

            for (int i = level; i >= 0; i--) {
                while (current->prev[i] != nullptr && current->prev[i]->value > value) {
                    current = current->prev[i];
                }
                backwards[i] = current;
            }
            
            int size = currentNext->next.size();

            if (currentNext != nullptr && currentNext->value == value ) {
            
                for (int i = 0; i < size; i++) {
                    forwards[i]->next[i] = currentNext->next[i];
                    backwards[i]->prev[i] = currentNext->prev[i];
                }
                delete currentNext;

                while (level > 0 && head->next[level] == nullptr) {
                    level--;
                }
            }

        }
        else {
            cout << "Value : " << value << " doesn't exist " << endl;
        }

    }

    bool Search(int value) {
        // TO BE COMPLETED
        // search for value in skip list and return true if it exists; false otherwise
        Node* current = head;
        for (int i = level; i >= 0; i--) {

            while (current->next[i] != nullptr && current->next[i]->value < value) {
                current = current->next[i];
            }

            if (current->next[i] != nullptr && current->next[i]->value == value) {
                return true;
            }

        }

        return false;

    }

    void Show() {

        Node* node = head;
        for (int i = 0; i <= level; i++) {
            node = head->next[i];
            cout << "Level " << i << ": ";
            while (node != nullptr && node->next[i] != nullptr) {
                cout << node->value << " -> ";
                node = node->next[i];
            }
            cout << node->value << " .\n";
        }
    }
    
    void TailShow() {

        Node* node = tail;
        for (int i = 0; i <= level; i++) {
            node = tail->prev[i];
            cout << "Level " << i << ": ";
            while (node != nullptr && node->prev[i] != nullptr) {
                cout << node->value << " <- ";
                node = node->prev[i];
            }

            cout << node->value <<".\n";
        }
        
    }
};

#endif


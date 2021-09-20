#include <iostream>
#include <string>
#include <fstream>
#include "ArgumentManager.h"
using namespace std;

//Paul - 33%, Eric 33%, David 33%

struct node {
    int data;
    node* left;
    node* right;
};

node* insert(int x, node* t)
{
    if (t == nullptr)
    {
        t = new node;
        t->data = x;
        t->left = t->right = nullptr;
    }
    else if (x < t->data)
        t->left = insert(x, t->left);
    else if (x > t->data)
        t->right = insert(x, t->right);
    return t;
}

void inorder(node* t) {
    if (t == nullptr) {
        return;
    }
    inorder(t->left);
    //cout << t->data << " ";
    inorder(t->right);
}

int numNodes(node* t, int key) {
    int count = 1; //maybe starts at 1
    if (t == nullptr || t->data == key) {
        return count;
    }
    else if (t->data < key) {
        count += numNodes(t->right, key);
    }
    else if (t->data > key) {
        count += numNodes(t->left, key);
    }
    return count;
}

class Vector {
private:
    int v_size;
    node* head;
public:
    Vector() { head = nullptr; v_size = 0; }

    void push_back(int _data) {
        node* temp = new node;
        temp->data = _data;
        temp->right = nullptr;
        node* current = head;
        if (head == nullptr) {
            head = temp;
        }
        else {
            if (head->right == nullptr) {
                current->right = temp;
            }
            else {
                while (current->right != nullptr) {
                    current = current->right;
                }
                current->right = temp;
            }
        }
        v_size++;
    }

    void clear() {
        node* temp = new node;
        temp->right = nullptr;
        temp = nullptr;
        head = temp;
        v_size = 0;
    }

    int size() {
        return v_size;
    }

    int at(int index) {
        node* current = head;
        for (int i = 0; i < index; i++) {
            if (current != nullptr) {
                current = current->right;
            }
            else {
                cout << "Index out of range." << endl;
                break;
            }
        }
        return current->data;
    }

    void setAt(int index, int _data) {
        node* current = head;
        for (int i = 0; i < index; i++) {
            if (current != nullptr) {
                current = current->right;
            }
            else {
                cout << "Index out of range." << endl;
                break;
            }
        }
        current->data = _data;
    }
};


class Hashing {
private:
    bool infinite;
    int lapse;
    bool validFound;
    int validHashSize;
public:
    Hashing() { infinite = false; lapse = 0; validFound = false; validHashSize = 0; }
    void printVec(Vector hT, int numOfHashes, ofstream& outfile);
    void hashing(Vector hT, Vector s1, Vector s2, int tsize, int numOfHashes, ofstream& outfile);
    int bintoDec(int n);
    bool getInf() {
        return infinite;
    }
    int getLapse() {
        return lapse;
    }
    int getValidHashSize() {
        return validHashSize;
    }
    //int convertBase10(int n);
};

void Hashing::printVec(Vector hT, int numOfHashes, ofstream& outfile)
{
    //cout << "lapse = " << lapse << endl;
    if (infinite != true) {
        outfile << "valid inventory log" << endl;
        validFound = true;
        validHashSize = numOfHashes;
    }
    else {
        outfile << "INVALID INVENTORY LOG!!!" << endl;
        if (!validFound) {
            lapse += numOfHashes;
        }
    }

    for (int i = 0; i < hT.size(); i++)
    {
        //cout << i << "-->" << hT[i] << endl;
    }
}

int Hashing::bintoDec(int n)
{
    int num = n;
    int dec_value = 0;
    int base = 1;
    int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;
        dec_value += last_digit * base;
        base = base * 2;
    }
    return dec_value;
}


void Hashing::hashing(Vector hT, Vector s1, Vector s2, int tsize, int numOfHashes, ofstream& outfile)
{
    int hold = -1;
    int indexHold;
    int z = 1;
    int infiniteCount = 0;
    bool firstrun = true;
    bool finished = false;
    infinite = false;
    Vector holds;
    for (int i = 0; i < s1.size(); i++)
    {
        int hv = s1.at(i) % tsize;
        int hv2 = s2.at(i) % tsize;
        if (hT.at(hv) == -1) {
            hT.setAt(hv, z);
        }
        else if (hT.at(hv) != -1) { //if there is a collision
            if (hT.at(hv2) == -1) { //if secondary is open
                hT.setAt(hv2, z);
            }
            else { //if both are collisions
                infiniteCount = 0;
                while (finished == false) {
                    if (firstrun == true) {
                        holds.push_back(hT.at(hv));
                        hT.setAt(hv, z);
                        firstrun = false;
                    }
                    else {
                        indexHold = s2.at(holds.at(infiniteCount - 1) - 1) % tsize;
                        if (hT.at(indexHold) == -1) {
                            hT.setAt(indexHold, holds.at(infiniteCount - 1));
                        }
                        holds.push_back(hT.at(indexHold));

                        hT.setAt(indexHold, holds.at(infiniteCount - 1));
                        if (infiniteCount == 11) {
                            infinite = true;
                            finished = true;
                        }
                    }
                    infiniteCount++;
                }
            }
        }
        z++;
    }

    for (int i = 0; i < holds.size(); i++) {
        //cout << holds[i] << " ";
    }
    //cout << endl;

    holds.clear();
    //cout << endl;
    printVec(hT, numOfHashes, outfile);
    //cout << endl;
}

int main(int argc, char* argv[]) {
    ArgumentManager am(argc, argv);
    string infilename = am.get("input");
    string outfilename = am.get("output");
    ofstream outfile(outfilename);
    ifstream infile(infilename);


    int numofTables;
    string line;
    int numofHashes;
    int hashSize;
    int primaryKey;
    int secondaryKey;
    Vector sauce1;
    Vector sauce2;
    Vector hashTable;
    Hashing d;

    infile >> line;
    numofTables = stoi(line);
    while (infile >> line) {
        if (isalpha(line[0])) {
            //cout << "IS ALPHA " << line << endl;
            break;
        }
        numofHashes = stoi(line);
        infile >> line;
        hashSize = stoi(line);
        for (int i = 0; i < hashSize; i++)
        {
            hashTable.push_back(-1);
        }
        for (int i = 0; i < numofHashes; i++) {
            infile >> line;
            primaryKey = stoi(line);
            sauce1.push_back(d.bintoDec(primaryKey));
            infile >> line;
            secondaryKey = stoi(line);
            sauce2.push_back(d.bintoDec(secondaryKey));
            //cout << "line = " << line << endl;
        }
        d.hashing(hashTable, sauce1, sauce2, hashSize, numofHashes, outfile);

        hashTable.clear();
        sauce1.clear();
        sauce2.clear();

    }

    string lines;
    int ascChar = 0;
    int query = 0; //output number of nodes travered in order to get to this node
    node* root = nullptr;
    //cout << "line = " << line << endl;
    for (int i = 0; i < line.length(); i++) {
        query += line[i];
    }
    getline(infile, lines);
    for (int i = 0; i < d.getLapse(); i++) {
        getline(infile, lines);
    }
    for (int i = 0; i < d.getValidHashSize(); i++) {
        getline(infile, lines);
        //cout << "lines = " << lines << endl;
        for (int j = 0; j < lines.length(); j++) {
            ascChar += lines[j];
        }
        root = insert(ascChar, root);
        ascChar = 0;
    }
    outfile << (numNodes(root, query));

    outfile.close();
    infile.close();

    return 0;
}

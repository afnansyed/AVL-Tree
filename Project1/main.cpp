//code references used from Prof. Aman Lecture 3 - Trees and 4- Balanced Trees slides
//additional conceptual and syntactical reference:
//https://www.youtube.com/watch?v=LXdi_4kSd1o ("5.15 AVL Tree Deletion in Data structures" by Jenny's Lectures)
//levelCount code is from my Eduagtor Programming Quiz 4: Sum of a Level
//reference for using queue:
//https://www.cplusplus.com/reference/queue/queue/front/
//https://www.cplusplus.com/reference/queue/queue/empty/
//https://www.cplusplus.com/reference/queue/queue/pop/

#include <iostream>
#include <algorithm>
#include <queue>
#include <sstream>
#include <string>
#include <regex>
#include <iomanip>

using namespace std;

struct Node{
    string name;
    int id;
    Node* left;
    Node* right;
    int height = 0;

    Node(){
        name = " ";
        id = 0;
        left = nullptr;
        right = nullptr;
        height = 0;
    }
    Node(string name, int id){
        this->name = name;
        this->id = id;
        left = nullptr;
        right = nullptr;
    }
};

class AVLTree{

public:
    Node* n = nullptr;

    //code reference: Prof. Aman Lecture 3 Trees slides pg. 16
    int heightN(Node* root){
        if(root == nullptr){ //if tree has just the root
            return 0; //height = 0
        }
        else{ //if tree has more that the root
            int l = heightN(root->left); //height of left subtree
            int r = heightN(root->right); //height of right subtree
            int maxi = max(l,r); //find the greater height
            return 1 + maxi; // height = 1+max(height(children)
        }
    }

    //code reference from Prof. Aman Lecture 4 Balanced Trees Lecture Slides pg. 12
    //code also used for submission on stepik 5.1.1
    Node* rotateLeft(Node *node)
    {
        Node* a = node;  //root node
        Node* b = a->right;   //right child
        Node* x = a->right->left;  //root's successor
        b->left = a;  //assign child's left as root
        a->right = x;  //assign root's right as the successor

        b->height = heightN(b); //update height of child
        a->height = heightN(a); //update height of root

        return b;
    }

    //code reference from Prof. Aman Lecture 4 Balanced Trees Lecture Slides pg. 12
    //code also used for submission on stepik 5.1.1
    Node* rotateRight(Node *node)
    {
        Node* c = node;    //root node
        Node* b = c->left;    //left child
        Node* y = c->left->right;    //root's predecessor
        b->right = c;    //assign child's right as root
        c->left = y;    //assign root's left as the predecessor

        b->height = heightN(b);  //update height of child
        c->height = heightN(c); //update height of root

        return b;
    }

    //code reference from Prof. Aman Lecture 4 Balanced Trees Lecture Slides
    //code also used for submission on stepik 5.1.1
    Node* rotateLeftRight(Node *node)
    {
        node->left = rotateLeft(node->left);  //perform left rotation on the root's left node
        Node* p = rotateRight(node);  //perform right rotation on the root

        p->height = heightN(p); //update height of root
        return p; //return root
    }

    //code reference from Prof. Aman Lecture 4 Balanced Trees Lecture Slides
    //code also used for submission on stepik 5.1.3
    Node* rotateRightLeft(Node *node)
    {
        node->right = rotateRight(node->right);  //perform right rotation on the root's right node
        Node* p = rotateLeft(node);  //perform left rotation on the root
        p->height = heightN(p); //update height of root
        return p; //return root
    }

    //find balance factor
    int balanceFactor(Node* root){
        int l = heightN(root->left); //height of left subtree
        int r = heightN(root->right); //height of right subtree
        int s = l-r; //balance factore = height of left subtree - height of right subtree

        return s;
    }

    //code written by using pseudocode provided by Prof. Aman Lecture 4 Balanced Trees slides pg. 15
    Node* updateBalance(Node* root){

        int hr = heightN(root->right); //height of root's right subtree
        int hl = heightN(root->left); //height of root's left subtree
        int rl = 0; //height of right subtree's left subtree
        int rr = 0; //height of right subtree's right subtree
        int ll = 0; //height of left subtree's left subtree
        int lr = 0; //height of left subtree's left subtree


        if(hr > 0){ //find height of root's right subtree's left and right subtree
            rl = heightN(root->right->left);
            rr = heightN(root->right->right);
        }
        if(hl > 0){ //find height of root's left subtree's left and right subtree
            ll = heightN(root->left->left);
            lr = heightN(root->left->right);
        }

        if(hr > hl){ //if tree is right heavy
            if(rl > rr){ //if tree's right subtree is left heavy
                root = rotateRightLeft(root);
            }
            else {
                root = rotateLeft(root);
            }
        }
        else if (hl > hr){ //if tree is left heavy
            if(lr > ll){ //if tree's left subtree is right heavy
                root = rotateLeftRight(root);
            }
            else{
                root = rotateRight(root);
            }
        }

        return root;
    }

    //insert helper function
    //code used written by Prof Aman Lecture 4 Balanced Trees slides pg 15
    Node* insertN(Node* root, string name, int id){

        if (root == nullptr){
            cout << "successful" << endl;
            return new Node(name, id); //insert node
        }
        else if(root->id == id){
            cout << "unsuccessful" << endl;
            return root;
        }
        else if (id < root->id){ //if id is less than the current root id
            root->left = insertN(root->left, name, id);
        }
        else{ //if id is greater than the current root id
            root->right = insertN(root->right, name, id);
        }

        root->height = heightN(root); //update height of node
        int b = balanceFactor(root); //find balance factor of node

        if(b != -1 && b != 1 && b != 0) { //if unbalanced
            root = updateBalance(root); //perform necessary rotations

        }
        return root;
    }

    //insert function
    Node* insertNAMEID(Node* root, string name, int id) {
        root = insertN(root, name, id); //insert name and id and return updated tree
        return root;
    }

    //count number of nodes in the tree using inorder traversal
    //inorder traversal code reference from Prof Aman Lecture 3 Trees slides pg 40
    void nodeCounter(Node* root, int& nodeCount){
        if(root == nullptr){ //if tree is empty
        }
        else{
            nodeCounter(root->left, nodeCount); //traversal left subtree
            nodeCount++; //increment root
            nodeCounter(root->right, nodeCount); //traversal right subtree
        }
    }

    //print inorder helper function
    //inorder traversal code reference from Prof Aman Lecture 3 Trees slides pg 40
    void printIn(Node* root, int& count, int N){
        if(root == nullptr){ //if tree is empty
        }
        else{
            printIn(root->left, count, N); //traversal left subtree
            if(count+1 == N){ //if at the last node
                cout << root->name <<  endl ; //print without comma
            }
            else{
                cout << root->name <<  ", " ; //print current node
            }
            count++; //increment count
            printIn(root->right, count, N); //traversal right subtree
        }
    }

    //print inorder
    void printInorder(Node* root){
        int numNodes = 0;
        int c = 0;
        nodeCounter(root, numNodes); //count number of nodes
        printIn(root, c, numNodes); //print inorder traversal
    }

    //print preorder helper function
    //pre-order traversal code reference from Prof Aman Lecture 3 Trees slides pg 41
    void printPre(Node* root, int& count, int N){
        if(root == nullptr){ //if tree is empty
        }
        else{
            if(count+1 == N){ //if at the last node
                cout << root->name <<  endl ; //print without comma
            }
            else{
                cout << root->name <<  ", " ;
            }
            count++; //increment count
            printPre(root->left, count, N); //traversal left subtree
            printPre(root->right, count, N); //traversal right subtree
        }
    }

    //print preorder
    void printPreorder(Node* root){
        int numNodes = 0;
        int c = 0;
        nodeCounter(root, numNodes); //count num of nodes
        printPre(root, c, numNodes); //print preorder traversal
    }

    //print postorder helper function
    //postorder traversal code reference from Prof Aman Lecture 3 Trees slides pg 43
    void printPost(Node* root, int& count, int N){
        if(root == nullptr){ //if tree is empty
        }
        else{
            printPost(root->left, count, N); //traversal left subtree
            printPost(root->right, count, N); //traversal right subtree
            if(count+1 == N){ //if at last node
                cout << root->name <<  endl ; //print without comma
            }
            else{
                cout << root->name <<  ", " ;
            }
            count++; //incremnt count
        }
    }

    //print postorder
    void printPostorder(Node* root){
        int numNodes = 0;
        int c = 0;
        nodeCounter(root, numNodes); //count num of nodes in the tree
        printPost(root, c, numNodes); //print postorder
    }


    //print Left most node (left most leaf node)
    Node* leftMost(Node* root){
        if(root->left == nullptr){ //if the next node is null
            return root; //return current root which is the left most leaf node
        }
        else{
            leftMost(root->left); //traversal through the left nodes
        }
    }

    //remove id
    //Code written using pseodocode provided by Prof Aman lecture 3 Trees slides pg. 35
    //Code reference also using from the Project 1 Overview slides
    //concept reference for deleting a node in an avl tree:
    //https://www.youtube.com/watch?v=LXdi_4kSd1o ("5.15 AVL Tree Deletion in Data structures" by Jenny's Lectures)
    Node* removeID(Node* root, int id ){
        if(root == nullptr){ //if root is null
            return nullptr; //return null
        }
        else if (id < root->id){ //if id is less than the root id
            root->left = removeID(root->left, id); //traversal left subtree  (code from Project 1 Overview Slides)
        }
        else if (id > root->id){ //if id is greater than the root id
            root->right = removeID(root->right, id); //traversal right subtree
        }
        else{ //the id matches the current id
            if(root->left == nullptr && root->right == nullptr){ //if the root has no children
                root = nullptr; //set root to null
                cout << "successful" << endl;
                return root;
            }
            else if(root->left != nullptr && root->right == nullptr){ //if root has only left child
                root = root->left; //replace the root with the left child
                cout << "successful" << endl;
                return root;
            }
            else if(root->right != nullptr && root->left == nullptr){ //if the root has only right child
                root = root->right; //replace the root with the right child
                cout << "successful" << endl;
                return root;
            }
            else{ //find the inorder successor
                if(root->right->left == nullptr){ //if the root's right child has no left child
                    //concept reference for deleting a node in an avl tree:
                    //https://www.youtube.com/watch?v=LXdi_4kSd1o ("5.15 AVL Tree Deletion in Data structures" by Jenny's Lectures)
                    //replace the current root with the root's right child
                    root->id = root->right->id;
                    root->name = root->right->name;
                    root->right = root->right->right;
                    cout << "successful" << endl;
                    return root;
                }
                else{
                    Node* nod = leftMost(root->right); //find the left most child of the subtree of the root's right child
                    //concept reference for deleting a node in an avl tree:
                    //https://www.youtube.com/watch?v=LXdi_4kSd1o ("5.15 AVL Tree Deletion in Data structures" by Jenny's Lectures)
                    //reference TA Andrew Penton on Slack
                    //replace the current root with the left most child
                    root->id = nod->id;
                    root->name = nod->name;
                    delete nod;
                    nod = nullptr; //assign that left most child as null
                    cout << "successful" << endl;
                    return root;

                }

            }

        }
        return root;
    }

    //find the node at given inorder index
    //inorder traversal code reference Prof Aman Lecture 3 Trees slides pg 40
    Node* searchRoot(Node* root, int N, int& node){
        if(root == nullptr){ //if tree is empty
        }
        else{
            searchRoot(root->left, N, node); //traverse through left subtree
            if(node == N){ //if at given index
                return root; //return current node
            }
            node++;
            searchRoot(root->right, N, node); //traverse through right subtree
        }
    }

    //remove inorder
    Node* removeInorder(Node* root, int N){
        int node = 0;
        Node* a = searchRoot(root, N, node); //find node at given index
        root = removeID(root, a->id); //remove node at given index and return updates tree
        return root;
    }



    //search name function
    //preorder traversal reference from Prof Aman Lecture 3 Trees slides pg 38
    //reference for set fill and setw: https://www.cplusplus.com/reference/iomanip/setfill/
    void searchName(Node* root, string name, string& s){

        if(root == nullptr){ //if id not found
        }
        else{
            if(name == root->name){
                cout << setfill ('0') << setw (8); //set id to have 8 digits (fill with 0)
                cout << root->id << endl;
                s = "successful";
            }
            searchName(root->left, name, s); //traverse through left subtree
            searchName(root->right, name, s); //traverse through right subtree
        }
    }

    //search ID fucntion
    //function writtin using pseudocode provided by Prof Aman Lecture 3 Trees pg 32
    void searchID(Node* root, int id){
        string s;
        if(root == nullptr){ //if tree is empty
            s = "unsuccessful";
            cout << s << endl;
        }
        else if(root->id == id && s != "unsuccessful"){ //else if id is found
            cout << root->name << endl; //print name
        }
        else if (id < root->id){ //if id is less than the root id
            searchID(root->left, id); //traverse through left subtree
        }
        else if (id > root->id){ //if id is greater that the root id
            searchID(root->right, id); //traverse through right subtree
        }
    }

    //count the number of levels in a tree
    //code is from my Eduagtor Programming Quiz 4: Sum of a Level
    //reference for using queue:
    //https://www.cplusplus.com/reference/queue/queue/front/
    //https://www.cplusplus.com/reference/queue/queue/empty/
    //https://www.cplusplus.com/reference/queue/queue/pop/
    int levelCount(Node* root){

        int c = 0;
        if(root == nullptr){
            c = 0;
        }
        else{
            queue<Node*> q; //initialize queue

            int a = 0; //child counter
            q.push(root);  //push root in queue
            if(q.front()->left != nullptr){ //if it has a left child
                a++; //increment
            }
            else if(q.front()->right != nullptr){
                a++;
            }

            while(q.empty() != true){ //iterate till level reached
                int b = 0; //child counter2

                //add children of nodes in current level and remove nodes at current level
                while(a != 0 ){
                    if(q.front()->left != nullptr){
                        q.push(q.front()->left); //push left child
                        b++;
                    }
                    if(q.front()->right != nullptr){
                        q.push(q.front()->right); //push right child
                        b++;
                    }

                    q.pop(); //remove parent
                    a--; //decrement child of previous parent

                }
                a = b;  //assign new child count
                c++;  //incremnt level counter
            }
        }
        return c;
    }

    //postorder traversal to delete all the nodes in the tree
    //postorder traversal reference from Prof Aman Lecture 3 Trees slides pg 43
    Node* traverDel(Node* root){
        if(root == nullptr){
            return nullptr;
        }
        else{
            traverDel(root->left);
            traverDel(root->right);
            delete root;
            root = nullptr;
        }
    }

    //destructor
    ~AVLTree(){
        n = traverDel(n);
    }

};

int main() {

    AVLTree av;

    //regex reference from in class Lecture by Prof Aman on Feb 9, 2022
    regex numOnly = regex("^[0-9]+$"); //numbers only
    regex alpOnly = regex("^[A-Za-z\\s]*$"); //alphabets and spaces

    int nu;
    cin >> nu;
    for(int i = 0; i <= nu; i++){
        string line;
        getline(cin, line);
        istringstream in(line);
        string command;
        in >> command; //read in command

        if(command == "insert"){
            string name;
            string number;
            int num;

            int com = command.length(); //length of command
            string line2 = line.substr(com+1, line.length()); //string without the command
            name = line2.substr(1,line2.length()); //only name with the last quotation
            int quot1 = name.find("\""); //find closing quotation
            string nam = name.substr(0, quot1); //final name
            number = name.substr(quot1+2, name.length()); //number

            bool isName = regex_search(nam, alpOnly); //check valid input
            bool isNumber = regex_search(number, numOnly); //check valid input
            if(isName == true && isNumber == true && number.length() == 8){ //if name is only letters and spaces and num is only num and if is 8 digits
                num = stoi(number); //convert id to string
                av.n = av.insertNAMEID(av.n, nam, num); //insert name and id
            }
            else{
                cout << "unsuccessful" << endl;
            }

        }
        else if(command == "search"){

            string number;
            int num;

            int com = command.length(); //length of command
            string input = line.substr(com+1, line.length()); //string without the command

            bool isNumber = regex_search(input, numOnly); //check valid input

            string nam = input.substr(1,input.length()-2);
            bool isName = regex_search(nam, alpOnly); //check valid input


            if(isNumber == true && input.length() == 8){ //if input is numbers
                num = stoi(input); //convert id to string
                av.searchID(av.n, num); //search id
            }
            else if(isName == true){
                string s;
                av.searchName(av.n, nam, s); //search name
                if(s != "successful"){
                    cout << "unsuccessful" << endl;
                }
            }
            else if(isName == false && (isNumber == false || input.length() != 8)){
                cout << "unsuccessful" << endl;
            }

        }
        else if(command == "printInorder"){
            av.printInorder(av.n);
        }
        else if(command == "printPreorder"){
            av.printPreorder(av.n);
        }
        else if(command == "printPostorder"){
            av.printPostorder(av.n);
        }
        else if(command == "printLevelCount"){
            cout << av.levelCount(av.n) << endl;
        }
        else if(command == "remove"){
            string input;
            string number;
            int num;
            in >> input;
            bool isNumber = regex_search(input, numOnly); //check valid input
            if(isNumber == true){
                num = stoi(input); //convert to int
                av.removeID(av.n, num);
            }
            else{
                cout << "unsuccessful" << endl;
            }

        }
        else if(command == "removeInorder"){
            int a;
            in >> a;
            av.removeInorder(av.n, a);
        }

    }



    return 0;
}
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <iterator>
#include <sstream>
#include <regex>

using namespace std;

/* Note:
	1. You will have to comment main() when unit testing your code because catch uses its own main().
	2. You will submit this main.cpp file on canvas. If you are using multiple header files,
	   you will have to include them directly in main.cpp so we can test your code as a single file.
*/

struct Node {
    string name;
    string id;
    Node *left;
    Node *right;
    int height;
    int balance;

    Node() {
        name = "";
        id = "";
        left = nullptr;
        right = nullptr;
        height = 1;
        balance = 0;
    }

    Node(string inputName, string inputId) {
        name = inputName;
        id = inputId;
        left = nullptr;
        right = nullptr;
        height = 1;
        balance = 0;
    }

    // recursively calculates the height of the tree
    int calculateHeight() {
        int leftHeight = 0;
        int rightHeight = 0;

        if (left != nullptr) {
            leftHeight = left->calculateHeight();
        }
        if (right != nullptr) {
            rightHeight = right->calculateHeight();
        }
        height = max(leftHeight, rightHeight) + 1;
        return height;
    }

    // calculates the balance of the tree
    void calculateBalance() {
        int leftHeight = 0;
        int rightHeight = 0;

        if(left != nullptr) {
            leftHeight = left->calculateHeight();
        }
        if(right != nullptr) {
            rightHeight = right->calculateHeight();
        }
        balance = leftHeight - rightHeight;
    }
};

class AVLTree {
    Node *head = nullptr;

    // applies a left rotation on the tree
    // node: the root of the tree
    Node* leftRotate(Node *node) {
        Node *grandchild = node->right->left;
        Node *newParent = node->right;
        newParent->left = node;
        node->right = grandchild;
        return newParent;
    }

    // applies a right rotation on the tree
    // node: the root of the tree
    Node* rightRotate(Node *node) {
        Node *grandchild = node->left->right;
        Node *newParent = node->left;
        newParent->right = node;
        node->left = grandchild;
        return newParent;
    }

    // balances the tree
    // node: the root of the tree
    Node* balanceTree(Node *node) {
        node->calculateBalance();
        int bfParent = node->balance;
        if (bfParent > 1) {
            node->left->calculateBalance();
            int bfChild = node->left->balance;
            if (bfChild > 0) { // left-left case
                return rightRotate(node);
            } else if (bfChild < 0) { // left-right case
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }
        else if (bfParent < -1) {
            node->right->calculateBalance();
            int bfChild = node->right->balance;
            if (bfChild > 0) { // right-left case
                node->right = rightRotate(node->right);
                return leftRotate(node);
            } else if (bfChild < 0) { // right-right case
                return leftRotate(node);
            }
        }
        return node;
    }

public:

    void setHead(Node* node) {
        head = node;
    }

    Node* getHead() {
        return head;
    }

    // insert a node with passed in name and id into the tree
    // node: the root of the tree
    // name: name of the node to insert
    // id: id of the node to insert
    // check: indicates whether insertion is successful
    // return with the root of the resulting tree (after balance)
    Node* insert(Node* node, string name, string id, bool& check) {
        if (node == nullptr) {
            node = new Node(name, id);
            return node;
        }
        if (stoi(id) < stoi(node->id)) {
            node->left = insert(node->left, name, id, check);
            node = balanceTree(node);
        }
        else if (stoi(id) > stoi(node->id)){
            node->right = insert(node->right, name, id, check);
            node = balanceTree(node);
        }
        else {
            check = true;
        }
        return node;
    }

    // remove the node with passed in id from the tree
    // node: the root of the tree
    // id: id of the node to insert
    // check: indicates whether removal is successful
    // return the root of the new tree
    Node* remove(Node *node, string id, bool& check) {
        if (node == nullptr) {
            return node;
        }
        if (stoi(id) < stoi(node->id)) {
            node->left = remove(node->left, id, check);
        }
        else if (stoi(id) > stoi(node->id)) {
            node->right = remove(node->right, id, check);
        }
        else if (stoi(id) == stoi(node->id)) { // find location of the node
            check = true;
            if (node->left == nullptr && node->right == nullptr) { // node has 0 children
                delete node;
                return nullptr;
            }
            else if (node->left == nullptr) { // node has 1 child
                Node* newSubRoot = node->right;
                delete node;
                return newSubRoot;
            }
            else if (node->right == nullptr) {
                Node* newSubRoot = node->left;
                delete node;
                return newSubRoot;
            }
            else { // node has 2 children
                Node* successor = node->right;
                while (successor != nullptr && successor->left != nullptr) {
                    successor = successor->left;
                }
                node->name = successor->name;
                node->id = successor->id;
                node->right = remove(node->right, successor->id, check);
                return node;
            }
        }
        return node;
    }

    // searches for the node with passed in id from the tree
    // node: the root of the tree
    // id: id of the node to insert
    // prints out the name of the node with matching id, unsuccessful if not found
    void searchID(Node *node, string id) {
        if (node == nullptr) {
            cout << "unsuccessful" << endl;
            return;
        }
        if (stoi(id) < stoi(node->id)) {
            searchID(node->left, id);
        }
        else if (stoi(id) > stoi(node->id)) {
            searchID(node->right, id);
        }
        else {
            cout << node->name << endl;
            return;
        }
    }

    // searches for the node with passed in name from the tree
    // node: the root of the tree
    // name: name of the node to insert
    // check: indicates whether name is found
    // prints out the id number of each node with matching name
    void searchName(Node *node, string name, bool& check) {
        if (node == nullptr) {
            return;
        }
        if (node->name == name) {
            check = true;
            cout << node->id << endl;
        }
        searchName(node->left, name, check);
        searchName(node->right, name, check);
    }

    // stores each node in a passed-in vector using an inorder traversal
    // node: the root of the tree
    // vect: passed-in vector
    void printInOrder(Node* node, vector<Node*>& vect) {
        if (node == nullptr) {
            return;
        }
        printInOrder(node->left, vect);
        vect.push_back(node);
        printInOrder(node->right, vect);
    }

    // stores each node in a passed-in vector using an preorder traversal
    // node: the root of the tree
    // vect: passed-in vector
    void printPreorder(Node* node, vector<Node*>& vect) {
        if (node == nullptr) {
            return;
        }
        vect.push_back(node);
        printPreorder(node->left, vect);
        printPreorder(node->right, vect);
    }

    // stores each node in a passed-in vector using an postorder traversal
    // node: the root of the tree
    // vect: passed-in vector
    void printPostorder(Node* node, vector<Node*>& vect) {
        if (node == nullptr) {
            return;
        }
        printPostorder(node->left, vect);
        printPostorder(node->right, vect);
        vect.push_back(node);
    }

    // prints the number of levels in the tree by accessing the height of the root node
    // node: the root of the tree
    void printLevelCount(Node* node) {
        if (node == nullptr) {
            cout << 0 << endl;
        }
        else {
            node->calculateHeight();
            cout << node->height << endl;
        }
    }

    // removes the Nth node from the tree when traversing inorder
    // node: the root of the tree
    // n: the node you want to remove
    // check: indicates whether removal is successful
    void removeInorder(Node* node, int n, bool& check)
    {
        int count = 0;
        stack<Node *> s;
        Node *curr = node;

        while (curr != nullptr || !s.empty())
        {
            while (curr != nullptr)
            {
                s.push(curr); // stores the nodes in a stack
                curr = curr->left;
            }
            curr = s.top();
            if(count == n) {
                check = true;
                bool remCheck = false;
                remove(node, curr->id, remCheck);
                return;
            }
            s.pop();
            count++;
            curr = curr->right;
        }
    }

    // checks that the passed-in name is valid
    static bool checkValidName(string name) {
        std::regex goodName("^[A-Za-z\\s]+$");
        return std::regex_match(name, goodName);
    }

    // checks that the passed-in id is valid
    static bool checkValidID(string id) {
        std::regex goodID("^[0-9]{8,8}$");
        return std::regex_match(id, goodID);
    }
};

int main() {

    AVLTree tree = AVLTree();
    int height = 0;

    string line;

    int numCommands = 0;
    getline(cin, line);
    numCommands = stoi(line);

    for (int i = 0; i < numCommands; i++) {
        getline(cin, line);
        stringstream streamVal(line);

        string function;
        streamVal >> function;

        if (function == "insert") {
            string inputName, inputID;
            getline(streamVal, inputName, '\"');
            getline(streamVal, inputName, '\"');
            getline(streamVal, inputID, ' ');
            getline(streamVal, inputID);
            if (AVLTree::checkValidName(inputName)) {
                if (AVLTree::checkValidID(inputID)) {
                    bool dupID = false;
                    Node *pHead = tree.insert(tree.getHead(), inputName, inputID, dupID);
                    tree.setHead(pHead);
                    if (!dupID) {
                        cout << "successful" << endl;
                    }
                    else {
                        cout << "unsuccessful" << endl;
                    }
                }
                else {
                    cout << "unsuccessful" << endl;
                }
            }
            else {
                cout << "unsuccessful" << endl;
            }
        }

        if (function == "search") {
            string inputName, inputID;
            if (line.find('\"') != string::npos) {
                getline(streamVal, inputName, '\"');
                getline(streamVal, inputName, '\"');
                if (AVLTree::checkValidName(inputName)) {
                    bool found = false;
                    tree.searchName(tree.getHead(), inputName, found);
                    if (!found) {
                        cout << "unsuccessful" << endl;
                    }
                }
            }
            else {
                getline(streamVal, inputID, ' ');
                getline(streamVal, inputID);
                if (AVLTree::checkValidID(inputID)) {
                    tree.searchID(tree.getHead(), inputID);
                }
                else {
                    cout << "unsuccessful" << endl;
                }
            }
        }

        if (function == "remove") {
            string string1;
            streamVal >> string1;
            if (AVLTree::checkValidID(string1)) {
                bool foundID = false;
                tree.setHead(tree.remove(tree.getHead(), string1, foundID));
                if (!foundID) {
                    cout << "unsuccessful" << endl;
                }
                else {
                    cout << "successful" << endl;
                }
            }
            else {
                cout << "unsuccessful" << endl;
            }
        }

        if (function == "printInorder") {
            vector<Node*> nodeVect;
            tree.printInOrder(tree.getHead(), nodeVect);
            cout << nodeVect[0]->name;
            for (int i = 1; i < nodeVect.size(); i++) {
                cout << ", " << nodeVect[i]->name;
            }
            cout << endl;
        }

        if (function == "printPreorder") {
            vector<Node*> nodeVect;
            tree.printPreorder(tree.getHead(), nodeVect);
            cout << nodeVect[0]->name;
            for (int i = 1; i < nodeVect.size(); i++) {
                cout << ", " << nodeVect[i]->name;
            }
            cout << endl;
        }

        if (function == "printPostorder") {
            vector<Node*> nodeVect;
            tree.printPostorder(tree.getHead(), nodeVect);
            cout << nodeVect[0]->name;
            for (int i = 1; i < nodeVect.size(); i++) {
                cout << ", " << nodeVect[i]->name;
            }
            cout << endl;
        }

        if (function == "printLevelCount") {
            tree.printLevelCount(tree.getHead());
        }

        if (function == "removeInorder") {
            bool validParam = true;
            string string1;
            streamVal >> string1;
            for (int i = 0; i < string1.length(); i++) { // check that N is a numerical value
                if (isdigit(string1[i]) == 0) {
                    validParam = false;
                }
            }
            if (validParam) {
                bool foundNode = false;
                tree.removeInorder(tree.getHead(), stoi(string1), foundNode);
                if (!foundNode) {
                    cout << "unsuccessful" << endl;
                }
                else {
                    cout << "successful" << endl;
                }
            }
            else {
                cout << "unsuccessful" << endl;
            }
        }
    }
}
#include <iostream>
#include <string>
#include <queue>
using namespace std;

class Node {
public:
    string key;
    string value;
    Node* left;
    Node* right;

    Node(string k, string v) {
        key = k;
        value = v;
        left = right = nullptr;
    }
};

class Dictionary {
private:
    Node* root;

    Node* insert(Node* node, string k, string v) {
        if (node == nullptr)
            return new Node(k, v);

        if (k < node->key)
            node->left = insert(node->left, k, v);
        else if (k > node->key)
            node->right = insert(node->right, k, v);
        else
            node->value = v;   

        return node;
    }

    Node* minValueNode(Node* node) {
        Node* curr = node;
        while (curr && curr->left != nullptr)
            curr = curr->left;
        return curr;
    }

    Node* deleteNode(Node* node, string k) {
        if (node == nullptr)
            return node;

        if (k < node->key)
            node->left = deleteNode(node->left, k);
        else if (k > node->key)
            node->right = deleteNode(node->right, k);
        else {
            // One or zero child
            if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            // Two children
            Node* temp = minValueNode(node->right);
            node->key = temp->key;
            node->value = temp->value;
            node->right = deleteNode(node->right, temp->key);
        }
        return node;
    }

    Node* search(Node* node, string k) {
        if (node == nullptr || node->key == k)
            return node;

        if (k < node->key)
            return search(node->left, k);
        return search(node->right, k);
    }

    void inorder(Node* node) {
        if (node == nullptr) return;
        inorder(node->left);
        cout << node->key << " : " << node->value << endl;
        inorder(node->right);
    }

    void mirror(Node* node) {
        if (node == nullptr) return;
        swap(node->left, node->right);
        mirror(node->left);
        mirror(node->right);
    }

    Node* copyTree(Node* node) {
        if (node == nullptr) return nullptr;

        Node* newNode = new Node(node->key, node->value);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        return newNode;
    }

public:
    Dictionary() {
        root = nullptr;
    }

    // 1. Insert
    void insertWord(string k, string v) {
        root = insert(root, k, v);
    }

    // 2. Delete
    void deleteWord(string k) {
        root = deleteNode(root, k);
    }

    // 3. Search
    void searchWord(string k) {
        Node* result = search(root, k);
        if (result)
            cout << "Found: " << result->key << " : " << result->value << endl;
        else
            cout << "Word not found\n";
    }

    // 4. Display (Inorder Traversal)
    void display() {
        inorder(root);
    }

    // 5. Mirror Image
    void mirrorDictionary() {
        mirror(root);
    }

    // 6. Copy Dictionary
    Dictionary createCopy() {
        Dictionary newDict;
        newDict.root = copyTree(root);
        return newDict;
    }

    // 7. Level-wise Display
    void levelOrderDisplay() {
        if (root == nullptr) return;

        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* temp = q.front();
            q.pop();

            cout << temp->key << " : " << temp->value << "  ";

            if (temp->left)
                q.push(temp->left);
            if (temp->right)
                q.push(temp->right);
        }
        cout << endl;
    }
};
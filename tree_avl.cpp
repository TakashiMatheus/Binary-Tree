#include <iostream>
#include <algorithm>

using namespace std;

class Node {
    private:
        int key;
        int height;
        Node* right;
        Node* left;
    public:

        Node(int _key) : left(nullptr), right(nullptr), height(1), key(_key) {};

        // getters
        int get_height(){
            return this->height;
        }

        int get_key() const{
            return this->key;
        }

        Node* get_left() const {
            return this->left;
        }

        Node* get_right() const {
            return this->right;
        }

        // seters

        void set_height(int value){
            this->height = value;
        }

        void set_key(int value){
            this->key = value;
        }

        void set_left(Node* new_node){
            this->left = new_node;
        }

        void set_right(Node* new_node){
            this->right = new_node;
        }

};

class Rb_tree{
    private:
        Node* root;

        void destroy_tree(Node* root){
            if(root == nullptr) return;
            destroy_tree(root->get_left());
            destroy_tree(root->get_right());
            delete root;
        }
    public:

    Rb_tree() : root(nullptr){}

    ~Rb_tree() {
        destroy_tree(root);
    }

    int height (Node* N){
        if(N == nullptr){
            return 0;
        }
        return N->get_height();
    }

    Node* insert(Node* node, int value) {
    if (node == nullptr) {
        return new Node(value);
    }

    if (value < node->get_key()) {
        node->set_left(insert(node->get_left(), value));
    } else {
        node->set_right(insert(node->get_right(), value));
    }

    node->set_height(max(height(node->get_left()), height(node->get_right())) + 1);

    int balance = getBalance(node);

    // Rotação direita (RR)
    if (balance > 1 && value < node->get_left()->get_key()) {
        return rightRotate(node);
    }
    // Rotação esquerda (LL)
    if (balance < -1 && value > node->get_right()->get_key()) {
        return leftRotate(node);
    }
    // Rotação esquerda-direita (LR)
    if (balance > 1 && value > node->get_left()->get_key()) {
        node->set_left(leftRotate(node->get_left()));
        return rightRotate(node);
    }
    // Rotação direita-esquerda (RL)
    if (balance < -1 && value < node->get_right()->get_key()) {
        node->set_right(rightRotate(node->get_right()));
        return leftRotate(node);
    }

    return node;
}

    Node* rightRotate(Node* y){
        Node* x = y->get_left();
        Node* T2 = x->get_right();

        x->set_right(y);
        y->set_left(T2);

        y->set_height(max(height(y->get_left()), height(y->get_right()) ) + 1);
        x->set_height(max(height(x->get_left()), height(x->get_right())) + 1);

        return x;
    }

    Node* leftRotate(Node* x){
        Node* y = x->get_right();
        Node* T2 = y->get_left();

        y->set_left(x);
        x->set_right(T2);

        x->set_height(max(height(x->get_left()), height(x->get_right())) + 1);
        y->set_height(max(height(y->get_left()), height(y->get_right())) + 1);

        return y;
    }

    int getBalance(Node* N){
        if(N == nullptr){
            return 0;
        }
        return height(N->get_left()) - height(N->get_right());
    }

    void preOrder(Node* root) {
        if(root != nullptr){
            preOrder(root->get_left());
            cout << root->get_key();
            preOrder(root->get_right());
        }
    }

    Node* minValueNode(Node* node){
        Node* current = node;

        while(current->get_left() != nullptr){
            current = current->get_left();
        }

        return current;
    }

    Node* deleteNode(Node* root, int value){

        if(root ==nullptr){
            return root;
        }

        if(value < root->get_key()){
            root->set_left(deleteNode(root->get_left(), value));
        }

        else if(value > root->get_key()){
                root->set_right(deleteNode(root->get_right(), value));
        }

        else {

            if( (root->get_left() == nullptr) || (root->get_right() == nullptr)){
                Node* temp;
                if(root->get_left() != nullptr){
                    temp = root->get_left();
                }
                else{
                    temp = root->get_right();
                }

                if(temp == nullptr){
                    temp = root;
                    root = nullptr;
                }
                else{
                    root = temp;
                }

                temp = nullptr;
            }
            else{
                Node* temp = minValueNode(root->get_right());

                root->set_key(temp->get_key());

                root->set_right(deleteNode(root->get_right(), temp->get_key()));
            }
        }

        if(root == nullptr){
            return root;
        }

        root->set_height(max(height(root->get_left()), height(root->get_right())) + 1);

        int balance = getBalance(root);

        if(balance > 1 && getBalance(root->get_left()) >= 0){
            return rightRotate(root);
        }

        if(balance > 1 && getBalance(root->get_left()) < 0){
            root->set_left(leftRotate(root->get_left()));
        }

        if(balance < -1 && getBalance(root->get_right()) <= 0){
            return leftRotate(root);
        }

        if(balance < -1 && getBalance(root->get_right()) > 0){
            root->set_right(rightRotate(root->get_right()));
            return leftRotate(root);
        }
    
        return root;
    }

    Node* search(Node* N, int value) {
        Node* x = N;

        if (x == nullptr) {
            cout << "Árvore vazia!\n";
            return nullptr;
        }

        while (x != nullptr && x->get_key() != value) {
            if (value < x->get_key()) {
                x = x->get_left();
            } else {
                x = x->get_right();
            }
        }

        return x;
}

    void print_tree(Node* no) {
        if (no == nullptr) {
            return;
        }
        cout << "Altura: " << no->get_height() << " No: " << no->get_key();

        if (no->get_left() != nullptr) {
            cout << ", Esquerda: " << no->get_left()->get_key();
        }

        if (no->get_right() != nullptr) {
            cout << ", Direita: " << no->get_right()->get_key();
        }
        cout << "\n";

        if (no->get_left() != nullptr) {
            print_tree(no->get_left());
        }

        if (no->get_right() != nullptr) {
            print_tree(no->get_right());
        }
    }

};

int main() {
    int tamanho = 12;
    int array[tamanho] = {5, 6, 4, 3, 2, 1, 0, 11, 14, 13, 20, 19};

    Node* root = nullptr;
    Rb_tree tree; // Melhor não usar new (evitar memory leaks)

    for(int i = 0; i < tamanho; i++) {
        root = tree.insert(root, array[i]);
        cout << "Inserido: " << array[i] << " | Root atual: " << (root ? root->get_key() : -1) << endl;
    }

    if (root != nullptr) {
        cout << "\nÁrvore construída:\n";
        tree.print_tree(root);
    } else {
        cout << "Erro: A árvore está vazia!\n";
    }

    Node* search = tree.search(root, 19);
    if(search == nullptr){
        cout << "Esse número não existe na árvore\n";
    }
    else{
        cout<< "O número: " << search->get_key() <<" esta na altura " << search->get_height();
    }

    return 0;
}
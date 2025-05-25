#include <iostream>

using namespace std;

class Node {
    private:
        int key;
        int height;
        int blf;
        Node* father;
        Node* right;
        Node* left;
    public:

        Node(Node* _father, int _key) : father(_father), left(nullptr), right(nullptr), height(1), blf(0), key(_key) {};

        // getters


        int get_blf(){
            return this->blf;
        }

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

        Node* get_father() const {
            return this->father;
        }

        // seters

        void set_blf(){
            this->blf = this->left->get_height() - this->right->get_height();
        }

        void increase_height(){ 
            this->height += 1;
        }

        void set_left(Node* new_node){
            this->left = new_node;
        }

        void set_right(Node* new_node){
            this->right = new_node;
        }

        void set_father(Node* new_node){
            this->father = new_node;
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

    Rb_tree(Node* _root) : root(_root){}

    ~Rb_tree() {
        destroy_tree(root);
    }

    void build_tree(int* array_numbers, int size){
            for(int i = 1; i < size; i++){
                insert(this->root, array_numbers[i]);
            }
        }

    void insert(Node*father, int number){
        if(father->get_key() < number){
            if(father->get_right() == nullptr){
                father->set_right(new Node(father, number));
                father->increase_height();
            }
            else{
                insert(father->get_right(), number);
            }
        }
        else{
            if(father->get_key() > number){
                if(father->get_left() == nullptr){
                    father->set_left(new Node(father, number));
                    father->increase_height();
                }
                else{
                    insert(father->get_left(), number);
                }
            }
        }
    }

    void print_tree(Node* no){
            cout <<"Nó: " <<no->get_key();

            if(no->get_left() != nullptr){
                cout << ", Esquerda: " << no->get_left()->get_key();
            }

            if(no->get_right() != nullptr){
                cout << ", Direita: " << no->get_right()->get_key();
            }
            
            cout << "\n";

            if(no->get_left() != nullptr){
                print_tree(no -> get_left());
            }

             if(no->get_right() != nullptr){
                print_tree(no -> get_right());
            }
        
        }

};

int main(void){
    int tamanho = 5;
    int array[tamanho] = {5, 6, 4, 3, 2};

    Node* root = new Node(nullptr, array[0]);
    Rb_tree* tree = new Rb_tree(root);
    tree->build_tree(array, tamanho);
    tree->print_tree(root);

    delete tree;
    return 0;
}
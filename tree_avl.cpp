#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <fstream>

using namespace std;

void salvar_resultados(const string& nome_arquivo, int tamanho,
                      long tempo_construcao, long tempo_busca_existente,
                      long tempo_busca_inexistente, long tempo_remocao_existente,
                      long tempo_remocao_inexistente) {
    ofstream arquivo(nome_arquivo, ios::app); // Abre em modo append

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir o arquivo " << nome_arquivo << endl;
        return;
    }

    arquivo << tamanho << ","
            << tempo_construcao << ","
            << tempo_busca_existente << ","
            << tempo_busca_inexistente << ","
            << tempo_remocao_existente << ","
            << tempo_remocao_inexistente << "\n";

    arquivo.close();
}

class Node {
    private:
        int key;
        int height;
        Node* right;
        Node* left;
    public:
        Node(int _key) : left(nullptr), right(nullptr), height(1), key(_key) {};

        // getters
        int get_height() { return this->height; }
        int get_key() const { return this->key; }
        Node* get_left() const { return this->left; }
        Node* get_right() const { return this->right; }

        // setters
        void set_height(int value) { this->height = value; }
        void set_key(int value) { this->key = value; }
        void set_left(Node* new_node) { this->left = new_node; }
        void set_right(Node* new_node) { this->right = new_node; }
};

class AVL_tree {
    public:
        AVL_tree() {}

        int height(Node* N) {
            if(N == nullptr) return 0;
            return N->get_height();
        }

        Node* insert(Node* node, int value) {
            if (node == nullptr){
                return new Node(value);
            }

            if (value < node->get_key()) {
                node->set_left(insert(node->get_left(), value));
            } else {
                node->set_right(insert(node->get_right(), value));
            }

            node->set_height(max(height(node->get_left()), height(node->get_right())) + 1);
            int balance = getBalance(node);

            if (balance > 1 && value < node->get_left()->get_key()) return rightRotate(node);
            if (balance < -1 && value > node->get_right()->get_key()) return leftRotate(node);
            if (balance > 1 && value > node->get_left()->get_key()) {
                node->set_left(leftRotate(node->get_left()));
                return rightRotate(node);
            }
            if (balance < -1 && value < node->get_right()->get_key()) {
                node->set_right(rightRotate(node->get_right()));
                return leftRotate(node);
            }

            return node;
        }

        Node* rightRotate(Node* y) {
            Node* x = y->get_left();
            Node* T2 = x->get_right();

            x->set_right(y);
            y->set_left(T2);

            y->set_height(max(height(y->get_left()), height(y->get_right())) + 1);
            x->set_height(max(height(x->get_left()), height(x->get_right())) + 1);

            return x;
        }

        Node* leftRotate(Node* x) {
            Node* y = x->get_right();
            Node* T2 = y->get_left();

            y->set_left(x);
            x->set_right(T2);

            x->set_height(max(height(x->get_left()), height(x->get_right())) + 1);
            y->set_height(max(height(y->get_left()), height(y->get_right())) + 1);

            return y;
        }

        int getBalance(Node* N) {
            if(N == nullptr) return 0;
            return height(N->get_left()) - height(N->get_right());
        }

        Node* minValueNode(Node* node) {
            Node* current = node;
            while(current->get_left() != nullptr) current = current->get_left();
            return current;
        }

        Node* deleteNode(Node* root, int value) {
            if(root == nullptr) return root;

            if(value < root->get_key()) {
                root->set_left(deleteNode(root->get_left(), value));
            } else if(value > root->get_key()) {
                root->set_right(deleteNode(root->get_right(), value));
            } else {
                if((root->get_left() == nullptr) || (root->get_right() == nullptr)) {
                    Node* temp = root->get_left() ? root->get_left() : root->get_right();
                    if(temp == nullptr) {
                        temp = root;
                        root = nullptr;
                    } else {
                        *root = *temp;
                    }
                    delete temp;
                } else {
                    Node* temp = minValueNode(root->get_right());
                    root->set_key(temp->get_key());
                    root->set_right(deleteNode(root->get_right(), temp->get_key()));
                }
            }

            if(root == nullptr) return root;

            root->set_height(max(height(root->get_left()), height(root->get_right())) + 1);
            int balance = getBalance(root);

            if(balance > 1 && getBalance(root->get_left()) >= 0) return rightRotate(root);
            if(balance > 1 && getBalance(root->get_left()) < 0) {
                root->set_left(leftRotate(root->get_left()));
                return rightRotate(root);
            }
            if(balance < -1 && getBalance(root->get_right()) <= 0) return leftRotate(root);
            if(balance < -1 && getBalance(root->get_right()) > 0) {
                root->set_right(rightRotate(root->get_right()));
                return leftRotate(root);
            }

            return root;
        }

        Node* search(Node* N, int value) {
            Node* x = N;
            while(x != nullptr && x->get_key() != value) {
                if(value < x->get_key()) x = x->get_left();
                else x = x->get_right();
            }
            return x;
        }
};

void destroy_tree(Node* root) {
            if(root == nullptr) return;
            destroy_tree(root->get_left());
            destroy_tree(root->get_right());
            delete root;
        }

void limpar_console() {
    cout << "\033[2J\033[1;1H";
}

int* gerar_array(int tamanho) {
    int* array = new int[tamanho];
    for(int i = 0; i < tamanho; i++) {
        array[i] = i + 1;
    }
    shuffle(array, array + tamanho, default_random_engine(random_device()()));
    return array;
}

void testar_operacoes(int tamanho, int teste_atual, ofstream& arquivo) {
    int* array = gerar_array(tamanho);
    AVL_tree tree;
    Node* root = nullptr;
    long tempo_construcao;

    if(tamanho == 1000){
        // Construção da árvore
        auto inicio = chrono::high_resolution_clock::now();
        for(int i = 0; i < tamanho; i++) {
            root = tree.insert(root, array[i]);
        }
        auto fim = chrono::high_resolution_clock::now();
        tempo_construcao = chrono::duration_cast<chrono::microseconds>(fim - inicio).count();
    }
    else{
      // Construção da árvore
      auto inicio = chrono::high_resolution_clock::now();
      for(int i = 0; i < tamanho; i++) {
          root = tree.insert(root, array[i]);
      }
      auto fim = chrono::high_resolution_clock::now();
      tempo_construcao = chrono::duration_cast<chrono::milliseconds>(fim - inicio).count();
    }

    // Elementos para teste
    int elemento_existente = array[tamanho/2];
    int elemento_inexistente = tamanho * 2;

    auto inicio_busca_existente = chrono::high_resolution_clock::now();
    auto dummy_existente = tree.search(root, elemento_existente);
    auto fim_busca_existente = chrono::high_resolution_clock::now();
    long double tempo_busca_existente = chrono::duration_cast<chrono::nanoseconds>(fim_busca_existente - inicio_busca_existente).count();

    auto inicio_busca_inexistente = chrono::high_resolution_clock::now();
    auto dummy = tree.search(root, elemento_inexistente);
    auto fim_busca_inexistente = chrono::high_resolution_clock::now();
    long double tempo_busca_inexistente = chrono::duration_cast<chrono::nanoseconds>(fim_busca_inexistente - inicio_busca_inexistente).count();


    auto inicio_remocao_existente = chrono::high_resolution_clock::now();
    root = tree.deleteNode(root, elemento_existente);
    auto fim_remocao_existente = chrono::high_resolution_clock::now();
    long double tempo_remocao_existente = chrono::duration_cast<chrono:: nanoseconds>(fim_remocao_existente - inicio_remocao_existente).count();


    auto inicio_remocao_inexistente = chrono::high_resolution_clock::now();
    root = tree.deleteNode(root, elemento_inexistente);
    auto fim_remocao_inexistente = chrono::high_resolution_clock::now();
    long double tempo_remocao_inexistente = chrono::duration_cast<chrono::nanoseconds>(fim_remocao_inexistente - inicio_remocao_inexistente).count();


    // Exibir resultados
    cout << "Teste " << teste_atual + 1 << ":\n";
    if(tamanho == 1000){
      cout << "Tempo construcao: " << tempo_construcao << "micro\n";
    }
    else{
      cout << "Tempo construcao: " << tempo_construcao << "ms\n";
    }
    
    cout << "Tempo busca (existente): " << tempo_busca_existente << "ns\n";
    cout << "Tempo busca (inexistente): " << tempo_busca_inexistente << "ns\n";
    cout << "Tempo remocao (existente): " << tempo_remocao_existente << "ns\n";
    cout << "Tempo remocao (inexistente): " << tempo_remocao_inexistente << "ns\n\n";

    // Salvar no arquivo
    arquivo << tamanho << ","
            << tempo_construcao << ","
            << tempo_busca_existente << ","
            << tempo_busca_inexistente << ","
            << tempo_remocao_existente << ","
            << tempo_remocao_inexistente << "\n";

    delete[] array;
    destroy_tree(root);

}

int main() {
    bool executar = true;
    while(executar) {
        limpar_console();
        cout << "Escolha o tamanho do array:\n"
             << "1. 1.000\n2. 10.000\n3. 100.000\n4. 1.000.000\n5. 10.000.000\n6. Sair\n";

        int opcao;
        cin >> opcao;

        int tamanho;
        switch(opcao) {
            case 1: tamanho = 1000; break;
            case 2: tamanho = 10000; break;
            case 3: tamanho = 100000; break;
            case 4: tamanho = 1000000; break;
            case 5: tamanho = 10000000; break;
            case 6: executar = false; continue;
            default:
                cout << "Opcao invalida!\n";
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
        }

        // Criar arquivo com nome baseado no tamanho
        string nome_arquivo = "resultados_" + to_string(tamanho) + ".csv";
        ofstream arquivo(nome_arquivo);

        // Cabeçalho do arquivo CSV
        arquivo << "Tamanho,TempoConstrucao(ms),BuscaExistente(ns),BuscaInexistente(ns),RemocaoExistente(ns),RemocaoInexistente(ns)\n";

        for(int i = 0; i < 50; i++) {
            testar_operacoes(tamanho, i, arquivo);
        }

        arquivo.close();
        cout << "\nResultados salvos em " << nome_arquivo << "\n";
        cout << "Pressione Enter para continuar...";
        cin.ignore();
        cin.get();
    }
    return 0;
}

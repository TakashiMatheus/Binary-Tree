    #include <iostream>
    #include <algorithm>
    #include <chrono>
    #include <random>
    #include <string>
    #include <fstream>

    using namespace std;

    enum Color { RED, BLACK };

    struct RBNode {
        int data;
        bool color;
        RBNode *left, *right, *parent;
        RBNode(int data): data(data), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    class RBTree {
    private:
        RBNode* root;

        void rotateLeft(RBNode*& root, RBNode*& pt) {
            RBNode* pt_right = pt->right;
            pt->right = pt_right->left;

            if (pt->right != nullptr) pt->right->parent = pt;

            pt_right->parent = pt->parent;

            if (pt->parent == nullptr) root = pt_right;
            else if (pt == pt->parent->left) pt->parent->left = pt_right;
            else pt->parent->right = pt_right;

            pt_right->left = pt;
            pt->parent = pt_right;
        }

        void rotateRight(RBNode*& root, RBNode*& pt) {
            RBNode* pt_left = pt->left;
            pt->left = pt_left->right;

            if (pt->left != nullptr) pt->left->parent = pt;

            pt_left->parent = pt->parent;

            if (pt->parent == nullptr) root = pt_left;
            else if (pt == pt->parent->left) pt->parent->left = pt_left;
            else pt->parent->right = pt_left;

            pt_left->right = pt;
            pt->parent = pt_left;
        }

        void fixInsert(RBNode*& root, RBNode*& pt) {
            while ((pt != root) && (pt->color != BLACK) && (pt->parent->color == RED)) {
                RBNode* parent_pt = pt->parent;
                RBNode* grand_parent_pt = pt->parent->parent;

                if (parent_pt == grand_parent_pt->left) {
                    RBNode* uncle_pt = grand_parent_pt->right;

                    if (uncle_pt != nullptr && uncle_pt->color == RED) {
                        grand_parent_pt->color = RED;
                        parent_pt->color = BLACK;
                        uncle_pt->color = BLACK;
                        pt = grand_parent_pt;
                    } else {
                        if (pt == parent_pt->right) {
                            rotateLeft(root, parent_pt);
                            pt = parent_pt;
                            parent_pt = pt->parent;
                        }
                        rotateRight(root, grand_parent_pt);
                        swap(parent_pt->color, grand_parent_pt->color);
                        pt = parent_pt;
                    }
                } else {
                    RBNode* uncle_pt = grand_parent_pt->left;

                    if ((uncle_pt != nullptr) && (uncle_pt->color == RED)) {
                        grand_parent_pt->color = RED;
                        parent_pt->color = BLACK;
                        uncle_pt->color = BLACK;
                        pt = grand_parent_pt;
                    } else {
                        if (pt == parent_pt->left) {
                            rotateRight(root, parent_pt);
                            pt = parent_pt;
                            parent_pt = pt->parent;
                        }
                        rotateLeft(root, grand_parent_pt);
                        swap(parent_pt->color, grand_parent_pt->color);
                        pt = parent_pt;
                    }
                }
            }

            root->color = BLACK;
        }

        void fixDelete(RBNode*& root, RBNode*& x) {
            while (x != root && x->color == BLACK) {
                if (x == x->parent->left) {
                    RBNode* sibling = x->parent->right;

                    if (sibling->color == RED) {
                        sibling->color = BLACK;
                        x->parent->color = RED;
                        rotateLeft(root, x->parent);
                        sibling = x->parent->right;
                    }

                    if ((sibling->left == nullptr || sibling->left->color == BLACK) &&
                        (sibling->right == nullptr || sibling->right->color == BLACK)) {
                        sibling->color = RED;
                        x = x->parent;
                    } else {
                        if (sibling->right == nullptr || sibling->right->color == BLACK) {
                            sibling->left->color = BLACK;
                            sibling->color = RED;
                            rotateRight(root, sibling);
                            sibling = x->parent->right;
                        }

                        sibling->color = x->parent->color;
                        x->parent->color = BLACK;
                        sibling->right->color = BLACK;
                        rotateLeft(root, x->parent);
                        x = root;
                    }
                } else {
                    RBNode* sibling = x->parent->left;

                    if (sibling->color == RED) {
                        sibling->color = BLACK;
                        x->parent->color = RED;
                        rotateRight(root, x->parent);
                        sibling = x->parent->left;
                    }

                    if ((sibling->right == nullptr || sibling->right->color == BLACK) &&
                        (sibling->left == nullptr || sibling->left->color == BLACK)) {
                        sibling->color = RED;
                        x = x->parent;
                    } else {
                        if (sibling->left == nullptr || sibling->left->color == BLACK) {
                            sibling->right->color = BLACK;
                            sibling->color = RED;
                            rotateLeft(root, sibling);
                            sibling = x->parent->left;
                        }

                        sibling->color = x->parent->color;
                        x->parent->color = BLACK;
                        sibling->left->color = BLACK;
                        rotateRight(root, x->parent);
                        x = root;
                    }
                }
            }
            x->color = BLACK;
        }

        void transplant(RBNode*& root, RBNode* u, RBNode* v) {
            if (u->parent == nullptr) {
                root = v;
            } else if (u == u->parent->left) {
                u->parent->left = v;
            } else {
                u->parent->right = v;
            }

            if (v != nullptr) {
                v->parent = u->parent;
            }
        }

        RBNode* minimum(RBNode* node) {
            while (node->left != nullptr) {
                node = node->left;
            }
            return node;
        }

        void inorderDelete(RBNode* node) {
        if (!node) return;
        inorderDelete(node->left);
        inorderDelete(node->right);
        delete node;
    }

    RBNode* searchNode(RBNode* root, int key) {
        while (root != nullptr && root->data != key) {
            if (key < root->data) root = root->left;
            else root = root->right;
        }
        return root;
    }

    public:
        RBTree(): root(nullptr) {}

        void insert(const int& data) {
            RBNode* pt = new RBNode(data);
            RBNode* y = nullptr;
            RBNode* x = root;

            while (x != nullptr) {
                y = x;
                if (pt->data < x->data) x = x->left;
                else x = x->right;
            }

            pt->parent = y;

            if (y == nullptr) root = pt;
            else if (pt->data < y->data) y->left = pt;
            else y->right = pt;

            fixInsert(root, pt);
        }

        bool search(int key) {
            return searchNode(root, key) != nullptr;
        }

        void clear() {
            inorderDelete(root);
            root = nullptr;
        }

        bool remove(int key) {
          RBNode* z = searchNode(root, key);
          if (z == nullptr) return false;

          RBNode* y = z;
          RBNode* x;
          bool y_original_color = y->color;

          if (z->left == nullptr) {
              x = z->right;
              transplant(root, z, z->right);
          } else if (z->right == nullptr) {
              x = z->left;
              transplant(root, z, z->left);
          } else {
              y = minimum(z->right);
              y_original_color = y->color;
              x = y->right;

              if (y->parent == z) {
                  if (x != nullptr) x->parent = y;
              } else {
                  transplant(root, y, y->right);
                  y->right = z->right;
                  y->right->parent = y;
              }

              transplant(root, z, y);
              y->left = z->left;
              y->left->parent = y;
              y->color = z->color;
          }

          delete z;

          if (y_original_color == BLACK && x != nullptr) {
              fixDelete(root, x);
          }

          return true;
      }
};

    // Funções auxiliares de benchmark
    void limpar_console() {
        cout << "\033[2J\033[1;1H";
    }

    int* gerar_array(int tamanho) {
        int* array = new int[tamanho];
        for(int i = 0; i < tamanho; i++) array[i] = i + 1;
        shuffle(array, array + tamanho, default_random_engine(random_device()()));
        return array;
    }

    void testar_operacoes(int tamanho, int teste_atual, ofstream& arquivo) {
        long tempo_construcao;
        int* array = gerar_array(tamanho);
        RBTree tree;

        if(tamanho == 1000) {
            auto inicio = chrono::high_resolution_clock::now();
            for(int i = 0; i < tamanho; i++) {
                tree.insert(array[i]);
            }
            auto fim = chrono::high_resolution_clock::now();
            tempo_construcao = chrono::duration_cast<chrono::microseconds>(fim - inicio).count();
        } else {
            auto inicio = chrono::high_resolution_clock::now();
            for(int i = 0; i < tamanho; i++) {
                tree.insert(array[i]);
            }
            auto fim = chrono::high_resolution_clock::now();
            tempo_construcao = chrono::duration_cast<chrono::milliseconds>(fim - inicio).count();
        }

        int elemento_existente = array[tamanho/2];
        int elemento_inexistente = tamanho * 2;

        auto inicio_busca_existente = chrono::high_resolution_clock::now(); 
        auto dummy_existente = tree.search(elemento_existente);
        auto fim_busca_inexistente = chrono::high_resolution_clock::now();
        long tempo_busca_existente = chrono::duration_cast<chrono::nanoseconds>(fim_busca_inexistente - inicio_busca_existente).count();

        auto inicio_busca = chrono::high_resolution_clock::now();
        auto dummy = tree.search(elemento_inexistente);
        auto fim_busca = chrono::high_resolution_clock::now();
        long tempo_busca_inexistente = chrono::duration_cast<chrono::nanoseconds>(fim_busca - inicio_busca).count();
  

        RBTree tree_para_delecao;
        for(int i = 0; i < tamanho; i++) {
            tree_para_delecao.insert(array[i]);
        }

        auto inicio_delecao = chrono::high_resolution_clock::now();
        bool remocao_existente = tree_para_delecao.remove(elemento_existente);
        auto fim_delecao = chrono::high_resolution_clock::now();
        long tempo_remocao_existente = chrono::duration_cast<chrono::nanoseconds>(fim_delecao - inicio_delecao).count();

        inicio_delecao = chrono::high_resolution_clock::now();
        bool remocao_inexistente = tree_para_delecao.remove(elemento_inexistente);
        fim_delecao = chrono::high_resolution_clock::now();
        long tempo_remocao_inexistente = chrono::duration_cast<chrono::nanoseconds>(fim_delecao - inicio_delecao).count();

        cout << "Teste " << teste_atual + 1 << ":\n";
        if (tamanho == 1000)
            cout << "Tempo construcao: " << tempo_construcao << " microseg\n";
        else
            cout << "Tempo construcao: " << tempo_construcao << " ms\n";

        cout << "Tempo busca (existente): " << tempo_busca_existente << "ns\n";
        cout << "Tempo busca (inexistente): " << tempo_busca_inexistente << "ns\n";
        cout << "Tempo remocao (existente): " << tempo_remocao_existente << "ns ("
            << (remocao_existente ? "sucesso" : "falha") << ")\n";
        cout << "Tempo remocao (inexistente): " << tempo_remocao_inexistente << "ns ("
            << (remocao_inexistente ? "sucesso" : "falha") << ")\n\n";

        arquivo << tamanho << ","
                << tempo_construcao << ","
                << tempo_busca_existente << ","
                << tempo_busca_inexistente << ","
                << tempo_remocao_existente << ","
                << tempo_remocao_inexistente << "\n";

        delete[] array;
        tree.clear();
        tree_para_delecao.clear();
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
                case 5: tamanho = 10000'000; break;
                case 6: executar = false; continue;
                default:
                    cout << "Opcao invalida!\n";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    continue;
            }

            string nome_arquivo = "resultados_rb_" + to_string(tamanho) + ".csv";
            ofstream arquivo(nome_arquivo);
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

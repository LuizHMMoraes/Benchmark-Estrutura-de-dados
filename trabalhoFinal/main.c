#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definições para a Lista Duplamente Encadeada
typedef struct Node {
    int user_id;
    char password[50];
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct DoublyLinkedList {
    Node* head;
    Node* tail;
} DoublyLinkedList;

DoublyLinkedList* create_list() {
    DoublyLinkedList* list = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    list->head = list->tail = NULL;
    return list;
}

void insert_list(DoublyLinkedList* list, int user_id, const char* password) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->user_id = user_id;
    strcpy(new_node->password, password);
    new_node->prev = list->tail;
    new_node->next = NULL;
    if (list->tail) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }
    list->tail = new_node;
}

char* search_list(DoublyLinkedList* list, int user_id) {
    Node* current = list->head;
    while (current) {
        if (current->user_id == user_id) {
            return current->password;
        }
        current = current->next;
    }
    return NULL;
}

// Definições para a Árvore AVL
typedef struct AVLNode {
    int user_id;
    char password[50];
    struct AVLNode* left;
    struct AVLNode* right;
    int height;
} AVLNode;

int max(int a, int b) {
    return (a > b) ? a : b;
}

int height(AVLNode* node) {
    return (node == NULL) ? 0 : node->height;
}

AVLNode* create_avl_node(int user_id, const char* password) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    node->user_id = user_id;
    strcpy(node->password, password);
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

AVLNode* right_rotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode* left_rotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int get_balance(AVLNode* node) {
    return (node == NULL) ? 0 : height(node->left) - height(node->right);
}

AVLNode* insert_avl(AVLNode* node, int user_id, const char* password) {
    if (node == NULL) {
        return create_avl_node(user_id, password);
    }
    if (user_id < node->user_id) {
        node->left = insert_avl(node->left, user_id, password);
    } else if (user_id > node->user_id) {
        node->right = insert_avl(node->right, user_id, password);
    } else {
        return node;
    }

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = get_balance(node);

    if (balance > 1 && user_id < node->left->user_id) {
        return right_rotate(node);
    }
    if (balance < -1 && user_id > node->right->user_id) {
        return left_rotate(node);
    }
    if (balance > 1 && user_id > node->left->user_id) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }
    if (balance < -1 && user_id < node->right->user_id) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }
    return node;
}

char* search_avl(AVLNode* node, int user_id) {
    if (node == NULL) {
        return NULL;
    }
    if (user_id == node->user_id) {
        return node->password;
    }
    if (user_id < node->user_id) {
        return search_avl(node->left, user_id);
    } else {
        return search_avl(node->right, user_id);
    }
}

// Funções para carregar dados e testar senhas
void load_data(DoublyLinkedList* list, AVLNode** avl_root, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return;
    }
    int user_id;
    char password[50];
    while (fscanf(file, "%d %s", &user_id, password) != EOF) {
        insert_list(list, user_id, password);
        *avl_root = insert_avl(*avl_root, user_id, password);
    }
    fclose(file);
}

void test_passwords(DoublyLinkedList* list, AVLNode* avl_root, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return;
    }
    int user_id;
    char password[50];
    int correct_count = 0, incorrect_count = 0;
    while (fscanf(file, "%d %s", &user_id, password) != EOF) {
        char* list_pwd = search_list(list, user_id);
        char* avl_pwd = search_avl(avl_root, user_id);
        if (list_pwd && strcmp(list_pwd, password) == 0) {
            correct_count++;
        } else {
            incorrect_count++;
        }
        if (avl_pwd && strcmp(avl_pwd, password) == 0) {
            correct_count++;
        } else {
            incorrect_count++;
        }
    }
    fclose(file);
    printf("Correct Passwords: %d\n", correct_count);
    printf("Incorrect Passwords: %d\n", incorrect_count);
}

int main() {
    DoublyLinkedList* list = create_list();
    AVLNode* avl_root = NULL;

    load_data(list, &avl_root, "data.txt");
    test_passwords(list, avl_root, "test_data.txt");

    return 0;
}

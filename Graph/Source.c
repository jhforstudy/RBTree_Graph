#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define MAX_SIZE 26 // number of alphabets( A~Z )
#define MAX_Q_SIZE 100   // max size of queue
#define AIRPLANE_SPEED 500
#define INF 100000
#define DAY_OF_ONE_MONTH 31
#define HASHTABLE_SIZE 13   // size of hash table

enum Color {
    RED = 0,
    BLACK = 1
};
enum TF {
    TRUE = 1,
    FALSE = 0
};
enum Cityname {
    A = 1,
    B = 2,
    C = 3,
    D = 4,
    E = 5,
    F = 6,
    G = 7,
    H = 8,
    I = 9,
    J = 10,
    K = 11,
    L = 12,
    M = 13,
    N = 14,
    O = 15,
    P = 16,
    Q = 17,
    R = 18,
    S = 19,
    T = 20,
    U = 21,
    V = 22,
    W = 23,
    X = 24,
    Y = 25,
    Z = 26
};
const char cityNames[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

bool checkList(int* list, int value) {

    for (int i = 0; i < 10; i++) {
        if (list[i] == value) {
            return true;
        }
    }
    return false;
}

// basic struct
struct date {
    int date;
    float hour; // 0 ~ 24�� �Ҽ���
};
char* PrintDate(struct date* d) {
    char print_str[4];
    int hour = (int)d->hour;
    int minute = (d->hour - hour) * 60;

    // 00:00 ���·� ��������

    return;
}
// struct for timetable
// ��� �ð��� ����� ��ü ���
struct FlightInfo {
    enum Cityname from;
    enum Cityname to;
    struct date from_date;
    struct date to_date;
};
struct FlightInfo* newFlightList() {
    struct FlightInfo* new_fl = (struct FlightInfo*)malloc(sizeof(struct FlightInfo) * 10);
    for (int i = 0; i < 10; i++) {
        struct FlightInfo null_fl;
        null_fl.from = A;
        null_fl.to = B;
        null_fl.from_date.date = 0;
        null_fl.from_date.hour = 0;
        null_fl.to_date.date = 0;
        null_fl.to_date.hour = 0;
        /*
        struct date null_f_date, null_t_date;
        null_f_date.date = 0;
        null_f_date.hour = 0;
        null_t_date.date = 0;
        null_t_date.hour = 0;
        null_fl.from_date = null_f_date;
        null_fl.to_date = null_t_date;*/
        new_fl[i] = null_fl;
    }
    return new_fl;
}
struct FlightInfo** newFlightInfo() {
    struct FlightInfo** new_fi = (struct FlightInfo**)malloc(sizeof(struct FlightInfo*) * MAX_SIZE);
    for (int i = 0; i < MAX_SIZE; i++) {
        struct FlightInfo* ten_flights = newFlightList();
        new_fi[i] = ten_flights;
    }
    return new_fi;
}
struct FlightInfo*** newTimetable() {
    struct FlightInfo*** new_tt = (struct FlightInfo***)malloc(sizeof(struct FlightInfo**) * DAY_OF_ONE_MONTH);
    for (int i = 0; i < DAY_OF_ONE_MONTH; i++) {
        // FlightInfo* �� �� : new_tt[i]
        struct FlightInfo** new_fi = newFlightInfo();
        new_tt[i] = new_fi;
    }
    // new_tt  :  31���� Flightinfo* �� ����ִ� �迭
    // new_tt[i] : 26���� FlightInfo�� ����ִ� �迭
    return new_tt;
}
// struct for City & Path map
// Path ������ ������.
struct PathNode {
    enum Cityname depart;  // ���� �̸�
    struct PathNode* link; // Path�� �ձ� ���� ������
    float distance;        // ���
    float time;            // �ҿ� �ð�
    struct FlightInfo depart_date;    // �����, ��߽ð�, ������, �����ð�
};
struct PathNode* newPathNode() {
    struct PathNode* path = (struct PathNode*)malloc(sizeof(struct PathNode));
    path->link = NULL;
    path->distance = 0.0;
    path->time = 0.0;
    return path;
}
struct Route {
    struct date depart_date;
    struct PathNode* start;
    int size;
};
struct Route* newRoute() {
    struct Route* new_route = (struct Route*)malloc(sizeof(struct Route));
    new_route->start = newPathNode();
    new_route->size = 0;

    return new_route;
}
// City ���� + ��������Ʈ�� ������.
struct linkedList {
    enum Cityname name;
    struct PathNode* curr_path;
    struct linkedList* next;
};
struct linkedList* newLinkedList() {
    struct linkedList* new_ll = (struct linkedList*)malloc(sizeof(struct linkedList));
    new_ll->name = A;
    new_ll->next = NULL;
    new_ll->curr_path = NULL;

    return new_ll;
}
struct CityInfo {
    // �̸�
    enum Cityname name;
    // ��ǥ
    int abs_x;
    int abs_y;
    char dir_x;
    char dir_y;
    int raw_x;
    int raw_y;
    // Path Node ����
    struct linkedList* next;
};
struct CityInfo* newCityInfo(int x, int y, enum Cityname name) {
    struct CityInfo* new_city = (struct CityInfo*)malloc(sizeof(struct CityInfo));
    // �̸�
    new_city->name = name;
    // ��ǥ (-3000 ~ +3000)
    new_city->raw_x = x;
    new_city->raw_y = y;
    // ��ǥ (W or S 3000 ~ E or N 3000)
    if (x >= 0) {
        new_city->dir_x = 'E';
    }
    else {
        new_city->dir_x = 'W';
    }
    if (y >= 0) {
        new_city->dir_y = 'N';
    }
    else {
        new_city->dir_y = 'S';
    }
    new_city->abs_x = abs(x);
    new_city->abs_y = abs(y);
    new_city->next = NULL;

    return new_city;
}
// A~Z ���� ��� ���� ������ ������.
struct GraphType {
    // ���� ����� ����
    int adj_n;
    // ���� ����Ʈ (���� ���� ����)
    struct CityInfo* adj_list[MAX_SIZE];
};
struct GraphType* newGraphType() {
    struct GraphType* g = (struct GraphType*)malloc(sizeof(struct GraphType));
    int v;
    g->adj_n = 0;
    // adj_list �ʱ�ȭ
    for (v = 0; v < MAX_SIZE; v++)
        g->adj_list[v] = NULL;
    return g;
}
// struct for reservation system
struct reservation {
    char* name;
    struct Route* path;
    struct date* date;
};
struct node {
    // key, left, right, parent, color
    int key;
    struct node* left;
    struct node* right;
    struct node* parent;
    enum Color color;
    struct reservation* data;
};
// struct for RB-tree
struct rbtree {
    struct node* root;
    struct node* NIL;
};
struct node create_nil = {
  .color = BLACK,
  .left = NULL,
  .right = NULL,
  .parent = NULL,
};
struct node* nil = &create_nil;
struct trunk {
    struct trunk* prev;
    char* str;
};


// functions for city & path map
// ���� ����
void insert_depart(struct GraphType* g, enum Cityname v) {
    int rand_int_x, rand_int_y;
    if (((g->adj_n) + 1) > MAX_SIZE) {
        // error
        fprintf(stderr, "overflow");
        return;
    }
    g->adj_n++;
    // ���� ���� �����ϰ� ����
    rand_int_x = rand() % 6001 - 3000;
    rand_int_y = rand() % 6001 - 3000;
    struct CityInfo* curr_city = newCityInfo(rand_int_x, rand_int_y, v);
    // ���� ���ø� adj_list�� �ְ�, �ֺ� ���� ������ �ʱ�ȭ
    g->adj_list[v - 1] = curr_city;
    g->adj_list[v - 1]->next = NULL;
}
struct PathNode* insert_edge(struct GraphType* g, int u, int v) {
    // u : tail (0~25)
    // v : head (0~25)
    // v -> u���� PathNode �����Ͽ�, adj_list�� �߰� (A ��������Ʈ�� �ִ� Path���� ������� A������)
    if (u >= g->adj_n || v >= g->adj_n) {
        fprintf(stderr, "depart index error");
        return;
    }
    // ������� ������ �������� ��������
    struct CityInfo* depart = g->adj_list[v];
    struct CityInfo* arrival = g->adj_list[u];
    // ���ο� path node ����
    struct PathNode* depart_path = newPathNode();
    struct PathNode* arrive_path = newPathNode();
    // head �κ��� �� ����
    enum Cityname depart_city = v+1;
    depart_path->depart = depart_city;
    // distance, time ����
    float x_diff = (depart->raw_x - arrival->raw_x);
    float y_diff = (depart->raw_y - arrival->raw_y);
    float distance = sqrt((x_diff * x_diff) + (y_diff * y_diff));
    float time = distance / AIRPLANE_SPEED;
    depart_path->distance = distance;
    depart_path->time = time;
    // ���� ���� ����
    arrive_path->depart = u + 1;
    depart_path->link = arrive_path;
    // link�� **** LinkedList ****�� �߰�!!!
    struct linkedList* new_city = newLinkedList();
    new_city->name = u + 1;
    new_city->curr_path = depart_path;
    // ���� ������� ó������ �ִ°Ÿ�,
    if (g->adj_list[v]->next == NULL) {
        // link�� �ٷ� �߰�
        g->adj_list[v]->next = new_city;
    }
    else {
        // �� ���� PathNode�� ������ ��
        struct linkedList* temp = g->adj_list[v]->next;
        // �� �տ� ���ο� Path�� �߰�
        g->adj_list[v]->next = new_city;
        // �� �ڿ� temp�� �ٿ���
        new_city->next = temp;
    }
    //printf("%c -> %c���� ��� ����\n", cityNames[depart_path->depart-1], cityNames[arrive_path->depart-1]);
    //printf("adj_list�� �߰� �Ϸ� (%.2f km), %c -> %c\n", new_city->curr_path->distance, cityNames[new_city->curr_path->depart - 1], cityNames[new_city->curr_path->link->depart-1]);
    return depart_path;
}
void print_adj_list(struct GraphType* g) {
    for (int i = 0; i < g->adj_n; i++) {
        struct linkedList* n = g->adj_list[i]->next;
        printf("���� %c�� ���� ����Ʈ", cityNames[i]);
        while (n != NULL) {
            printf("->%c", cityNames[n->name - 1]);
            n = n->next;
        }
        printf("\n");
    }
}
// to find reachable nodes
int reachable_nodes = 1;
typedef struct QueueType {
    int element[MAX_SIZE];
    int front, rear, size;
}QueueType;
enum TF visited[MAX_SIZE];
QueueType* queue_init() {
    QueueType* q = (QueueType*)malloc(sizeof(QueueType));
    q->size = 0;
    q->front = -1;
    q->rear = -1;
    for (int i = 0; i < MAX_SIZE; i++) {
        q->element[i] = 0;
    }
    return q;
}
void enqueue(QueueType* q, int item) {
    q->element[++(q->rear)] = item;
    q->size++;
}
int dequeue(QueueType* q) {
    int item;
    item = q->element[++(q->front)];
    q->size--;
    return item;
}

void dfs(struct GraphType* g, struct Route* whole_route, enum Cityname dep, enum Cityname arr) {
    // �ٳణ ������ ǥ��
    visited[dep-1] = TRUE;
    // whole_route�� �� ���� �߰�
    struct PathNode* temp = whole_route->start;
    if (whole_route->size == 0) {
    }
    else {
        while (temp->link != NULL) {
            temp = temp->link;
        }
        // ������ ������, ����� ����
        temp->link = newPathNode();
        temp->link->depart = dep;
    }
    whole_route->size++;

    if (dep == arr) {
        // ���� ��� �����ֱ�
        /*
        temp = whole_route->start;
        while (temp != NULL) {
            printf("%c -> ", cityNames[temp->depart - 1]);
            temp = temp->link;
        }*/
        return;
    }
    struct linkedList* st = g->adj_list[dep-1]->next;
    int cnt = 0;
    while (st!=NULL) {
        // �� ó�� ���� ��尡 ������
        if (st->name == whole_route->start->depart) {
            cnt++;
            if (cnt == 10) {
                whole_route->start = NULL;
                return;
            }
            st = st->next;
            continue;
        }
        if (!visited[st->name - 1]) {
            // �湮���� �����Ÿ�, ã��
            dfs(g, whole_route, st->name, arr);
        }
        else {
            cnt++;
            if (cnt == 10) {
                whole_route->start = NULL;
                return;
            }
            // �湮�ѰŸ�, �ѱ��
            st = st->next;
            continue;
        }

        break;
    }

}
struct Route* path_search(struct GraphType* g, enum Cityname depart, enum Cityname arrival) {
    // ��ü ��� ����
    struct Route* whole_route = newRoute();
    whole_route->start->depart = depart;
    dfs(g, whole_route, depart, arrival);
    return whole_route;
}

bool CheckNoWay(enum TF* cl) {
    int check_cnt = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        if (!cl[i]) {
            return false;
        }
        check_cnt++;
    }
    if (check_cnt == reachable_nodes - 1) {
        return true;
    }
}

// functions for timetable
struct date RandomDateGenerate(int day) {
    float rand_hr = rand() % 2400 / (float)100;
    struct date newDate;
    newDate.date = day;
    newDate.hour = rand_hr;
    return newDate;
}
struct date CalculateDate(struct date today, float hr) {
    // today : ���� ��¥
    // hr : �ҿ� �ð�
    struct date arrive_time;
    arrive_time.date = today.date;
    arrive_time.hour = hr + today.hour;
    // ����ߴµ� 24���� ũ��,
    if (arrive_time.hour >= 24) {
        // ��¥�� �Ϸ� �ø��� �ð��� 24�ð� ���ֱ�
        arrive_time.date++;
        arrive_time.hour -= 24;
    }
    return arrive_time;
}
void DateToString(struct date d) {
    int date = d.date;
    printf("(%d��, ", date);

    float hour = d.hour;
    int pr_hour = (int)hour;
    int pr_minute = (hour - pr_hour) * 60;

    if (pr_hour < 10) {
        printf("0");
    }
    printf("%d:", pr_hour);

    if (pr_minute < 10) {
        printf("0");
    }
    printf("%d)", pr_minute);
}
bool nextDay(struct date dep, struct date arr) {
    // �Ϸ簡 ������
    if (dep.date < arr.date) {
        // �������� �ؾ� ��
        return true;
    }
    else {
        return false;
    }
}
bool TomorrowFlight(struct date dep, struct date arr) {
    if (dep.hour > arr.hour) {
        return true;
    }
    else if (dep.date > arr.date) {
        return true;
    }
    else {
        return false;
    }
}
void PrintLinkedPath(struct PathNode* p) {
    printf("%c ", cityNames[p->depart - 1]);
    while (p->link != NULL) {
        printf("-> %c ", cityNames[p->link->depart - 1]);
        p = p->link;
    }
    return;
}

// functions for RB-tree
bool check_keyList(int* list, int value) {
    for (int i = 0; i < 10; i++) {
        if (list[i] == value) {
            return true;
        }
    }
    return false;
}
void del_node(struct node* n) {
    if (n != NULL) {
        del_node(n->left);
        del_node(n->right);
    }
    free(n);
}
void delete_rbtree(struct rbtree* tree) {
    del_node(tree->root);
    free(tree);
}
struct node* newNode(int k) {
    struct node* new = (struct node*)malloc(sizeof(struct node));

    new->parent = NULL;
    new->left = NULL;
    new->right = NULL;
    new->key = k;
    new->color = RED;

    return new;
}
struct rbtree* newRBtree() {
    struct rbtree* tree = (struct rbtree*)malloc(sizeof(struct rbtree));
    struct node* temp_nil = (struct node*)malloc(sizeof(struct node));

    temp_nil->left = NULL;
    temp_nil->right = NULL;
    temp_nil->parent = NULL;
    temp_nil->color = RED;
    temp_nil->data = NULL;

    tree->root = tree->NIL;

    return tree;
}
struct node* node_nil(struct node* x) {
    if (x == NULL) {
        return nil;
    }
    else {
        return x;
    }
}
struct node* tree_successor(struct rbtree* t, struct node* n) {
    // right child�� ������ ���, ���� ����� ���� ������ ã���� ��
    if (n->right != NULL) {
        // temp�� NULL�� �ɶ����� �������� �̵�
        struct node* temp_c = n->right;
        while (temp_c->left != NULL) {
            temp_c = temp_c->left;
        }
        return temp_c;
    }
    // right node�� ������, n�� parent ���� ��
    struct node* temp_p = n->parent;
    // ���� �ö󰡸鼭 temp node�� left child�� �ɶ����� �ݺ�
    while (temp_p != NULL && n == temp_p->right) {
        n = temp_p;
        temp_p = temp_p->parent;
    }

    return temp_p;
}
struct node* tree_predeccessor(struct rbtree* t, struct node* n) {
    // left child�� ������ ���, ���� ����� ���� �������� ã���� ��
    if (n->left != NULL) {
        // temp�� NULL�� �ɶ����� �������� �̵�
        struct node* temp_c = n->left;
        while (temp_c->right != NULL) {
            temp_c = temp_c->right;
        }
        return temp_c;
    }
    // left node�� ������, n�� parent ���� ��
    struct node* temp_p = n->parent;
    // ���� �ö󰡸鼭 temp node�� right child�� �ɶ����� �ݺ�
    while (temp_p != NULL && n == temp_p->left) {
        n = temp_p;
        temp_p = temp_p->parent;
    }

    return temp_p;
}
void left_Rotate(struct rbtree* tree, struct node* x) {
    // x�� right child
    struct node* y = x->right;

    // y�� left child�� x�� right�� ����
    x->right = y->left;

    // x�� right child�� parent ����
    if (x->right != NULL) {
        x->right->parent = x;
    }

    // y�� parent ����
    y->parent = x->parent;

    // x�� �θ� ��尡 NULL�̸�, y�� root
    if (x->parent == NULL) {
        tree->root = y;
    }

    // x�� left child�̸�
    else if (x == x->parent->left) {
        // x�� �θ����� left�� y�� ����
        x->parent->left = y;
    }
    else {
        // x�� �θ����� right�� y�� ����
        x->parent->right = y;
    }

    // y�� left�� x��
    y->left = x;
    // x�� parent�� y��
    x->parent = y;
}
void right_Rotate(struct rbtree* tree, struct node* x) {

    // x�� left child
    struct node* y = x->left;

    // y�� right child�� x�� left�� ����
    x->left = y->right;

    // x�� right�� �Ⱥ������, right child�� parent ����
    if (x->right != NULL) {
        x->right->parent = x;
    }

    // y�� parent ����
    y->parent = x->parent;

    // x�� �θ� ��尡 NULL�̸�, y�� root
    if (x->parent == NULL) {
        tree->root = y;
    }

    // x�� right child�̸�
    else if (x == x->parent->right) {
        // x�� �θ����� right�� y�� ����
        x->parent->right = y;
    }
    else {
        // x�� �θ����� left�� y�� ����
        x->parent->left = y;
    }

    // y�� left�� x��
    y->right = x;
    // x�� parent�� y��
    x->parent = y;
}
struct node* RB_find(struct rbtree* tree, int key) {
    struct node* temp;
    temp = tree->root;

    while (temp != NULL) {
        if (key == temp->key) {
            return temp;
        }
        else if (key > temp->key) {
            temp = temp->right;
        }

        else if (key < temp->key) {
            temp = temp->left;
        }
    }

    return NULL;
}
void RB_insert_fixup(struct rbtree* tree, struct node* new_node) {
    struct node* y;
    // new_node�� root�� �ƴϸ鼭 RED�� ���� �ݺ� 
    while (new_node->parent != NULL && new_node->parent->color == RED) {
        if (new_node->parent == new_node->parent->parent->left) {
            y = new_node->parent->parent->right;

            // �θ�, ���� RED
            if (y != NULL && y->color == RED) {
                new_node->parent->color = BLACK;
                y->color = BLACK;
                new_node->parent->parent->color = RED;
                new_node = new_node->parent->parent;
            }
            // �θ� RED, ���� BLACK
            else {
                if (new_node == new_node->parent->right) {
                    new_node = new_node->parent;
                    left_Rotate(tree, new_node);
                }

                new_node->parent->color = BLACK;
                new_node->parent->parent->color = RED;
                right_Rotate(tree, new_node->parent->parent);
            }
        }
        else {
            y = new_node->parent->parent->left;

            // �θ�, ���� RED
            if (y != NULL && y->color == RED) {
                new_node->parent->color = BLACK;
                y->color = BLACK;
                new_node->parent->parent->color = RED;
                new_node = new_node->parent->parent;
            }
            // �θ� RED, ���� BLACK
            else {
                if (new_node == new_node->parent->left) {
                    new_node = new_node->parent;
                    right_Rotate(tree, new_node);
                }

                new_node->parent->color = BLACK;
                new_node->parent->parent->color = RED;
                left_Rotate(tree, new_node->parent->parent);
            }
        }

    }
    // �Ϸ� ��, root�� ���� black����
    tree->root->color = BLACK;
}
void RB_node_insert(struct rbtree* tree, struct node* new_node, int k) {
    // NIL�� root node
    struct node* y = tree->NIL;
    struct node* temp = tree->root;

    // �޾ƿ� ��带 �ֱ�
    // root�� null�̸�, ���ο� ��带 root��
    if (tree->root == tree->NIL) {
        new_node->color = BLACK;
        tree->root = new_node;
    }
    else {
        // BST Ʈ���� ���� ������� ��� ����
        while (temp != NULL) {
            y = temp;
            // ���� ���ο� ����� key�� x���� ũ��
            if (k > temp->key) {
                // ���������� �̵�
                temp = temp->right;
            }
            else {
                // �ƴϸ�, ����
                temp = temp->left;
            }
        }

        // ���ο� ��带 y �Ʒ��� ����
        new_node->parent = y;

        // ���ο� ����� Ű ���� ����, �� or �� ����
        if (k > y->key) {
            y->right = new_node;
        }
        else {
            y->left = new_node;
        }

        // Fixup ����
        RB_insert_fixup(tree, new_node);

    }
}
void tree_transplant(struct rbtree* tree, struct node* n_1, struct node* n_2) {
    // tree�� Ư�� ����
    // n_1�� parent ������, n_2�� root�� ����
    if (n_1->parent == NULL) {
        tree->root = n_2;
    }
    // n_1�� left child �̸�, n_1�� left child�� n_2�� ����
    else if (n_1 == n_1->parent->left) {
        n_1->parent->left = n_2;
    }
    // n_1�� right child �̸�, n_1�� right child�� n_2�� ����
    else {
        n_1->parent->right = n_2;
    }

    // n_2�� ������
    if (n_2 != NULL) {
        // n_2�� parent�� n_1�� parent�� ����
        n_2->parent = n_1->parent;
    }
}
void RB_delete_fixup(struct rbtree* tree, struct node* delete_node) {
    struct node* s;
    if (delete_node->parent->left == delete_node) {
        s = delete_node->parent->right;
    }
    else {
        s = delete_node->parent->left;
    }
    struct node* s_left = s->left;
    struct node* s_right = s->right;
    while (delete_node != tree->root && delete_node->color == BLACK) {
        if (delete_node == delete_node->parent->left) {
            s = delete_node->parent->right;
            if (s->color == RED) {
                s->color = BLACK;
                delete_node->parent->color = RED;
                left_Rotate(tree, delete_node->parent);
                s = delete_node->parent->right;
            }
            s_left = node_nil(s_left);
            s_right = node_nil(s_right);
            if ((s_left->color == BLACK) && (s_right->color == BLACK)) {
                s->color = RED;
                delete_node = delete_node->parent;
            }
            else {
                if (s_right->color == BLACK) {
                    s_left->color = BLACK;
                    s->color = RED;
                    right_Rotate(tree, s);
                    s = delete_node->parent->right;
                }
                s->color = delete_node->parent->color;
                delete_node->parent->color = BLACK;
                s->right->color = BLACK;
                left_Rotate(tree, delete_node->parent);
                delete_node = tree->root;
            }
        }
        else {
            s = delete_node->parent->left;
            if (s->color == RED) {
                s->color = BLACK;
                delete_node->parent->color = RED;
                right_Rotate(tree, delete_node->parent);
                s = delete_node->parent->left;
            }
            s_left = node_nil(s_left);
            s_right = node_nil(s_right);
            if ((s_right->color == BLACK) && (s_left->color == BLACK)) {
                s->color = RED;
                delete_node = delete_node->parent;
            }
            else {
                if (s_left->color == BLACK) {
                    s_right->color = BLACK;
                    s->color = RED;
                    left_Rotate(tree, s);
                    s = delete_node->parent->left;
                }
                s->color = delete_node->parent->color;
                delete_node->parent->color = BLACK;
                s->left->color = BLACK;
                right_Rotate(tree, delete_node->parent);
                delete_node = tree->root;
            }
        }
    }
    delete_node->color = BLACK;
}
void RB_delete(struct rbtree* tree, int k) {
    // ������ ���
    struct node* delete_node = RB_find(tree, k);

    if (delete_node == NULL) {
        printf("ID�� �������� �ʾ� �������� ���߽��ϴ�.\n");
        return;
    }

    // �� ��带 ����Ű�� �ӽ� ���
    struct node* y = delete_node;
    // �� ����� ���� ��ġ �� �� ���
    struct node* temp;
    enum Color temp_col = delete_node->color;

    // ������ ����� ������ ���ٸ�,
    if (delete_node->left == NULL) {
        temp = node_nil(delete_node->right);
        tree_transplant(tree, delete_node, temp);
        free(delete_node);
    }
    // ������ ����� �������� ���ٸ�,
    else if (delete_node->right == NULL) {
        temp = node_nil(delete_node->left);
        tree_transplant(tree, delete_node, temp);
        free(delete_node);
    }

    // ������ ����� ���� child�� �� ������ ���
    else {
        y = tree_successor(tree, delete_node);
        temp_col = y->color;
        temp = node_nil(y->right);


        if (y->parent == delete_node) {
            temp->parent = y;
        }
        else {
            tree_transplant(tree, y, temp);
            y->right = delete_node->right;
            y->right->parent = y;
        }
        tree_transplant(tree, delete_node, y);
        y->left = delete_node->left;
        y->left->parent = y;
        y->color = delete_node->color;
        free(delete_node);
    }

    if (temp_col == BLACK) {
        RB_delete_fixup(tree, temp);
    }

    if (tree->root == nil) {
        tree->root = NULL;
    }
    else if (nil->parent != NULL) {
        if (nil->parent->left == nil) {
            nil->parent->left = NULL;
        }
        else {
            nil->parent->right = NULL;
        }
    }
    nil->parent = NULL;

    printf("ID : %d ���� �Ϸ�\n\n", k);
}
struct node* get_closest(struct node* n1, struct node* n2, int k) {
    if (abs(k - n1->key) < abs(k - n2->key)) {
        return n1;
    }
    else {
        return n2;
    }
}
struct node* find_closest(struct node* n, int k) {
    if (k < n->key && n->left != NULL) {
        struct node* temp = find_closest(n->left, k);
        return get_closest(n, temp, k);
    }
    else if (k > n->key && n->right != NULL) {
        struct node* temp = find_closest(n->right, k);
        return get_closest(n, temp, k);
    }
    else {
        return n;
    }
}
int nearest_neighbor(struct rbtree* tree, int k) {
    struct node* n;
    n = find_closest(tree->root, k);

    return n->key;
}
void show_trunks(struct trunk* p) {
    if (p == NULL) {
        return;
    }
    show_trunks(p->prev);
    printf("%s", p->str);
}
void Print_BST(struct node* n, struct trunk* prev, bool is_left) {
    if (n == NULL) {
        return;
    }
    char* prev_str = "    ";

    struct trunk* trunk_temp = (struct trunk*)malloc(sizeof(struct trunk));
    trunk_temp->prev = prev;
    trunk_temp->str = prev_str;

    Print_BST(n->right, trunk_temp, true);

    if (!prev) {
        trunk_temp->str = "---";
    }
    else if (is_left) {
        trunk_temp->str = ",---";
        prev_str = "   |";
    }
    else {
        trunk_temp->str = "`---";
        prev->str = prev_str;
    }
    show_trunks(trunk_temp);
    printf(" %d", n->key);
    if (n->color == RED) {
        printf("%s\n", "��");
    }
    else {
        printf("%s\n", "��");
    }

    if (prev) {
        prev->str = prev_str;
    }
    trunk_temp->str = "   |";
    Print_BST(n->left, trunk_temp, false);
}

// functions for Reservation Database
// for random hash
int* hashtable[HASHTABLE_SIZE];
int hash_index = 0;
struct reservation* newReservation(char* name, struct Route* path, int date) {
    struct reservation* new_reserv = (struct reservation*)malloc(sizeof(struct reservation));

    new_reserv->date = date;
    new_reserv->path = path;
    new_reserv->name = name;

    return new_reserv;
}
void RB_insert(struct rbtree* tree, struct GraphType* g, struct reservation* reservation) {
    // ���ο� node ����, ������ key ���� (�ߺ��� ��� ����)
    int key;
    do {
        key = rand() % 100;
    } while (check_keyList(hashtable, key));
    // hash table�� �����ϰ�, hash index + 1
    hashtable[hash_index] = key;
    hash_index++;

    // key ���� ������ ��� ����
    struct node* new_n = newNode(key);
    // ��忡 key�� reservation ����
    new_n->key = key;
    new_n->data = reservation;
    // ��带 RB-tree�� �߰�
    RB_node_insert(tree, new_n, key);
    printf("���� ID : %d\n", key);
}
struct reservation* MakeReservation(struct GraphType* g, struct FlightInfo*** timetable, char* name, enum Cityname depart, enum Cityname arrival, int reserv_date) {
    int init_reserv_date = reserv_date;
    printf("===================================================\n");
    printf("\t�����: %s, %d�� %c -> %c�� ���� �õ�...\n\n", name, reserv_date, cityNames[depart - 1], cityNames[arrival - 1]);
    for (int i = 0; i < MAX_SIZE; i++) {
        visited[i] = FALSE; // �ʱ�ȭ
    }
    // ��� �����ϴ��� ã�� �Լ�
    struct Route* found_route = path_search(g, depart, arrival);
    // ��ΰ� �ִٸ�
    if (found_route->start != NULL) {
        struct PathNode* flight_way = found_route->start;
        struct PathNode* temp = found_route->start;
        printf("\t��� Ȯ��...\n\n");
        PrintLinkedPath(temp);
        printf("\n\n\t��� Ȯ�� �Ϸ�\n\n");
        printf("\t���� ���...\n\n");
        struct FlightInfo before_arrival;
        struct FlightInfo eta;

        before_arrival = flight_way->depart_date;
        // ��ΰ� ���� �� ����, �ݺ�
        while (flight_way != NULL) {
            // ������� ���� ����Ʈ���� ���� ���ø� ã��
            struct linkedList* l = g->adj_list[flight_way->depart - 1]->next;
            eta = timetable[0][0][0];   // ���� �ʱ�ȭ
            while (l != NULL) {
                // �������� ���� �� Ȯ��
                if (l->name == flight_way->link->depart) {
                    // ������, timetable���� �ش� �������� ã�ƾ� ��.
                    for (int i = 0; i < 10; i++) {
                        if ((reserv_date) > DAY_OF_ONE_MONTH) {
                            printf("\n\t��� ���� �� ��¥ ������ �ʰ��Ͽ� ������ �� �����ϴ�.\n");
                            return NULL;
                        }
                        if (timetable[reserv_date - 1][flight_way->depart - 1][i].to == l->name) {
                            eta = timetable[reserv_date - 1][flight_way->depart - 1][i];
                            // ã����, ȣ���Ͽ� ���� ������ �����´�.
                            // ���� 1. �Ϸ� �þ�� ��� reserv_date + 1
                            // ���� 2. ������� ��߽ð��� ���� �������� �����ð����� ������ ���� �ȵ�
                            
                            if (nextDay(eta.from_date, eta.to_date)) {
                                if (++(reserv_date) > DAY_OF_ONE_MONTH) {
                                    printf("\n\t��� ���� �� ��¥ ������ �ʰ��Ͽ� ������ �� �����ϴ�.\n");
                                    return NULL;
                                }
                                eta = timetable[reserv_date - 1][flight_way->depart - 1][i];
                            }
                            else if (TomorrowFlight(before_arrival.to_date, eta.from_date)) {
                                if (++(reserv_date) > DAY_OF_ONE_MONTH) {
                                    printf("\n\t��� ���� �� ��¥ ������ �ʰ��Ͽ� ������ �� �����ϴ�.\n");
                                    return NULL;
                                }
                                eta = timetable[reserv_date-1][flight_way->depart - 1][i];
                                // ������ ������ 31���� �Ѱܹ�����
                                if (eta.to_date.date > DAY_OF_ONE_MONTH) {
                                    printf("\n\t��� ���� �� ��¥ ������ �ʰ��Ͽ� ������ �� �����ϴ�.\n");
                                    return NULL;
                                }
                            }
                            
                            printf("%c -> %c ", cityNames[eta.from - 1], cityNames[eta.to - 1]);
                            DateToString(eta.from_date);
                            printf(" ");
                            DateToString(eta.to_date);
                            printf("\n");
                            break;
                        }
                    }
                    break;
                }
                l = l->next;
            }
            // to_date ������ flight_way�� ����
            flight_way->depart_date = eta;
            flight_way = flight_way->link;
            // ���� ������� ���ϱ� ���� ����
            before_arrival.from_date = eta.from_date;
            before_arrival.to_date = eta.to_date;
            // ���� ã�� ���� ���������, ������
            if (arrival == eta.to) {
                break;
            }
        }
        // ���ο� ���� ����
        struct reservation* new_reserv = newReservation(name, found_route, init_reserv_date);
        printf("\n\t������ ���� : %s\n", new_reserv->name);
        printf("\t���������� ��� : ");
        PrintLinkedPath(new_reserv->path->start);
        printf("\n");
        // ��� + �ð� �����ֱ�
        struct PathNode* print_path = found_route->start;
        printf("\t������ ������ : \n");
        printf("\t��� %c ", cityNames[print_path->depart - 1]);
        DateToString(print_path->depart_date.from_date);
        printf(" -> ");
        DateToString(print_path->depart_date.to_date);
        printf(" %c ����\n", cityNames[print_path->link->depart - 1]);
        while (print_path->link != NULL) {
            if (print_path->link->depart == arrival) {
                break;
            }
            printf("\t��� %c ", cityNames[print_path->link->depart - 1]);
            DateToString(print_path->link->depart_date.from_date);
            printf(" -> ");
            DateToString(print_path->link->depart_date.to_date);
            printf(" %c ����\n", cityNames[print_path->link->link->depart - 1]);
            print_path = print_path->link;
        }
        return new_reserv;
    }
    else {
        printf("\t��ΰ� ���� ������ �� �����ϴ�.\n");
        return NULL;
    }
}
void ReservationToBST(struct rbtree* tree, struct GraphType* g, struct reservation* new_reserv) {
    printf("\n���� �Ϸ�! ");
    // ������ RB tree�� �ֱ�
    RB_insert(tree, g, new_reserv);
    printf("---------------------------------------------------\n");
    printf("BST ���...\n\n");
    Print_BST(tree->root, NULL, false);
}

int main()
{
    // random int ����
    srand(time(NULL));
    int rand_int;
    int check_list[10] = { 0 };
    int i, j, day;

    // ���ο� Graphtype ����
    struct GraphType* g = newGraphType();
    // Timetable ���� [i: ��¥-1][j: ���ù�ȣ]
    struct FlightInfo*** timetable = newTimetable();
    // ���� FlightInfo �� Path ����
    struct PathNode* curr_path;
    // ������ ����
    int reserv_date;
    enum Cityname depart;
    enum Cityname arrival;
    char* booker;
    struct reservation* new_reserv;

    // 26���� ���� ����
    for (int i = 0; i < 26; i++) {
        enum Cityname city_i = i + 1;
        insert_depart(g, city_i);
    }
    printf("���� ���� �Ϸ�\n");

    // path �� schedule ���� (edge�� ǥ��), ��� ���ÿ� ���� �ݺ�
    for (i = 0; i < 26; i++) {
        // �� ���ÿ� ���� 10���� �ݺ�
        for (j = 0; j < 10; j++) {
            struct FlightInfo curr_fi;
            do {
                rand_int = (rand() % 26) + 1; // 1~26�� rand_int ����
            } while (checkList(check_list, rand_int) || i + 1 == rand_int); // rand_int�� �ߺ����� Ȯ�� + ���� ��ȣ�� ������ Ȯ��
            // ������ ���� �����Ǿ�����, ����
            check_list[j] = rand_int;
            // edge ���� (i -> rand_int - 1)
            curr_path = insert_edge(g, rand_int-1, i);
            // ���ϸ��� random�� �ð� ����
            for (day = 0; day < DAY_OF_ONE_MONTH; day++) {
                // printf("%c -> %c", cityNames[curr_path->depart-1], cityNames[curr_path->link->depart-1]);
                // day+1 ���� ��¥
                curr_fi.from_date = RandomDateGenerate(day + 1);
                curr_fi.from = curr_path->depart;   // ����� : i + 1
                // �ð� ���
                curr_fi.to_date = CalculateDate(curr_fi.from_date, curr_path->time); // ��� �ð� + �ҿ� �ð�
                curr_fi.to = curr_path->link->depart;  // ������ : ������ ���� ����
                // �̰��� �ش��ϴ� timetable�� ����
                // ��¥ : day
                // ���ù�ȣ : i
                // 10�� ����Ʈ : j
                timetable[day][i][j] = curr_fi;
                /*
                printf("%d �Ͽ� %c -> %c���� timetable ���� �Ϸ� ", day, cityNames[curr_fi.from-1], cityNames[curr_fi.to-1]);
                DateToString(curr_fi.from_date);
                printf(" ");
                DateToString(curr_fi.to_date);
                printf("\n");*/
            }
        }
        // �Ϸ� ��, check_list �ʱ�ȭ
        for (int k = 0; k < 10; k++) {
            check_list[k] = 0;
        }
    }
    printf("Path �� Timetable ���� �Ϸ�\n");

    // RB tree ����
    struct rbtree* tree = newRBtree();
    printf("RB tree ���� �Ϸ�\n");

    // ���� ����Ʈ ���
    print_adj_list(g);

    printf("\n��� �ý��� ���� �Ϸ�\n");

    // ������ ����
    booker = "Kim";
    depart = A;
    arrival = B;
    reserv_date = 21;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    // ������ RB tree�� �ֱ�
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }
    // ������ ����
    booker = "Lee";
    depart = C;
    arrival = D;
    reserv_date = 17;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }

    // ������ ����
    booker = "Park";
    depart = Q;
    arrival = Z;
    reserv_date = 23;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }

    // ������ ����
    booker = "Elly";
    depart = W;
    arrival = R;
    reserv_date = 11;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }

    // ������ ����
    booker = "Greg";
    depart = Q;
    arrival = Z;
    reserv_date = 30;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }

    // ������ ����
    booker = "Paul";
    depart = A;
    arrival = R;
    reserv_date = 3;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }

    // ������ ����
    booker = "Simon";
    depart = N;
    arrival = H;
    reserv_date = 14;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }

    // ������ ����
    booker = "James";
    depart = K;
    arrival = V;
    reserv_date = 12;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }

    // ������ ����
    booker = "Hue";
    depart = G;
    arrival = Y;
    reserv_date = 9;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }

    // ������ ����
    booker = "Laura";
    depart = P;
    arrival = O;
    reserv_date = 1;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }

    // ������ ����
    booker = "Jay";
    depart = A;
    arrival = C;
    reserv_date = 11;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }

    // ������ ����
    booker = "Sam";
    depart = W;
    arrival = X;
    reserv_date = 21;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }

    // ������ ����
    booker = "Saera";
    depart = T;
    arrival = V;
    reserv_date = 5;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }
    // ����
    int delete_key = 0;
    for (int i = 0; i < 5; i++) {
        printf("������ Ű�� �Է��� �ֽʽÿ�.\n");
        scanf_s("%d", &delete_key);
        RB_delete(tree, delete_key);
        Print_BST(tree->root, NULL, false);
    }
    
    // �޸� �ݳ�
    free(timetable);
    free(new_reserv);
    free(g);
    printf("===================================================\n");
    free(tree);
    //delete_rbtree(tree);
    printf("�޸� �ݳ� �Ϸ�");

    return 0;

}
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
    float hour; // 0 ~ 24의 소수값
};
char* PrintDate(struct date* d) {
    char print_str[4];
    int hour = (int)d->hour;
    int minute = (d->hour - hour) * 60;

    // 00:00 형태로 내보내기

    return;
}
// struct for timetable
// 출발 시간이 고려된 전체 경로
struct Route {
    struct date depart_date;
    struct PathNode* depart;
};
struct Route* newRoute() {
    struct Route* new_route = (struct Route*)malloc(sizeof(struct Route));
    new_route->depart = NULL;

    return new_route;
}
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
        // FlightInfo* 한 개 : new_tt[i]
        struct FlightInfo** new_fi = newFlightInfo();
        new_tt[i] = new_fi;
    }
    // new_tt  :  31개의 Flightinfo* 를 담고있는 배열
    // new_tt[i] : 26개의 FlightInfo를 담고있는 배열
    return new_tt;
}
// struct for City & Path map
// Path 정보를 저장함.
struct PathNode {
    enum Cityname depart;   // vertex
    struct PathNode* link;  // link
    float distance;    // weight
    float time;     // elapsed_time
    struct FlightInfo depart_date;    // 출발 도시에서 시각
};
struct PathNode* newPathNode() {
    struct PathNode* path = (struct PathNode*)malloc(sizeof(struct PathNode));
    path->link = NULL;
    path->distance = 0.0;
    path->time = 0.0;
    return path;
}
// City 정보 + 인접리스트를 저장함.
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
    // 이름
    enum Cityname name;
    // 좌표
    int abs_x;
    int abs_y;
    char dir_x;
    char dir_y;
    int raw_x;
    int raw_y;
    // Path Node 저장
    struct linkedList* next;
};
struct CityInfo* newCityInfo(int x, int y, enum Cityname name) {
    struct CityInfo* new_city = (struct CityInfo*)malloc(sizeof(struct CityInfo));
    // 이름
    new_city->name = name;
    // 좌표 (-3000 ~ +3000)
    new_city->raw_x = x;
    new_city->raw_y = y;
    // 좌표 (W or S 3000 ~ E or N 3000)
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
// A~Z 별로 모든 도시 정보를 저장함.
struct GraphType {
    // 인접 노드의 개수
    int adj_n;
    // 인접 리스트 (도시 정보 저장)
    struct CityInfo* adj_list[MAX_SIZE];
};
struct GraphType* newGraphType() {
    struct GraphType* g = (struct GraphType*)malloc(sizeof(struct GraphType));
    int v;
    g->adj_n = 0;
    // adj_list 초기화
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
// 도시 생성
void insert_depart(struct GraphType* g, enum Cityname v) {
    int rand_int_x, rand_int_y;
    if (((g->adj_n) + 1) > MAX_SIZE) {
        // error
        fprintf(stderr, "overflow");
        return;
    }
    g->adj_n++;
    // 도시 정보 랜덤하게 생성
    rand_int_x = rand() % 6000 - 3000;
    rand_int_y = rand() % 6000 - 3000;
    struct CityInfo* curr_city = newCityInfo(rand_int_x, rand_int_y, v);
    // 현재 도시를 adj_list에 넣고, 주변 도시 포인터 초기화
    g->adj_list[v - 1] = curr_city;
    g->adj_list[v - 1]->next = NULL;
}
struct PathNode* insert_edge(struct GraphType* g, int u, int v) {
    // u : tail (0~25)
    // v : head (0~25)
    // v -> u로의 PathNode 생성하여, adj_list에 추가 (A 인접리스트에 있는 Path들은 출발지가 A여야함)
    if (u >= g->adj_n || v >= g->adj_n) {
        fprintf(stderr, "depart index error");
        return;
    }
    // 출발지와 도착지 도시정보 가져오기
    struct CityInfo* depart = g->adj_list[v];
    struct CityInfo* arrival = g->adj_list[u];
    // 새로운 path node 생성
    struct PathNode* depart_path = newPathNode();
    struct PathNode* arrive_path = newPathNode();
    // head 부분의 값 대입
    enum Cityname depart_city = v+1;
    depart_path->depart = depart_city;
    // distance, time 지정
    float x_diff = (depart->raw_x - arrival->raw_x);
    float y_diff = (depart->raw_y - arrival->raw_y);
    float distance = sqrt((x_diff * x_diff) + (y_diff * y_diff));
    float time = distance / AIRPLANE_SPEED;
    depart_path->distance = distance;
    depart_path->time = time;
    // 다음 도시 저장
    arrive_path->depart = u + 1;
    depart_path->link = arrive_path;
    // link에 **** LinkedList ****를 추가!!!
    struct linkedList* new_city = newLinkedList();
    new_city->name = u + 1;
    new_city->curr_path = depart_path;
    // 만약 출발지에 처음으로 넣는거면,
    if (g->adj_list[v]->next == NULL) {
        // link에 바로 추가
        g->adj_list[v]->next = new_city;
    }
    else {
        // 맨 앞의 PathNode를 저장한 뒤
        struct linkedList* temp = g->adj_list[v]->next;
        // 맨 앞에 새로운 Path를 추가
        g->adj_list[v]->next = new_city;
        // 그 뒤에 temp를 붙여줌
        new_city->next = temp;
    }
    printf("%c -> %c로의 경로 생성\n", cityNames[depart_path->depart-1], cityNames[arrive_path->depart-1]);
    printf("adj_list에 추가 완료 (%.2f km), %c -> %c\n", new_city->curr_path->distance, cityNames[new_city->curr_path->depart - 1], cityNames[new_city->curr_path->link->depart-1]);
    return depart_path;
}
void print_adj_list(struct GraphType* g) {
    for (int i = 0; i < g->adj_n; i++) {
        struct linkedList* n = g->adj_list[i]->next;
        printf("도시 %c의 인접 리스트", cityNames[i]);
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
int bfs(struct GraphType* g, int v) {
    struct linkedList* p;
    QueueType* q = queue_init();
    visited[v] = TRUE;
    enqueue(q, v);
    while (q->size != 0) {
        v = dequeue(q);
        for (p = g->adj_list[v]->next; p; p = p->next) {
            if (!visited[p->name - 1]) {
                // 발견
                reachable_nodes++;
                visited[p->name - 1] = TRUE;
                enqueue(q, p->name - 1);
            }
        }
    }
    return reachable_nodes;
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
struct Route* path_map_search(struct GraphType* g, enum Cityname depart, enum Cityname arrival) {
    // 인접리스트는 "순방향"으로 검색
    int start = depart - 1;
    int end = arrival - 1;
    float distance[MAX_SIZE];		// 거리
    int	path_cnt[MAX_SIZE];		// 경로 수
    enum TF check[MAX_SIZE];  // 방문했는지 여부
    // 전체 경로 생성
    struct Route* whole_route = newRoute();
    // 출발지는 처음 도시고, 목적지는 없는 빈 PathNode 생성
    struct PathNode* init_path = newPathNode();
    init_path->depart = depart;
    whole_route->depart = init_path;

    for (int i = 0; i < MAX_SIZE; i++) {
        distance[i] = INF;  // 초기화
        path_cnt[i] = 0;
        check[i] = FALSE;
    }
    int cycle = 0;
    float min;
    int now = start;
    int before = now;
    // 시작점의 거리는 0
    distance[start] = 0;
    // 시작점은 방문한 것으로 가정
    check[start] = TRUE;
    // 한 도시 안에서 주변 도시를 탐색할 때 필요 (즉, 다음 도시들 중의 후보)
    struct linkedList* temp = g->adj_list[now]->next;
    struct PathNode* added_path = whole_route->depart;






    // 경로 탐색 과정


    // 1. 첫 경로 초기화
    // temp를 돌면서, 거리값 업데이트
    struct PathNode* determined_path = temp->curr_path;
    // 한 도시에서 주변 도시를 탐색
    // (temp : 주변 도시 하나씩)
    min = INF;  // 최솟값을 초기화
    while (temp != NULL) {
        // 1. 주변 노드들의 distance 정보 업데이트 (인접리스트를 돌면서)
        // 현재 노드에서 각 도시까지의 distance 갱신
        /*
        for (int i = 0; i < 26; i++) {
            printf("%d ", check[i]);
        }*/
        printf("\n");
        // 안간 노드이면
        if (!check[temp->name - 1]) {
            // Path 정보를 꺼내와서
            float curr_dis = temp->curr_path->distance;
            // 거리를 distance 리스트에 갱신
            distance[temp->name - 1] = curr_dis;
            printf("업데이트 %c (%.2f)\n", cityNames[temp->name - 1], curr_dis);
        }
        temp = temp->next;
    }

    // 2. 선택
    // 다시 한번 돌면서 최단 거리 선택
    temp = g->adj_list[now]->next;
    // 최단 거리 구하기 (인접리스트를 돌면서)
    while (temp != NULL) {
        // 만약 목적지를 발견하면, 바로 나가기
        if (temp->name - 1 == end) {
            // 이 경로를 path로 설정
            determined_path = temp->curr_path;
            // min, now 지정
            min = temp->curr_path->distance;
            now = temp->name - 1;
            break;
        }

        // 짧은 거리가 나오면, 갱신 (들른 곳이 아닐 때만)
        if (!check[temp->name - 1]) {
            // 더 작으면, 갱신함
            if (temp->curr_path->distance < min) {
                // 이 경로를 path로 설정
                determined_path = temp->curr_path;
                // min, now 지정
                min = temp->curr_path->distance;
                now = temp->name - 1;
            }
        }
    }
    // 방문한 것으로 처리
    check[now] = TRUE;
    // determined_path; 정해진 경로
    // whole_route 뒤로 이동해서
    while (added_path->link != NULL) {
        added_path = added_path->link;
    }
    // 경로를 추가함
    added_path->link = determined_path;

    // 3. 목적지가 나올 때까지 반복
    while (determined_path->depart != arrival) {
        // 현재 탐색해야 하는 곳 : adj_list[determined_path->depart-1]
        // 거리 정보 다시 업데이트
        temp = g->adj_list[determined_path->depart - 1]->next;
        while (temp != NULL) {
            // 현재 값이 더 적어지면, 업데이트
            // 이전 값 : distance[temp->name-1]
            // 현재 값 : temp->curr_path->distance + distance[before]
            if (distance[temp->name - 1] > temp->curr_path->distance + distance[before]) {
                distance[temp->name - 1] = temp->curr_path->distance + distance[before];
            }
            temp = temp->next;
        }

        // 최단 거리 구하기 (이번엔 전체 노드를 돌면서)
        min = INF;
        for (int i = 0; i < MAX_SIZE; i++) {
            if (!check[i]) {
                if (distance[i] < min) {
                    min = distance[i];
                    now = i;
                }
            }
        }
        // 찾은 도시 번호는 i

        /*
        temp = g->adj_list[determined_path->depart - 1]->next;
        while (temp != NULL) {
            // 만약 목적지를 발견하면, 바로 나가기
            if (temp->name - 1 == end) {
                // 이 경로를 path로 설정
                determined_path = temp->curr_path;
                // min, now 지정
                min = temp->curr_path->distance;
                now = temp->name - 1;
                break;
            }

            // 짧은 거리가 나오면, 갱신 (들른 곳이 아닐 때만)
            if (!check[temp->name - 1]) {
                // 더 작으면, 갱신함
                if (temp->curr_path->distance < min) {
                    // 이 경로를 path로 설정
                    determined_path = temp->curr_path;
                    // min, now 지정
                    min = temp->curr_path->distance;
                    now = temp->name - 1;
                }
            }
        }
        */
        // 방문한 것으로 처리
        check[now] = TRUE;
        // determined_path; 정해진 경로
        // whole_route 뒤로 이동해서
        while (added_path->link != NULL) {
            added_path = added_path->link;
        }
        // 경로를 추가함
        added_path->link = determined_path;

    }
    











    
    while (next_path != NULL) {
        if (CheckNoWay(check)) {
            printf("경로를 찾을 수 없습니다.");
            return NULL;
        }
        
        // 다시 한번 더 탐색
        temp = next_path;
        // 최단 거리 구하기 (인접리스트를 돌면서)
        while (temp != NULL) {
            // 만약 목적지를 발견하면, 바로 나가기
            if (temp->name - 1 == end) {
                // 이 경로를 path로 설정
                determined_path = temp->curr_path;
                // min, now 지정
                min = temp->curr_path->distance;
                now = temp->name - 1;
                break;
                // 
                /*
                // 출발지에서 이 도시로의 path를 가져오기
                struct linkedList* find_arrival = g->adj_list[temp->name - 1]->next;
                // 이 도시의 인접 리스트에 접근해서 
                for (int i = 0; i < 10; i++) {
                    printf("%c %c\n", cityNames[find_arrival->name-1], cityNames[g->adj_list[now]->name-1]);
                    // 출발 도시이면, 이 패스를 가져오기
                    if (find_arrival->name == g->adj_list[now]->name) {
                        printf("출발 도시 찾음.\n");
                        break;
                    }
                }*/
                // 
            }
            // 들른 곳이 아닐 때만 min, now 결정
            if (!check[temp->name - 1]) {
                // 더 작으면, 갱신함
                if (temp->curr_path->distance < min) {
                    // 이 경로를 path로 설정
                    determined_path = temp->curr_path;
                    // min, now 지정
                    min = temp->curr_path->distance;
                    now = temp->name - 1;
                    /*
                    // 출발지에서 이 도시로의 path를 가져오기
                    struct linkedList* find_arrival = g->adj_list[temp->name - 1]->next;
                    // 이 도시의 인접 리스트에 접근해서 
                    for (int i = 0; i < 10; i++) {
                        printf("%c %c\n", cityNames[find_arrival->name - 1], cityNames[g->adj_list[now]->name - 1]);
                        // 출발 도시이면, 이 패스를 가져오기
                        if (find_arrival->name == g->adj_list[now]->name) {
                            printf("출발 도시 찾음.\n");
                            break;
                        }
                    }*/
                }
            }
            // 다음으로 넘어가기
            temp = temp->next;
        }
        // 다음 경로 결정 완료(determined_path), 도시 번호는 now
        // 현재 도시를 방문으로 표시
        check[now] = TRUE;
        // 전체 경로에 합치기
        struct PathNode* path_temp = whole_route->depart;
        while (path_temp->link != NULL) {
            path_temp = path_temp->link;
        }
        // 추가
        path_temp->link = determined_path;
        printf("네네 경로에 도시 %c 추가완료\n", cityNames[determined_path->depart - 1]);
        // 만약 끝 점에 도달하면, 나가기
        if (now == end) {
            break;
        }
        // 아니면, 다음 주변 도시를 탐색
        else {
            next_path = g->adj_list[now]->next;
        }
    }

    // 완성된 경로를 확인하기
    
    printf("\nwhole_route 확인\n");
    struct PathNode* print_temp = whole_route->depart;
    while (print_temp != NULL) {
        printf("%c -> ", cityNames[print_temp->depart - 1]);
        print_temp = print_temp->link;
    }

    return whole_route;
    
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
    // today : 현재 날짜
    // hr : 소요 시간
    struct date arrive_time;
    arrive_time.date = today.date;
    arrive_time.hour = hr + today.hour;
    // 계산했는데 24보다 크면,
    if (arrive_time.hour >= 24) {
        // 날짜는 하루 늘리고 시간을 24시간 빼주기
        arrive_time.date++;
        arrive_time.hour -= 24;
    }
    return arrive_time;
}
void DateToString(struct date d) {
    int date = d.date;
    printf("(%d일, ", date);

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
    // 하루가 지나면
    if (dep.date < arr.date) {
        // 다음날로 해야 함
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
    // right child가 존재할 경우, 우측 노드의 제일 왼쪽을 찾으면 됨
    if (n->right != NULL) {
        // temp가 NULL이 될때까지 왼쪽으로 이동
        struct node* temp_c = n->right;
        while (temp_c->left != NULL) {
            temp_c = temp_c->left;
        }
        return temp_c;
    }
    // right node가 없으면, n의 parent 선언 후
    struct node* temp_p = n->parent;
    // 위로 올라가면서 temp node가 left child가 될때까지 반복
    while (temp_p != NULL && n == temp_p->right) {
        n = temp_p;
        temp_p = temp_p->parent;
    }

    return temp_p;
}
struct node* tree_predeccessor(struct rbtree* t, struct node* n) {
    // left child가 존재할 경우, 좌측 노드의 제일 오른쪽을 찾으면 됨
    if (n->left != NULL) {
        // temp가 NULL이 될때까지 왼쪽으로 이동
        struct node* temp_c = n->left;
        while (temp_c->right != NULL) {
            temp_c = temp_c->right;
        }
        return temp_c;
    }
    // left node가 없으면, n의 parent 선언 후
    struct node* temp_p = n->parent;
    // 위로 올라가면서 temp node가 right child가 될때까지 반복
    while (temp_p != NULL && n == temp_p->left) {
        n = temp_p;
        temp_p = temp_p->parent;
    }

    return temp_p;
}
void left_Rotate(struct rbtree* tree, struct node* x) {
    // x의 right child
    struct node* y = x->right;

    // y의 left child를 x의 right에 붙임
    x->right = y->left;

    // x의 right child의 parent 연결
    if (x->right != NULL) {
        x->right->parent = x;
    }

    // y의 parent 연결
    y->parent = x->parent;

    // x의 부모 노드가 NULL이면, y는 root
    if (x->parent == NULL) {
        tree->root = y;
    }

    // x가 left child이면
    else if (x == x->parent->left) {
        // x의 부모노드의 left에 y를 연결
        x->parent->left = y;
    }
    else {
        // x의 부모노드의 right에 y를 연결
        x->parent->right = y;
    }

    // y의 left를 x로
    y->left = x;
    // x의 parent를 y로
    x->parent = y;
}
void right_Rotate(struct rbtree* tree, struct node* x) {

    // x의 left child
    struct node* y = x->left;

    // y의 right child를 x의 left에 붙임
    x->left = y->right;

    // x의 right이 안비었으면, right child의 parent 연결
    if (x->right != NULL) {
        x->right->parent = x;
    }

    // y의 parent 연결
    y->parent = x->parent;

    // x의 부모 노드가 NULL이면, y는 root
    if (x->parent == NULL) {
        tree->root = y;
    }

    // x가 right child이면
    else if (x == x->parent->right) {
        // x의 부모노드의 right에 y를 연결
        x->parent->right = y;
    }
    else {
        // x의 부모노드의 left에 y를 연결
        x->parent->left = y;
    }

    // y의 left를 x로
    y->right = x;
    // x의 parent를 y로
    x->parent = y;
}
struct node* RB_find(struct rbtree* tree, int key) {
    struct node* temp;
    temp = tree->root;

    while (temp != nil) {
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
    // new_node가 root가 아니면서 RED일 동안 반복 
    while (new_node->parent != NULL && new_node->parent->color == RED) {
        if (new_node->parent == new_node->parent->parent->left) {
            y = new_node->parent->parent->right;

            // 부모, 삼촌 RED
            if (y != NULL && y->color == RED) {
                new_node->parent->color = BLACK;
                y->color = BLACK;
                new_node->parent->parent->color = RED;
                new_node = new_node->parent->parent;
            }
            // 부모 RED, 삼촌 BLACK
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

            // 부모, 삼촌 RED
            if (y != NULL && y->color == RED) {
                new_node->parent->color = BLACK;
                y->color = BLACK;
                new_node->parent->parent->color = RED;
                new_node = new_node->parent->parent;
            }
            // 부모 RED, 삼촌 BLACK
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
    // 완료 후, root의 색을 black으로
    tree->root->color = BLACK;
}
void RB_node_insert(struct rbtree* tree, struct node* new_node, int k) {
    // NIL과 root node
    struct node* y = tree->NIL;
    struct node* temp = tree->root;

    // 받아온 노드를 넣기
    // root가 null이면, 새로운 노드를 root로
    if (tree->root == tree->NIL) {
        new_node->color = BLACK;
        tree->root = new_node;
    }
    else {
        // BST 트리와 같은 방식으로 노드 삽입
        while (temp != NULL) {
            y = temp;
            // 만약 새로운 노드의 key가 x보다 크면
            if (k > temp->key) {
                // 오른쪽으로 이동
                temp = temp->right;
            }
            else {
                // 아니면, 왼쪽
                temp = temp->left;
            }
        }

        // 새로운 노드를 y 아래에 연결
        new_node->parent = y;

        // 새로운 노드의 키 값에 따라, 왼 or 오 선택
        if (k > y->key) {
            y->right = new_node;
        }
        else {
            y->left = new_node;
        }

        // Fixup 수행
        RB_insert_fixup(tree, new_node);

    }
}
void tree_transplant(struct rbtree* tree, struct node* n_1, struct node* n_2) {
    // tree의 특성 유지
    // n_1의 parent 없으면, n_2를 root로 설정
    if (n_1->parent == NULL) {
        tree->root = n_2;
    }
    // n_1이 left child 이면, n_1의 left child을 n_2로 설정
    else if (n_1 == n_1->parent->left) {
        n_1->parent->left = n_2;
    }
    // n_1이 right child 이면, n_1의 right child를 n_2로 설정
    else {
        n_1->parent->right = n_2;
    }

    // n_2가 있으면
    if (n_2 != NULL) {
        // n_2의 parent을 n_1의 parent로 설정
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
    // 삭제될 노드
    struct node* delete_node = RB_find(tree, k);
    // 그 노드를 가리키는 임시 노드
    struct node* y = delete_node;
    // 그 노드의 원래 위치 및 색 기억
    struct node* temp;
    enum Color temp_col = delete_node->color;

    // 삭제할 노드의 왼쪽이 없다면,
    if (delete_node->left == NULL) {
        temp = node_nil(delete_node->right);
        tree_transplant(tree, delete_node, temp);
        free(delete_node);
    }
    // 삭제할 노드의 오른쪽이 없다면,
    else if (delete_node->right == NULL) {
        temp = node_nil(delete_node->left);
        tree_transplant(tree, delete_node, temp);
        free(delete_node);
    }

    // 삭제할 노드의 양쪽 child가 다 존재할 경우
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
        printf("%s\n", "○");
    }
    else {
        printf("%s\n", "●");
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
    // 새로운 node 생성, 고유한 key 생성 (중복될 경우 무시)
    int key;
    do {
        key = rand() % 100;
    } while (check_keyList(hashtable, key));
    // hash table에 대입하고, hash index + 1
    hashtable[hash_index] = key;
    hash_index++;

    // key 값을 가지고 노드 생성
    struct node* new_n = newNode(key);
    // 노드에 key와 reservation 대입
    new_n->key = key;
    new_n->data = reservation;
    // 노드를 RB-tree에 추가
    RB_node_insert(tree, new_n, key);
    printf("고유 ID : %d\n", key);
}
struct reservation* MakeReservation(struct GraphType* g, struct FlightInfo*** timetable, char* name, enum Cityname depart, enum Cityname arrival, int reserv_date) {
    int init_reserv_date = reserv_date;
    // 경로 존재하는지 찾는 함수
    struct Route* route_1 = path_map_search(g, depart, arrival);
    printf("일단 경로는 찾음\n");
    struct PathNode* flight_way = route_1->depart;
    struct PathNode* temp = route_1->depart;
    printf("===================================================\n");
    printf("%d일 %c -> %c의 예약 시도...\n\n", reserv_date, cityNames[depart-1], cityNames[arrival-1]);
    printf("경로 확인 : \n");
    PrintLinkedPath(temp);
    printf("\n경로 확인 완료\n\n");
    printf("예약 결과...\n\n");
    struct FlightInfo before_arrival;
    struct FlightInfo eta;
    // 경로가 있다면
    if (flight_way != NULL) {
        before_arrival = flight_way->depart_date;
        // 경로가 끝날 떄까지
        printf("loop ");
        while (flight_way->link != NULL) {
            // 인접 리스트에서 도착 도시의 date를 timetable에서 찾아서, date 대입하기
            struct linkedList* l = g->adj_list[flight_way->link->depart - 1]->next;
            eta = timetable[reserv_date - 1][l->name][0];
            for (int i = 0; i < 10; i++) {
                printf("bbbb ");
                // 출발지와 같은지 확인
                if (l->name == flight_way->depart) {
                    // B를 찾았음. (l->name) B->N의 FlightInfo를 가져오기
                    // timetable 속에서 N 찾기 
                    printf("found ");
                    for (int j = 0; j < 10; j++) {
                        // 만약 도착지가 31일을 넘어버리면, 그냥 나가기
                        if (reserv_date > DAY_OF_ONE_MONTH) {
                            printf("날짜 범위를 초과하여 예약할 수 없습니다.\n");
                            return NULL;
                        }
                        printf("%c %c\n", cityNames[flight_way->depart - 1], cityNames[timetable[reserv_date - 1][flight_way->link->depart - 1][j].to - 1]);
                        // timetable 안의 목적지 : timetable[reserv_date-1][flight_way->link->depart-1][j]
                        if (flight_way->depart == timetable[reserv_date - 1][flight_way->link->depart - 1][j].to) {
                            if (cityNames[arrival - 1] == cityNames[timetable[reserv_date - 1][l->name - 1][j].to - 1]) {
                                printf("목적지 찾아버림~");
                                eta = timetable[reserv_date - 1][l->name - 1][j];
                                // 혹은, 하루 늘어났을 경우 reserv_date + 1
                                if (nextDay(eta.from_date, eta.to_date)) {
                                    reserv_date++;
                                    if (reserv_date > DAY_OF_ONE_MONTH) {
                                        printf("날짜 범위를 초과하여 예약할 수 없습니다.\n");
                                        return NULL;
                                    }
                                }
                                // 근데 출발지의 출발시간이 저번 도착지의 도착시간보다 빠르면 말이 안됨
                                else if (TomorrowFlight(before_arrival.to_date, eta.from_date)) {
                                    if (reserv_date > DAY_OF_ONE_MONTH) {
                                        printf("날짜 범위를 초과하여 예약할 수 없습니다.\n");
                                        return NULL;
                                    }
                                    eta = timetable[++(reserv_date)-1][l->name - 1][j];
                                    // 도착지 일정이 31일을 넘겨버리면
                                    if (eta.to_date.date > DAY_OF_ONE_MONTH) {
                                        printf("날짜 범위를 초과하여 예약할 수 없습니다.\n");
                                        return NULL;
                                    }
                                }

                                break;
                            }
                            // 찾았으므로, 정보 가져오기 (B -> N)
                            if (reserv_date > DAY_OF_ONE_MONTH) {
                                printf("날짜 범위를 초과하여 예약할 수 없습니다.\n");
                                return NULL;
                            }
                            eta = timetable[reserv_date - 1][l->name - 1][j];
                            
                            // 혹은, 하루 늘어났을 경우 reserv_date + 1
                            if (nextDay(eta.from_date, eta.to_date)) {
                                reserv_date++;
                                if (reserv_date > DAY_OF_ONE_MONTH) {
                                    printf("날짜 범위를 초과하여 예약할 수 없습니다.\n");
                                    return NULL;
                                }
                            }
                            // 근데 출발지의 출발시간이 저번 도착지의 도착시간보다 빠르면 말이 안됨
                            else if (TomorrowFlight(before_arrival.to_date, eta.from_date)) {
                                if (reserv_date > DAY_OF_ONE_MONTH) {
                                    printf("날짜 범위를 초과하여 예약할 수 없습니다.\n");
                                    return NULL;
                                }
                                eta = timetable[++(reserv_date)-1][l->name - 1][j];
                                // 도착지 일정이 31일을 넘겨버리면
                                if (eta.to_date.date > DAY_OF_ONE_MONTH) {
                                    printf("날짜 범위를 초과하여 예약할 수 없습니다.\n");
                                    return NULL;
                                }
                            }
                            printf("찾았으니 나가자");
                            break;
                        }
                    }
                    break;
                }
                l = l->next;
            }
            DateToString(eta.from_date);
            DateToString(eta.to_date);
            // to_date 정보를 flight_way에 대입
            flight_way->depart_date = eta;
            flight_way = flight_way->link;
            // 다음 출발지와 비교하기 위해 저장
            before_arrival.from_date = eta.from_date;
            before_arrival.to_date = eta.to_date;
            // 만약 찾은 곳이 목적지라면, 끝내기
            if (cityNames[arrival - 1] == eta.to) {
                break;
            }
        }
        // 새로운 예약 생성
        struct reservation* new_reserv = newReservation(name, route_1, init_reserv_date);
        printf("예약자 정보 : %s\n", new_reserv->name);
        printf("목적지까지 최단 경로 : ");
        PrintLinkedPath(new_reserv->path->depart);
        printf("\n");
        // 경로 + 시간 보여주기
        struct PathNode* print_path = route_1->depart;
        printf("생성된 스케줄 : \n");
        printf("출발 %c ", cityNames[print_path->depart - 1]);
        DateToString(print_path->depart_date.from_date);
        printf(" -> ");
        DateToString(print_path->depart_date.to_date);
        printf(" %c 도착\n", cityNames[print_path->link->depart - 1]);
        while (print_path->link != NULL) {
            if (print_path->link->depart == arrival) {
                break;
            }
            printf("출발 %c ", cityNames[print_path->link->depart - 1]);
            DateToString(print_path->link->depart_date.from_date);
            printf(" -> ");
            DateToString(print_path->link->depart_date.to_date);
            printf(" %c 도착\n", cityNames[print_path->link->link->depart - 1]);
            print_path = print_path->link;
        }
        return new_reserv;
    }
    else {
        printf("경로가 없습니다.");
    }
}
void ReservationToBST(struct rbtree* tree, struct GraphType* g, struct reservation* new_reserv) {
    printf("예약 완료!");
    // 예약을 RB tree에 넣기
    if (new_reserv != NULL) {
        // 예약을 RB tree에 넣기
        RB_insert(tree, g, new_reserv);
        printf("---------------------------------------------------\n");
        printf("BST 출력...\n\n");
        Print_BST(tree->root, NULL, false);
        printf("출력 ok...\n\n");
    }
}

int main()
{
    // random int 생성
    srand(time(NULL));
    int rand_int;
    int check_list[10] = { 0 };
    int i, j, day;

    // 새로운 Graphtype 생성
    struct GraphType* g = newGraphType();
    // Timetable 생성 [i: 날짜-1][j: 도시번호]
    struct FlightInfo*** timetable = newTimetable();
    // 현재 FlightInfo 와 Path 정보
    struct PathNode* curr_path;
    // 예약자 정보
    int reserv_date;
    enum Cityname depart;
    enum Cityname arrival;
    char* booker;
    struct reservation* new_reserv;

    // 26개의 도시 생성
    for (int i = 0; i < 26; i++) {
        enum Cityname city_i = i + 1;
        insert_depart(g, city_i);
    }
    printf("도시 생성 완료\n");

    // path 와 schedule 생성 (edge로 표현), 모든 도시에 대해 반복
    for (i = 0; i < 26; i++) {
        // 각 도시에 대해 10번씩 반복
        for (j = 0; j < 10; j++) {
            struct FlightInfo curr_fi;
            do {
                rand_int = (rand() % 26) + 1; // 1~26의 rand_int 생성
            } while (checkList(check_list, rand_int) || i + 1 == rand_int); // rand_int가 중복인지 확인 + 현재 번호랑 같은지 확인
            // 적절한 난수 생성되었으면, 대입
            check_list[j] = rand_int;
            // edge 생성 (i -> rand_int - 1)
            curr_path = insert_edge(g, rand_int-1, i);
            // 매일마다 random한 시간 생성
            for (day = 0; day < DAY_OF_ONE_MONTH; day++) {
                // printf("%c -> %c", cityNames[curr_path->depart-1], cityNames[curr_path->link->depart-1]);
                // day+1 실제 날짜
                curr_fi.from_date = RandomDateGenerate(day + 1);
                curr_fi.from = curr_path->depart;   // 출발지 : i + 1
                // 시간 계산
                curr_fi.to_date = CalculateDate(curr_fi.from_date, curr_path->time); // 출발 시간 + 소요 시간
                curr_fi.to = curr_path->link->depart;  // 도착지 : 생성된 랜덤 도시
                // 이것을 해당하는 timetable에 대입
                // 날짜 : day
                // 도시번호 : i
                // 10개 리스트 : j
                timetable[day][i][j] = curr_fi;
                /*
                printf("%d 일에 %c -> %c로의 timetable 생성 완료 ", day, cityNames[curr_fi.from-1], cityNames[curr_fi.to-1]);
                DateToString(curr_fi.from_date);
                printf(" ");
                DateToString(curr_fi.to_date);
                printf("\n");*/
            }
        }
        // 완료 후, check_list 초기화
        for (int k = 0; k < 10; k++) {
            check_list[k] = 0;
        }
    }
    printf("Path 및 Timetable 생성 완료\n");

    // RB tree 생성
    struct rbtree* tree = newRBtree();
    printf("RB tree 생성 완료\n");
    
    // 도달할 수 있는 노드의 개수 확인
    printf("도달 가능한 도시의 개수 : %d\n", bfs(g, 0));

    // 인접 리스트 출력
    print_adj_list(g);

    printf("\n모든 시스템 생성 완료\n");

    // 예약자 정보
    booker = "Kim";
    depart = A;
    arrival = B;
    reserv_date = 2;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    // 예약을 RB tree에 넣기
    /*
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }*/
    /*
    // 예약자 정보
    booker = "Lee";
    depart = C;
    arrival = D;
    reserv_date = 17;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }

    // 예약자 정보
    booker = "Park";
    depart = Q;
    arrival = Z;
    reserv_date = 23;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }

    // 예약자 정보
    booker = "Elly";
    depart = W;
    arrival = R;
    reserv_date = 11;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }

    // 예약자 정보
    booker = "Greg";
    depart = Q;
    arrival = Z;
    reserv_date = 30;
    new_reserv = MakeReservation(g, timetable, booker, depart, arrival, reserv_date);
    if (new_reserv != NULL) {
        ReservationToBST(tree, g, new_reserv);
    }
    */
    // 메모리 반납
    free(timetable);
    free(new_reserv);
    free(g);
    printf("Graph까지 삭제 완료\n");
    delete_rbtree(tree);
    printf("메반완");

    return 0;

}
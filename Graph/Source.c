#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>

#define MAX_SIZE 26 // number of alphabets( A~Z )
#define MAX_Q_SIZE 100   // max size of queue
#define AIRPLANE_SPEED 500
#define INF 10000
#define DAY_OF_ONE_MONTH 31

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
    struct linkedList* next;
};
struct linkedList* newLinkedList() {
    struct linkedList* new_ll = (struct linkedList*)malloc(sizeof(struct linkedList));
    new_ll->name = A;
    new_ll->next = NULL;

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
    printf("도시 %c 생성 완료, 좌표 : %c%d, %c%d\n", cityNames[v - 1], g->adj_list[v - 1]->dir_x, g->adj_list[v - 1]->abs_x, g->adj_list[v - 1]->dir_y, g->adj_list[v - 1]->abs_y);
}
struct PathNode* insert_edge(struct GraphType* g, int u, int v) {
    // u : tail (0~25)
    // v : head (0~25)
    // v -> u로의 PathNode 생성하여, adj_list에 추가
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
    printf("%c -> %c", cityNames[depart->name - 1], cityNames[arrival->name - 1]);
    // head 부분의 값 대입
    enum Cityname depart_city = v + 1;
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
    // 역순으로 구할 것 - 인접 리스트의 특성 (인접 리스트는 출발지의 정보를 담고있음)
    int start = arrival - 1;
    int end = depart - 1;
    float distance[MAX_SIZE];		// 거리
    int	path_cnt[MAX_SIZE];		// 경로 수
    enum TF check[MAX_SIZE];  // 방문했는지 여부
    // 전체 경로 생성
    struct Route* whole_route = newRoute();
    // 목적지는 현재고, link는 없는 PathNode를 생성
    struct PathNode* init_path = newPathNode();
    init_path->depart = arrival;
    whole_route->depart = init_path;

    for (int i = 0; i < MAX_SIZE; i++) {
        distance[i] = INF;  // 초기화
        path_cnt[i] = 0;
        check[i] = FALSE;
    }
    int cycle = 0;
    float min;
    int now = start;
    // 현재 도시의 next부터 시작 (시작점의 거리는 0)
    distance[start] = 0;
    check[start] = TRUE;
    // 한 도시 안에서 주변 도시를 탐색할 때 필요
    struct PathNode* temp = g->adj_list[now]->next;
    // 결정된 다음 도시를 저장할 때 필요
    struct PathNode* next_path = g->adj_list[now]->next;
    // 전체 경로에 저장할 때 필요

    // 경로 탐색 과정
    while (next_path != NULL) {
        if (CheckNoWay(check)) {
            printf("경로를 찾을 수 없습니다.");
            return NULL;
        }
        min = INF;  // 최솟값을 초기화
        // 현재 노드에서 각 도시까지의 distance 갱신
        for (int i = 0; i < MAX_SIZE; i++) {
            if (i == now) {
                // 현재 노드는 제외
                continue;
            }
            // 방문한 곳이 아니면
            if (!check[i]) {
                // 거리를 distance 리스트에 갱신
                distance[i] = next_path->distance;
            }
        }
        // next_path의 주변 도시 탐색
        temp = next_path;
        struct PathNode* determined_path = next_path;
        // 한 도시에서 주변 도시를 탐색
        while (temp != NULL) {
            
            // 만약 목적지를 발견하면, 바로 나가기
            if (temp->depart - 1 == end) {
                min = temp->distance;
                now = temp->depart - 1;
                // 다음 노드들을 떼어버리기
                temp->link = NULL;
                determined_path = temp;
                break;
            }
            // 들른 곳이 아닐 때만 min, now 결정
            if (!check[temp->depart - 1]) {
                
                // 더 작으면, 갱신함
                if (temp->distance < min) {
                    min = temp->distance;
                    now = temp->depart - 1;
                    determined_path = temp;
                }
            }
            // 다음으로 넘어가기
            temp = temp->link;
        }
        // 다음 경로 결정 완료(determined_path), 도시 번호는 now
        // 현재 도시를 방문으로 표시
        check[now] = TRUE;
        // 전체 경로에 합치기
        if (whole_route->depart->link == NULL) {
            struct PathNode* first = whole_route->depart;
            whole_route->depart = determined_path;
            determined_path->link = first;
        }
        else {
            determined_path->link = whole_route->depart;
            whole_route->depart = determined_path;
            //printf("%c", cityNames[whole_route->depart->depart - 1]);
        }
        //printf("%c", cityNames[whole_route->depart->depart - 1]);
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
    /*
    printf("\nwhole_route 확인\n");
    struct PathNode* print_temp = whole_route->depart;
    while (print_temp != NULL) {
        printf("%c -> ", cityNames[print_temp->depart - 1]);
        print_temp = print_temp->link;
    }*/

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
    while (p != NULL) {
        printf("%c -> ", cityNames[p->depart - 1]);
        p = p->link;
    }
    return;
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

    int reserv_date = 2;
    enum Cityname depart = W;
    enum Cityname arrival = Q;

    // 26개의 도시 생성
    for (int i = 0; i < 26; i++) {
        enum Cityname city_i = i + 1;
        insert_depart(g, city_i);
    }
    printf("\n도시 생성 완료\n");
    // **************************** ok *******************************************

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
            // edge 생성 (i -> rand_int - 1), 현재 path 가져오기
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
                printf("생성된 schedule : %c -> %c ", cityNames[timetable[day][i][j].from-1], cityNames[timetable[day][i][j].to - 1]);
                DateToString(timetable[day][i][j].from_date);
                printf(" ");
                DateToString(timetable[day][i][j].to_date);
                printf("\n");*/
            }
            printf("도시 %c 로의 모든 스케줄 생성 완료\n", cityNames[rand_int - 1]);
        }
        // 완료 후, check_list 초기화
        for (int k = 0; k < 10; k++) {
            check_list[k] = 0;
        }
        printf("\n");
    }
    printf("Path 생성 완료\n");
    
    // 도달할 수 있는 노드의 개수 확인
    printf("Reachable nodes: %d\n", bfs(g, 0));

    // 인접 리스트 출력
    print_adj_list(g);

    // 경로 존재하는지 찾는 함수
    struct Route* route_1 = path_map_search(g, depart, arrival);
    struct PathNode* flight_way = route_1->depart;
    struct PathNode* temp = route_1->depart;
    PrintLinkedPath(temp);
    printf("\n경로 확인 완료\n\n");
    struct FlightInfo before_arrival;
    struct FlightInfo eta;
    // 경로가 있다면
    if (flight_way != NULL) {
        before_arrival = flight_way->depart_date;
        // 경로가 끝날 떄까지
        while (flight_way->link != NULL) {
            // 인접 리스트에서 도착 도시의 date를 timetable에서 찾아서, date 대입하기
            struct linkedList* l = g->adj_list[flight_way->link->depart - 1]->next;
            for (int i = 0; i < 10; i++) {
                // 출발지와 같은지 확인
                if (l->name == flight_way->depart) {
                    // B를 찾았음. (l->name) B->N의 FlightInfo를 가져오기
                    // timetable 속에서 N 찾기 
                    for (int j = 0; j < 10; j++) {
                        // timetable 안의 목적지 : timetable[reserv_date-1][flight_way->link->depart-1][j]
                        if (flight_way->depart == timetable[reserv_date - 1][flight_way->link->depart - 1][j].to) {
                            // 찾았으므로, 정보 가져오기 (B -> N)
                            eta = timetable[reserv_date - 1][l->name - 1][j];

                            // 혹은, 하루 늘어났을 경우 reserv_date + 1
                            if (nextDay(eta.from_date, eta.to_date)) {
                                reserv_date++;
                            }
                            // 근데 출발지의 출발시간이 저번 도착지의 도착시간보다 빠르면 말이 안됨
                            else if (TomorrowFlight(before_arrival.to_date, eta.from_date)) {
                                eta = timetable[++(reserv_date) - 1][l->name - 1][j];
                            }
                            break;
                        }
                    }
                    break;
                }
                l = l->next;
            }
            // to_date 정보를 flight_way에 대입
            flight_way->depart_date = eta;
            flight_way = flight_way->link;
            // 다음 출발지와 비교하기 위해 저장
            before_arrival.from_date = eta.from_date;
            before_arrival.to_date = eta.to_date;
        }
    }
    else {
        printf("경로가 없습니다.");
    }

    // **************************** ok *******************************************
    // 경로 + 시간 보여주기

    struct PathNode* print_path = route_1->depart;
    printf("%c ", cityNames[print_path->depart - 1]);
    DateToString(print_path->depart_date.from_date);
    while (print_path->link != NULL) {
        // DateToString(print_path->depart_date.to_date);
        printf(" -> ");
        DateToString(print_path->depart_date.to_date);
        printf(" %c ", cityNames[print_path->link->depart-1]);
        if (print_path->link->depart == arrival) {
            break;
        }
        DateToString(print_path->link->depart_date.from_date);
        print_path = print_path->link;
    }

    // 메모리 반납
    free(g);
    free(timetable);
    free(route_1);

    return 0;


}
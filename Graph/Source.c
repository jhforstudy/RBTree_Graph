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
// struct for City & Path map
// Path 정보를 저장함.
struct PathNode {
    enum Cityname depart;   // vertex
    struct PathNode* link;  // link
    float distance;    // weight
    float time;
};
struct PathNode* newPathNode() {
    struct PathNode* path = (struct PathNode*)malloc(sizeof(struct PathNode));
    path->link = NULL;
    path->distance = 0.0;
    path->time = 0.0;
    return path;
}
// City 정보 + 인접리스트를 저장함.
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
    struct PathNode* link;
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
    new_city->link = NULL;

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
// struct for timetable
// 시간 순서 고려된 전체 경로
struct Route {
    struct date* date;
    struct PathNode* depart;
};
struct Route* newRoute() {
    struct Route* new_route = (struct Route*)malloc(sizeof(struct Route));
    new_route->date = NULL;
    struct PathNode* first = newPathNode();
    new_route->depart = first;

    return new_route;
}
struct FlightInfo {
    enum Cityname from;
    enum Cityname to;
    struct date from_date;
    struct date to_date;
};
struct FlightInfo* newFlightInfo() {
    struct FlightInfo* new_fi = (struct FlightInfo*)malloc(sizeof(struct FlightInfo)*MAX_SIZE);
    for (int i = 0; i < MAX_SIZE; i++) {
        new_fi[i].from = A;
        new_fi[i].to = B;
    }
    
    return new_fi;
}
struct FlightInfo** newTimetable() {
    struct FlightInfo** new_tt = (struct FlightInfo**)malloc(sizeof(struct FlightInfo*) * DAY_OF_ONE_MONTH);
    for (int i = 0; i < DAY_OF_ONE_MONTH; i++) {
        // FlightInfo* 한 개 : new_tt[i]
        struct FlightInfo* new_fi = newFlightInfo();
        new_tt[i] = new_fi;
    }
    return new_tt;
}


// functions for city & path map
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
    g->adj_list[v - 1] = curr_city;
    printf("도시 %c 생성 완료, 좌표 : %c%d, %c%d\n", cityNames[v - 1], g->adj_list[v - 1]->dir_x, g->adj_list[v - 1]->abs_x, g->adj_list[v - 1]->dir_y, g->adj_list[v - 1]->abs_y);
}
struct PathNode* insert_edge(struct GraphType* g, int u, int v) {
    // u : tail (0~25)
    // v : head (0~25)
    if (u >= g->adj_n || v >= g->adj_n) {
        fprintf(stderr, "depart index error");
        return;
    }
    struct CityInfo* depart = g->adj_list[v];
    struct CityInfo* arrival = g->adj_list[u];
    // 새로운 path node 생성
    struct PathNode* path = newPathNode();
    // head 부분의 값 대입
    enum Cityname depart_city = v + 1;
    path->depart = depart_city;
    // distance, time 지정
    float x_diff = (depart->raw_x - arrival->raw_x);
    float y_diff = (depart->raw_y - arrival->raw_y);
    float distance = sqrt((x_diff * x_diff) + (y_diff * y_diff));
    float time = distance / AIRPLANE_SPEED;
    path->distance = distance;
    path->time = time;
    // link에 tail 도시를 추가
    // 만약 처음으로 넣는거면,
    if (g->adj_list[u]->link == NULL) {
        // link에 바로 추가
        g->adj_list[u]->link = path;
    }
    else {
        // 맨 앞의 PathNode를 저장한 뒤
        struct PathNode* temp = g->adj_list[u]->link;
        // 새로 만든 path를 맨 앞으로 지정하고
        g->adj_list[u]->link = path;
        // path 뒤에 붙여줌
        path->link = temp;
    }
    return path;
}
void print_adj_list(struct GraphType* g) {
    for (int i = 0; i < g->adj_n; i++) {
        struct PathNode* n = g->adj_list[i]->link;
        printf("도시 %c의 인접 리스트", cityNames[i]);
        while (n != NULL) {
            printf("->%c", cityNames[n->depart - 1]);
            n = n->link;
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
    struct PathNode* p;
    QueueType* q = queue_init();
    visited[v] = TRUE;
    enqueue(q, v);
    while (q->size != 0) {
        v = dequeue(q);
        for (p = g->adj_list[v]->link; p; p = p->link) {
            if (!visited[p->depart - 1]) {
                // 발견
                reachable_nodes++;
                visited[p->depart - 1] = TRUE;
                enqueue(q, p->depart - 1);
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
    struct PathNode* temp = g->adj_list[now]->link;
    // 결정된 다음 도시를 저장할 때 필요
    struct PathNode* next_path = g->adj_list[now]->link;
    // 전체 경로에 저장할 때 필요
    struct PathNode* determined_path;
    whole_route->depart->depart = arrival;
    printf("경로 : %c ", cityNames[start]);

    // 경로 탐색 과정
    while (next_path != NULL) {
        if (CheckNoWay(check)) {
            printf("경로를 찾을 수 없습니다.");
            break;
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
        determined_path = next_path;
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
        // 다음 경로 결정 완료, 도시 번호는 now
        // 현재 도시를 방문으로 표시
        check[now] = TRUE;
        printf("<- %c ", cityNames[now]);
        // 전체 경로에 합치기
        if (whole_route->depart == NULL) {
            // 비어있으면, 첫 경로
            struct PathNode* first = g->adj_list[start]->link;
            first->link = NULL;
            whole_route->depart = first;
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
            next_path = g->adj_list[now]->link;
        }
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
    printf("%d일 ", date);

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
    printf("%d", pr_minute);
}
bool nextDay(struct date dep, struct date arr) {
    // 도착지 시간이 더 빠르면
    if (dep.hour > arr.hour) {
        // 다음날로 해야 함
        printf("다음날\n");
        return true;
    }
    else {
        return false;
    }
}

int main()
{
    // random int 생성
    srand(time(NULL));
    int rand_int;
    int check_list[10] = { 0 };

    // 새로운 Graphtype 생성
    struct GraphType* g = newGraphType();
    // Timetable 생성 [i: 날짜-1][j: 도시번호]
    struct FlightInfo** timetable = newTimetable();
    // 현재 FlightInfo 와 Path 정보
    struct PathNode* curr_path;

    // 26개의 도시 생성
    for (int i = 0; i < 26; i++) {
        enum Cityname city_i = i + 1;
        insert_depart(g, city_i);
    }
    printf("\n도시 생성 완료\n");

    // path 생성 (edge로 표현), 모든 도시에 대해 반복
    for (int i = 0; i < 26; i++) {
        // 각 도시에 대해 10번씩 반복
        for (int j = 0; j < 10; j++) {
            struct FlightInfo curr_fi;
            do {
                rand_int = (rand() % 26) + 1; // 1~26의 rand_int 생성
            } while (checkList(check_list, rand_int) || i + 1 == rand_int); // rand_int가 중복인지 확인 + 현재 번호랑 같은지 확인
            // 적절한 난수 생성되었으면, 대입
            check_list[j] = rand_int;
            // edge 생성 (i -> rand_int - 1), 현재 path 가져오기
            curr_path = insert_edge(g, rand_int-1, i);

            // 매일마다 random한 시간 생성
            for (int day = 0; day < DAY_OF_ONE_MONTH; day++) {
                curr_fi.from_date = RandomDateGenerate(day + 1);
                curr_fi.from = i + 1;
                // 시간 계산
                curr_fi.to_date = CalculateDate(curr_fi.from_date, curr_path->time);
                curr_fi.to = rand_int;

                // 이것을 timetable에 대입
                timetable[day][i] = curr_fi;
            }
            printf("Path: %c -> %c", cityNames[curr_fi.from - 1], cityNames[curr_fi.to - 1]);
            printf(" %.2f ", curr_path->distance);
            printf(" 출발 시간 : ");
            DateToString(curr_fi.from_date);
            printf(" 도착 시간 : ");
            DateToString(curr_fi.to_date);
            printf("\n");
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

    int reserv_date = 15;

    // 경로 존재하는지 찾는 함수
    struct Route* route_1 = path_map_search(g, B, C);
    struct Route* print = route_1->depart;
    printf("경로 : ");
    while (print->depart != NULL) {
        printf("-> %s ", cityNames[route_1->depart->depart - 1]);
        print = print->depart;
    }
    struct PathNode* temp = route_1->depart;
    // 경로가 있다면
    if (route_1 != NULL) {
        // 이것을 시간표에서 찾아서 date 대입하기
        struct date dep = timetable[reserv_date - 1][temp->depart - 1].from_date;
        struct date arr;
        struct date after_flight = timetable[reserv_date - 1][temp->depart - 1].from_date;
        struct PathNode* temp = route_1->depart;
        enum Cityname dep_city;
        enum Cityname arr_city;
        printf("\n");
        while (temp->link != NULL) {
            dep_city = timetable[reserv_date - 1][temp->depart - 1].from;
            arr_city = timetable[reserv_date - 1][temp->depart - 1].to;
            // 현재 출발지 시간표를 찾기
            dep = timetable[reserv_date - 1][temp->depart - 1].from_date;
            // 도착시각과 현재 출발지 시간표를 비교하여, 빠르다면 다음날로
            if (nextDay(after_flight, dep)) {
                dep = timetable[++(reserv_date) - 1][temp->depart - 1].from_date;
            }
            printf("출발 : %c (", cityNames[dep_city - 1]);
            DateToString(dep);
            printf(")\n");
            // 목적지 시간표를 찾기
            arr = timetable[reserv_date - 1][temp->depart - 1].to_date;
            printf("도착 : %c (", cityNames[arr_city - 1]);
            DateToString(arr);
            printf(")\n");
            // 도착한 시간을 저장
            after_flight = arr;
            temp = temp->link;
        }
    }
    else {
        printf("경로가 없습니다.");
    }
    

    // 메모리 반납
    free(g);
    free(timetable);
    free(route_1);

    return 0;


}
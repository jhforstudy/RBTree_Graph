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
    enum Cityname depart;   // vertex
    struct PathNode* link;  // link
    float distance;    // weight
    float time;     // elapsed_time
    struct FlightInfo depart_date;    // ��� ���ÿ��� �ð�
};
struct PathNode* newPathNode() {
    struct PathNode* path = (struct PathNode*)malloc(sizeof(struct PathNode));
    path->link = NULL;
    path->distance = 0.0;
    path->time = 0.0;
    return path;
}
// City ���� + ��������Ʈ�� ������.
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
    rand_int_x = rand() % 6000 - 3000;
    rand_int_y = rand() % 6000 - 3000;
    struct CityInfo* curr_city = newCityInfo(rand_int_x, rand_int_y, v);
    // ���� ���ø� adj_list�� �ְ�, �ֺ� ���� ������ �ʱ�ȭ
    g->adj_list[v - 1] = curr_city;
    g->adj_list[v - 1]->next = NULL;
    printf("���� %c ���� �Ϸ�, ��ǥ : %c%d, %c%d\n", cityNames[v - 1], g->adj_list[v - 1]->dir_x, g->adj_list[v - 1]->abs_x, g->adj_list[v - 1]->dir_y, g->adj_list[v - 1]->abs_y);
}
struct PathNode* insert_edge(struct GraphType* g, int u, int v) {
    // u : tail (0~25)
    // v : head (0~25)
    // v -> u���� PathNode �����Ͽ�, adj_list�� �߰�
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
    printf("%c -> %c", cityNames[depart->name - 1], cityNames[arrival->name - 1]);
    // head �κ��� �� ����
    enum Cityname depart_city = v + 1;
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
int bfs(struct GraphType* g, int v) {
    struct linkedList* p;
    QueueType* q = queue_init();
    visited[v] = TRUE;
    enqueue(q, v);
    while (q->size != 0) {
        v = dequeue(q);
        for (p = g->adj_list[v]->next; p; p = p->next) {
            if (!visited[p->name - 1]) {
                // �߰�
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
    // �������� ���� �� - ���� ����Ʈ�� Ư�� (���� ����Ʈ�� ������� ������ �������)
    int start = arrival - 1;
    int end = depart - 1;
    float distance[MAX_SIZE];		// �Ÿ�
    int	path_cnt[MAX_SIZE];		// ��� ��
    enum TF check[MAX_SIZE];  // �湮�ߴ��� ����
    // ��ü ��� ����
    struct Route* whole_route = newRoute();
    // �������� �����, link�� ���� PathNode�� ����
    struct PathNode* init_path = newPathNode();
    init_path->depart = arrival;
    whole_route->depart = init_path;

    for (int i = 0; i < MAX_SIZE; i++) {
        distance[i] = INF;  // �ʱ�ȭ
        path_cnt[i] = 0;
        check[i] = FALSE;
    }
    int cycle = 0;
    float min;
    int now = start;
    // ���� ������ next���� ���� (�������� �Ÿ��� 0)
    distance[start] = 0;
    check[start] = TRUE;
    // �� ���� �ȿ��� �ֺ� ���ø� Ž���� �� �ʿ�
    struct PathNode* temp = g->adj_list[now]->next;
    // ������ ���� ���ø� ������ �� �ʿ�
    struct PathNode* next_path = g->adj_list[now]->next;
    // ��ü ��ο� ������ �� �ʿ�

    // ��� Ž�� ����
    while (next_path != NULL) {
        if (CheckNoWay(check)) {
            printf("��θ� ã�� �� �����ϴ�.");
            return NULL;
        }
        min = INF;  // �ּڰ��� �ʱ�ȭ
        // ���� ��忡�� �� ���ñ����� distance ����
        for (int i = 0; i < MAX_SIZE; i++) {
            if (i == now) {
                // ���� ���� ����
                continue;
            }
            // �湮�� ���� �ƴϸ�
            if (!check[i]) {
                // �Ÿ��� distance ����Ʈ�� ����
                distance[i] = next_path->distance;
            }
        }
        // next_path�� �ֺ� ���� Ž��
        temp = next_path;
        struct PathNode* determined_path = next_path;
        // �� ���ÿ��� �ֺ� ���ø� Ž��
        while (temp != NULL) {
            
            // ���� �������� �߰��ϸ�, �ٷ� ������
            if (temp->depart - 1 == end) {
                min = temp->distance;
                now = temp->depart - 1;
                // ���� ������ ���������
                temp->link = NULL;
                determined_path = temp;
                break;
            }
            // �鸥 ���� �ƴ� ���� min, now ����
            if (!check[temp->depart - 1]) {
                
                // �� ������, ������
                if (temp->distance < min) {
                    min = temp->distance;
                    now = temp->depart - 1;
                    determined_path = temp;
                }
            }
            // �������� �Ѿ��
            temp = temp->link;
        }
        // ���� ��� ���� �Ϸ�(determined_path), ���� ��ȣ�� now
        // ���� ���ø� �湮���� ǥ��
        check[now] = TRUE;
        // ��ü ��ο� ��ġ��
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
        // ���� �� ���� �����ϸ�, ������
        if (now == end) {
            break;
        }
        // �ƴϸ�, ���� �ֺ� ���ø� Ž��
        else {
            next_path = g->adj_list[now]->next;
        }
    }

    // �ϼ��� ��θ� Ȯ���ϱ�
    /*
    printf("\nwhole_route Ȯ��\n");
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

    int reserv_date = 2;
    enum Cityname depart = W;
    enum Cityname arrival = Q;

    // 26���� ���� ����
    for (int i = 0; i < 26; i++) {
        enum Cityname city_i = i + 1;
        insert_depart(g, city_i);
    }
    printf("\n���� ���� �Ϸ�\n");
    // **************************** ok *******************************************

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
            // edge ���� (i -> rand_int - 1), ���� path ��������
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
                printf("������ schedule : %c -> %c ", cityNames[timetable[day][i][j].from-1], cityNames[timetable[day][i][j].to - 1]);
                DateToString(timetable[day][i][j].from_date);
                printf(" ");
                DateToString(timetable[day][i][j].to_date);
                printf("\n");*/
            }
            printf("���� %c ���� ��� ������ ���� �Ϸ�\n", cityNames[rand_int - 1]);
        }
        // �Ϸ� ��, check_list �ʱ�ȭ
        for (int k = 0; k < 10; k++) {
            check_list[k] = 0;
        }
        printf("\n");
    }
    printf("Path ���� �Ϸ�\n");
    
    // ������ �� �ִ� ����� ���� Ȯ��
    printf("Reachable nodes: %d\n", bfs(g, 0));

    // ���� ����Ʈ ���
    print_adj_list(g);

    // ��� �����ϴ��� ã�� �Լ�
    struct Route* route_1 = path_map_search(g, depart, arrival);
    struct PathNode* flight_way = route_1->depart;
    struct PathNode* temp = route_1->depart;
    PrintLinkedPath(temp);
    printf("\n��� Ȯ�� �Ϸ�\n\n");
    struct FlightInfo before_arrival;
    struct FlightInfo eta;
    // ��ΰ� �ִٸ�
    if (flight_way != NULL) {
        before_arrival = flight_way->depart_date;
        // ��ΰ� ���� ������
        while (flight_way->link != NULL) {
            // ���� ����Ʈ���� ���� ������ date�� timetable���� ã�Ƽ�, date �����ϱ�
            struct linkedList* l = g->adj_list[flight_way->link->depart - 1]->next;
            for (int i = 0; i < 10; i++) {
                // ������� ������ Ȯ��
                if (l->name == flight_way->depart) {
                    // B�� ã����. (l->name) B->N�� FlightInfo�� ��������
                    // timetable �ӿ��� N ã�� 
                    for (int j = 0; j < 10; j++) {
                        // timetable ���� ������ : timetable[reserv_date-1][flight_way->link->depart-1][j]
                        if (flight_way->depart == timetable[reserv_date - 1][flight_way->link->depart - 1][j].to) {
                            // ã�����Ƿ�, ���� �������� (B -> N)
                            eta = timetable[reserv_date - 1][l->name - 1][j];

                            // Ȥ��, �Ϸ� �þ�� ��� reserv_date + 1
                            if (nextDay(eta.from_date, eta.to_date)) {
                                reserv_date++;
                            }
                            // �ٵ� ������� ��߽ð��� ���� �������� �����ð����� ������ ���� �ȵ�
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
            // to_date ������ flight_way�� ����
            flight_way->depart_date = eta;
            flight_way = flight_way->link;
            // ���� ������� ���ϱ� ���� ����
            before_arrival.from_date = eta.from_date;
            before_arrival.to_date = eta.to_date;
        }
    }
    else {
        printf("��ΰ� �����ϴ�.");
    }

    // **************************** ok *******************************************
    // ��� + �ð� �����ֱ�

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

    // �޸� �ݳ�
    free(g);
    free(timetable);
    free(route_1);

    return 0;


}
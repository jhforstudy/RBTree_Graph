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
// struct for City & Path map
// Path ������ ������.
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
// City ���� + ��������Ʈ�� ������.
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
    struct PathNode* link;
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
    new_city->link = NULL;

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
// struct for timetable
// �ð� ���� ����� ��ü ���
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
        // FlightInfo* �� �� : new_tt[i]
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
    // ���� ���� �����ϰ� ����
    rand_int_x = rand() % 6000 - 3000;
    rand_int_y = rand() % 6000 - 3000;
    struct CityInfo* curr_city = newCityInfo(rand_int_x, rand_int_y, v);
    g->adj_list[v - 1] = curr_city;
    printf("���� %c ���� �Ϸ�, ��ǥ : %c%d, %c%d\n", cityNames[v - 1], g->adj_list[v - 1]->dir_x, g->adj_list[v - 1]->abs_x, g->adj_list[v - 1]->dir_y, g->adj_list[v - 1]->abs_y);
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
    // ���ο� path node ����
    struct PathNode* path = newPathNode();
    // head �κ��� �� ����
    enum Cityname depart_city = v + 1;
    path->depart = depart_city;
    // distance, time ����
    float x_diff = (depart->raw_x - arrival->raw_x);
    float y_diff = (depart->raw_y - arrival->raw_y);
    float distance = sqrt((x_diff * x_diff) + (y_diff * y_diff));
    float time = distance / AIRPLANE_SPEED;
    path->distance = distance;
    path->time = time;
    // link�� tail ���ø� �߰�
    // ���� ó������ �ִ°Ÿ�,
    if (g->adj_list[u]->link == NULL) {
        // link�� �ٷ� �߰�
        g->adj_list[u]->link = path;
    }
    else {
        // �� ���� PathNode�� ������ ��
        struct PathNode* temp = g->adj_list[u]->link;
        // ���� ���� path�� �� ������ �����ϰ�
        g->adj_list[u]->link = path;
        // path �ڿ� �ٿ���
        path->link = temp;
    }
    return path;
}
void print_adj_list(struct GraphType* g) {
    for (int i = 0; i < g->adj_n; i++) {
        struct PathNode* n = g->adj_list[i]->link;
        printf("���� %c�� ���� ����Ʈ", cityNames[i]);
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
                // �߰�
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
    // �������� ���� �� - ���� ����Ʈ�� Ư�� (���� ����Ʈ�� ������� ������ �������)
    int start = arrival - 1;
    int end = depart - 1;
    float distance[MAX_SIZE];		// �Ÿ�
    int	path_cnt[MAX_SIZE];		// ��� ��
    enum TF check[MAX_SIZE];  // �湮�ߴ��� ����
    // ��ü ��� ����
    struct Route* whole_route = newRoute();

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
    struct PathNode* temp = g->adj_list[now]->link;
    // ������ ���� ���ø� ������ �� �ʿ�
    struct PathNode* next_path = g->adj_list[now]->link;
    // ��ü ��ο� ������ �� �ʿ�
    struct PathNode* determined_path;
    whole_route->depart->depart = arrival;
    printf("��� : %c ", cityNames[start]);

    // ��� Ž�� ����
    while (next_path != NULL) {
        if (CheckNoWay(check)) {
            printf("��θ� ã�� �� �����ϴ�.");
            break;
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
        determined_path = next_path;
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
        // ���� ��� ���� �Ϸ�, ���� ��ȣ�� now
        // ���� ���ø� �湮���� ǥ��
        check[now] = TRUE;
        printf("<- %c ", cityNames[now]);
        // ��ü ��ο� ��ġ��
        if (whole_route->depart == NULL) {
            // ���������, ù ���
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
        // ���� �� ���� �����ϸ�, ������
        if (now == end) {
            break;
        }
        // �ƴϸ�, ���� �ֺ� ���ø� Ž��
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
    printf("%d�� ", date);

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
    // ������ �ð��� �� ������
    if (dep.hour > arr.hour) {
        // �������� �ؾ� ��
        printf("������\n");
        return true;
    }
    else {
        return false;
    }
}

int main()
{
    // random int ����
    srand(time(NULL));
    int rand_int;
    int check_list[10] = { 0 };

    // ���ο� Graphtype ����
    struct GraphType* g = newGraphType();
    // Timetable ���� [i: ��¥-1][j: ���ù�ȣ]
    struct FlightInfo** timetable = newTimetable();
    // ���� FlightInfo �� Path ����
    struct PathNode* curr_path;

    // 26���� ���� ����
    for (int i = 0; i < 26; i++) {
        enum Cityname city_i = i + 1;
        insert_depart(g, city_i);
    }
    printf("\n���� ���� �Ϸ�\n");

    // path ���� (edge�� ǥ��), ��� ���ÿ� ���� �ݺ�
    for (int i = 0; i < 26; i++) {
        // �� ���ÿ� ���� 10���� �ݺ�
        for (int j = 0; j < 10; j++) {
            struct FlightInfo curr_fi;
            do {
                rand_int = (rand() % 26) + 1; // 1~26�� rand_int ����
            } while (checkList(check_list, rand_int) || i + 1 == rand_int); // rand_int�� �ߺ����� Ȯ�� + ���� ��ȣ�� ������ Ȯ��
            // ������ ���� �����Ǿ�����, ����
            check_list[j] = rand_int;
            // edge ���� (i -> rand_int - 1), ���� path ��������
            curr_path = insert_edge(g, rand_int-1, i);

            // ���ϸ��� random�� �ð� ����
            for (int day = 0; day < DAY_OF_ONE_MONTH; day++) {
                curr_fi.from_date = RandomDateGenerate(day + 1);
                curr_fi.from = i + 1;
                // �ð� ���
                curr_fi.to_date = CalculateDate(curr_fi.from_date, curr_path->time);
                curr_fi.to = rand_int;

                // �̰��� timetable�� ����
                timetable[day][i] = curr_fi;
            }
            printf("Path: %c -> %c", cityNames[curr_fi.from - 1], cityNames[curr_fi.to - 1]);
            printf(" %.2f ", curr_path->distance);
            printf(" ��� �ð� : ");
            DateToString(curr_fi.from_date);
            printf(" ���� �ð� : ");
            DateToString(curr_fi.to_date);
            printf("\n");
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

    int reserv_date = 15;

    // ��� �����ϴ��� ã�� �Լ�
    struct Route* route_1 = path_map_search(g, B, C);
    struct Route* print = route_1->depart;
    printf("��� : ");
    while (print->depart != NULL) {
        printf("-> %s ", cityNames[route_1->depart->depart - 1]);
        print = print->depart;
    }
    struct PathNode* temp = route_1->depart;
    // ��ΰ� �ִٸ�
    if (route_1 != NULL) {
        // �̰��� �ð�ǥ���� ã�Ƽ� date �����ϱ�
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
            // ���� ����� �ð�ǥ�� ã��
            dep = timetable[reserv_date - 1][temp->depart - 1].from_date;
            // �����ð��� ���� ����� �ð�ǥ�� ���Ͽ�, �����ٸ� ��������
            if (nextDay(after_flight, dep)) {
                dep = timetable[++(reserv_date) - 1][temp->depart - 1].from_date;
            }
            printf("��� : %c (", cityNames[dep_city - 1]);
            DateToString(dep);
            printf(")\n");
            // ������ �ð�ǥ�� ã��
            arr = timetable[reserv_date - 1][temp->depart - 1].to_date;
            printf("���� : %c (", cityNames[arr_city - 1]);
            DateToString(arr);
            printf(")\n");
            // ������ �ð��� ����
            after_flight = arr;
            temp = temp->link;
        }
    }
    else {
        printf("��ΰ� �����ϴ�.");
    }
    

    // �޸� �ݳ�
    free(g);
    free(timetable);
    free(route_1);

    return 0;


}
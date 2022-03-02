#define _CRT_SECURE_NO_WARNINGS
#include "graph_evaluation.h"

#define LINK size()

#define MAX_VERTEX 100000
// #define MAX_COUNT 100000
#define MAX_RAND 32768
#define MAX_LENGTH 1024
#define INF 1e8

#define DEFAULT_VALUE 0

FILE* fileName = NULL;

vector <pair <int, int> > vertex[MAX_VERTEX];// ���� 2���� �迭�� ������ ����ϱ� ���� [index] ����

int* rand_vData, * d;
int* graph_checkArray;

int disc[MAX_VERTEX + 1];
bool cut[MAX_VERTEX + 1];

bool cyclic_control = false;

char csv_data[1024];
int storage_max_data = DEFAULT_VALUE;
int pair_cnt = DEFAULT_VALUE;
int cnt = DEFAULT_VALUE;
int d_cnt = DEFAULT_VALUE;
int max_vNum = DEFAULT_VALUE;
int min_vNum = DEFAULT_VALUE;

/*
void gotoxy(int x, int y)
{
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
*/

void display_menu()
{
    cout << "1. DataSet File ���� " << "\n";
    cout << "2. Graph�� ���� ���� ���� " << "\n";
    cout << "3. ��� �� shortest path ���" << "\n";
    cout << "4. �������� ���� " << "\n";
    cout << "Q. ���α׷� ����(0�� �Է��ϸ� ����)" << "\n\n";
}

/* ����� �� ������ ��ȯ */
int vertex_count()
{
    int size = 0;
    for (int i = 1; i < MAX_VERTEX; i++)
        if (vertex[i].LINK > 0)
            size++;
    return size;
}

void swap(int* data1, int* data2)
{
    int* tmp = data1;
    data1 = data2;
    data2 = tmp;
}

/* ����� �ּ� ��ȣ�� �ִ� ��ȣ�� ���� */
void max_and_min_value_setting(int firstData, int secondData, int cnt)
{
    /* ó�� �������� ���(���� �� ���� ����) */
    if (cnt == 0) {
        if (firstData > secondData) { max_vNum = firstData; min_vNum = secondData; }
        else { max_vNum = secondData; min_vNum = firstData; }
    }
    else {
        if (firstData > max_vNum || secondData > max_vNum)
            (firstData > secondData) ? max_vNum = firstData : max_vNum = secondData;
        else if (firstData < min_vNum || secondData < min_vNum)
            (firstData < secondData) ? min_vNum = firstData : min_vNum = secondData;
    }
}

/* 1�� �޴� ���� �Լ� */
int random_vertex_link()
{
    int k = 1;
    int r1 = 0; int r2 = 0;
    int n = DEFAULT_VALUE;
    int v_count = vertex_count();
    //   cout << size << endl;

    /* ���� �޸� ũ��� �ٸ� ���, ���� ���� �Ҵ�� �޸𸮸� �����ϰ�, ����� */
    rand_vData = new int[v_count];

    fill(rand_vData, rand_vData + v_count, DEFAULT_VALUE);

    /* ����� ��ȣ�� ���ʴ�� ���� ��, �������� swap */
    for (int i = min_vNum; i <= max_vNum; i++) {
        if (vertex[i].LINK > 0)
            rand_vData[n++] = i;
    }

    if (n > MAX_RAND)
        k = n / MAX_RAND;

    for (int i = 0; i < 1000000; i++) {
        for (int j = 0; j < k; j++) {
            r1 += rand();
            r2 += rand();
        }
        swap(rand_vData[r1 % v_count], rand_vData[r2 % v_count]);
        r1 = 0, r2 = 0;
    }

    /* graph cyclic */
    for (int i = 0; i < v_count; i++) {
        vertex[rand_vData[i]].push_back({ rand_vData[(i + v_count + 1) % v_count], 1 });
        vertex[rand_vData[i]].push_back({ rand_vData[(i + v_count - 1) % v_count], 1 });
        cnt++;
    }
    return v_count;
    delete[] rand_vData;
}

int dataset_file_setting(const char* file_name)
{
    int v[3] = { DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE }; // 0��° index�� ���X, Dataset�� �� ��带 ����
    char* p;
    int a, b;

    //cout << "�����͸� ������ ������ �����ϼ��� >> "; ��� ����
    //cin >> a >> b; ��� ����

    if (fileName = fopen(file_name, "r")) {
        fgets(csv_data, MAX_LENGTH, fileName);
        while (1) {
            fgets(csv_data, MAX_LENGTH, fileName);
            if (feof(fileName)) {
                storage_max_data = vertex_count(); // ����� �ִ� ������ ����
                //cout << cnt << endl;
                //cout << storage_max_data << endl;
                //cout << max_vNum << endl;
                //cout << min_vNum << endl;
                fclose(fileName);
                break;
            }
            else {
                int cell = 0;
                p = strtok(csv_data, ",");
                while (p != NULL) {
                    if (cell == 1 || cell == 2) {
                        v[cell] = atoi(p);
                    }
                    cell++;
                    p = strtok(NULL, ",");
                }
    //            if (cnt >= a-1 && cnt <= b-1) { ��� ����
                    //printf("%s\n", csv_data); ��� ����
                    vertex[v[1]].push_back({ v[2], 1 });
                    vertex[v[2]].push_back({ v[1], 1 });
                    max_and_min_value_setting(v[1], v[2], pair_cnt);
                    pair_cnt++;
    //            } ��� ����

                cnt++;
            }
        }
        cnt = DEFAULT_VALUE;
        pair_cnt = 1;
        return true;
    }
    else // ������ ������ �ʰų�, ������ ��ο� �������� �ʴ� ���
        return false;
}

/* �����ͼ� ���� �Է� �Լ� */
int dataset_file_input_name()
{
    int num;
    int a, b;
    cout << "1. �����ͼ��� �߰�" << endl;
    cout << "2. ���� �����ͼ¿� cyclic ����" << endl;
    cin >> num;

    if (num == 1) {
        char* datafile_name = new char[50];
        cout << "���� �̸��� �Է��ϼ��� : ";
        cin >> datafile_name;
        if (!dataset_file_setting(datafile_name)) { // ������ ã�� ���� ���, -1�� ��ȯ
            cout << "Failed to find file" << endl;
            delete[] datafile_name;
            return -1;
        }
        cout << "�����ͼ� �߰� �Ϸ�! " << endl;
        delete[] datafile_name;
    }
    else if (num == 2) {
        if (storage_max_data == 0) {
            cout << "�����ͼ��� �������� �ʽ��ϴ�" << endl;
            return -1;
        }
        else {
            if (cyclic_control == false) {
                cyclic_control = true;
                random_vertex_link();
                cout << "cyclic ���� �Ϸ�!" << endl;
            }
            else
                cout << "�̹� �����Ͽ����ϴ�." << endl;
        }
    }
    else {
        cout << "�߸��� �����Դϴ�. " << endl;
        return -1;
    }

    return 0;
}


/* 2�� �޴� ���� �Լ� */

int dfs(int start, int check[])
{
    stack<int> st;
    int v_size = max_vNum;
    int v_count = 1;
    int* visited_array = new int[++v_size];
    fill(visited_array, visited_array + v_size, DEFAULT_VALUE);

    st.push(start);
    visited_array[start] = true;

    while (!st.empty()) {
        int cur = st.top();
        st.pop();

        for (int i = 0; i < vertex[cur].LINK; i++) {
            int next = vertex[cur][i].first;

            if (!visited_array[next]) {
                check[next]++;
                visited_array[next]++;
                v_count++;

                st.push(cur);
                st.push(next);
                break;
            }
        }
    }
    return v_count;
}

int graph_connect_check()
{
    int i = min_vNum;
    int graph_count = 0;
    graph_checkArray = new int[max_vNum+1];
    fill(graph_checkArray, graph_checkArray + max_vNum + 1, DEFAULT_VALUE);
    while (i < max_vNum) {
        if (graph_checkArray[i] == false && vertex[i].size() > 0) {
            dfs(i, graph_checkArray);
            graph_count++;
        }
        i++;
    }
    
    if (storage_max_data == 0) {
        cout << "�����Ͱ� �������� �ʽ��ϴ�. " << endl;
        return -1;
    }
    else {
        if (graph_count != 1) {
            cout << "�ش� �׷����� ���� �׷����Դϴ�." << endl;
            cout << "�׷����� ���� : " << graph_count << endl;
            delete[] graph_checkArray;
            return 0;
        }
        cout << "�ش� �׷����� ���� ������ �����ϴ�. " << endl;
    }
    delete[] graph_checkArray;
    return 0;
}

/* 3�� �޴� ���� �Լ� */

int max_length_value(int num)
{
    int lmax = 0;
    for (int i = min_vNum; i <= max_vNum; i++) {
        if (d[i] != INF && d[i] > lmax) {
            lmax = d[i];
        }
    }
    return lmax;
}

int min_length_value(int num)
{
    int lmin = INF; // �ִ� shortest path�� index ǥ��
    for (int i = min_vNum; i <= max_vNum; i++) {
        if (d[i] != INF && d[i] != 0 && d[i] < lmin) {
            lmin = d[i];
        }
    }
    return lmin;
}

float average_value(int num)
{
    float average = 0;
    int count = 0;
    for (int i = min_vNum; i <= max_vNum; i++) {
        if (d[i] != 0 && d[i] != INF) {
            average += d[i];
            count++;
        }
    }
    average /= count;
    return average;
}

void pq_dijkstra(int start)
{
    fill(d, d + max_vNum + 1, INF);

    priority_queue <pair <int, int>> pq;

    pq.push({ DEFAULT_VALUE, start });
    d[start] = DEFAULT_VALUE;

    while (!pq.empty()) {
        float current = -pq.top().first;
        int distance = pq.top().second;
        pq.pop();

        if (d[distance] < current) continue;
        for (int i = 0; i < vertex[distance].LINK; i++) {
            int cost = current + vertex[distance][i].second;
            if (cost < d[vertex[distance][i].first]) {
                d[vertex[distance][i].first] = cost;
                pq.push({ -cost, vertex[distance][i].first });
            }
        }
    }
}

int all_data_shortest_path()
{
    int max = 0, min = 0;
    float average = 0;

    int vsize = max_vNum + 1;
    d = new int[vsize];
    //fill(d, d + vsize, DEFAULT_VALUE);

    if (storage_max_data == 0) {
        cout << "�����Ͱ� �������� �ʽ��ϴ�. " << endl;
        return -1;
    }
    for (int i = min_vNum; i <= max_vNum; i++) {
        if (vertex[i].size() > 0) {
            fill(d, d + vsize, DEFAULT_VALUE);
            pq_dijkstra(i);
            max = max_length_value(i);
            min = min_length_value(i);
            average = average_value(i);
            cout << "[V] " << i << " -> ";
            cout << "MAX : " << max << "/ MIN : " << min << "/ AVERAGE : " << average << endl;
        }
    }

    delete[] d;
    return 0;
}


/* 4�� �޴� ���� �Լ� */
int dfs_spanning_tree(int here, bool r)
{
    disc[here] = ++d_cnt;
    int ret = disc[here];
    int child = DEFAULT_VALUE;

    for (int there = DEFAULT_VALUE; there < vertex[here].LINK; there++) {
        if (!disc[vertex[here][there].first]) {
            child++;
            int df = dfs_spanning_tree(vertex[here][there].first, DEFAULT_VALUE);
            if (!r && df >= disc[here])
                cut[here] = true;
            ret = min(ret, df);
        }
        else
            ret = min(ret, disc[vertex[here][there].first]);
    }
    if (r && child > 1)
        cut[here] = true;
    return ret;
}

int articulation_point_number()
{
    fill(disc, disc + MAX_VERTEX + 1, 0);
    fill(cut, cut + MAX_VERTEX + 1, false);
    int ans = DEFAULT_VALUE;
    for (int i = min_vNum; i <= max_vNum; i++) {
        if (!disc[i])
            dfs_spanning_tree(i, 1);
        if (cut[i])
            ans++;
    }

    if (storage_max_data == 0) {
        cout << "�����Ͱ� �������� �ʽ��ϴ�. " << endl;
        return -1;
    }
    if (ans == DEFAULT_VALUE) cout << "�������� �������� �ʽ��ϴ�." << "\n";
    else {
        cout << "�������� ���� : " << ans << "\n";
        cout << "< ";
        for (int i = min_vNum; i <= max_vNum; i++)
            if (cut[i])
                cout << i << " ";
        cout << ">" << endl;
    }
    return 0;
}

// ����� �� �ּ� ��ó��
void vertex_space_check()
{
    int v;
    cin >> v;
    if (v > 0 && v <= 100000) {
        if (vertex[v].size() > 0) {
            cout << v << " -> ";
            for (int i = 0; i < vertex[v].LINK; i++)
                cout << vertex[v][i].first << " ";
        }
        else
            cout << "NO DATA" << endl;
    }
    else
        cout << "������ ���� ����!" << endl;
    cout << endl;
}

/* ���� �޴� ���� �Լ� */

void exit_program()
{
    cout << "Program Exit" << endl;
    exit(0);
}
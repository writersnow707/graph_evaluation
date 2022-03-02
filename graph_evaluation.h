#pragma once
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <stack>
#include <queue>
#include <vector>
#include <ctime>
#include <Windows.h>

using namespace std;

void display_menu();
// void gotoxy(int x, int y);

/* 기본 함수 */
void swap(int* data1, int* data2);
void max_and_min_value_setting(int firstData, int secondData, int cnt);

/* 1번 메뉴 관련 함수 */
int random_vertex_link();
int dataset_file_setting(const char *file_name);
int dataset_file_input_name();

/* 2번 메뉴 관련 함수 */
int dfs(int start, int check[]);
int graph_connect_check();

/* 3번 메뉴 관련 함수 */
int max_length_value(int num);
int min_length_value(int num);
float average_value(int num);
void pq_dijkstra(int start);
int all_data_shortest_path();

/* 4번 메뉴 관련 함수 */
int dfs_spanning_tree(int here, bool r);
int articulation_point_number();

// 임시 함수
void vertex_space_check();

/* 종료 함수 */
void exit_program();
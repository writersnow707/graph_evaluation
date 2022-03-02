#include "graph_evaluation.h"
using namespace std;

int main(int argc, char *argv[])
{
	int num;
	srand(time(NULL));
	while (1) {
		display_menu();
		cin >> num;
		switch (num) {
		case 1: { dataset_file_input_name(); break; }
		case 2: { graph_connect_check(); break; }
		case 3: { all_data_shortest_path(); break; }
		case 4: { articulation_point_number(); break; }
		case 5: { vertex_space_check(); break; }
		case 0: { exit_program(); break; }
		}
		system("PAUSE");
		system("cls");
	}

	return 0;
}
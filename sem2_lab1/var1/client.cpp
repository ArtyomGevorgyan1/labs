#include <cstdio>
#include <windows.h>
#include <conio.h>

struct node {
	char type[32];
};

struct container {
	node typecollector[64];
};

struct meal {
	char name[64];
	char type[32];
	float price;
	bool show_flag;
	long int index;
};

container collect_types () {
	container types;
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 32; j++) {
			types.typecollector[i].type[j] = '\0';
		}
	}
	FILE* fp = fopen("meal.dat", "rb");
	fseek(fp, -sizeof(long int), SEEK_END);
	long int ind = 0;
	fread(&ind, sizeof(long int), 1, fp);
	fseek(fp, 0, SEEK_SET);
	int cnt = 0;
	for (int i = 0; i < ind; i++) {
		meal item;
		fread(&item, sizeof(item), 1, fp);
		for (int j = 0; j < 64; j ++) {
			if (types.typecollector[j].type[0] == '\0') {
				bool duplicate = false;
				for (int h = 0; h < 64; h++) {
					if (!strcmp(item.type, types.typecollector[h].type)) {
						duplicate = true;
						break;
					}
				}
				if (!duplicate) {
					for (int k = 0; k < sizeof(item.type); k++) {
						types.typecollector[j].type[k] = item.type[k];
					}
					cnt++;
				}
			}
		}
	}
	fclose(fp);
	return types;
}


void showasusual() {
	meal item;
	FILE* f = fopen("meal.dat", "rb");
	fseek(f, -sizeof(item), SEEK_END);
	fread(&item, sizeof(item), 1, f);
	long int index = item.index;
	fseek(f, 0, SEEK_SET);
	for (long int i = 0; i < index; i++) {
		meal item;
		fread(&item, sizeof(item), 1, f);
		if(item.show_flag) {
			printf("%s %s %0.2f %ld\n", item.name, item.type, item.price, item.index);
		}
	}
	fclose(f);
}

void showbytype() {
	meal item;
	FILE* f = fopen("meal.dat", "rb");
	fseek(f, -sizeof(item), SEEK_END);
	fread(&item, sizeof(item), 1, f);
	long int index = item.index;
	fseek(f, 0, SEEK_SET);
	container types = collect_types();
	int cnt = 0;
	for (int i = 0; i < 64; i++) {
		if (types.typecollector[i].type[0] != '\0') {
			cnt++;
		}
	}
	for (int i = 0; i < cnt; i++) {
		for (long int j = 0; j < index; j++) {
			fread(&item, sizeof(item), 1, f);
			if (strcmp(types.typecollector[i].type, item.type)) {
				if (item.show_flag) {
					printf("%s %s %0.2f %ld\n", item.name, item.type, item.price, item.index);
				}
			}
		}
		fseek(f, 0, SEEK_SET);
	}
	fclose(f);
}

void showmenu() {
	printf("Press 1 to show as usual, press 2 to show in terms of meal types\n");
	printf("ESC to exit this menu\n");
	char c;
	while (c != 27) {
		c = getch();
		system("cls");
		switch (c) {
			case '1': 
			showasusual();
			break;
			case '2':
			showbytype();
			break;
		}
	}
	
}

int main () {
	showmenu();
}
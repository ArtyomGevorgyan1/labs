#include <cstdio>
#include <windows.h>
#include <conio.h>

// TODO - NAVIGATION CROSS-PLATFORM USING ARROWS -  now using 49 50 51 52 keys

struct password {
	char* word;
};

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

void admin_check();
void add();
void remove();
void update();
void remove();
void showasusual();
void showbytype();
void showmenu();
container collect_types();

void menu() {
	char c;
	while (c != 27) {
		printf("What would you like to do?\n");
		printf("1 - add an item\n");
		printf("2 - delete an item\n");
		printf("3 - update an item\n");
		printf("4 - show table of meals\n");
		c = getch();
		system("cls");
		switch(c)  {
			case '1':
			add();
			break;
			case '2':
			remove();
			break;
			case '3':
			update();
			break;
			case '4':
			showmenu();
			break;
		}
	}
}

void add() {
	FILE* fp = fopen("meal.dat", "r+b");
	meal item;
	printf("Enter the name of the item\n");
	scanf("%s", &item.name);
	printf("Enter the type\n");
	scanf("%s", &item.type);
	printf("Price?\n");
	scanf("%f", &item.price);
	long int ind  = 0;
	fseek(fp, -sizeof(long int), SEEK_END);
	if (fread(&ind, sizeof(long int), 1, fp)) {
		item.index = ind + 1;
	} else {
		item.index = 1;
	}
	item.show_flag = true;
	fseek(fp, 0, SEEK_END);
	fwrite(&item, sizeof(item), 1, fp);
	fclose(fp);
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
			/// TF
			if (!strcmp(types.typecollector[i].type, item.type)) {
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

void remove() {
	printf("Type in the index\n");
	long int ind = 0;
	while (ind == 0) {
		printf("Remember, we count from 1, not 0!\n");
		scanf("%ld", &ind);
	}
	FILE* f = fopen("meal.dat", "r+b");
	meal item;
	long int last = 0;
	fseek(f, -sizeof(item), SEEK_END);
	fread(&item, sizeof(item), 1, f);
	last = item.index;
	fseek(f, 0, SEEK_SET);
	for (long int i = 0; i < last; i++) {
		fread(&item, sizeof(item), 1, f);
		if (item.index == ind) {
			printf("%ld\n", item.index);
			item.show_flag = false;
			fseek(f, -sizeof(item), SEEK_CUR);
			fwrite(&item, sizeof(item), 1, f);
			printf("Meal number %ld has been successfully deleted \n", item.index);
			break;
		} 
	}
	fclose(f);
}

void update () {
	printf("Type in the index\n");
	long int ind = 0;
	while (ind == 0) {
		printf("Remember, we count from 1, not 0!\n");
		scanf("%ld", &ind);
	}
	FILE* f = fopen("meal.dat", "r+b");
	meal item;
	long int last = 0;
	fseek(f, -sizeof(item), SEEK_END);
	fread(&item, sizeof(item), 1, f);
	last = item.index;
	fseek(f, 0, SEEK_SET);
	for (long int i = 0; i < last; i++) {
		fread(&item, sizeof(item), 1, f);
		if (item.index == ind) {
			printf("Enter the name of the item\n");
			scanf("%s", &item.name);
			printf("Enter the type\n");
			scanf("%s", &item.type);
			printf("Price?\n");
			scanf("%f", &item.price);
			item.show_flag = true;
			fseek(f, -sizeof(item), SEEK_CUR);
			fwrite(&item, sizeof(item), 1, f);
			printf("Meal number %ld has been succesfully updated\n", item.index);
			break;
		} 
	}
	fclose(f);
}

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

void cleaner() {
	FILE* temp = fopen("temp.dat","wb");
	FILE* fp = fopen("meal.dat", "rb");
	meal item;
	fread(&item, sizeof(item), 1, fp);
	long int ind = item.index;
	fseek(fp, 0, SEEK_SET);
	for (long int j = 0; j < ind; j++) {
		fread(&item, sizeof(item), 1, fp);
		if (item.show_flag) {
			fwrite(&item, sizeof(item), 1, temp);
		}
	}
	fclose(fp);
	fclose(temp);
	remove("meal.dat");
	rename("temp.dat","meal.dat");
}

void admin_check() {
	FILE* fp = fopen("admin.dat", "wb");
	password properword;
	char word[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', '\0'};
	properword.word = word;
	fwrite(&properword, sizeof(properword), 1, fp);
	fclose(fp);
	FILE* f = fopen("admin.dat", "rb");
	fread(&properword, sizeof(properword), 1, f);
	printf("Enter the password. Here is a little hint \n");
	printf("Do not show it to anyone %s\n", properword.word);
	char yourword[8];
	scanf("%s", &yourword);
	if (strcmp(yourword, properword.word)) {
		printf("You may try again next time \n");
	} else {
		menu();
		cleaner();
	}
}

int main() {
	admin_check();
}
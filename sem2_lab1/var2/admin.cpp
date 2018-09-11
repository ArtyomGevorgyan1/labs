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

struct meta {
	long int num;
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

//works 
void add() {
	meta info;
	meal item;
	printf("Enter the name of the item\n");
	scanf("%s", &item.name);
	printf("Enter the type\n");
	scanf("%s", &item.type);
	printf("Price?\n");
	scanf("%f", &item.price);
	FILE* fr = fopen("./meal/meta.dat", "rb");
	if (fread(&info, sizeof(info), 1, fr)) {
		item.index = info.num + 1;
		info.num = info.num + 1;
	} else {
		item.index = 1;
		info.num = 1;
	}
	FILE* w = fopen("./meal/meta.dat", "wb");
	fwrite(&info, sizeof(info), 1, w);
	fclose(w);
	long int index = item.index;
	item.show_flag = true;
	fclose(fr);

	// path 
	char name [50];
  	sprintf (name, "./meal/%ld.dat", index);

	FILE* fw = fopen(name, "wb");
	fwrite(&item, sizeof(item), 1, fw);
	fclose(fw);
}

//works
void showasusual() {
	meta info;
	meal item;
	FILE* fr = fopen("./meal/meta.dat", "rb");
	fread(&info, sizeof(info), 1, fr);
	fclose(fr);
	long int index = info.num;
	for (long int i = 1; i < index+1; i++) {
		char name[50];
		sprintf(name, "./meal/%ld.dat", i);
		FILE* frr = fopen(name, "rb");
		if (frr) {
			fread(&item, sizeof(item), 1, frr);
			if(item.show_flag) {
				printf("%s %s %0.2f %ld\n", item.name, item.type, item.price, item.index);
			}
			fclose(frr);
		}
		
	}
}

//works
void showbytype() { 
	meta info;
	meal item;
	FILE* f = fopen("./meal/meta.dat", "rb");
	fread(&info, sizeof(info), 1, f);
	long int index = info.num;
	fclose(f);
	container types = collect_types();
	int cnt = 0;
	for (int i = 0; i < 64; i++) {
		if (types.typecollector[i].type[0] != '\0') {
			cnt++;
		}
	}
	for (int i = 0; i < cnt; i++) {
		for (long int j = 1; j < index+1; j++) {
			char name[50];
			sprintf(name, "./meal/%ld.dat", j);
			FILE* ff = fopen(name, "rb");
			if (ff) {
				fread(&item, sizeof(item), 1, ff);
				fclose(ff);
				// tf
				if (!strcmp(types.typecollector[i].type, item.type)) {
					if (item.show_flag) {
						printf("%s %s %0.2f %ld\n", item.name, item.type, item.price, item.index);
					}
				}
			}
			
		}
		fseek(f, 0, SEEK_SET);
	}
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
	char name[50];
	sprintf(name, "./meal/%ld.dat", ind);
	FILE* f = fopen(name, "r+b");
	if (f) {
		meal item;
		fread(&item, sizeof(item), 1, f);
		fseek(f, 0, SEEK_SET);
		item.show_flag = false;
		fwrite(&item, sizeof(item), 1, f);
		printf("Meal number %ld has been successfully deleted \n", item.index);
		fclose(f);
	}
	
}

void update () {
	printf("Type in the index\n");
	long int ind = 0;
	while (ind == 0) {
		printf("Remember, we count from 1, not 0!\n");
		scanf("%ld", &ind);
	}
	char name[50];
	sprintf(name, "./meal/%ld.dat", ind);
	FILE* f = fopen(name, "wb");
	if (f) {
		meal item;
		printf("Enter the name of the item\n");
		scanf("%s", &item.name);
		printf("Enter the type\n");
		scanf("%s", &item.type);
		printf("Price?\n");
		scanf("%f", &item.price);
		item.show_flag = true;
		item.index = ind;
		fwrite(&item, sizeof(item), 1, f);
		printf("Meal number %ld has been succesfully updated\n", item.index);
	}
	fclose(f);
}

// fine 
container collect_types () {
	meta info;
	container types;
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 32; j++) {
			types.typecollector[i].type[j] = '\0';
		}
	}

	FILE* f = fopen("./meal/meta.dat", "rb");
	fread(&info, sizeof(info), 1, f);
	long int ind = info.num;
	fclose(f);

	int cnt = 0;
	for (long int i = 1; i < ind+1; i++) {
		meal item;
		char name[50];
		sprintf(name, "./meal/%ld.dat", i);
		FILE* ff = fopen(name, "rb");
		fread(&item, sizeof(item), 1, ff);
		fclose(ff);
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
	return types;
}

void cleaner() {
	meta info;
	FILE* f = fopen("./meal/meta.dat", "r+b");
	fread(&info, sizeof(info), 1, f);
	long int num = info.num;
	for (long int i = 1; i < num + 1; i++) {
		char name[50];
		sprintf(name, "./meal/%ld.dat", i);
		FILE* ff = fopen(name, "rb+");
		meal item;
		fread(&item, sizeof(item), 1, ff);
		fclose(ff);
		if (!item.show_flag) {
			//printf("esketit\n");
			char c = getch();
			remove(name);
			num--;
		}
	}
	info.num = num;
	fseek(f, 0, SEEK_SET);
	fwrite(&info, sizeof(info), 1, f);
	fclose(f);
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
#include "basic.h"

int round_cnt = 1, turn=1;
int player, lantern_direction;
char **alibi, detective_cards[7][3], jack_cards[7][3];
int a, d, c;
bool load=true;
bool prev_game=false;
bool two_characters = false;
bool arrest = false;
int color[39][108];

char *ToString(enum character_type character) {
    char *ptr = (char *) calloc(2, sizeof(char));
    if (character == SH)
        ptr = "SH";
    else if (character == JW)
        ptr = "JW";
    else if (character == JS)
        ptr = "JS";
    else if (character == IL)
        ptr = "IL";
    else if (character == MS)
        ptr = "MS";
    else if (character == SG)
        ptr = "SG";
    else if (character == WG)
        ptr = "WG";
    else if (character == JB)
        ptr = "JB";
    return ptr;
}

enum character_type ToEnum(char str[3]) {
    enum character_type character;
    if (strcmp(str, "SH") == 0) {
        character = SH;
    } else if (strcmp(str, "JW") == 0) {
        character = JW;
    } else if (strcmp(str, "JS") == 0) {
        character = JS;
    } else if (strcmp(str, "IL") == 0) {
        character = IL;
    } else if (strcmp(str, "MS") == 0) {
        character = MS;
    } else if (strcmp(str, "SG") == 0) {
        character = SG;
    } else if (strcmp(str, "WG") == 0) {
        character = WG;
    } else if (strcmp(str, "JB") == 0) {
        character = JB;
    }
    return character;
}

// این تابع نقشه گرافیکی را در فایل خروجی ذخیره می کند
void save_graphic_map() {
    FILE *fp = fopen("base.txt", "w");
    for (int i = 0; i < 39; i++) {
        fprintf(fp, "%s\n", graphic_map[i]);
    }
    fclose(fp);
}

// این تابع اطلاعات منطقی بازی را در یک فایل باینری ذخیره میکند.
void save_logical_map(){
    FILE *fp= fopen("logical_save.bin", "wb");
    fwrite(&round_cnt, sizeof (int), 1, fp);
    fwrite(&turn, sizeof (int), 1, fp);
    fwrite(&lantern_direction, sizeof (int), 1, fp);
    fwrite(&player, sizeof (int), 1, fp);
    fwrite(&a, sizeof (int), 1, fp);
    for (int i=0; i<=a; i++){
        fwrite(alibi[i],3 * sizeof (char), 1, fp );
    }
    fwrite(&d, sizeof (int), 1, fp);
    fwrite(detective_cards, 3* sizeof (char), d+1, fp);
    fwrite(&c, sizeof (int), 1, fp);
    fwrite(jack_cards, 3* sizeof (char), c+1, fp);
    fwrite(&mr_jack, sizeof (jack), 1, fp);
    fwrite(logical_map, sizeof (cell), 117, fp);
    fwrite(characters_status, sizeof (struct node), 8, fp);
    while (this_round!=NULL){
        fwrite(&(this_round->type), sizeof (int), 1, fp);
        this_round= this_round->next;
    }
    while (next_round!=NULL){
        fwrite(&(next_round->type), sizeof (int), 1, fp);
        next_round= next_round->next;
    }
    fclose(fp);
}

// در صورتی که کاربر تصمیم بگیرد بازی قبلی را ادامه دهد این تابع مپ گرافیکی ان را لود می کند
void load_graphic(){
    FILE  *fp= fopen("base.txt", "r");
    for (int i=0; i<39; i++){
        fscanf(fp, "%[^\n]%*c", graphic_map[i]);
    }
    fclose(fp);
}

// در صورتی که کاربر تصمیم بگیرد بازی قبلی را ادامه دهد این تابع مپ منطقی آن را از فایل باینری لود می کند.
void load_logic(){
    int limit1=4, limit2=4, i;
    char tmp[3];
    this_round = (struct node *) malloc(sizeof(struct node));
    next_round = (struct node *) malloc(sizeof(struct node));
    struct node *head;
    FILE *fp= fopen("logical_save.bin", "rb");
    if (fp==NULL){
        printf2(12, "%s", "\n There is no saved game, sorry\n")
        load=false;
        return;
    }
    fread(&round_cnt, sizeof (int), 1, fp);
    fread(&turn, sizeof (int), 1, fp);
    fread(&lantern_direction, sizeof (int), 1, fp);
    fread(&player, sizeof (int), 1, fp);
    fread(&a, sizeof (int), 1, fp);
    alibi= (char **) calloc(a+1, sizeof (char *));
    for (i=0; i<=a; i++){
        alibi[i]= (char *) calloc(3, sizeof (char));
        fread(tmp, sizeof (char), 3, fp);
        strcpy(alibi[i], tmp);
    }
    fread(&d, sizeof (int), 1, fp);
    fread(detective_cards, 3* sizeof (char), d+1, fp);
    fread(&c, sizeof (int), 1, fp);
    fread(jack_cards, 3* sizeof (char), c+1, fp);
    fread(&mr_jack, sizeof (jack), 1, fp);
    fread(logical_map, sizeof (cell), 117, fp);
    fread(characters_status, sizeof (struct node), 8, fp);
    if (round_cnt%2==1){
        limit1=5- turn;
        head=this_round;
        for (i=1; i< limit1; i++){
            fread(&(head->type), sizeof (int), 1, fp);
            head->next= (struct node *) malloc(sizeof(struct node));
            head=head->next;
        }
        fread(&(head->type), sizeof (int), 1,fp);
        head->next=NULL;
    }
    else {
        this_round=NULL;
        limit2= 5 -turn;
    }
    head=next_round;
    for (i=1; i< limit2; i++){
        fread(&(head->type), sizeof (int), 1, fp);
        head->next= (struct node *) malloc(sizeof(struct node));
        head=head->next;
    }
    fread(&(head->type), sizeof (int), 1,fp);
    head->next=NULL;
    fclose(fp);
}

// این تابع منوی بازی را در آغاز و حین بازی به کاربر نشان می دهد.
void menu(enum condition input, bool exit_option){
    int choice, i;
    int tmp;
    if (input==start){
        printf2(12, "%s", "\n\n\n                                                    WELCOME!\n")
        sleep(5);
        system("cls");
        printf("\n main menu:\n");
        printf("  1)start a new game\n  2)load the previous game\n  3)game rules\n");
        scanf("%d", &choice);
        system("cls");
        while (choice==3) {
            FILE *fp = fopen("rules.txt", "r");
            char str[500];
            while ((tmp = fgetc(fp)) != EOF) {
                fscanf(fp, "%[^\n]%*c", str);
                printf("%s\n", str);
            }
            printf("\n press any key to continue\n");
            getche();
            system("cls");
            fclose(fp);
            printf("\n main menu:\n");
            printf("  1)start a new game\n  2)load the previous game\n  3)game rules\n");
            scanf("%d", &choice);
            system("cls");
        }
        if (choice==2){
            load_logic();
            if (load==false){
                printf("\n press any key to start a new game\n");
                getche();
                choice=1;
            }
            else {
                prev_game=true;
                load_graphic();
                print_map();
            }
        }
        if (choice==1){
            creat_map();
            fill_logical_map();
            fill_graphic_map();
            save_graphic_map();
            print_map();
            shuffle_cards();
            mr_jack.character = list->type;
            printf2(12, "%s", "\n WARNING! ONLY MR.JACK SHOULD READ THIS!\n")
            sleep(3);
            system("cls");
            print_map();
            printf2(12,"\n Mr.Jack character is %s\n", ToString(mr_jack.character))
            sleep(4);
            system("cls");
            print_map();
            free(list);
            // تشکیل لیست مظنونین برای اکشن شرلوک
            alibi = (char **) calloc(7, sizeof(char *));
            for (i = 1; i <= 8; i++) {
                if (i != mr_jack.character) {
                    alibi[a] = (char *) calloc(3, sizeof(char));
                    strcpy(alibi[a], ToString(i));
                    a++;
                }
            }
            a--;
            // تشکیل لیست کارکتر ها برای فاز شهود
            for (i = 1; i <= 8; i++) {
                characters_status[i - 1].type = i;
            }
        }

    }


    else {
        printf("\n main menu:\n");
        printf("  0)back to game\n  1)game rules\n  2)show my alibi cards\n");
        if (exit_option==true){
            printf("  3)exit\n");
        }
        scanf("%d", &choice);
        while (choice==2 || choice==1) {
            while (choice == 1) {
                FILE *fp = fopen("rules.txt", "r");
                char str[500];
                while ((tmp = fgetc(fp)) != EOF) {
                    fscanf(fp, "%[^\n]%*c", str);
                    printf("%s\n", str);
                }
                printf("\n press any key to continue\n");
                getche();
                system("cls");
                fclose(fp);
                printf("\n main menu:\n");
                printf("  0)back to game\n  1)game rules\n  2)show my alibi cards\n");
                if (exit_option == true) {
                    printf("  3)exit\n");
                }
                scanf("%d", &choice);
            }
            while (choice == 2) {
                if (player == detective) {
                    if (d == 0) {
                        printf("\n you have no alibi cards!\n");
                        sleep(2);
                        system("cls");
                        printf("\n main menu:\n");
                        printf("  0)back to game\n  1)game rules\n  2)show my alibi cards\n");
                        if (exit_option == true) {
                            printf("  3)exit\n");
                        }
                        scanf("%d", &choice);
                    } else {
                        printf2(12, "%s", "\n WARNING! ONLY DETECTIVE SHOULD READ THIS!\n")
                        printf(" your alibi card(s): ");
                        for (i = 0; i <= d; i++) {
                            printf("%s ", detective_cards[i]);
                        }
                        fflush(stdin);
                        printf("\n press any key to continue\n");
                        getche();
                        system("cls");
                        printf("\n main menu:\n  0)back to game\n  1)game rules\n  2)show my alibi cards\n");
                        if (exit_option == true) {
                            printf("  3)exit\n");
                        }
                        scanf("%d", &choice);
                    }
                } else {
                    if (c == 0) {
                        printf("\n you have no alibi cards!\n");
                        sleep(2);
                        system("cls");
                        printf("\n main menu:\n  0)back to game\n  1)game rules\n  2)show my alibi cards\n");
                        if (exit_option == true) {
                            printf("  3)exit\n");
                        }
                        scanf("%d", &choice);
                    } else {
                        printf2(12, "%s", "\n WARNING! ONLY MR.JACK SHOULD READ THIS!\n")
                        printf(" your alibi card(s): ");
                        for (i = 0; i <= c; i++) {
                            printf("%s ", jack_cards[i]);
                        }
                        fflush(stdin);
                        printf("\n press any key to continue\n");
                        getche();
                        system("cls");
                        printf("\n main menu:\n  0)back to game\n  1)game rules\n  2)show my alibi cards\n");
                        if (exit_option == true) {
                            printf("  3)exit\n");
                        }
                        scanf("%d", &choice);
                    }
                }
            }
        }
        if (choice==3) {
            printf("\n do you want to save the game?\n  1)yes    2)no\n");
            scanf("%d", &choice);
            if (choice == 1) {
                save_graphic_map();
                save_logical_map();
                system("cls");
                printf2(12, "%s", "\n\n                                                see you later\n")
                sleep(3);
                system("cls");
            }
            else {
                remove("logical_save.bin");
            }
            mr_jack.end_of_game= true;
        }
        else if (choice==0){
            sleep(1);
            system("cls");
            print_map();
            return;
        }
    }
}

//این تابع اکشن های موجود در هر مرحله از بازی را برای بازیکن نمایش می دهد
void print_actions(struct node *actions) {
    char inname[10] = "1.txt";
    char info[3][500];
    int j = 1, tmp;
    printf("\n 0)main menu\n");
    while (actions != NULL) {
        inname[0] = actions->type + 48;
        FILE *fp = fopen(inname, "r");
        printf(" %d) ", j);
        j++;
        for (int i = 0; i < 3; i++) {
            fscanf(fp, "%[^\n]%*c", info[i]);
            printf("%s\n", info[i]);
        }
        actions = actions->next;
    }
}
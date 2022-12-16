/* egg_after_dragon.c
 * gcc -Wall -z execstack egg_after_dragon.c -o egg_after_dragon
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_DRAGONS 5

#define MENU_FIND_EGG       1
#define MENU_INCUBATE_EGG   2
#define MENU_SEND_HUNTING   3
#define MENU_VIEW_EGG       4
#define MENU_VIEW_DRAGONS   5
#define MENU_QUIT           6

#define SMALL_EGG   0
#define MEDIUM_EGG  1
#define LARGE_EGG   2

#define SMALL_EGG_TEXT_LEN      0x10
#define MEDIUM_EGG_TEXT_LEN     0x30
#define LARGE_EGG_TEXT_LEN      0x50

#define BABY_DRAGON_DESC_LEN        0x20
#define CHILD_DRAGON_DESC_LEN       0x40
#define YOUNG_DRAGON_DESC_LEN       0x60
#define GIANT_DRAGON_DESC_LEN       0x80
#define ULTIMATE_DRAGON_DESC_LEN    0x30
#define LEGEND_DRAGON_DESC_LEN      0x50

struct egg_t {
    unsigned int text_len;
    char name[48];
    char *text;
};

struct dragon_t {
    char name[16];
    void (*skill)();
    unsigned int desc_len;
    char desc[1];
};

void Init();
void PrintMenu();
int GetNum();

void FindEgg();
void IncubateEgg();
void SendHunting();
void ViewEgg();
void ViewDragons();

void DragonSkillBite();
void DragonSkillScratch();
void DragonSkillTailSwinging();
void DragonSkillFireball();
void DragonSkillRapidFireballs();
void DragonSkillDragonBreath();

void HatchDragon(unsigned int idx,
                 unsigned int new_desc_len,
                 char *new_name,
                 void (*new_skill)(),
                 char *new_desc);

void UpgradeDragon(unsigned int idx,
                   unsigned int prev_desc_len,
                   unsigned int new_desc_len,
                   char *new_name,
                   void (*new_skill)(),
                   char *new_desc);

void Fight(unsigned int idx);
void LevelUp(unsigned int idx);
void Flee(unsigned int idx);

struct egg_t *egg;
struct dragon_t *dragon_list[MAX_DRAGONS];

char *monster_names[] = {
    "spider",
    "bee",
    "zombie",
};

char *monster_prefixes[] = {
    "deadly",
    "giant",
    "strong",
    "nimble",
};

int main(void) {
    int menu;

    Init();
    do {
        PrintMenu();
        menu = GetNum();
        switch (menu) {
        case MENU_FIND_EGG:
            FindEgg();
            break;
        case MENU_INCUBATE_EGG:
            IncubateEgg();
            break;
        case MENU_SEND_HUNTING:
            SendHunting();
            break;
        case MENU_VIEW_EGG:
             ViewEgg();
            break;
        case MENU_VIEW_DRAGONS:
            ViewDragons();
            break;
        case MENU_QUIT:
            break;
        default:
            puts("wrong menu!");
        }
    } while (menu != MENU_QUIT);
    return 0;
}

void Init() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    srand(time(0));
}

void PrintMenu() {
    puts("====================================================");
    puts("||                                                ||");
    puts("||               Dragon Raising Game              ||");
    puts("||          - From egg to legend dragon -         ||");
    puts("||                                                ||");
    puts("====================================================");
    puts("1. Find an egg");
    puts("2. Incubate an egg");
    puts("3. Send hunting");
    puts("4. View your egg");
    puts("5. View your dragon");
    puts("6. quit");
    printf("> ");
}

int GetNum() {
    char buf[11];

    memset(buf, 0x00, sizeof(buf));
    read(0, buf, sizeof(buf) - 1);
    return atoi(buf);
}

void FindEgg() {
    char yn;

    if (egg != NULL) {
        puts("\nYou already have egg!\n");
        return;
    }

    printf("\nYou went out to the forest...");

    egg = (struct egg_t *)malloc(sizeof(struct egg_t));
    switch (rand() % 3) {
    case SMALL_EGG:
        strcpy(egg->name, " a small egg");
        egg->text_len = SMALL_EGG_TEXT_LEN;
        break;
    case MEDIUM_EGG:
        strcpy(egg->name, " a medium egg");
        egg->text_len = MEDIUM_EGG_TEXT_LEN;
        break;
    case LARGE_EGG:
        strcpy(egg->name, " a large egg");
        egg->text_len = LARGE_EGG_TEXT_LEN;
        break;
    }
    egg->text = malloc(egg->text_len);

    printf("\n\nYou found%s in the forest. "
            "Do you want to bring it to home (y/n)? ", egg->name);
    scanf(" %c%*c", &yn);
    if (yn == 'n') {
        memset(egg->text, 0x00, egg->text_len);
        free(egg->text);
        egg->text = NULL;
        free(egg);
        egg = NULL;
        puts("OK. you just threw away the egg.");
        return;
    }

    printf("OK. You took the egg to home.\n"
            "Do you want to engrave some text on the egg (y/n)? ");
    scanf(" %c%*c", &yn);
    if (yn == 'y') {
        printf("Write text: ");
        read(0, egg->text, egg->text_len - 1);
        printf("Done.%s is engraved with "
                "following text: %s", egg->name, egg->text);
    }
    puts("");
}

void IncubateEgg() {
    int i;

    if (egg == NULL) {
        puts("\nYou don't have egg!\n");
        return;
    }

    for (i = 0; i < MAX_DRAGONS; i++)
        if (dragon_list[i] == NULL)
            break;
    if (i == MAX_DRAGONS) {
        puts("\nYour dragon list is full!\n");
        return;
    }

    if (!strcmp(egg->name, " a small egg")) {
        HatchDragon(i,
                    BABY_DRAGON_DESC_LEN,
                    "baby dragon",
                    DragonSkillBite,
                    "this is just a small dragon.");

    } else if (!strcmp(egg->name, " a medium egg")) {
        HatchDragon(i,
                    CHILD_DRAGON_DESC_LEN,
                    "child dragon",
                    DragonSkillScratch,
                    "this dragon is bigger than baby dragon "
                    "and has sharp claws.");

    } else {
        HatchDragon(i,
                    YOUNG_DRAGON_DESC_LEN,
                    "young dragon",
                    DragonSkillTailSwinging,
                    "this dragon attacks monsters with swinging its tail.");
    }

    printf("\nIncubating%s...\n", egg->name);
    printf("\'%s\' is hatched :)\n\n", dragon_list[i]->name);

    free(egg->text);
    egg->text = NULL;
    free(egg);
    egg = NULL;
}


void SendHunting() {
    int i;

    printf("\nIndex of dragon you want to send hunting? ");
    scanf(" %u", &i);
    if (dragon_list[i] && dragon_list[i]->skill) {
        printf("\nYou sent %s to hunting...\n", dragon_list[i]->name);
        Fight(i);
        if (rand() % 2 != 0)
            LevelUp(i);
        else
            Flee(i);
    } else {
        puts("\nno such dragon!\n");
    }
}

void ViewEgg() {
    if (egg) {
        puts("\n << Egg info >>");
        printf("- Egg name:%s\n", egg->name);
        printf("- Engraved text: %s\n", egg->text);
    } else {
        puts("\nYou don't have egg.\n");
    }
}

void ViewDragons() {
    int i;

    puts("\n << List of your dragons >>");
    for (i = 0; i < MAX_DRAGONS; i++) {
        if (dragon_list[i] && dragon_list[i]->skill) {
            printf("- [%u] name: %s\n", i, dragon_list[i]->name);
            printf("      description: %s\n", dragon_list[i]->desc);
        } else {
            printf("- [%u] there is no dragon\n", i);
        }
    }
    puts("");
}

void DragonSkillBite() {
    puts("Your dragon bit the monster hard!");
    puts("The monster is defeated!");
}

void DragonSkillScratch() {
    puts("Your dragon scratched the monster by using claws!");
    puts("The monster is defeated!");
}

void DragonSkillTailSwinging() {
    puts("Your dragon attacked the monster by using the tail!");
    puts("The monster is defeated!");
}

void DragonSkillFireball() {
    puts("Your dragon shot out fireball to the monster!");
    puts("The monster disappeared leaving ashes..!");
}

void DragonSkillRapidFireballs() {
    puts("Your dragon rapidly shot out multiple fireballs to the monster!");
    puts("The monster disappeared leaving little ashes..!");
}

void DragonSkillDragonBreath() {
    puts("Your dragon shot out dragon's breath to the monster!");
    puts("The monster disappeared without any ashes...!");
}

void HatchDragon(unsigned int idx,
                 unsigned int new_desc_len,
                 char *new_name,
                 void (*new_skill)(),
                 char *new_desc) {
    dragon_list[idx] = malloc(sizeof(struct dragon_t) + new_desc_len);
    strcpy(dragon_list[idx]->name, new_name);
    dragon_list[idx]->skill = new_skill;
    dragon_list[idx]->desc_len = new_desc_len;
    strcpy(dragon_list[idx]->desc, new_desc);
}

void UpgradeDragon(unsigned int idx,
                   unsigned int prev_desc_len,
                   unsigned int new_desc_len,
                   char *new_name,
                   void (*new_skill)(),
                   char *new_desc) {
    memset(dragon_list[idx], 0x00, sizeof(struct dragon_t) + prev_desc_len);
    free(dragon_list[idx]);

    dragon_list[idx] = malloc(sizeof(struct dragon_t) + new_desc_len);
    strcpy(dragon_list[idx]->name, new_name);
    dragon_list[idx]->skill = new_skill;
    strcpy(dragon_list[idx]->desc, new_desc);
}

void Fight(unsigned int idx) {
    int idx1;
    int idx2;
    char monster_name[14];

    idx1 = rand() % 4;
    idx2 = rand() % 3;
    sprintf(monster_name, "%s %s", monster_prefixes[idx1], monster_names[idx2]);

    printf("%s found %s!\n\n", dragon_list[idx]->name, monster_name);
    dragon_list[idx]->skill();
    printf("%s knocked down %s!\n\n", dragon_list[idx]->name, monster_name);
}

void LevelUp(unsigned int idx) {
    if (!strcmp(dragon_list[idx]->name, "baby dragon")) {
        UpgradeDragon(idx,
                      BABY_DRAGON_DESC_LEN,
                      CHILD_DRAGON_DESC_LEN,
                      "child dragon",
                      DragonSkillScratch,
                      "this dragon is bigger than "
                      "baby dragon and has sharp claws.");

    } else if (!strcmp(dragon_list[idx]->name, "child dragon")) {
        UpgradeDragon(idx,
                      CHILD_DRAGON_DESC_LEN,
                      YOUNG_DRAGON_DESC_LEN,
                      "young dragon",
                      DragonSkillTailSwinging,
                      "this dragon attacks monsters by swinging its tail.");

    } else if (!strcmp(dragon_list[idx]->name, "young dragon")) {
        UpgradeDragon(idx,
                      YOUNG_DRAGON_DESC_LEN,
                      GIANT_DRAGON_DESC_LEN,
                      "giant dragon",
                      DragonSkillFireball,
                      "giant dragon has huge body size. this dragon "
                      "wipes out emenies by using strong fireball.");

    } else if (!strcmp(dragon_list[idx]->name, "giant dragon")) {
        UpgradeDragon(idx,
                      GIANT_DRAGON_DESC_LEN,
                      ULTIMATE_DRAGON_DESC_LEN,
                      "ultimate dragon",
                      DragonSkillRapidFireballs,
                      "ultimate dragon is small but very powerful.");

    } else if (!strcmp(dragon_list[idx]->name, "ultimate dragon")) {
        UpgradeDragon(idx,
                      ULTIMATE_DRAGON_DESC_LEN,
                      LEGEND_DRAGON_DESC_LEN,
                      "legend dragon",
                      DragonSkillDragonBreath,
                      "this dragon uses dragon breath. it's catastrophic.");
    } else {
        return;
    }
    printf("Congratulations! "
            "Your dragon Level up to %s!\n\n", dragon_list[idx]->name);
}

void Flee(unsigned int idx) {
    puts("Unfortunately, your dragon ran away to find freedom...\n");
    dragon_list[idx]->skill = NULL;
    free(dragon_list[idx]);
}

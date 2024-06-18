#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define MAX_LINE_LENGTH 1024

void logSDLError(const char *msg) {
    fprintf(stderr, "%s error: %s\n", msg, SDL_GetError());
}

void logTTFError(const char *msg) {
    fprintf(stderr, "%s error: %s\n", msg, TTF_GetError());
}
// struct 

//font크기 모음 구조체
typedef struct Fonts {
    TTF_Font* font10;
    TTF_Font* font20;
    TTF_Font* font30;
    TTF_Font* font40;
    TTF_Font* font50;
    TTF_Font* font60;
} Fonts;

typedef struct Box {
    SDL_Rect rect;
    SDL_Color background_color;
    SDL_Texture* texture;
} Box;
typedef struct Button {
    Box box;
    SDL_Color textColor;
    const char* text;
    TTF_Font* font;
} Button;


SDL_Texture* renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_Color color) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (surface == NULL) {
        logTTFError("TTF_RenderText_Solid");
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL) {
        logSDLError("SDL_CreateTextureFromSurface");
    }
    return texture;
}

bool isMouseOverButton(Button button, int x, int y) {
    return x >= button.box.rect.x && x <= button.box.rect.x + button.box.rect.w &&
           y >= button.box.rect.y && y <= button.box.rect.y + button.box.rect.h;
}

void renderButton(SDL_Renderer *renderer, Button button) {
    SDL_SetRenderDrawColor(renderer, button.box.background_color.r, button.box.background_color.g, button.box.background_color.b, button.box.background_color.a);
    SDL_RenderFillRect(renderer, &button.box.rect);
    int texW = 0;
    int texH = 0;
    SDL_QueryTexture(button.box.texture, NULL, NULL, &texW, &texH);
    SDL_Rect dstRect = {button.box.rect.x + (button.box.rect.w - texW) / 2, button.box.rect.y + (button.box.rect.h - texH) / 2, texW, texH};
    SDL_RenderCopy(renderer, button.box.texture, NULL, &dstRect);
}
//선수 데이터 구조체

typedef struct Body {
    int height;
    int weight;
} Body;

typedef struct Foot_ability
{
    int left;
    int right;
} Foot_ability;

typedef struct Ability_stat{
    int corners;            int crossing;       int dribbling;          int finishing;              int first_touch;
    int free_kick_taking;   int heading;        int long_shot;          int long_throws;            int marking;
    int passing;            int penalty_taking; int tackling;           int technique;              int aggressiion;
    int anticipation;       int bravery;        int composure;          int concentration;          int vision;
    int decision;           int determination;  int flair;              int leadership;             int off_the_ball;
    int position;           int teamwork;       int work_rate;          int acceleration;           int agility;
    int balance;            int jumping_reach;  int natural_fitness;    int pace;                   int stamina;
    int strength;           int stability;      int foul;               int contest_performance;    int injury;
    int diversity;          int aerial_reach;   int command_of_area;    int communication;          int eccentricity;
    int handling;           int kicking;        int one_on_ones;        int reflexes;               int rushing_out; 
    int throwing;           int adaptation;     int ambition;           int argue;                  int loyal; 
    int professional;       int sportmanship;   int emotional_control;  int punching;               int resistant_to_stress;
} Ability_stat;

typedef struct Position_stat {
    //GK
    int GK;
    //Defender
    int DL; int DC; int DR; int WBL; int WBR;
    //Midfielder
    int DM; int ML; int MC; int MR; 
    //attacker
    int AML; int AMC; int AMR; int ST;
} Position_stat;

typedef struct Player {
    char name[100];
    char position[100];
    int age;
    int ca;
    int pa;
    char nationality[100];
    char club[100];
    Ability_stat abiltiy_stat;
    Position_stat position_stat;
    Body body;
    Foot_ability foot_ability;
    int values;
    int current_reputation;
    int domestic_reputation;
    int world_reputation;
    int RCA;
    char birth[20];
    int number_of_national_team_appearance;
    int goals_scored_for_the_national_team;
    int salary;
    int UID;

} Player;

typedef struct Node {
    Player player;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct Circularlist {
    Node* head;
} Circularlist;

void initList(Circularlist* list) {
    list->head = NULL;
}
Node* createNode(Player player) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->player = player;
    newNode->next = NULL;
    return newNode;
}

void appendNode(Circularlist* list, Player player) {
    Node* newNode = createNode(player);
    if (list->head == NULL) {
        list->head = newNode;
        list->head->next = list->head;
        list->head->prev = list->head;
    }
    else {
        Node* tail = list->head->prev;
        tail->next = newNode;
        newNode->prev = tail;
        newNode->next = list->head;
        list->head->prev = newNode;
    }
}

void printList(Circularlist* list) {
    if (list->head == NULL) {
        printf("List is empty\n");
        return;
    }
    Node* current = list->head;
    do {
        printf("Name: %s, Position: %s, Age: %d, UID: %d\n", current->player.name, current->player.position, current->player.age, current->player.UID);
        current = current->next;
    } while (current != list->head);
}

void readCSV(const char* filename, Circularlist* list) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        return;
    }

    
    char line[MAX_LINE_LENGTH];
    Player player;
    while (fgets(line, sizeof(line), file)) {

        char* token = strtok(line, ",");
        if (token != NULL) strcpy(player.name, token);
        
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(player.position, token);

        token = strtok(NULL, ",");
        if (token != NULL) player.age = atoi(token);

        token = strtok(NULL, ",");
        if (token != NULL) player.ca = atoi(token);

        token = strtok(NULL,",");
        if (token != NULL) player.pa = atoi(token);

        token = strtok(NULL,",");
        if (token != NULL) strcpy(player.nationality, token);

        token = strtok(NULL,",");
        if (token != NULL) strcpy(player.club, token);
        //능력치 데이터
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.corners = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.crossing = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.dribbling = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.finishing = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.first_touch = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.free_kick_taking = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.heading = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.long_shot = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.long_throws = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.marking = atoi(token);
        //10
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.passing = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.penalty_taking = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.tackling =atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.technique = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.aggressiion = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.anticipation = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.bravery = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.composure = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.concentration = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.vision = atoi(token);
        //20
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.decision = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.determination = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.flair = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.leadership = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.off_the_ball = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.position = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.teamwork = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.work_rate = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.acceleration = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.agility = atoi(token);
        //30
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.balance = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.jumping_reach = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.natural_fitness = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.pace = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.stamina = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.strength = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.stability = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.foul = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.contest_performance = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.injury = atoi(token);
        //40
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.diversity = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.aerial_reach = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.command_of_area = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.communication = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.eccentricity = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.handling = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.kicking = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.one_on_ones = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.reflexes = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.rushing_out = atoi(token);
        //50
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.punching = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.throwing = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.adaptation = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.ambition = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.argue = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.loyal = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.resistant_to_stress = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.professional = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.sportmanship = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.abiltiy_stat.emotional_control = atoi(token);
        //60
        //포지션 스탯
        token = strtok(NULL,","); if (token != NULL) player.position_stat.GK = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.position_stat.DL = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.position_stat.DC = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.position_stat.DR = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.position_stat.WBL = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.position_stat.WBR = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.position_stat.DM = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.position_stat.ML = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.position_stat.MC = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.position_stat.MR = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.position_stat.AML = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.position_stat.AMC = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.position_stat.AMR = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.position_stat.ST = atoi(token);

        //키 몸무게
        token = strtok(NULL,",");
        if (token != NULL) player.body.height = atoi(token);
        token = strtok(NULL,",");
        if (token != NULL) player.body.weight = atoi(token);
        //발 능력치
        token = strtok(NULL,",");
        if (token != NULL) player.foot_ability.left = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) player.foot_ability.right = atoi(token);
        //선수 가치
        token = strtok(NULL, ",");
        if (token != NULL) player.values = atoi(token);
        //current reputation
        token = strtok(NULL, ",");
        if (token != NULL) player.current_reputation = atoi(token);
        //domestic reputation
        token = strtok(NULL, ",");
        if (token != NULL) player.domestic_reputation = atoi(token);
        //world reputation 
        token = strtok(NULL, ",");
        if (token != NULL) player.world_reputation = atoi(token);
        //RCA
        token = strtok(NULL, ",");
        if (token != NULL) player.RCA = atoi(token);
        //date of birth
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(player.birth, token);
        //number of national team appearance
        token = strtok(NULL, ",");
        if (token != NULL) player.number_of_national_team_appearance = atoi(token);
        //goals scored for the national team
        token = strtok(NULL, ",");
        if (token != NULL) player.goals_scored_for_the_national_team = atoi(token);
        //salary
        token = strtok(NULL, ",");
        if (token != NULL) player.salary = atoi(token);
        //UID
        token = strtok(NULL, ",");
        if (token != NULL) player.UID = atoi(token);

        appendNode(list, player);
    }
    fclose(file);
}

void searchPlayer(Circularlist* player_list, Circularlist* search_list) {
    if (player_list->head == NULL) {
        printf("List is empty\n");
        return;
    }
    Node* current = player_list->head;
    do {
        if (current->player.pa > 170 && current->player.ca <150 && current->player.age < 20) {
            appendNode(search_list,current->player);
        }
        current = current->next;
    } while (current != player_list->head);
}

void startMain(SDL_Renderer* r, SDL_Event* e,Fonts* fonts) {
    SDL_Color textColor = {0,0,0,255};
    SDL_Color box_color = {255,255,255,255};
    SDL_Color button_color = {200,100,150,255};
    SDL_Color button_hover_color = {200,200,150,255};

    Button titleBox = {{{490, 300, 300, 150},box_color,NULL},textColor,"Football Agency", fonts->font60};
    Button startButton = {{{550,500,200, 50},button_color,NULL}, textColor,"Start",fonts->font40};

    titleBox.box.texture = renderText(r,titleBox.font,titleBox.text,titleBox.textColor);
    startButton.box.texture = renderText(r, startButton.font, startButton.text, startButton.textColor);

    if(titleBox.box.texture == NULL || startButton.box.texture == NULL) {
        SDL_DestroyTexture(titleBox.box.texture);   
        SDL_DestroyTexture(startButton.box.texture);
    }
    bool quit = false;
    while(!quit) {
        while(SDL_PollEvent(e)) {
            if (e->type == SDL_KEYDOWN) {
                if (e->key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
            }
            else if (e->type == SDL_QUIT) {
                quit = true;
            }
            else if (e->type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (isMouseOverButton(startButton, x, y)) {
                    printf("start button clicked\n");
                    quit = true;
                } 
            }
        }
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (isMouseOverButton(startButton, x, y)) {
            startButton.box.background_color = button_hover_color;
        } else {
            startButton.box.background_color = button_color;
        }
        SDL_SetRenderDrawColor(r, 255,255,255,255);
        SDL_RenderClear(r);
        renderButton(r, titleBox);
        renderButton(r, startButton);
        SDL_RenderPresent(r);
    }
    SDL_DestroyTexture(titleBox.box.texture);   
    SDL_DestroyTexture(startButton.box.texture);
}

void where_position_recommended_screen(SDL_Renderer* r, SDL_Event* e, Fonts* fonts) {
    SDL_Color textColor = {0,0,0,255};
    SDL_Color box_color = {255,255,255,255};
    SDL_Color button_color = {200,100,150,255};
    SDL_Color button_hover_color = {200,200,150,255};

    Button attackerButton = {{{500,100, 300,100},box_color,NULL},textColor,"Attacker",fonts->font40};
    Button midfielderButton = {{{500,250,300,100},box_color,NULL},textColor,"Midfielder",fonts->font40};
    Button defenderButton = {{{500,400,300,100},box_color,NULL},textColor,"Defender",fonts->font40};
    Button goalkeeperButton = {{{500,550,300,100},box_color,NULL},textColor,"GoalKeeper",fonts->font40};

    attackerButton.box.texture = renderText(r,attackerButton.font,attackerButton.text,attackerButton.textColor);
    midfielderButton.box.texture = renderText(r,midfielderButton.font,midfielderButton.text,midfielderButton.textColor);
    defenderButton.box.texture = renderText(r,defenderButton.font,defenderButton.text,defenderButton.textColor);
    goalkeeperButton.box.texture = renderText(r,goalkeeperButton.font,goalkeeperButton.text,goalkeeperButton.textColor);

    if(attackerButton.box.texture == NULL || midfielderButton.box.texture == NULL || \
        defenderButton.box.texture == NULL || goalkeeperButton.box.texture == NULL) {
        SDL_DestroyTexture(attackerButton.box.texture);   
        SDL_DestroyTexture(midfielderButton.box.texture);
        SDL_DestroyTexture(defenderButton.box.texture);
        SDL_DestroyTexture(goalkeeperButton.box.texture);
    }
    bool quit = false;
    while(!quit) {
        while(SDL_PollEvent(e)) {
            if (e->type == SDL_KEYDOWN) {
                if (e->key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
            }
            else if (e->type == SDL_QUIT) {
                quit = true;
            }
            else if (e->type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (isMouseOverButton(attackerButton, x, y)) {
                    printf("attacker button clicked\n");
                    quit = true;
                }
                else if (isMouseOverButton(midfielderButton,x,y)) {
                    printf("mid button");
                } 
                else if (isMouseOverButton(defenderButton,x,y)) {
                    printf("defend button");
                }
                else if (isMouseOverButton(goalkeeperButton, x, y)) {
                    printf("goal button");
                }
            }
        }
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (isMouseOverButton(attackerButton, x, y)) {
            attackerButton.box.background_color = button_hover_color;
        } else {
            attackerButton.box.background_color = button_color;
        }
        if (isMouseOverButton(midfielderButton, x, y)) {
            midfielderButton.box.background_color = button_hover_color;
        } else {
            midfielderButton.box.background_color = button_color;
        }
        if (isMouseOverButton(defenderButton, x, y)) {
            defenderButton.box.background_color = button_hover_color;
        } else {
            defenderButton.box.background_color = button_color;
        }
        if (isMouseOverButton(goalkeeperButton, x, y)) {
            goalkeeperButton.box.background_color = button_hover_color;
        } else {
            goalkeeperButton.box.background_color = button_color;
        }
        SDL_SetRenderDrawColor(r, 255,255,255,255);
        SDL_RenderClear(r);
        renderButton(r, attackerButton);
        renderButton(r, midfielderButton);
        renderButton(r, defenderButton);
        renderButton(r, goalkeeperButton);
        SDL_RenderPresent(r);
    }
    SDL_DestroyTexture(attackerButton.box.texture);   
    SDL_DestroyTexture(midfielderButton.box.texture);
    SDL_DestroyTexture(defenderButton.box.texture);
    SDL_DestroyTexture(goalkeeperButton.box.texture);
}

void player_data_screen(SDL_Renderer* r, SDL_Event* e, Fonts* fonts, Circularlist* list) {
    Node* current = list->head;

    SDL_Color textColor = {0,0,0,255};
    SDL_Color box_color = {255,255,255,0};
    SDL_Color button_color = {200,100,150,255};
    SDL_Color button_hover_color = {200,200,150,255};
    char buffer[12];
    snprintf(buffer, sizeof(buffer), "%d", current->player.age);
    int name_x = 10;
    int name_y = 10;
    Button nameBox = {{{name_x,name_y,50,20},box_color,NULL}, textColor, "Name:",fonts->font20}; 
    Button name = {{{name_x+130,name_y,100,20},box_color,NULL}, textColor, current->player.name,fonts->font20};
    Button birthBox = {{{name_x+300,name_y,100,20},box_color,NULL}, textColor, "Birth:",fonts->font20};
    Button birth = {{{name_x+ 400,name_y,100,20},box_color,NULL}, textColor, current->player.birth,fonts->font20};
    Button ageBox = {{{name_x+500,name_y,100,20},box_color,NULL}, textColor, "Age:",fonts->font20};
    Button age = {{{name_x+600,name_y,50,20},box_color,NULL}, textColor, buffer,fonts->font20};
    Button nationBox = {{{name_x+700,name_y,100,20},box_color,NULL}, textColor, "Nation:",fonts->font20};
    Button nation = {{{name_x+800,name_y,100,20},box_color,NULL}, textColor, current->player.nationality,fonts->font20};

    Button prevButton = {{{50, 400, 150, 50}, box_color,NULL}, textColor, "Previous", fonts->font20};
    Button nextButton = {{{1000, 400, 150, 50}, box_color,NULL}, textColor, "Next", fonts->font20};

    nameBox.box.texture = renderText(r, nameBox.font,nameBox.text,nameBox.textColor);
    name.box.texture = renderText(r,name.font,name.text,name.textColor);
    birthBox.box.texture = renderText(r,birthBox.font, birthBox.text,birthBox.textColor);
    birth.box.texture = renderText(r,birth.font,birth.text,name.textColor);
    ageBox.box.texture = renderText(r, ageBox.font, ageBox.text, ageBox.textColor);
    age.box.texture = renderText(r,age.font,age.text,age.textColor);
    nationBox.box.texture = renderText(r, nationBox.font, nationBox.text, nationBox.textColor);
    nation.box.texture = renderText(r,nation.font,nation.text,nation.textColor);

    prevButton.box.texture = renderText(r,prevButton.font,prevButton.text,prevButton.textColor);
    nextButton.box.texture = renderText(r, nextButton.font, nextButton.text,nextButton.textColor);

    bool quit = false;
    while(!quit) {
        while(SDL_PollEvent(e)) {
            if (e->type == SDL_KEYDOWN) {
                if (e->key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                }
            }
            else if (e->type == SDL_QUIT) {
                quit = true;
            }
            else if (e->type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (isMouseOverButton(prevButton, x, y)) {
                    printf("prev button clicked\n");
                    current = current->prev;
                    name.text = current->player.name;
                    name.box.texture = renderText(r, name.font,name.text,name.textColor);
                    birth.text = current->player.birth;
                    birth.box.texture = renderText(r, birth.font,birth.text,birth.textColor);
                    snprintf(buffer, sizeof(buffer), "%d", current->player.age);
                    age.text = buffer;
                    age.box.texture = renderText(r, age.font,age.text,age.textColor);
                    nation.text = current->player.nationality;
                    nation.box.texture = renderText(r,nation.font,nation.text,nation.textColor);
                }
                else if (isMouseOverButton(nextButton,x,y)) {
                    printf("next button");
                    current = current->next;
                    name.text = current->player.name;
                    name.box.texture = renderText(r, name.font,name.text,name.textColor);
                    birth.text = current->player.birth;
                    birth.box.texture = renderText(r, birth.font,birth.text,birth.textColor);
                    snprintf(buffer, sizeof(buffer), "%d", current->player.age);
                    age.text = buffer;
                    age.box.texture = renderText(r, age.font,age.text,age.textColor);
                    nation.text = current->player.nationality;
                    nation.box.texture = renderText(r,nation.font,nation.text,nation.textColor);
                } 
            }
        }
        
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (isMouseOverButton(prevButton, x, y)) {
            prevButton.box.background_color = button_hover_color;
        } else {
            prevButton.box.background_color = button_color;
        }
        if (isMouseOverButton(nextButton, x, y)) {
            nextButton.box.background_color = button_hover_color;
        } else {
            nextButton.box.background_color = button_color;
        }
        SDL_SetRenderDrawColor(r, 255,255,255,255);
        SDL_RenderClear(r);
        renderButton(r, nameBox);
        renderButton(r, name);
        renderButton(r, birthBox);
        renderButton(r, birth);
        renderButton(r, ageBox);
        renderButton(r, age);
        renderButton(r, nationBox);
        renderButton(r, nation);  
        renderButton(r, prevButton);
        renderButton(r,nextButton);

        SDL_RenderPresent(r);
    }
    SDL_DestroyTexture(name.box.texture);
    SDL_DestroyTexture(birth.box.texture);
    SDL_DestroyTexture(age.box.texture);
    SDL_DestroyTexture(nation.box.texture);
}
int main() {
    int w = 1280;
    int h = 800;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        logSDLError("SDL_Init");
        return 1;
    }

    if (TTF_Init() == -1) {
        return 0;
    }
    SDL_Window* window= SDL_CreateWindow("SDL2 Buttons", 10, 10, w, h, SDL_WINDOW_SHOWN);;
    if (window == NULL) {
        logSDLError("SDL_CreateWindow");
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);;
    if (renderer == NULL) {
        logSDLError("SDL_CreateRenderer");
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Fonts font_collection = {
        TTF_OpenFont("font/MangoDdobak.ttf",10),
        TTF_OpenFont("font/MangoDdobak.ttf",20),
        TTF_OpenFont("font/MangoDdobak.ttf",30),
        TTF_OpenFont("font/MangoDdobak.ttf",40),
        TTF_OpenFont("font/MangoDdobak.ttf",50),
        TTF_OpenFont("font/MangoDdobak.ttf",60)
    };
    if (font_collection.font10 == NULL || font_collection.font20 == NULL || font_collection.font30 == NULL || \
        font_collection.font40 == NULL || font_collection.font50 == NULL || font_collection.font60 == NULL) {
        logTTFError("TTF_OpenFont error");
        TTF_CloseFont(font_collection.font10);
        TTF_CloseFont(font_collection.font20);
        TTF_CloseFont(font_collection.font30);
        TTF_CloseFont(font_collection.font40);
        TTF_CloseFont(font_collection.font50);
        TTF_CloseFont(font_collection.font60);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    //Node 
    Circularlist player_list;
    Circularlist search_list;
    initList(&player_list);
    initList(&search_list);

    readCSV("FM2023.csv", &player_list);
    searchPlayer(&player_list, &search_list);
    printList(&search_list);
    

    SDL_Event e;
    startMain(renderer,&e,&font_collection);
    where_position_recommended_screen(renderer,&e,&font_collection);
    player_data_screen(renderer,&e,&font_collection,&search_list);
    
    TTF_CloseFont(font_collection.font10);
    TTF_CloseFont(font_collection.font20);
    TTF_CloseFont(font_collection.font30);
    TTF_CloseFont(font_collection.font40);
    TTF_CloseFont(font_collection.font50);
    TTF_CloseFont(font_collection.font60);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
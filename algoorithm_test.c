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
    TTF_Font* font15;
    TTF_Font* font18;
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

void renderBox(SDL_Renderer* r, SDL_Rect rect, SDL_Color color) {
    SDL_SetRenderDrawColor(r,color.r,color.g,color.b,color.a);
    SDL_RenderFillRect(r, &rect);
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

typedef struct pos{
    int x;
    int y;
} pos;
typedef struct ability_pos {
    pos corners;            pos crossing;       pos dribbling;          pos finishing;              pos first_touch;
    pos free_kick_taking;   pos heading;        pos long_shot;          pos long_throws;            pos marking;
    pos passing;            pos penalty_taking; pos tackling;           pos technique;              pos aggressiion;
    pos anticipation;       pos bravery;        pos composure;          pos concentration;          pos vision;
    pos decision;           pos determination;  pos flair;              pos leadership;             pos off_the_ball;
    pos position;           pos teamwork;       pos work_rate;          pos acceleration;           pos agility;
    pos balance;            pos jumping_reach;  pos natural_fitness;    pos pace;                   pos stamina;
    pos strength;           pos stability;      pos foul;               pos contest_performance;    pos injury;
    pos diversity;          pos aerial_reach;   pos command_of_area;    pos communication;          pos eccentricity;
    pos handling;           pos kicking;        pos one_on_ones;        pos reflexes;               pos rushing_out; 
    pos throwing;           pos adaptation;     pos ambition;           pos argue;                  pos loyal; 
    pos professional;       pos sportmanship;   pos emotional_control;  pos punching;               pos resistant_to_stress;
} ability_pos;
typedef struct position_pos {
    //GK
    pos GK;
    //Defender
    pos DL; pos DC; pos DR; pos WBL; pos WBR;
    //Midfielder
    pos DM; pos ML; pos MC; pos MR; 
    //attacker
    pos AML; pos AMC; pos AMR; pos ST;
} position_pos;
typedef struct data_pos{
    pos numId;
    pos name;
    pos position;
    pos age;
    pos ca; pos pa;
    pos nationality;
    pos club;
    ability_pos ability_pos;
    position_pos position_pos;
    pos height;
    pos weight;
    pos left;
    pos right;
    pos values;
    pos current_reputation;
    pos domestic_reputation;
    pos world_reputation;
    pos RCA;
    pos birth;
    pos number_of_national_team_appearance;
    pos goals_scored_for_the_national_team;
    pos salary;
    pos UID;
} Data_pos;

typedef struct Node {
    Player player;
    int id;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct Circularlist {
    Node* head;
} Circularlist;

void initList(Circularlist* list) {
    list->head = NULL;
}
Node* createNode(Player player,int id) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->player = player;
    newNode->id = id;
    newNode->next = NULL;
    return newNode;
}

void appendNode(Circularlist* list, Player player, int* idCounter) {
    Node* newNode = createNode(player,(*idCounter)++);
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

void readCSV(const char* filename, Circularlist* list,int* idCounter) {
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

        appendNode(list, player,idCounter);
    }
    fclose(file);
}

void searchPlayer(Circularlist* player_list, Circularlist* search_list,int* idCounter) {
    if (player_list->head == NULL) {
        printf("List is empty\n");
        return;
    }
    Node* current = player_list->head;
    do {
        if (current->player.pa > 170 && current->player.ca <150 && current->player.age < 20) {
            appendNode(search_list,current->player, idCounter);
        }
        current = current->next;
    } while (current != player_list->head);
}
//선수 찾기
void findStrikers(Circularlist* player_list,Circularlist* search_list, int* idCounter) {
    
    if (player_list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node* current = player_list->head;
    do {
        Player p = current->player;
        // 평균 계산
        float avg1 = p.abiltiy_stat.acceleration;
        float avg2 = p.abiltiy_stat.finishing;
        float avg4 = p.abiltiy_stat.pace;
        float avg5 = (p.abiltiy_stat.composure + p.abiltiy_stat.off_the_ball + p.abiltiy_stat.agility +
                      p.abiltiy_stat.stamina + p.abiltiy_stat.strength + p.abiltiy_stat.first_touch +
                      p.abiltiy_stat.heading) / 7.0;
        float avg6 = (p.abiltiy_stat.anticipation + p.abiltiy_stat.decision + p.abiltiy_stat.jumping_reach +
                      p.abiltiy_stat.dribbling) / 4.0;
        float avg7 = p.abiltiy_stat.technique;
        float avg8 = (p.abiltiy_stat.concentration + p.abiltiy_stat.position + p.abiltiy_stat.vision +
                      p.abiltiy_stat.work_rate + p.abiltiy_stat.balance + p.abiltiy_stat.crossing +
                      p.abiltiy_stat.long_shot + p.abiltiy_stat.passing) / 8.0;
        float avg9 = (p.abiltiy_stat.bravery + p.abiltiy_stat.leadership + p.abiltiy_stat.teamwork +
                      p.abiltiy_stat.marking + p.abiltiy_stat.tackling) / 5.0;

        // 평균 기준 만족 검사
        if (avg1 >= 13 && avg2 >= 13 && avg4 >= 12 && avg5 >= 12 &&
            avg6 >= 12 && avg7 >= 11 && avg8 >= 11 && avg9 >= 10) {
            appendNode(search_list,p,idCounter);
        }

        current = current->next;
    }
    while (current != player_list->head);
}

void startMain(SDL_Renderer* r, SDL_Event* e,Fonts* fonts) {
    SDL_Color textColor = {0,0,0,255};
    SDL_Color box_color = {255,255,255,255};
    SDL_Color button_color = {200,100,150,255};
    SDL_Color button_hover_color = {200,200,150,255};

    Button titleBox = {{{490, 300, 300, 150},box_color,NULL},textColor,"Football Agency", fonts->font60};
    Button startButton = {{{550,500,200, 80},button_color,NULL}, textColor,"Start",fonts->font40};

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
char* to_char(int num) {
    // 버퍼 크기를 충분히 크게 설정합니다.
    size_t buffer_size = 12; // 일반적으로 int의 범위 내에서 12 바이트는 충분합니다.
    char *buffer = (char *)malloc(buffer_size * sizeof(char));
    if (buffer == NULL) {
        // 메모리 할당 실패 시 처리
        return NULL;
    }
    snprintf(buffer, buffer_size, "%d", num);
    return buffer;
}

void player_data_screen(SDL_Renderer* r, SDL_Event* e, Fonts* fonts, Circularlist* list, int* idCounter) {
    Node* current = list->head;

    SDL_Color textColor = {0,0,0,255};
    SDL_Color box_color = {255,255,255,0};
    SDL_Color box_color_yellow = {255,226,193,255};
    SDL_Color box_0 = {255, 255, 255, 0};
    SDL_Color button_color = {200,100,150,255};
    SDL_Color button_hover_color = {200,200,150,255};

    Data_pos data_pos = {
      {50,680}, //num id
      {10,10}, //name
      {400,50}, //position
      {560,10}, //age
      {40,85}, //ca
      {40,105}, //pa
      {740,10}, //nationality
      {10,40}, //club
      {{65, 125}, {65,145}, {65,165}, {65,185}, {75,205}, {85,225}, {65,245}, {70,265}, {80,285}, {65,305},
       {30, 110}, {40,110}, {50,110}, {60,110}, {70,110}},
      {310,10},  
    };
    int box_w = 0; int box_h = 0;
    int name_x = 10;
    int name_y = 10;
    Button nameBox = {{{data_pos.name.x,data_pos.name.y,50,20},box_color,NULL}, textColor, "Name",fonts->font20}; 
    Button birthBox = {{{name_x+310,name_y,50,20},box_color,NULL}, textColor, "Birth",fonts->font20};
    Button ageBox = {{{data_pos.age.x,data_pos.age.y,50,20},box_color,NULL}, textColor, "Age",fonts->font20};
    Button nationBox = {{{data_pos.nationality.x,data_pos.nationality.y,50,20},box_color,NULL}, textColor, "Nation",fonts->font20};
    Button clubBox = {{{data_pos.club.x,data_pos.club.y,50,20},box_color,NULL}, textColor, "Club",fonts->font20};

    Button positionBox = {{{data_pos.position.x, data_pos.position.y, box_w, box_h}, box_color, NULL}, textColor, "Position", fonts->font20};
    Button caBox = {{{data_pos.ca.x, data_pos.ca.y, box_w, box_h}, box_color, NULL}, textColor, "CA", fonts->font18};
    Button paBox = {{{data_pos.pa.x, data_pos.pa.y, box_w, box_h}, box_color, NULL}, textColor, "PA", fonts->font18};

    Button cornersBox = {{{data_pos.ability_pos.corners.x, data_pos.ability_pos.corners.y, box_w, box_h}, box_color, NULL}, textColor, "Corners", fonts->font18};
    Button crossingBox = {{{data_pos.ability_pos.crossing.x, data_pos.ability_pos.crossing.y, box_w, box_h}, box_color, NULL}, textColor, "Crossing", fonts->font18};
    Button dribblingBox = {{{data_pos.ability_pos.dribbling.x, data_pos.ability_pos.dribbling.y, box_w, box_h}, box_color, NULL}, textColor, "Dribbling", fonts->font18};
    Button finishingBox = {{{data_pos.ability_pos.finishing.x, data_pos.ability_pos.finishing.y, box_w, box_h}, box_color, NULL}, textColor, "Finishing", fonts->font18};
    Button first_touchBox = {{{data_pos.ability_pos.first_touch.x, data_pos.ability_pos.first_touch.y, box_w, box_h}, box_color, NULL}, textColor, "First touch", fonts->font18};
    Button free_kick_takingBox = {{{data_pos.ability_pos.free_kick_taking.x, data_pos.ability_pos.free_kick_taking.y, box_w, box_h}, box_color, NULL}, textColor, "Free kick taking", fonts->font18};
    Button headingBox = {{{data_pos.ability_pos.heading.x, data_pos.ability_pos.heading.y, box_w, box_h}, box_color, NULL}, textColor, "Heading", fonts->font18};
    Button long_shotBox = {{{data_pos.ability_pos.long_shot.x, data_pos.ability_pos.long_shot.y, box_w, box_h}, box_color, NULL}, textColor, "Long shot", fonts->font18};
    Button long_throwsBox = {{{data_pos.ability_pos.long_throws.x, data_pos.ability_pos.long_throws.y, box_w, box_h}, box_color, NULL}, textColor, "Long throws", fonts->font18};
    Button markingBox = {{{data_pos.ability_pos.marking.x, data_pos.ability_pos.marking.y, box_w, box_h}, box_color, NULL}, textColor, "Marking", fonts->font18};
    Button passingBox = {{{data_pos.ability_pos.passing.x, data_pos.ability_pos.passing.y, box_w, box_h}, box_color, NULL}, textColor, "Passing", fonts->font18};
    Button penalty_takingBox = {{{data_pos.ability_pos.penalty_taking.x, data_pos.ability_pos.penalty_taking.y, box_w, box_h}, box_color, NULL}, textColor, "Penalty_taking", fonts->font18};
    Button tacklingBox = {{{data_pos.ability_pos.tackling.x, data_pos.ability_pos.tackling.y, box_w, box_h}, box_color, NULL}, textColor, "Tackling", fonts->font18};
    Button techniqueBox = {{{data_pos.ability_pos.technique.x, data_pos.ability_pos.technique.y, box_w, box_h}, box_color, NULL}, textColor, "Technique", fonts->font18};
    Button aggressiionBox = {{{data_pos.ability_pos.aggressiion.x, data_pos.ability_pos.aggressiion.y, box_w, box_h}, box_color, NULL}, textColor, "Aggression", fonts->font18};
    Button anticipationBox = {{{data_pos.ability_pos.anticipation.x, data_pos.ability_pos.anticipation.y, box_w, box_h}, box_color, NULL}, textColor, "Anticipation", fonts->font18};
    Button braveryBox = {{{data_pos.ability_pos.bravery.x, data_pos.ability_pos.bravery.y, box_w, box_h}, box_color, NULL}, textColor, "Bravery", fonts->font18};
    Button composureBox = {{{data_pos.ability_pos.composure.x, data_pos.ability_pos.composure.y, box_w, box_h}, box_color, NULL}, textColor, "Composure", fonts->font18};
    Button concentrationBox = {{{data_pos.ability_pos.concentration.x, data_pos.ability_pos.concentration.y, box_w, box_h}, box_color, NULL}, textColor, "Concentration", fonts->font18};
    Button visionBox = {{{data_pos.ability_pos.vision.x, data_pos.ability_pos.vision.y, box_w, box_h}, box_color, NULL}, textColor, "Vision", fonts->font18};
    Button decisionBox = {{{data_pos.ability_pos.decision.x, data_pos.ability_pos.decision.y, box_w, box_h}, box_color, NULL}, textColor, "Decision", fonts->font18};
    Button determinationBox = {{{data_pos.ability_pos.determination.x, data_pos.ability_pos.determination.y, box_w, box_h}, box_color, NULL}, textColor, "Determination", fonts->font18};
    Button flairBox = {{{data_pos.ability_pos.flair.x, data_pos.ability_pos.flair.y, box_w, box_h}, box_color, NULL}, textColor, "Flair", fonts->font18};
    Button leadershipBox = {{{data_pos.ability_pos.leadership.x, data_pos.ability_pos.leadership.y, box_w, box_h}, box_color, NULL}, textColor, "Leadership", fonts->font18};
    Button off_the_ballBox = {{{data_pos.ability_pos.off_the_ball.x, data_pos.ability_pos.off_the_ball.y, box_w, box_h}, box_color, NULL}, textColor, "Off the ball", fonts->font18};
    Button positionBox = {{{data_pos.ability_pos.position.x, data_pos.ability_pos.position.y, box_w, box_h}, box_color, NULL}, textColor, "Position", fonts->font18};
    Button teamworkBox = {{{data_pos.ability_pos.teamwork.x, data_pos.ability_pos.teamwork.y, box_w, box_h}, box_color, NULL}, textColor, "Teamwork", fonts->font18};
    Button work_rateBox = {{{data_pos.ability_pos.work_rate.x, data_pos.ability_pos.work_rate.y, box_w, box_h}, box_color, NULL}, textColor, "Work rate", fonts->font18};
    Button acceleration = {{{data_pos.ability_pos.acceleration.x, data_pos.ability_pos.acceleration.y, box_w, box_h}, box_color, NULL}, textColor, "Acceleration", fonts->font18};
    Button agilityBox = {{{data_pos.ability_pos.agility.x, data_pos.ability_pos.agility.y, box_w, box_h}, box_color, NULL}, textColor, "Agility", fonts->font18};
    Button balanceBox = {{{data_pos.ability_pos.balance.x, data_pos.ability_pos.balance.y, box_w, box_h}, box_color, NULL}, textColor, "Balance", fonts->font18};
    Button jumping_reachBox = {{{data_pos.ability_pos.jumping_reach.x, data_pos.ability_pos.jumping_reach.y, box_w, box_h}, box_color, NULL}, textColor, "Jumping reach", fonts->font18};
    Button natural_fitnessBox = {{{data_pos.ability_pos.natural_fitness.x, data_pos.ability_pos.natural_fitness.y, box_w, box_h}, box_color, NULL}, textColor, "Natural fitness", fonts->font18};
    Button paceBox = {{{data_pos.ability_pos.pace.x, data_pos.ability_pos.pace.y, box_w, box_h}, box_color, NULL}, textColor, "Pace", fonts->font18};
    Button staminaBox = {{{data_pos.ability_pos.stamina.x, data_pos.ability_pos.stamina.y, box_w, box_h}, box_color, NULL}, textColor, "Stamina", fonts->font18};
    Button foulBox = {{{data_pos.ability_pos.foul.x, data_pos.ability_pos.foul.y, box_w, box_h}, box_color, NULL}, textColor, "Foul", fonts->font18};
    Button contest_performanceBox = {{{data_pos.ability_pos.contest_performance.x, data_pos.ability_pos.contest_performance.y, box_w, box_h}, box_color, NULL}, textColor, "Contest_performance", fonts->font18};
    
    Box data_box = {{name_x+10,name_y+60, 1200,500},box_color_yellow, NULL};

    Button numIdBox = {{{data_pos.numId.x,data_pos.numId.y,box_w,box_h},box_color,NULL}, textColor, to_char(*idCounter-1) ,fonts->font20};
    Button IdBox = {{{data_pos.numId.x + 50,data_pos.numId.y,box_w,box_h},box_color,NULL}, textColor, to_char(current->id) ,fonts->font20};

    Button name = {{{data_pos.name.x+180,data_pos.name.y,50,20},box_color,NULL}, textColor, current->player.name,fonts->font20};
    Button birth = {{{name_x+ 400,name_y,50,20},box_color,NULL}, textColor, current->player.birth,fonts->font20};
    Button age = {{{data_pos.age.x+100,data_pos.age.y,50,20},box_color,NULL}, textColor, to_char(current->player.age),fonts->font20};
    Button nation = {{{data_pos.nationality.x+100,data_pos.nationality.y,50,20},box_color,NULL}, textColor, current->player.nationality,fonts->font20};
    Button club = {{{data_pos.club.x + 180,data_pos.club.y,50,20},box_color,NULL}, textColor, current->player.club,fonts->font20};
    
    Button position = {{{data_pos.position.x + 100, data_pos.position.y, box_w, box_h}, box_color, NULL}, textColor, current->player.position, fonts->font20};
    
    Button ca = {{{data_pos.ca.x + 50, data_pos.ca.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ca), fonts->font18};
    Button pa = {{{data_pos.pa.x + 50, data_pos.pa.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.pa), fonts->font18};

    Button corners = {{{data_pos.ability_pos.corners.x + 50, data_pos.ability_pos.corners.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.abiltiy_stat.corners), fonts->font18};
    Button crossing = {{{data_pos.ability_pos.crossing.x + 50, data_pos.ability_pos.crossing.y, box_w, box_h}, box_color, NULL}, textColor,to_char(current->player.abiltiy_stat.crossing), fonts->font18};
    Button dribbling = {{{data_pos.ability_pos.dribbling.x + 50, data_pos.ability_pos.dribbling.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.abiltiy_stat.dribbling), fonts->font18};
    Button finishing = {{{data_pos.ability_pos.finishing.x + 50 , data_pos.ability_pos.finishing.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.abiltiy_stat.finishing), fonts->font18};
    Button first_touch = {{{data_pos.ability_pos.first_touch.x + 70, data_pos.ability_pos.first_touch.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.abiltiy_stat.first_touch), fonts->font18};
    Button free_kick_taking = {{{data_pos.ability_pos.free_kick_taking.x + 80, data_pos.ability_pos.free_kick_taking.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.abiltiy_stat.free_kick_taking), fonts->font18};
    Button heading = {{{data_pos.ability_pos.heading.x + 50, data_pos.ability_pos.heading.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.abiltiy_stat.heading), fonts->font18};
    Button long_shot = {{{data_pos.ability_pos.long_shot.x + 60, data_pos.ability_pos.long_shot.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.abiltiy_stat.long_shot), fonts->font18};
    Button long_throws = {{{data_pos.ability_pos.long_throws.x + 60, data_pos.ability_pos.long_throws.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.abiltiy_stat.long_throws), fonts->font18};
    Button marking = {{{data_pos.ability_pos.marking.x + 50, data_pos.ability_pos.marking.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.abiltiy_stat.marking), fonts->font18};

    Button prevButton = {{{50, 700, 150, 50}, box_color,NULL}, textColor, "Previous", fonts->font20};
    Button nextButton = {{{1080, 700, 150, 50}, box_color,NULL}, textColor, "Next", fonts->font20};

    numIdBox.box.texture = renderText(r, numIdBox.font, numIdBox.text, numIdBox.textColor);
    IdBox.box.texture = renderText(r, IdBox.font, IdBox.text, IdBox.textColor);

    nameBox.box.texture = renderText(r, nameBox.font,nameBox.text,nameBox.textColor);
    birthBox.box.texture = renderText(r,birthBox.font, birthBox.text,birthBox.textColor);
    ageBox.box.texture = renderText(r, ageBox.font, ageBox.text, ageBox.textColor);
    nationBox.box.texture = renderText(r, nationBox.font, nationBox.text, nationBox.textColor);
    clubBox.box.texture = renderText(r, clubBox.font, clubBox.text, clubBox.textColor);

    positionBox.box.texture = renderText(r, positionBox.font, positionBox.text, positionBox.textColor);
    caBox.box.texture = renderText(r, caBox.font, caBox.text, caBox.textColor);
    paBox.box.texture = renderText(r, paBox.font, paBox.text, paBox.textColor);

    cornersBox.box.texture = renderText(r, cornersBox.font, cornersBox.text, cornersBox.textColor);
    crossingBox.box.texture = renderText(r, crossingBox.font, crossingBox.text, crossingBox.textColor);
    dribblingBox.box.texture = renderText(r, dribblingBox.font, dribblingBox.text, dribblingBox.textColor);
    finishingBox.box.texture = renderText(r, finishingBox.font, finishingBox.text, finishingBox.textColor);
    first_touchBox.box.texture = renderText(r, first_touchBox.font, first_touchBox.text, first_touchBox.textColor);
    free_kick_takingBox.box.texture = renderText(r, free_kick_takingBox.font, free_kick_takingBox.text, free_kick_takingBox.textColor);
    headingBox.box.texture = renderText(r, headingBox.font, headingBox.text, headingBox.textColor);
    long_shotBox.box.texture = renderText(r, long_shotBox.font, long_shotBox.text, long_shotBox.textColor);
    long_throwsBox.box.texture = renderText(r, long_throwsBox.font, long_throwsBox.text, long_throwsBox.textColor);
    markingBox.box.texture = renderText(r, markingBox.font, markingBox.text, markingBox.textColor);

    name.box.texture = renderText(r,name.font,name.text,name.textColor);
    birth.box.texture = renderText(r,birth.font,birth.text,name.textColor);
    age.box.texture = renderText(r,age.font,age.text,age.textColor);
    nation.box.texture = renderText(r,nation.font,nation.text,nation.textColor);
    club.box.texture = renderText(r, club.font, club.text, club.textColor);

    position.box.texture = renderText(r, position.font, position.text, position.textColor);
    ca.box.texture = renderText(r, ca.font, ca.text, ca.textColor);
    pa.box.texture = renderText(r, pa.font, pa.text, pa.textColor);

    corners.box.texture = renderText(r, corners.font, corners.text, corners.textColor);
    crossing.box.texture = renderText(r, crossing.font, crossing.text, crossing.textColor);
    dribbling.box.texture = renderText(r, dribbling.font, dribbling.text, dribbling.textColor);
    finishing.box.texture = renderText(r, finishing.font, finishing.text, finishing.textColor);
    first_touch.box.texture = renderText(r, first_touch.font, first_touch.text, first_touch.textColor);
    free_kick_taking.box.texture = renderText(r, free_kick_taking.font, free_kick_taking.text, free_kick_taking.textColor);
    heading.box.texture = renderText(r, heading.font, heading.text, heading.textColor);
    long_shot.box.texture = renderText(r, long_shot.font, long_shot.text, long_shot.textColor);
    long_throws.box.texture = renderText(r, long_throws.font, long_throws.text, long_throws.textColor);
    marking.box.texture = renderText(r, marking.font, marking.text, marking.textColor);

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
                   
                    current = current->prev;

                    IdBox.text = to_char(current->id);
                    IdBox.box.texture = renderText(r, IdBox.font, IdBox.text, IdBox.textColor);

                    name.text = current->player.name;
                    name.box.texture = renderText(r, name.font,name.text,name.textColor);

                    birth.text = current->player.birth;
                    birth.box.texture = renderText(r, birth.font,birth.text,birth.textColor);

                    age.text = to_char(current->player.age);
                    age.box.texture = renderText(r, age.font,age.text,age.textColor);

                    nation.text = current->player.nationality;
                    nation.box.texture = renderText(r,nation.font,nation.text,nation.textColor);

                    club.text = current->player.club;
                    club.box.texture = renderText(r, club.font, club.text, club.textColor);

                    position.text = current->player.position;
                    position.box.texture = renderText(r, position.font, position.text, position.textColor);

                    ca.text = to_char(current->player.ca);
                    ca.box.texture = renderText(r, ca.font, ca.text, ca.textColor);

                    pa.text = to_char(current->player.pa);
                    pa.box.texture = renderText(r, pa.font, pa.text, pa.textColor);

                    corners.text = to_char(current->player.abiltiy_stat.corners);
                    corners.box.texture = renderText(r, corners.font, corners.text, corners.textColor);
                    crossing.text = to_char(current->player.abiltiy_stat.crossing);
                    crossing.box.texture = renderText(r, crossing.font, crossing.text, crossing.textColor);
                    dribbling.text = to_char(current->player.abiltiy_stat.dribbling);
                    dribbling.box.texture = renderText(r, dribbling.font, dribbling.text, dribbling.textColor);
                    finishing.text = to_char(current->player.abiltiy_stat.finishing);
                    finishing.box.texture = renderText(r, finishing.font, finishing.text, finishing.textColor);
                    first_touch.text = to_char(current->player.abiltiy_stat.first_touch);
                    first_touch.box.texture = renderText(r, first_touch.font, first_touch.text, first_touch.textColor);
                    free_kick_taking.text = to_char(current->player.abiltiy_stat.free_kick_taking);
                    free_kick_taking.box.texture = renderText(r, free_kick_taking.font, free_kick_taking.text, free_kick_taking.textColor);
                    heading.text = to_char(current->player.abiltiy_stat.heading);
                    heading.box.texture = renderText(r, heading.font, corners.text, heading.textColor);
                    long_shot.text = to_char(current->player.abiltiy_stat.long_shot);
                    long_shot.box.texture = renderText(r, long_shot.font, long_shot.text, long_shot.textColor);
                    long_throws.text = to_char(current->player.abiltiy_stat.long_throws);
                    long_throws.box.texture = renderText(r, long_throws.font, long_throws.text, long_throws.textColor);
                    marking.text = to_char(current->player.abiltiy_stat.marking);
                    marking.box.texture = renderText(r, marking.font, marking.text, marking.textColor);
                }
                else if (isMouseOverButton(nextButton,x,y)) {
                    
                    current = current->next;

                    IdBox.text = to_char(current->id);
                    IdBox.box.texture = renderText(r, IdBox.font, IdBox.text, IdBox.textColor);

                    name.text = current->player.name;
                    name.box.texture = renderText(r, name.font,name.text,name.textColor);

                    birth.text = current->player.birth;
                    birth.box.texture = renderText(r, birth.font,birth.text,birth.textColor);

                    age.text = to_char(current->player.age);
                    age.box.texture = renderText(r, age.font,age.text,age.textColor);

                    nation.text = current->player.nationality;
                    nation.box.texture = renderText(r,nation.font,nation.text,nation.textColor);

                    club.text = current->player.club;
                    club.box.texture = renderText(r, club.font, club.text, club.textColor);

                    position.text = current->player.position;
                    position.box.texture = renderText(r, position.font, position.text, position.textColor);

                    ca.text = to_char(current->player.ca);
                    ca.box.texture = renderText(r, ca.font, ca.text, ca.textColor);

                    pa.text = to_char(current->player.pa);
                    pa.box.texture = renderText(r, pa.font, pa.text, pa.textColor);

                    corners.text = to_char(current->player.abiltiy_stat.corners);
                    corners.box.texture = renderText(r, corners.font, corners.text, corners.textColor);
                    crossing.text = to_char(current->player.abiltiy_stat.crossing);
                    crossing.box.texture = renderText(r, crossing.font, crossing.text, crossing.textColor);
                    dribbling.text = to_char(current->player.abiltiy_stat.dribbling);
                    dribbling.box.texture = renderText(r, dribbling.font, dribbling.text, dribbling.textColor);
                    finishing.text = to_char(current->player.abiltiy_stat.finishing);
                    finishing.box.texture = renderText(r, finishing.font, finishing.text, finishing.textColor);
                    first_touch.text = to_char(current->player.abiltiy_stat.first_touch);
                    first_touch.box.texture = renderText(r, first_touch.font, first_touch.text, first_touch.textColor);
                    free_kick_taking.text = to_char(current->player.abiltiy_stat.free_kick_taking);
                    free_kick_taking.box.texture = renderText(r, free_kick_taking.font, free_kick_taking.text, free_kick_taking.textColor);
                    heading.text = to_char(current->player.abiltiy_stat.heading);
                    heading.box.texture = renderText(r, heading.font, corners.text, heading.textColor);
                    long_shot.text = to_char(current->player.abiltiy_stat.long_shot);
                    long_shot.box.texture = renderText(r, long_shot.font, long_shot.text, long_shot.textColor);
                    long_throws.text = to_char(current->player.abiltiy_stat.long_throws);
                    long_throws.box.texture = renderText(r, long_throws.font, long_throws.text, long_throws.textColor);
                    marking.text = to_char(current->player.abiltiy_stat.marking);
                    marking.box.texture = renderText(r, marking.font, marking.text, marking.textColor);
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
        renderBox(r,data_box.rect,data_box.background_color);

        renderButton(r, numIdBox); renderButton(r, IdBox);
        renderButton(r, nameBox); renderButton(r, birthBox); renderButton(r, ageBox); renderButton(r, nationBox); renderButton(r, clubBox);
        renderButton(r, positionBox); renderButton(r, caBox); renderButton(r, paBox); 
        renderButton(r, cornersBox); renderButton(r, crossingBox); renderButton(r, dribblingBox); renderButton(r, finishingBox); renderButton(r, first_touchBox);
        renderButton(r, free_kick_takingBox); renderButton(r, headingBox); renderButton(r, long_shotBox); renderButton(r, long_throwsBox); renderButton(r, markingBox);

        renderButton(r, name); renderButton(r, birth); renderButton(r, age); renderButton(r, nation); renderButton(r, club); 
        renderButton(r, position); renderButton(r, ca); renderButton(r, pa);
        renderButton(r, corners); renderButton(r, crossing); renderButton(r, dribbling); renderButton(r, finishing); renderButton(r, first_touch);
        renderButton(r, free_kick_taking); renderButton(r, heading); renderButton(r, long_shot); renderButton(r, long_throws); renderButton(r, marking);

        renderButton(r, prevButton); renderButton(r,nextButton);

        SDL_RenderPresent(r);
    }
    SDL_DestroyTexture(name.box.texture);
    SDL_DestroyTexture(birth.box.texture);
    SDL_DestroyTexture(age.box.texture);
    SDL_DestroyTexture(nation.box.texture);
    SDL_DestroyTexture(clubBox.box.texture);
}
//선수 찾기

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
        TTF_OpenFont("font/MangoDdobak.ttf",15),
        TTF_OpenFont("font/MangoDdobak.ttf",18),
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
    int all_idCounter = 1;
    int search_idCounter = 1;
    readCSV("FM2023.csv", &player_list, &all_idCounter);
    //searchPlayer(&player_list, &search_list);
    findStrikers(&player_list, &search_list, &search_idCounter);
    //printList(&search_list);
    

    SDL_Event e;
    //startMain(renderer,&e,&font_collection);
    //where_position_recommended_screen(renderer,&e,&font_collection);
    player_data_screen(renderer,&e,&font_collection,&search_list,&search_idCounter);
    
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
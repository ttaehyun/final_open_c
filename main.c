#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#define MAX_LINE_LENGTH 1024

//에러 처리 함수
void logSDLError(const char *msg) {
    fprintf(stderr, "%s error: %s\n", msg, SDL_GetError());
}

void logTTFError(const char *msg) {
    fprintf(stderr, "%s error: %s\n", msg, TTF_GetError());
}

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

// Button 내부에 필요 모음 구조체
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

// 글자 출력 함수
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
// Box 화면 출력 함수
void renderBox(SDL_Renderer* r, SDL_Rect rect, SDL_Color color) {
    SDL_SetRenderDrawColor(r,color.r,color.g,color.b,color.a);
    SDL_RenderFillRect(r, &rect);
}
//마우스가 버튼 위에 있는지 확인하는 함수
bool isMouseOverButton(Button button, int x, int y) {
    return x >= button.box.rect.x && x <= button.box.rect.x + button.box.rect.w &&
           y >= button.box.rect.y && y <= button.box.rect.y + button.box.rect.h;
}

//화면에 버튼 출력 함수
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
//신체 능력치
typedef struct Body {
    int height;
    int weight;
} Body;
//발 능력치
typedef struct Foot_ability
{
    int left;
    int right;
} Foot_ability;
//선수 세부 스탯
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
//포지션별 능력치
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
//선수 내부 필요 데이터
typedef struct Player {
    char name[100];
    char position[100];
    int age;
    int ca;
    int pa;
    char nationality[100];
    char club[100];
    Ability_stat ability_stat;
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
//x,y좌표
typedef struct pos{
    int x;
    int y;
} pos;
//세부 능력치 별 출력 위치 모음
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
//포지션 별 출력 위치 모음
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
//선수 데이터 출력 위치 모음
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

//연결리스트 구조체
typedef struct Node {
    Player player;
    int id;
    struct Node* prev;
    struct Node* next;
} Node;
//순환 연결리스트를 위한 첫 노드 
typedef struct Circularlist {
    Node* head;
} Circularlist;
//노드 초기화
void initList(Circularlist* list) {
    list->head = NULL;
}
//노드 생성 함수 
Node* createNode(Player player,int id) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->player = player;
    newNode->id = id;
    newNode->next = NULL;
    return newNode;
}
//노드 추가 함수
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
//엑셀 파일에서 선수 데이터 불러오는 함수
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
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.corners = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.crossing = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.dribbling = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.finishing = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.first_touch = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.free_kick_taking = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.heading = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.long_shot = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.long_throws = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.marking = atoi(token);
        //10
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.passing = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.penalty_taking = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.tackling =atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.technique = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.aggressiion = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.anticipation = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.bravery = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.composure = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.concentration = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.vision = atoi(token);
        //20
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.decision = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.determination = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.flair = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.leadership = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.off_the_ball = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.position = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.teamwork = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.work_rate = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.acceleration = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.agility = atoi(token);
        //30
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.balance = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.jumping_reach = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.natural_fitness = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.pace = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.stamina = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.strength = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.stability = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.foul = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.contest_performance = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.injury = atoi(token);
        //40
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.diversity = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.aerial_reach = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.command_of_area = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.communication = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.eccentricity = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.handling = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.kicking = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.one_on_ones = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.reflexes = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.rushing_out = atoi(token);
        //50
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.punching = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.throwing = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.adaptation = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.ambition = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.argue = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.loyal = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.resistant_to_stress = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.professional = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.sportmanship = atoi(token);
        token = strtok(NULL,","); if (token != NULL) player.ability_stat.emotional_control = atoi(token);
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

//선수 찾기 알고리즘

//스트라이커 찾기 함수
void findStrikers(Circularlist* player_list,Circularlist* search_list, int* idCounter) {
    
    if (player_list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node* current = player_list->head;
    do {
        Player p = current->player;
        // 평균 계산
        float avg1 = p.ability_stat.acceleration;
        float avg2 = p.ability_stat.finishing;
        float avg4 = p.ability_stat.pace;
        float avg5 = (p.ability_stat.composure + p.ability_stat.off_the_ball + p.ability_stat.agility +
                      p.ability_stat.stamina + p.ability_stat.strength + p.ability_stat.first_touch +
                      p.ability_stat.heading) / 7.0;
        float avg6 = (p.ability_stat.anticipation + p.ability_stat.decision + p.ability_stat.jumping_reach +
                      p.ability_stat.dribbling) / 4.0;
        float avg7 = p.ability_stat.technique;
        float avg8 = (p.ability_stat.concentration + p.ability_stat.position + p.ability_stat.vision +
                      p.ability_stat.work_rate + p.ability_stat.balance + p.ability_stat.crossing +
                      p.ability_stat.long_shot + p.ability_stat.passing) / 8.0;
        float avg9 = (p.ability_stat.bravery + p.ability_stat.leadership + p.ability_stat.teamwork +
                      p.ability_stat.marking + p.ability_stat.tackling) / 5.0;

        // 평균 기준 만족 검사
        if (avg1 >= 9 && avg2 >= 9 && avg4 >= 9 && avg5 >= 9 &&
            avg6 >= 8 && avg7 >= 7 && avg8 >= 7 && avg9 >= 7 && p.age <=25 && p.ca <=120 && p.pa >=145 && p.position_stat.ST >18 && p.values <=7000000) {
            appendNode(search_list,p,idCounter);
        }

        current = current->next;
    }
    while (current != player_list->head);
}
// 윙어 찾기
void findWingers(Circularlist* player_list,Circularlist* search_list, int* idCounter) {
    if (player_list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node* current = player_list->head;
    do {
        Player p = current->player;
        // 평균 계산
        float avg1 = (p.ability_stat.acceleration + p.ability_stat.pace) / 2.0;
        float avg2 = p.ability_stat.stamina;
        float avg3 = p.ability_stat.agility;
        float avg5 = (p.ability_stat.decision + p.ability_stat.crossing + p.ability_stat.dribbling + p.ability_stat.first_touch) / 4.0;
        float avg6 = p.ability_stat.technique;
        float avg7 = (p.ability_stat.anticipation + p.ability_stat.composure + p.ability_stat.vision + p.ability_stat.work_rate + p.ability_stat.strength) / 5.0;
        float avg8 = (p.ability_stat.concentration + p.ability_stat.off_the_ball + p.ability_stat.teamwork + p.ability_stat.balance + p.ability_stat.finishing + p.ability_stat.long_shot + p.ability_stat.passing + p.ability_stat.tackling) / 8.0;
        float avg9 = (p.ability_stat.bravery + p.ability_stat.leadership + p.ability_stat.position + p.ability_stat.jumping_reach + p.ability_stat.heading + p.ability_stat.marking) / 6.0;

        // 평균 기준 만족 검사
        if (avg1 >= 9 && avg2 >= 9 && avg3 >= 9 && avg5 >= 9 &&
            avg6 >= 8 && avg7 >= 8 && avg8 >= 8 && avg9 >= 8 && p.age <=25 && p.ca <=120 && p.pa >=145 && p.values <=7000000 ) {
            if (p.position_stat.AML >=18 || p.position_stat.AMR >= 18) {
                appendNode(search_list,p,idCounter);
            }
        }

        current = current->next;
    }
    while (current != player_list->head);
}
//측면 미드필더 찾기
void findWideMidfielders(Circularlist* player_list,Circularlist* search_list, int* idCounter) {
    if (player_list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node* current = player_list->head;
		do {
        Player p = current->player;
        // 평균 계산
        float avg1 = p.ability_stat.acceleration; // 1순위
        float avg2 = (p.ability_stat.agility + p.ability_stat.pace) / 2.0; // 2순위
        float avg3 = (p.ability_stat.decision + p.ability_stat.stamina + p.ability_stat.crossing) / 3.0; // 3순위
        float avg4 = (p.ability_stat.first_touch + p.ability_stat.technique) / 2.0; // 4순위
        float avg5 = (p.ability_stat.anticipation + p.ability_stat.composure + p.ability_stat.vision +
                      p.ability_stat.work_rate + p.ability_stat.strength + p.ability_stat.dribbling +
                      p.ability_stat.passing) / 7.0; // 5순위
        float avg6 = (p.ability_stat.concentration + p.ability_stat.off_the_ball + p.ability_stat.teamwork +
                      p.ability_stat.balance + p.ability_stat.finishing + p.ability_stat.long_shot +
                      p.ability_stat.tackling) / 7.0; // 6순위
        float avg7 = (p.ability_stat.bravery + p.ability_stat.leadership + p.ability_stat.position +
                      p.ability_stat.jumping_reach + p.ability_stat.heading + p.ability_stat.marking) / 6.0; // 7순위

        // 평균 기준 만족 검사
        if (avg1 >= 9 && avg2 >= 9 && avg3 >= 9 && avg4 >= 9 &&
            avg5 >= 9 && avg6 >= 8 && avg7 >= 8 && p.age <=25 && p.ca <=120 && p.pa >=145 && p.values <=7000000) {
            if (p.position_stat.AML >= 18 || p.position_stat.AMR >= 18) {
                appendNode(search_list,p,idCounter);
            }
        }

        current = current->next;
    }
    while (current != player_list->head); 
}
// 중앙 미드필더 선수 찾기
void findCentralMidfielders(Circularlist* player_list,Circularlist* search_list, int* idCounter) {
    if (player_list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node* current = player_list->head;
		do {
        Player p = current->player;
        // 평균 계산
        float avg1 = p.ability_stat.decision; // 1순위
        float avg2 = (p.ability_stat.vision + p.ability_stat.acceleration + p.ability_stat.agility +
                      p.ability_stat.stamina + p.ability_stat.first_touch + p.ability_stat.passing) / 6.0; // 2순위
        float avg3 = p.ability_stat.pace; // 3순위
        float avg4 = (p.ability_stat.strength + p.ability_stat.technique) / 2.0; // 4순위
        float avg5 = (p.ability_stat.anticipation + p.ability_stat.composure + p.ability_stat.off_the_ball +
                      p.ability_stat.position + p.ability_stat.work_rate + p.ability_stat.long_shot +
                      p.ability_stat.marking + p.ability_stat.tackling) / 8.0; // 5순위
        float avg6 = (p.ability_stat.concentration + p.ability_stat.teamwork + p.ability_stat.balance +
                      p.ability_stat.dribbling + p.ability_stat.finishing) / 5.0; // 6순위
        float avg7 = (p.ability_stat.bravery + p.ability_stat.leadership + p.ability_stat.jumping_reach +
                      p.ability_stat.crossing + p.ability_stat.heading) / 5.0; // 7순위

        // 평균 기준 만족 검사
        if (avg1 >= 9 && avg2 >= 9 && avg3 >= 9 && avg4 >= 9 &&
            avg5 >= 9 && avg6 >= 8 && avg7 >= 8 && p.age <=25 && p.ca <=120 && p.pa >=150 && p.values <=7000000) {
            if (p.position_stat.MC >= 18 || p.position_stat.ML >= 18 || p.position_stat.MR >= 18) {
                appendNode(search_list,p,idCounter);
            }
        }

        current = current->next;
    }
    while (current != player_list->head);   
}
// 공격형 미드필더 찾기 
void findAttackingMidfielders(Circularlist* player_list,Circularlist* search_list, int* idCounter) {
    if (player_list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node* current = player_list->head;
		do {
        Player p = current->player;
        // 평균 계산
        float avg1 = p.ability_stat.acceleration; // 1순위
        float avg2 = p.ability_stat.pace; // 2순위
        float avg3 = (p.ability_stat.decision + p.ability_stat.vision + p.ability_stat.agility + p.ability_stat.stamina) / 4.0; // 3순위
        float avg4 = (p.ability_stat.first_touch + p.ability_stat.technique) / 2.0; // 4순위
        float avg5 = p.ability_stat.passing; // 5순위
        float avg6 = (p.ability_stat.anticipation + p.ability_stat.composure + p.ability_stat.off_the_ball +
                      p.ability_stat.work_rate + p.ability_stat.strength + p.ability_stat.dribbling +
                      p.ability_stat.finishing + p.ability_stat.long_shot) / 8.0; // 6순위
        float avg7 = (p.ability_stat.concentration + p.ability_stat.position + p.ability_stat.teamwork +
                      p.ability_stat.balance + p.ability_stat.tackling) / 5.0; // 7순위
        float avg8 = (p.ability_stat.bravery + p.ability_stat.leadership + p.ability_stat.jumping_reach +
                      p.ability_stat.crossing + p.ability_stat.heading + p.ability_stat.marking) / 6.0; // 8순위

        // 평균 기준 만족 검사
        if (avg1 >= 9 && avg2 >= 9 && avg3 >= 9 && avg4 >= 9 &&
            avg5 >= 9 && avg6 >= 8 && avg7 >= 8 && avg8 >= 8 && p.age <=25 && p.ca <=120 && p.pa >=147 && p.values <=7000000) {
            if (p.position_stat.AMC >= 18) {
                appendNode(search_list,p,idCounter);
            }
        }

        current = current->next;
    }
    while (current != player_list->head);  
}
//수비형 미드필더 찾기
void findDefensiveMidfielders(Circularlist* player_list,Circularlist* search_list, int* idCounter) {
    if (player_list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node* current = player_list->head;
		do {
        Player p = current->player;
        // 평균 계산
        float avg1 = p.ability_stat.decision; // 1 순위
        float avg2 = p.ability_stat.tackling; // 2 순위
        float avg3 = (p.ability_stat.acceleration + p.ability_stat.agility) / 2.0; // 3 순위
        float avg4 = (p.ability_stat.anticipation + p.ability_stat.position + p.ability_stat.strength) / 3.0; // 4 순위
        float avg5 = (p.ability_stat.vision + p.ability_stat.work_rate + p.ability_stat.pace + p.ability_stat.stamina + p.ability_stat.first_touch + p.ability_stat.passing) / 6.0; // 5 순위
        float avg6 = (p.ability_stat.concentration + p.ability_stat.marking + p.ability_stat.long_shot + p.ability_stat.technique) / 4.0; // 6 순위
        float avg7 = (p.ability_stat.composure + p.ability_stat.teamwork + p.ability_stat.balance + p.ability_stat.dribbling + p.ability_stat.finishing) / 5.0; // 7 순위
        float avg8 = (p.ability_stat.bravery + p.ability_stat.leadership + p.ability_stat.off_the_ball + p.ability_stat.jumping_reach + p.ability_stat.crossing + p.ability_stat.heading) / 6.0; // 8 순위

        // 평균 기준 만족 검사
        if (avg1 >= 9 && avg2 >= 9 && avg3 >= 9 && avg4 >= 9 &&
            avg5 >= 9 && avg6 >= 8 && avg7 >= 8 && avg8 >= 8 && p.age <=25 && p.ca <=120 && p.pa >=147 && p.values <=7000000) {
            if (p.position_stat.DM >= 18) {
                appendNode(search_list,p,idCounter);
            }
        }

        current = current->next;
    }
    while (current != player_list->head);
}
// 풀백 찾기
void findFullbacks(Circularlist* player_list,Circularlist* search_list, int* idCounter) {
    if (player_list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node* current = player_list->head;
		do {
        Player p = current->player;
        // 평균 계산
        float avg1 = (p.ability_stat.decision + p.ability_stat.acceleration) / 2.0; // 1순위
        float avg2 = (p.ability_stat.agility + p.ability_stat.stamina) / 2.0; // 2순위
        float avg3 = p.ability_stat.pace; // 3순위
        float avg4 = (p.ability_stat.concentration + p.ability_stat.position + p.ability_stat.strength + p.ability_stat.tackling) / 4.0; // 4순위
        float avg5 = (p.ability_stat.anticipation + p.ability_stat.first_touch + p.ability_stat.marking) / 3.0; // 5순위
        float avg6 = (p.ability_stat.bravery + p.ability_stat.composure + p.ability_stat.teamwork + p.ability_stat.vision +
                      p.ability_stat.work_rate + p.ability_stat.balance + p.ability_stat.jumping_reach + p.ability_stat.crossing +
                      p.ability_stat.heading + p.ability_stat.passing + p.ability_stat.technique) / 11.0; // 6순위
        float avg7 = (p.ability_stat.leadership + p.ability_stat.off_the_ball + p.ability_stat.dribbling +
                      p.ability_stat.finishing + p.ability_stat.long_shot) / 5.0; // 7순위

        // 평균 기준 만족 검사
        if (avg1 >= 9 && avg2 >= 9 && avg3 >= 9 && avg4 >= 9 &&
            avg5 >= 9 && avg6 >= 8 && avg7 >= 8 && p.age <=25 && p.ca <=120 && p.pa >=143 && p.values <=7000000) {
            if (p.position_stat.DL >= 18 || p.position_stat.DR >= 18) {
                appendNode(search_list,p,idCounter);
            }
        }

        current = current->next;
    }
    while (current != player_list->head);   
}
// 윙백 찾기
void findWingBacks(Circularlist* player_list,Circularlist* search_list, int* idCounter) {
    if (player_list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node* current = player_list->head;
		do {
        Player p = current->player;
        // 평균 계산
        float avg1 = p.ability_stat.acceleration; // 1순위
        float avg2 = p.ability_stat.stamina; // 2순위
        float avg3 = p.ability_stat.pace; // 3순위
        float avg4 = (p.ability_stat.decision + p.ability_stat.agility) / 2.0; // 4순위
        float avg5 = p.ability_stat.strength; // 5순위
        float avg6 = (p.ability_stat.anticipation + p.ability_stat.concentration + p.ability_stat.position +
                      p.ability_stat.crossing + p.ability_stat.first_touch + p.ability_stat.passing +
                      p.ability_stat.tackling + p.ability_stat.technique) / 8.0; // 6순위
        float avg7 = (p.ability_stat.composure + p.ability_stat.off_the_ball + p.ability_stat.teamwork +
                      p.ability_stat.vision + p.ability_stat.work_rate + p.ability_stat.balance +
                      p.ability_stat.dribbling + p.ability_stat.marking) / 8.0; // 7순위
        float avg8 = (p.ability_stat.bravery + p.ability_stat.leadership + p.ability_stat.jumping_reach +
                      p.ability_stat.finishing + p.ability_stat.heading + p.ability_stat.long_shot) / 6.0; // 8순위

        // 평균 기준 만족 검사
        if (avg1 >= 9 && avg2 >= 9 && avg3 >= 9 && avg4 >= 9 &&
            avg5 >= 9 && avg6 >= 8 && avg7 >= 8 && avg8 >= 8 && p.age <=25 && p.ca <=120 && p.pa >=145 && p.values <=7000000) {
            if (p.position_stat.WBL >= 18 || p.position_stat.WBR >= 18) {
                appendNode(search_list,p,idCounter);
            }
        }
        current = current->next;
    }
    while (current != player_list->head);
}
//센터백 찾기
void findCenterBacks(Circularlist* player_list,Circularlist* search_list, int* idCounter) {
    if (player_list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node* current = player_list->head;
		do {
        Player p = current->player;
        // 평균 계산
        float avg1 = p.ability_stat.decision; // 1순위
        float avg2 = (p.ability_stat.position + p.ability_stat.marking) / 2.0; // 2순위
        float avg3 = (p.ability_stat.acceleration + p.ability_stat.agility + p.ability_stat.jumping_reach + p.ability_stat.strength) / 4.0; // 3순위
        float avg4 = (p.ability_stat.anticipation + p.ability_stat.pace + p.ability_stat.heading + p.ability_stat.tackling) / 4.0; // 4순위
        float avg6 = p.ability_stat.concentration; // 6순위
        float avg7 = p.ability_stat.stamina; // 7순위
        float avg8 = (p.ability_stat.bravery + p.ability_stat.leadership + p.ability_stat.composure +
                      p.ability_stat.work_rate + p.ability_stat.balance + p.ability_stat.first_touch +
                      p.ability_stat.passing) / 7.0; // 8순위
        float avg9 = (p.ability_stat.off_the_ball + p.ability_stat.teamwork + p.ability_stat.vision +
                      p.ability_stat.crossing + p.ability_stat.dribbling + p.ability_stat.finishing +
                      p.ability_stat.long_shot + p.ability_stat.technique) / 8.0; // 9순위

        // 평균 기준 만족 검사
        if (avg1 >= 9 && avg2 >= 9 && avg3 >= 9 && avg4 >= 9 &&
            avg6 >= 8 && avg7 >= 8 && avg8 >= 8 && avg9 >= 8 && p.age <=25 && p.ca <=120 && p.pa >=150 && p.values <=7000000) {
            if (p.position_stat.DC >= 18) {
                appendNode(search_list,p,idCounter);
            }
        }

        current = current->next;
    }
    while (current != player_list->head); 
}
// 골키퍼 찾기
void findGoalkeepers(Circularlist* player_list,Circularlist* search_list, int* idCounter) {
    if (player_list->head == NULL) {
        printf("List is empty\n");
        return;
    }

    Node* current = player_list->head;
		do {
        Player p = current->player;
        // 평균 계산
        float avg1 = p.ability_stat.decision; // 1 순위
        float avg2 = (p.ability_stat.handling + p.ability_stat.reflexes + p.ability_stat.agility) / 3.0; // 2 순위
        float avg3 = (p.ability_stat.aerial_reach + p.ability_stat.command_of_area + p.ability_stat.concentration +
                      p.ability_stat.bravery + p.ability_stat.acceleration) / 5.0; // 3 순위
        float avg4 = (p.ability_stat.communication + p.ability_stat.kicking + p.ability_stat.position) / 3.0; // 4 순위
        float avg5 = (p.ability_stat.one_on_ones + p.ability_stat.strength) / 2.0; // 5 순위
        float avg6 = (p.ability_stat.throwing + p.ability_stat.anticipation + p.ability_stat.pace + p.ability_stat.passing) / 4.0; // 6 순위
        float avg7 = (p.ability_stat.composure + p.ability_stat.leadership + p.ability_stat.teamwork + p.ability_stat.balance) / 4.0; // 7 순위
        float avg8 = (p.ability_stat.heading + p.ability_stat.first_touch + p.ability_stat.technique + p.ability_stat.vision +
                      p.ability_stat.work_rate + p.ability_stat.jumping_reach + p.ability_stat.stamina) / 7.0; // 8 순위

        // 평균 기준 만족 검사
        if (avg1 >= 9 && avg2 >= 9 && avg3 >= 9 && avg4 >= 9 &&
            avg5 >= 9 && avg6 >= 8 && avg7 >= 8 && avg8 >= 8 && p.age <=25 && p.ca <=120 && p.values <=7000000) {
            if (p.pa >= 140 || p.pa <= -80) {
                if (p.position_stat.GK >= 18) {
                    appendNode(search_list,p,idCounter);
                }
            }
        }

        current = current->next;
    }
    while (current != player_list->head);
}
//초기 시작 화면 함수
void startMain(SDL_Renderer* r, SDL_Event* e,Fonts* fonts) {
    SDL_Color textColor = {0,0,0,255};
    SDL_Color box_color = {255,255,255,255};
    SDL_Color button_color = {200,100,150,255};
    SDL_Color button_hover_color = {200,200,150,255};

    Button titleBox = {{{590, 300, 300, 150},box_color,NULL},textColor,"Football Agency", fonts->font60};
    Button startButton = {{{640,500,200, 80},button_color,NULL}, textColor,"Start",fonts->font40};

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
// 포지션 선택 함수
void where_position_recommended_screen(SDL_Renderer* r, SDL_Event* e, Fonts* fonts,int* select) {
    SDL_Color textColor = {0,0,0,255};
    SDL_Color box_color = {255,255,255,255};
    SDL_Color button_color = {200,100,150,255};
    SDL_Color button_hover_color = {200,200,150,255};

    Button attackerButton = {{{600,100, 300,100},box_color,NULL},textColor,"Attacker",fonts->font40};
    Button midfielderButton = {{{600,250,300,100},box_color,NULL},textColor,"Midfielder",fonts->font40};
    Button defenderButton = {{{600,400,300,100},box_color,NULL},textColor,"Defender",fonts->font40};
    Button goalkeeperButton = {{{600,550,300,100},box_color,NULL},textColor,"GoalKeeper",fonts->font40};

    attackerButton.box.texture = renderText(r,attackerButton.font,attackerButton.text,attackerButton.textColor);
    midfielderButton.box.texture = renderText(r,midfielderButton.font,midfielderButton.text,midfielderButton.textColor);
    defenderButton.box.texture = renderText(r,defenderButton.font,defenderButton.text,defenderButton.textColor);
    goalkeeperButton.box.texture = renderText(r,goalkeeperButton.font,goalkeeperButton.text,goalkeeperButton.textColor);

    bool quit = false; bool button_click = false;
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
                    *select = 0;
            
                    quit = true;
                    
                }
                else if (isMouseOverButton(midfielderButton,x,y)) {
                    *select = 1;
                
                    quit = true; 
                
                } 
                else if (isMouseOverButton(defenderButton,x,y)) {
                    *select = 2;
                    
                    quit = true;
                }
                else if (isMouseOverButton(goalkeeperButton, x, y)) {
                    *select = 3;
                    quit = true;
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
    SDL_DestroyTexture(attackerButton.box.texture); SDL_DestroyTexture(midfielderButton.box.texture); SDL_DestroyTexture(defenderButton.box.texture); SDL_DestroyTexture(goalkeeperButton.box.texture);
    
}
// 포지션 세부적인 알고리즘 선택 함수
void select_position_algorithm(SDL_Renderer* r, SDL_Event* e, Fonts* fonts,int* select,int* what_choice) {
    SDL_Color textColor = {0,0,0,255};
    SDL_Color box_color = {255,255,255,255};
    SDL_Color button_color = {200,100,150,255};
    SDL_Color button_hover_color = {200,200,150,255};

    Button striker = {{{600,250, 300,100},box_color,NULL},textColor,"Striker",fonts->font40};
    Button winger = {{{600,400, 300,100},box_color,NULL},textColor,"Winger",fonts->font40};

    Button wideMid = {{{600,100, 300,100},box_color,NULL},textColor,"WideMidfielder",fonts->font40};
    Button centralMid = {{{600,250, 300,100},box_color,NULL},textColor,"CentralMidfielder",fonts->font40};
    Button attackMid = {{{600,400, 300,100},box_color,NULL},textColor,"AttackingMidfielder",fonts->font40};
    Button defenseMid = {{{600,550, 300,100},box_color,NULL},textColor,"DefenseMidfielder",fonts->font40};

    Button fullback = {{{600,200, 300,100},box_color,NULL},textColor,"Fullback",fonts->font40};
    Button wingback = {{{600,400, 300,100},box_color,NULL},textColor,"Wingback",fonts->font40};
    Button centerback = {{{600,600, 300,100},box_color,NULL},textColor,"Centerback",fonts->font40};

    striker.box.texture = renderText(r, striker.font, striker.text, striker.textColor);
    winger.box.texture = renderText(r, winger.font, winger.text, winger.textColor);

    wideMid.box.texture = renderText(r, wideMid.font, wideMid.text, wideMid.textColor);
    centralMid.box.texture = renderText(r, centralMid.font, centralMid.text, centralMid.textColor);
    attackMid.box.texture = renderText(r, attackMid.font, attackMid.text, attackMid.textColor);
    defenseMid.box.texture = renderText(r, defenseMid.font, defenseMid.text, defenseMid.textColor);

    fullback.box.texture = renderText(r, fullback.font, fullback.text, fullback.textColor);
    wingback.box.texture = renderText(r, wingback.font, wingback.text, wingback.textColor);
    centerback.box.texture = renderText(r, centerback.font, centerback.text, centerback.textColor);

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
                
                if (*select == 0) {
                    if (isMouseOverButton(striker, x, y)) {
                        *what_choice = 0;
                        quit = true;
                    }
                    else if (isMouseOverButton(winger,x,y)) {
                        *what_choice = 1;
                        quit = true;
                    }
                }
                else if (*select == 1) {
                    if (isMouseOverButton(wideMid, x, y)) {
                        *what_choice = 2;
                        quit = true;
                    }
                    else if (isMouseOverButton(centralMid, x, y)) {
                        *what_choice = 3;
                        quit = true;
                    }
                    else if (isMouseOverButton(attackMid, x, y)) {
                        *what_choice = 4;
                        quit = true;
                    }
                    else if (isMouseOverButton(defenseMid, x, y)) {
                            *what_choice = 5;
                            quit = true;
                    }
                }
                else if (*select == 2) {
                    if (isMouseOverButton(fullback, x, y)) {
                        *what_choice = 6;
                        quit = true;
                    }
                    else if (isMouseOverButton(wingback, x, y)) {
                        *what_choice = 7;
                        quit = true;
                    }
                    else if (isMouseOverButton(centerback, x, y)) {
                        *what_choice = 8;
                        quit = true;
                    }
                }
            }
        }
        if (*select != 3) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            
            if (isMouseOverButton(striker, x,y)) {
                striker.box.background_color = button_hover_color;
            } else {
                striker.box.background_color = button_color;
            }
            if (isMouseOverButton(winger, x, y)) {
                winger.box.background_color = button_hover_color;
            } else {
                winger.box.background_color = button_color;
            }
            if (isMouseOverButton(wideMid, x, y)) {
                wideMid.box.background_color = button_hover_color;
            } else {
                wideMid.box.background_color = button_color;
            }
            if (isMouseOverButton(centralMid, x, y)) {
                centralMid.box.background_color = button_hover_color;
            } else {
                centralMid.box.background_color = button_color;
            }
            if (isMouseOverButton(attackMid, x, y)) {
                attackMid.box.background_color = button_hover_color;
            } else {
                attackMid.box.background_color = button_color;
            }
            if (isMouseOverButton(defenseMid, x, y)) {
                defenseMid.box.background_color = button_hover_color;
            } else {
                defenseMid.box.background_color = button_color;
            }
            if (isMouseOverButton(fullback, x, y)) {
                fullback.box.background_color = button_hover_color;
            } else {
                fullback.box.background_color = button_color;
            }
            if (isMouseOverButton(wingback, x, y)) {
                wingback.box.background_color = button_hover_color;
            } else {
                wingback.box.background_color = button_color;
            }
            if (isMouseOverButton(centerback, x, y)) {
                centerback.box.background_color = button_hover_color;
            } else {
                centerback.box.background_color = button_color;
            }
        }
        else {
            *what_choice = 9;
            quit = true;
        }
        SDL_SetRenderDrawColor(r, 255,255,255,255);
        SDL_RenderClear(r);
        if (*select == 0) {
            SDL_RenderClear(r);
            renderButton(r, striker);
            renderButton(r, winger);
        }
        
        else if (*select == 1) {
            SDL_RenderClear(r);
            renderButton(r,wideMid);
            renderButton(r, centralMid);
            renderButton(r, attackMid);
            renderButton(r, defenseMid);
        }
        
        else if (*select == 2) {
            SDL_RenderClear(r);
            renderButton(r, fullback);
            renderButton(r, wingback);
            renderButton(r, centerback);
        }
        
        SDL_RenderPresent(r);

    }
    SDL_DestroyTexture(striker.box.texture); SDL_DestroyTexture(winger.box.texture); SDL_DestroyTexture(wideMid.box.texture); SDL_DestroyTexture(centralMid.box.texture);
    SDL_DestroyTexture(attackMid.box.texture); SDL_DestroyTexture(defenseMid.box.texture); SDL_DestroyTexture(fullback.box.texture); SDL_DestroyTexture(wingback.box.texture);
    SDL_DestroyTexture(wingback.box.texture); SDL_DestroyTexture(centerback.box.texture); 
}
// 정수형을 char 형태로 바꿔주는 함수
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
// 찾은 선수들 화면 출력 함수
void player_data_screen(SDL_Renderer* r, SDL_Event* e, Fonts* fonts, Circularlist* list, int* idCounter, bool* home_quit) {
    Node* current = list->head;

    SDL_Surface* football = IMG_Load("football.jpg");
    SDL_Texture* texture_football = SDL_CreateTextureFromSurface(r, football);
    SDL_Rect football_img = {1230,40, football->w, football->h};
    SDL_Color textColor = {0,0,0,255};
    SDL_Color textRedColor = {255,0,0,255};
    SDL_Color box_color = {255,255,255,0};
    SDL_Color box_color_yellow = {255,226,193,255};
    SDL_Color box_color_blue = {114,201,255,255};
    SDL_Color box_0 = {255, 255, 255, 0};
    SDL_Color button_color = {200,100,150,255};
    SDL_Color button_hover_color = {200,200,150,255};

    int word5 = 50; int word10 = 60; int word15 = 70; int word20 = 80;
    Data_pos data_pos = {
      {50,680}, //num id
      {40,20}, //name
      {560,50}, //position
      {560,20}, //age
      {40,85}, //ca
      {180,85}, //pa
      {760,20}, //nationality
      {40,50}, //club
      {{60,125}, {260,125}, {460,125}, {660,125}, {860,125}, {1060,125},
       {60,165}, {270,165}, {460,165}, {660,165}, {860,165}, {1060,165},
       {60,205}, {270,205}, {460,205}, {660,205}, {860,205}, {1060,205},
       {85,245}, {250,245}, {460,245}, {660,245}, {860,245}, {1060,245},
       {75,285}, {260,285}, {460,285}, {660,285}, {860,285}, {1060,285},
       {60,325}, {280,325}, {460,325}, {660,325}, {860,325}, {1060,325},
       {60,365}, {260,365}, {460,365}, {660,365}, {860,365}, {1060,365},
       {100,405}, {300,405}, {460,405}, {660,405}, {860,405}, {1060,405},
       {60,445}, {270,445}, {460,445}, {660,445}, {860,445}, {1060,445},
       {60,485}, {270,485}, {460,485}, {660,485}, {860,485}, {1060,485}},
      {{1328, 310}, {1298,280}, {1328,280},{1358,280}, {1258,270}, {1398,270}, {1328,250}, {1288,220}, {1328,220}, {1368,220},{1258,190},{1328,190},{1398,190},{1328,160}}, //position
      {1270,365}, //height
      {1270,405}, //weight
      {1270,445}, //left
      {1270,485}, //right
      {1270,525}, //values
      {110,545}, //current_reputation
      {120,570}, //domestice
      {100,595}, //world
      {860,545}, //RCA
      {340,20}, //birth
      {480,545}, //number of national team
      {460,570}, //goals
      {1270,565}, //salary
      {760,50} //UID
    };
    int box_w = 0; int box_h = 0;
    Button cornersBox = {{{data_pos.ability_pos.corners.x, data_pos.ability_pos.corners.y, box_w, box_h}, box_color, NULL}, textColor, "Corners", fonts->font18};
    Button crossingBox = {{{data_pos.ability_pos.crossing.x, data_pos.ability_pos.crossing.y, box_w, box_h}, box_color, NULL}, textColor, "Crossing", fonts->font18};
    Button dribblingBox = {{{data_pos.ability_pos.dribbling.x, data_pos.ability_pos.dribbling.y, box_w, box_h}, box_color, NULL}, textColor, "Dribbling", fonts->font18};
    Button finishingBox = {{{data_pos.ability_pos.finishing.x, data_pos.ability_pos.finishing.y, box_w, box_h}, box_color, NULL}, textColor, "Finishing", fonts->font18};
    Button firstTouchBox = {{{data_pos.ability_pos.first_touch.x, data_pos.ability_pos.first_touch.y, box_w, box_h}, box_color, NULL}, textColor, "First Touch", fonts->font18};
    Button freeKickTakingBox = {{{data_pos.ability_pos.free_kick_taking.x, data_pos.ability_pos.free_kick_taking.y, box_w, box_h}, box_color, NULL}, textColor, "Free Kick Taking", fonts->font18};
    Button headingBox = {{{data_pos.ability_pos.heading.x, data_pos.ability_pos.heading.y, box_w, box_h}, box_color, NULL}, textColor, "Heading", fonts->font18};
    Button longShotBox = {{{data_pos.ability_pos.long_shot.x, data_pos.ability_pos.long_shot.y, box_w, box_h}, box_color, NULL}, textColor, "Long Shot", fonts->font18};
    Button longThrowsBox = {{{data_pos.ability_pos.long_throws.x, data_pos.ability_pos.long_throws.y, box_w, box_h}, box_color, NULL}, textColor, "Long Throws", fonts->font18};
    Button markingBox = {{{data_pos.ability_pos.marking.x, data_pos.ability_pos.marking.y, box_w, box_h}, box_color, NULL}, textColor, "Marking", fonts->font18};
    Button passingBox = {{{data_pos.ability_pos.passing.x, data_pos.ability_pos.passing.y, box_w, box_h}, box_color, NULL}, textColor, "Passing", fonts->font18};
    Button penaltyTakingBox = {{{data_pos.ability_pos.penalty_taking.x, data_pos.ability_pos.penalty_taking.y, box_w, box_h}, box_color, NULL}, textColor, "Penalty Taking", fonts->font18};
    Button tacklingBox = {{{data_pos.ability_pos.tackling.x, data_pos.ability_pos.tackling.y, box_w, box_h}, box_color, NULL}, textColor, "Tackling", fonts->font18};
    Button techniqueBox = {{{data_pos.ability_pos.technique.x, data_pos.ability_pos.technique.y, box_w, box_h}, box_color, NULL}, textColor, "Technique", fonts->font18};
    Button aggressionBox = {{{data_pos.ability_pos.aggressiion.x, data_pos.ability_pos.aggressiion.y, box_w, box_h}, box_color, NULL}, textColor, "Aggression", fonts->font18};
    Button anticipationBox = {{{data_pos.ability_pos.anticipation.x, data_pos.ability_pos.anticipation.y, box_w, box_h}, box_color, NULL}, textColor, "Anticipation", fonts->font18};
    Button braveryBox = {{{data_pos.ability_pos.bravery.x, data_pos.ability_pos.bravery.y, box_w, box_h}, box_color, NULL}, textColor, "Bravery", fonts->font18};
    Button composureBox = {{{data_pos.ability_pos.composure.x, data_pos.ability_pos.composure.y, box_w, box_h}, box_color, NULL}, textColor, "Composure", fonts->font18};
    Button concentrationBox = {{{data_pos.ability_pos.concentration.x, data_pos.ability_pos.concentration.y, box_w, box_h}, box_color, NULL}, textColor, "Concentration", fonts->font18};
    Button visionBox = {{{data_pos.ability_pos.vision.x, data_pos.ability_pos.vision.y, box_w, box_h}, box_color, NULL}, textColor, "Vision", fonts->font18};
    Button decisionBox = {{{data_pos.ability_pos.decision.x, data_pos.ability_pos.decision.y, box_w, box_h}, box_color, NULL}, textColor, "Decision", fonts->font18};
    Button determinationBox = {{{data_pos.ability_pos.determination.x, data_pos.ability_pos.determination.y, box_w, box_h}, box_color, NULL}, textColor, "Determination", fonts->font18};
    Button flairBox = {{{data_pos.ability_pos.flair.x, data_pos.ability_pos.flair.y, box_w, box_h}, box_color, NULL}, textColor, "Flair", fonts->font18};
    Button leadershipBox = {{{data_pos.ability_pos.leadership.x, data_pos.ability_pos.leadership.y, box_w, box_h}, box_color, NULL}, textColor, "Leadership", fonts->font18};
    Button offTheBallBox = {{{data_pos.ability_pos.off_the_ball.x, data_pos.ability_pos.off_the_ball.y, box_w, box_h}, box_color, NULL}, textColor, "Off The Ball", fonts->font18};
    Button positionBox = {{{data_pos.ability_pos.position.x, data_pos.ability_pos.position.y, box_w, box_h}, box_color, NULL}, textColor, "Position", fonts->font18};
    Button teamworkBox = {{{data_pos.ability_pos.teamwork.x, data_pos.ability_pos.teamwork.y, box_w, box_h}, box_color, NULL}, textColor, "Teamwork", fonts->font18};
    Button workRateBox = {{{data_pos.ability_pos.work_rate.x, data_pos.ability_pos.work_rate.y, box_w, box_h}, box_color, NULL}, textColor, "Work Rate", fonts->font18};
    Button accelerationBox = {{{data_pos.ability_pos.acceleration.x, data_pos.ability_pos.acceleration.y, box_w, box_h}, box_color, NULL}, textColor, "Acceleration", fonts->font18};
    Button agilityBox = {{{data_pos.ability_pos.agility.x, data_pos.ability_pos.agility.y, box_w, box_h}, box_color, NULL}, textColor, "Agility", fonts->font18};
    Button balanceBox = {{{data_pos.ability_pos.balance.x, data_pos.ability_pos.balance.y, box_w, box_h}, box_color, NULL}, textColor, "Balance", fonts->font18};
    Button jumpingReachBox = {{{data_pos.ability_pos.jumping_reach.x, data_pos.ability_pos.jumping_reach.y, box_w, box_h}, box_color, NULL}, textColor, "Jumping Reach", fonts->font18};
    Button naturalFitnessBox = {{{data_pos.ability_pos.natural_fitness.x, data_pos.ability_pos.natural_fitness.y, box_w, box_h}, box_color, NULL}, textColor, "Natural Fitness", fonts->font18};
    Button paceBox = {{{data_pos.ability_pos.pace.x, data_pos.ability_pos.pace.y, box_w, box_h}, box_color, NULL}, textColor, "Pace", fonts->font18};
    Button staminaBox = {{{data_pos.ability_pos.stamina.x, data_pos.ability_pos.stamina.y, box_w, box_h}, box_color, NULL}, textColor, "Stamina", fonts->font18};
    Button strengthBox = {{{data_pos.ability_pos.strength.x, data_pos.ability_pos.strength.y, box_w, box_h}, box_color, NULL}, textColor, "Strength", fonts->font18};
    Button stabilityBox = {{{data_pos.ability_pos.stability.x, data_pos.ability_pos.stability.y, box_w, box_h}, box_color, NULL}, textColor, "Stability", fonts->font18};
    Button foulBox = {{{data_pos.ability_pos.foul.x, data_pos.ability_pos.foul.y, box_w, box_h}, box_color, NULL}, textColor, "Foul", fonts->font18};
    Button contestPerformanceBox = {{{data_pos.ability_pos.contest_performance.x, data_pos.ability_pos.contest_performance.y, box_w, box_h}, box_color, NULL}, textColor, "Contest Performance", fonts->font18};
    Button injuryBox = {{{data_pos.ability_pos.injury.x, data_pos.ability_pos.injury.y, box_w, box_h}, box_color, NULL}, textColor, "Injury", fonts->font18};
    Button diversityBox = {{{data_pos.ability_pos.diversity.x, data_pos.ability_pos.diversity.y, box_w, box_h}, box_color, NULL}, textColor, "Diversity", fonts->font18};
    Button aerialReachBox = {{{data_pos.ability_pos.aerial_reach.x, data_pos.ability_pos.aerial_reach.y, box_w, box_h}, box_color, NULL}, textColor, "Aerial Reach", fonts->font18};
    Button commandOfAreaBox = {{{data_pos.ability_pos.command_of_area.x, data_pos.ability_pos.command_of_area.y, box_w, box_h}, box_color, NULL}, textColor, "Command Of Area", fonts->font18};
    Button communicationBox = {{{data_pos.ability_pos.communication.x, data_pos.ability_pos.communication.y, box_w, box_h}, box_color, NULL}, textColor, "Communication", fonts->font18};
    Button eccentricityBox = {{{data_pos.ability_pos.eccentricity.x, data_pos.ability_pos.eccentricity.y, box_w, box_h}, box_color, NULL}, textColor, "Eccentricity", fonts->font18};
    Button handlingBox = {{{data_pos.ability_pos.handling.x, data_pos.ability_pos.handling.y, box_w, box_h}, box_color, NULL}, textColor, "Handling", fonts->font18};
    Button kickingBox = {{{data_pos.ability_pos.kicking.x, data_pos.ability_pos.kicking.y, box_w, box_h}, box_color, NULL}, textColor, "Kicking", fonts->font18};
    Button oneOnOnesBox = {{{data_pos.ability_pos.one_on_ones.x, data_pos.ability_pos.one_on_ones.y, box_w, box_h}, box_color, NULL}, textColor, "One On Ones", fonts->font18};
    Button reflexesBox = {{{data_pos.ability_pos.reflexes.x, data_pos.ability_pos.reflexes.y, box_w, box_h}, box_color, NULL}, textColor, "Reflexes", fonts->font18};
    Button rushingOutBox = {{{data_pos.ability_pos.rushing_out.x, data_pos.ability_pos.rushing_out.y, box_w, box_h}, box_color, NULL}, textColor, "Rushing Out", fonts->font18};
    Button throwingBox = {{{data_pos.ability_pos.throwing.x, data_pos.ability_pos.throwing.y, box_w, box_h}, box_color, NULL}, textColor, "Throwing", fonts->font18};
    Button adaptationBox = {{{data_pos.ability_pos.adaptation.x, data_pos.ability_pos.adaptation.y, box_w, box_h}, box_color, NULL}, textColor, "Adaptation", fonts->font18};
    Button ambitionBox = {{{data_pos.ability_pos.ambition.x, data_pos.ability_pos.ambition.y, box_w, box_h}, box_color, NULL}, textColor, "Ambition", fonts->font18};
    Button argueBox = {{{data_pos.ability_pos.argue.x, data_pos.ability_pos.argue.y, box_w, box_h}, box_color, NULL}, textColor, "Argue", fonts->font18};
    Button loyalBox = {{{data_pos.ability_pos.loyal.x, data_pos.ability_pos.loyal.y, box_w, box_h}, box_color, NULL}, textColor, "Loyal", fonts->font18};
    Button professionalBox = {{{data_pos.ability_pos.professional.x, data_pos.ability_pos.professional.y, box_w, box_h}, box_color, NULL}, textColor, "Professional", fonts->font18};
    Button sportmanshipBox = {{{data_pos.ability_pos.sportmanship.x, data_pos.ability_pos.sportmanship.y, box_w, box_h}, box_color, NULL}, textColor, "Sportmanship", fonts->font18};
    Button emotionalControlBox = {{{data_pos.ability_pos.emotional_control.x, data_pos.ability_pos.emotional_control.y, box_w, box_h}, box_color, NULL}, textColor, "Emotional Control", fonts->font18};
    Button punchingBox = {{{data_pos.ability_pos.punching.x, data_pos.ability_pos.punching.y, box_w, box_h}, box_color, NULL}, textColor, "Punching", fonts->font18};
    Button resistantToStressBox = {{{data_pos.ability_pos.resistant_to_stress.x, data_pos.ability_pos.resistant_to_stress.y, box_w, box_h}, box_color, NULL}, textColor, "Resistant To Stress", fonts->font18};

    // Position Boxes
    Button GKBox = {{{data_pos.position_pos.GK.x, data_pos.position_pos.GK.y, box_w, box_h}, box_color, NULL}, textRedColor, "GK", fonts->font15};
    Button DLBox = {{{data_pos.position_pos.DL.x, data_pos.position_pos.DL.y, box_w, box_h}, box_color, NULL}, textRedColor, "DL", fonts->font15};
    Button DCBox = {{{data_pos.position_pos.DC.x, data_pos.position_pos.DC.y, box_w, box_h}, box_color, NULL}, textRedColor, "DC", fonts->font15};
    Button DRBox = {{{data_pos.position_pos.DR.x, data_pos.position_pos.DR.y, box_w, box_h}, box_color, NULL}, textRedColor, "DR", fonts->font15};
    Button WBLBox = {{{data_pos.position_pos.WBL.x, data_pos.position_pos.WBL.y, box_w, box_h}, box_color, NULL}, textRedColor, "WBL", fonts->font15};
    Button WBRBox = {{{data_pos.position_pos.WBR.x, data_pos.position_pos.WBR.y, box_w, box_h}, box_color, NULL}, textRedColor, "WBR", fonts->font15};
    Button DMBox = {{{data_pos.position_pos.DM.x, data_pos.position_pos.DM.y, box_w, box_h}, box_color, NULL}, textRedColor, "DM", fonts->font15};
    Button MLBox = {{{data_pos.position_pos.ML.x, data_pos.position_pos.ML.y, box_w, box_h}, box_color, NULL}, textRedColor, "ML", fonts->font15};
    Button MCBox = {{{data_pos.position_pos.MC.x, data_pos.position_pos.MC.y, box_w, box_h}, box_color, NULL}, textRedColor, "MC", fonts->font15};
    Button MRBox = {{{data_pos.position_pos.MR.x, data_pos.position_pos.MR.y, box_w, box_h}, box_color, NULL}, textRedColor, "MR", fonts->font15};
    Button AMLBox = {{{data_pos.position_pos.AML.x, data_pos.position_pos.AML.y, box_w, box_h}, box_color, NULL}, textRedColor, "AML", fonts->font15};
    Button AMCBox = {{{data_pos.position_pos.AMC.x, data_pos.position_pos.AMC.y, box_w, box_h}, box_color, NULL}, textRedColor, "AMC", fonts->font15};
    Button AMRBox = {{{data_pos.position_pos.AMR.x, data_pos.position_pos.AMR.y, box_w, box_h}, box_color, NULL}, textRedColor, "AMR", fonts->font15};
    Button STBox = {{{data_pos.position_pos.ST.x, data_pos.position_pos.ST.y, box_w, box_h}, box_color, NULL}, textRedColor, "ST", fonts->font15};

    // Additional Data Fields
    
    Button nameBox = {{{data_pos.name.x, data_pos.name.y, box_w, box_h}, box_color, NULL}, textColor, "Name", fonts->font20};
    Button where_positionBox = {{{data_pos.position.x, data_pos.position.y, box_w, box_h}, box_color, NULL}, textColor, "Position", fonts->font20};
    Button ageBox = {{{data_pos.age.x, data_pos.age.y, box_w, box_h}, box_color, NULL}, textColor, "Age", fonts->font20};
    Button caBox = {{{data_pos.ca.x, data_pos.ca.y, box_w, box_h}, box_color, NULL}, textColor, "CA", fonts->font18};
    Button paBox = {{{data_pos.pa.x, data_pos.pa.y, box_w, box_h}, box_color, NULL}, textColor, "PA", fonts->font18};
    Button nationalityBox = {{{data_pos.nationality.x, data_pos.nationality.y, box_w, box_h}, box_color, NULL}, textColor, "Nation", fonts->font20};
    Button clubBox = {{{data_pos.club.x, data_pos.club.y, box_w, box_h}, box_color, NULL}, textColor, "Club", fonts->font20};
    Button heightBox = {{{data_pos.height.x, data_pos.height.y, box_w, box_h}, box_color, NULL}, textColor, "Height", fonts->font18};
    Button weightBox = {{{data_pos.weight.x, data_pos.weight.y, box_w, box_h}, box_color, NULL}, textColor, "Weight", fonts->font18};
    Button leftBox = {{{data_pos.left.x, data_pos.left.y, box_w, box_h}, box_color, NULL}, textColor, "Left", fonts->font18};
    Button rightBox = {{{data_pos.right.x, data_pos.right.y, box_w, box_h}, box_color, NULL}, textColor, "Right", fonts->font18};
    Button valuesBox = {{{data_pos.values.x, data_pos.values.y, box_w, box_h}, box_color, NULL}, textColor, "Values", fonts->font18};
    Button currentReputationBox = {{{data_pos.current_reputation.x, data_pos.current_reputation.y, box_w, box_h}, box_color, NULL}, textColor, "Current Reputation", fonts->font18};
    Button domesticReputationBox = {{{data_pos.domestic_reputation.x, data_pos.domestic_reputation.y, box_w, box_h}, box_color, NULL}, textColor, "Domestic Reputation", fonts->font18};
    Button worldReputationBox = {{{data_pos.world_reputation.x, data_pos.world_reputation.y, box_w, box_h}, box_color, NULL}, textColor, "World Reputation", fonts->font18};
    Button RCABBox = {{{data_pos.RCA.x, data_pos.RCA.y, box_w, box_h}, box_color, NULL}, textColor, "RCA", fonts->font18};
    Button birthBox = {{{data_pos.birth.x, data_pos.birth.y, box_w, box_h}, box_color, NULL}, textColor, "Birth", fonts->font20};
    Button numberOfNationalTeamAppearanceBox = {{{data_pos.number_of_national_team_appearance.x, data_pos.number_of_national_team_appearance.y, box_w, box_h}, box_color, NULL}, textColor, "National Team Appearances", fonts->font18};
    Button goalsScoredForNationalTeamBox = {{{data_pos.goals_scored_for_the_national_team.x, data_pos.goals_scored_for_the_national_team.y, box_w, box_h}, box_color, NULL}, textColor, "National Team Goals", fonts->font18};
    Button salaryBox = {{{data_pos.salary.x, data_pos.salary.y, box_w, box_h}, box_color, NULL}, textColor, "Salary", fonts->font18};
    Button UIDBox = {{{data_pos.UID.x, data_pos.UID.y, box_w, box_h}, box_color, NULL}, textColor, "UID", fonts->font20};

    Box data_box = {{20,70, 1200,580},box_color_yellow, NULL};
    Box blue_box = {{data_pos.height.x-40,data_pos.height.y-15, 180,230},box_color_blue, NULL};

    Button numIdBox = {{{data_pos.numId.x,data_pos.numId.y,box_w,box_h},box_color,NULL}, textColor, to_char(*idCounter-1) ,fonts->font20};
    Button IdBox = {{{data_pos.numId.x + 50,data_pos.numId.y,box_w,box_h},box_color,NULL}, textColor, to_char(current->id) ,fonts->font20};

    Button corners = {{{data_pos.ability_pos.corners.x+word5, data_pos.ability_pos.corners.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.corners), fonts->font18};
    Button crossing = {{{data_pos.ability_pos.crossing.x+word5, data_pos.ability_pos.crossing.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.crossing), fonts->font18};
    Button dribbling = {{{data_pos.ability_pos.dribbling.x+word5, data_pos.ability_pos.dribbling.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.dribbling), fonts->font18};
    Button finishing = {{{data_pos.ability_pos.finishing.x+word5, data_pos.ability_pos.finishing.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.finishing), fonts->font18};
    Button firstTouch = {{{data_pos.ability_pos.first_touch.x+word10, data_pos.ability_pos.first_touch.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.first_touch), fonts->font18};
    Button freeKickTaking = {{{data_pos.ability_pos.free_kick_taking.x+word20, data_pos.ability_pos.free_kick_taking.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.free_kick_taking), fonts->font18};
    Button heading = {{{data_pos.ability_pos.heading.x+word5, data_pos.ability_pos.heading.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.heading), fonts->font18};
    Button longShot = {{{data_pos.ability_pos.long_shot.x+word10, data_pos.ability_pos.long_shot.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.long_shot), fonts->font18};
    Button longThrows = {{{data_pos.ability_pos.long_throws.x+word15, data_pos.ability_pos.long_throws.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.long_throws), fonts->font18};
    Button marking = {{{data_pos.ability_pos.marking.x+word5, data_pos.ability_pos.marking.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.marking), fonts->font18};
    Button passing = {{{data_pos.ability_pos.passing.x+word5, data_pos.ability_pos.passing.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.passing), fonts->font18};
    Button penaltyTaking = {{{data_pos.ability_pos.penalty_taking.x+word20, data_pos.ability_pos.penalty_taking.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.penalty_taking), fonts->font18};
    Button tackling = {{{data_pos.ability_pos.tackling.x+word5, data_pos.ability_pos.tackling.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.tackling), fonts->font18};
    Button technique = {{{data_pos.ability_pos.technique.x+word10, data_pos.ability_pos.technique.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.technique), fonts->font18};
    Button aggression = {{{data_pos.ability_pos.aggressiion.x+word15, data_pos.ability_pos.aggressiion.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.aggressiion), fonts->font18};
    Button anticipation = {{{data_pos.ability_pos.anticipation.x+word15, data_pos.ability_pos.anticipation.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.anticipation), fonts->font18};
    Button bravery = {{{data_pos.ability_pos.bravery.x+word5, data_pos.ability_pos.bravery.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.bravery), fonts->font18};
    Button composure = {{{data_pos.ability_pos.composure.x+word10, data_pos.ability_pos.composure.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.composure), fonts->font18};
    Button concentration = {{{data_pos.ability_pos.concentration.x+word20, data_pos.ability_pos.concentration.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.concentration), fonts->font18};
    Button vision = {{{data_pos.ability_pos.vision.x+word5, data_pos.ability_pos.vision.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.vision), fonts->font18};
    Button decision = {{{data_pos.ability_pos.decision.x+word5, data_pos.ability_pos.decision.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.decision), fonts->font18};
    Button determination = {{{data_pos.ability_pos.determination.x+word20, data_pos.ability_pos.determination.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.determination), fonts->font18};
    Button flair = {{{data_pos.ability_pos.flair.x+word5, data_pos.ability_pos.flair.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.flair), fonts->font18};
    Button leadership = {{{data_pos.ability_pos.leadership.x+word10, data_pos.ability_pos.leadership.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.leadership), fonts->font18};
    Button offTheBall = {{{data_pos.ability_pos.off_the_ball.x+word15, data_pos.ability_pos.off_the_ball.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.off_the_ball), fonts->font18};
    Button position = {{{data_pos.ability_pos.position.x+word10, data_pos.ability_pos.position.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.position), fonts->font18};
    Button teamwork = {{{data_pos.ability_pos.teamwork.x+word10, data_pos.ability_pos.teamwork.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.teamwork), fonts->font18};
    Button workRate = {{{data_pos.ability_pos.work_rate.x+word10, data_pos.ability_pos.work_rate.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.work_rate), fonts->font18};
    Button acceleration = {{{data_pos.ability_pos.acceleration.x+word15, data_pos.ability_pos.acceleration.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.acceleration), fonts->font18};
    Button agility = {{{data_pos.ability_pos.agility.x+word5, data_pos.ability_pos.agility.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.agility), fonts->font18};
    Button balance = {{{data_pos.ability_pos.balance.x+word5, data_pos.ability_pos.balance.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.balance), fonts->font18};
    Button jumpingReach = {{{data_pos.ability_pos.jumping_reach.x+word20, data_pos.ability_pos.jumping_reach.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.jumping_reach), fonts->font18};
    Button naturalFitness = {{{data_pos.ability_pos.natural_fitness.x+word20, data_pos.ability_pos.natural_fitness.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.natural_fitness), fonts->font18};
    Button pace = {{{data_pos.ability_pos.pace.x+word5, data_pos.ability_pos.pace.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.pace), fonts->font18};
    Button stamina = {{{data_pos.ability_pos.stamina.x+word10, data_pos.ability_pos.stamina.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.stamina), fonts->font18};
    Button strength = {{{data_pos.ability_pos.strength.x+word10, data_pos.ability_pos.strength.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.strength), fonts->font18};
    Button stability = {{{data_pos.ability_pos.stability.x+word10, data_pos.ability_pos.stability.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.stability), fonts->font18};
    Button foul = {{{data_pos.ability_pos.foul.x+word5, data_pos.ability_pos.foul.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.foul), fonts->font18};
    Button contestPerformance = {{{data_pos.ability_pos.contest_performance.x+120, data_pos.ability_pos.contest_performance.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.contest_performance), fonts->font18};
    Button injury = {{{data_pos.ability_pos.injury.x+word5, data_pos.ability_pos.injury.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.injury), fonts->font18};
    Button diversity = {{{data_pos.ability_pos.diversity.x+word10, data_pos.ability_pos.diversity.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.diversity), fonts->font18};
    Button aerialReach = {{{data_pos.ability_pos.aerial_reach.x+word15, data_pos.ability_pos.aerial_reach.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.aerial_reach), fonts->font18};
    Button commandOfArea = {{{data_pos.ability_pos.command_of_area.x+100, data_pos.ability_pos.command_of_area.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.command_of_area), fonts->font18};
    Button communication = {{{data_pos.ability_pos.communication.x+word20, data_pos.ability_pos.communication.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.communication), fonts->font18};
    Button eccentricity = {{{data_pos.ability_pos.eccentricity.x+word15, data_pos.ability_pos.eccentricity.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.eccentricity), fonts->font18};
    Button handling = {{{data_pos.ability_pos.handling.x+word10, data_pos.ability_pos.handling.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.handling), fonts->font18};
    Button kicking = {{{data_pos.ability_pos.kicking.x+word5, data_pos.ability_pos.kicking.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.kicking), fonts->font18};
    Button oneOnOnes = {{{data_pos.ability_pos.one_on_ones.x+word15, data_pos.ability_pos.one_on_ones.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.one_on_ones), fonts->font18};
    Button reflexes = {{{data_pos.ability_pos.reflexes.x+word5, data_pos.ability_pos.reflexes.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.reflexes), fonts->font18};
    Button rushingOut = {{{data_pos.ability_pos.rushing_out.x+word15, data_pos.ability_pos.rushing_out.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.rushing_out), fonts->font18};
    Button throwing = {{{data_pos.ability_pos.throwing.x+word10, data_pos.ability_pos.throwing.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.throwing), fonts->font18};
    Button adaptation = {{{data_pos.ability_pos.adaptation.x+word15, data_pos.ability_pos.adaptation.y, box_w, box_h}, box_color, NULL}, textColor,to_char(current->player.ability_stat.adaptation), fonts->font18};
    Button ambition = {{{data_pos.ability_pos.ambition.x+word10, data_pos.ability_pos.ambition.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.ambition), fonts->font18};
    Button argue = {{{data_pos.ability_pos.argue.x+word5, data_pos.ability_pos.argue.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.argue), fonts->font18};
    Button loyal = {{{data_pos.ability_pos.loyal.x+word5, data_pos.ability_pos.loyal.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.loyal), fonts->font18};
    Button professional = {{{data_pos.ability_pos.professional.x+word15, data_pos.ability_pos.professional.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.professional), fonts->font18};
    Button sportmanship = {{{data_pos.ability_pos.sportmanship.x+word20, data_pos.ability_pos.sportmanship.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.sportmanship), fonts->font18};
    Button emotionalControl = {{{data_pos.ability_pos.emotional_control.x+100, data_pos.ability_pos.emotional_control.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.emotional_control), fonts->font18};
    Button punching = {{{data_pos.ability_pos.punching.x+word5, data_pos.ability_pos.punching.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.punching), fonts->font18};
    Button resistantToStress = {{{data_pos.ability_pos.resistant_to_stress.x+110, data_pos.ability_pos.resistant_to_stress.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ability_stat.resistant_to_stress), fonts->font18};

    // Position Buttons
    Button GK = {{{data_pos.position_pos.GK.x, data_pos.position_pos.GK.y+15, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.position_stat.GK), fonts->font18};
    Button DL = {{{data_pos.position_pos.DL.x, data_pos.position_pos.DL.y+15, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.position_stat.DL), fonts->font18};
    Button DC = {{{data_pos.position_pos.DC.x, data_pos.position_pos.DC.y+15, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.position_stat.DC), fonts->font18};
    Button DR = {{{data_pos.position_pos.DR.x, data_pos.position_pos.DR.y+15, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.position_stat.DR), fonts->font18};
    Button WBL = {{{data_pos.position_pos.WBL.x, data_pos.position_pos.WBL.y+15, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.position_stat.WBL), fonts->font18};
    Button WBR = {{{data_pos.position_pos.WBR.x, data_pos.position_pos.WBR.y+15, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.position_stat.WBR), fonts->font18};
    Button DM = {{{data_pos.position_pos.DM.x, data_pos.position_pos.DM.y+15, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.position_stat.DM), fonts->font18};
    Button ML = {{{data_pos.position_pos.ML.x, data_pos.position_pos.ML.y+15, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.position_stat.ML), fonts->font18};
    Button MC = {{{data_pos.position_pos.MC.x, data_pos.position_pos.MC.y+15, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.position_stat.MC), fonts->font18};
    Button MR = {{{data_pos.position_pos.MR.x, data_pos.position_pos.MR.y+15, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.position_stat.MR), fonts->font18};
    Button AML = {{{data_pos.position_pos.AML.x, data_pos.position_pos.AML.y+15, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.position_stat.AML), fonts->font18};
    Button AMC = {{{data_pos.position_pos.AMC.x, data_pos.position_pos.AMC.y+15, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.position_stat.AMC), fonts->font18};
    Button AMR = {{{data_pos.position_pos.AMR.x, data_pos.position_pos.AMR.y+15, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.position_stat.AMR), fonts->font18};
    Button ST = {{{data_pos.position_pos.ST.x, data_pos.position_pos.ST.y+15, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.position_stat.ST), fonts->font18};

    // Additional Data Fields

    Button name = {{{data_pos.name.x+150, data_pos.name.y, box_w, box_h}, box_color, NULL}, textColor, current->player.name, fonts->font20};
    Button where_position = {{{data_pos.position.x+100, data_pos.position.y, box_w, box_h}, box_color, NULL}, textColor, current->player.position, fonts->font20};
    Button age = {{{data_pos.age.x+word5, data_pos.age.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.age), fonts->font20};
    Button ca = {{{data_pos.ca.x+word5, data_pos.ca.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.ca), fonts->font18};
    Button pa = {{{data_pos.pa.x+word5, data_pos.pa.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.pa), fonts->font18};
    Button nationality = {{{data_pos.nationality.x+180, data_pos.nationality.y, box_w, box_h}, box_color, NULL}, textColor, current->player.nationality, fonts->font20};
    Button club = {{{data_pos.club.x+200, data_pos.club.y, box_w, box_h}, box_color, NULL}, textColor, current->player.club, fonts->font20};
    Button height = {{{data_pos.height.x+word5, data_pos.height.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.body.height), fonts->font18};
    Button weight = {{{data_pos.weight.x+word5, data_pos.weight.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.body.weight), fonts->font18};
    Button left = {{{data_pos.left.x+word5, data_pos.left.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.foot_ability.left), fonts->font18};
    Button right = {{{data_pos.right.x+word5, data_pos.right.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.foot_ability.right), fonts->font18};
    Button values = {{{data_pos.values.x+word20, data_pos.values.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.values), fonts->font18};
    Button currentReputation = {{{data_pos.current_reputation.x+120, data_pos.current_reputation.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.current_reputation), fonts->font18};
    Button domesticReputation = {{{data_pos.domestic_reputation.x+120, data_pos.domestic_reputation.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.domestic_reputation), fonts->font18};
    Button worldReputation = {{{data_pos.world_reputation.x+120, data_pos.world_reputation.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.world_reputation), fonts->font18};
    Button RCAB = {{{data_pos.RCA.x+word5, data_pos.RCA.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.RCA), fonts->font18};
    Button birth = {{{data_pos.birth.x+word20, data_pos.birth.y, box_w, box_h}, box_color, NULL}, textColor, current->player.birth, fonts->font20};
    Button numberOfNationalTeamAppearance = {{{data_pos.number_of_national_team_appearance.x+150, data_pos.number_of_national_team_appearance.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.number_of_national_team_appearance), fonts->font18};
    Button goalsScoredForNationalTeam = {{{data_pos.goals_scored_for_the_national_team.x+120, data_pos.goals_scored_for_the_national_team.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.goals_scored_for_the_national_team), fonts->font18};
    Button salary = {{{data_pos.salary.x+word15, data_pos.salary.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.salary), fonts->font18};
    Button UID = {{{data_pos.UID.x+word20, data_pos.UID.y, box_w, box_h}, box_color, NULL}, textColor, to_char(current->player.UID), fonts->font20};


    Button prevButton = {{{50, 700, 150, 50}, box_color,NULL}, textColor, "Previous", fonts->font20};
    Button nextButton = {{{1280, 700, 150, 50}, box_color,NULL}, textColor, "Next", fonts->font20};
    Button homeButton = {{{650, 700, 150, 50}, box_color,NULL}, textColor, "HOME", fonts->font20};

    numIdBox.box.texture = renderText(r, numIdBox.font, numIdBox.text, numIdBox.textColor);
    IdBox.box.texture = renderText(r, IdBox.font, IdBox.text, IdBox.textColor);
    

    // Assuming renderText is a function that renders text and returns a texture
    // And r is the renderer
    cornersBox.box.texture = renderText(r, cornersBox.font, cornersBox.text,cornersBox.textColor);
    markingBox.box.texture = renderText(r, markingBox.font, markingBox.text, markingBox.textColor);
    crossingBox.box.texture = renderText(r, crossingBox.font, crossingBox.text, crossingBox.textColor);
    dribblingBox.box.texture = renderText(r, dribblingBox.font, dribblingBox.text, dribblingBox.textColor);
    finishingBox.box.texture = renderText(r, finishingBox.font, finishingBox.text, finishingBox.textColor);
    firstTouchBox.box.texture = renderText(r, firstTouchBox.font, firstTouchBox.text, firstTouchBox.textColor);
    freeKickTakingBox.box.texture = renderText(r, freeKickTakingBox.font, freeKickTakingBox.text, freeKickTakingBox.textColor);
    headingBox.box.texture = renderText(r, headingBox.font, headingBox.text, headingBox.textColor);
    longShotBox.box.texture = renderText(r, longShotBox.font, longShotBox.text, longShotBox.textColor);
    longThrowsBox.box.texture = renderText(r, longThrowsBox.font, longThrowsBox.text, longThrowsBox.textColor);
    markingBox.box.texture = renderText(r, markingBox.font, markingBox.text, markingBox.textColor);
    passingBox.box.texture = renderText(r, passingBox.font, passingBox.text, passingBox.textColor);
    penaltyTakingBox.box.texture = renderText(r, penaltyTakingBox.font, penaltyTakingBox.text, penaltyTakingBox.textColor);
    tacklingBox.box.texture = renderText(r, tacklingBox.font, tacklingBox.text, tacklingBox.textColor);
    techniqueBox.box.texture = renderText(r, techniqueBox.font, techniqueBox.text, techniqueBox.textColor);
    aggressionBox.box.texture = renderText(r, aggressionBox.font, aggressionBox.text, aggressionBox.textColor);
    anticipationBox.box.texture = renderText(r, anticipationBox.font, anticipationBox.text, anticipationBox.textColor);
    braveryBox.box.texture = renderText(r, braveryBox.font, braveryBox.text, braveryBox.textColor);
    composureBox.box.texture = renderText(r, composureBox.font, composureBox.text, composureBox.textColor);
    concentrationBox.box.texture = renderText(r, concentrationBox.font, concentrationBox.text, concentrationBox.textColor);
    visionBox.box.texture = renderText(r, visionBox.font, visionBox.text, visionBox.textColor);
    decisionBox.box.texture = renderText(r, decisionBox.font, decisionBox.text, decisionBox.textColor);
    determinationBox.box.texture = renderText(r, determinationBox.font, determinationBox.text, determinationBox.textColor);
    flairBox.box.texture = renderText(r, flairBox.font, flairBox.text, flairBox.textColor);
    leadershipBox.box.texture = renderText(r, leadershipBox.font, leadershipBox.text, leadershipBox.textColor);
    offTheBallBox.box.texture = renderText(r, offTheBallBox.font, offTheBallBox.text, offTheBallBox.textColor);
    positionBox.box.texture = renderText(r, positionBox.font, positionBox.text, positionBox.textColor);
    teamworkBox.box.texture = renderText(r, teamworkBox.font, teamworkBox.text, teamworkBox.textColor);
    workRateBox.box.texture = renderText(r, workRateBox.font, workRateBox.text, workRateBox.textColor);
    accelerationBox.box.texture = renderText(r, accelerationBox.font, accelerationBox.text, accelerationBox.textColor);
    agilityBox.box.texture = renderText(r, agilityBox.font, agilityBox.text, agilityBox.textColor);
    balanceBox.box.texture = renderText(r, balanceBox.font, balanceBox.text, balanceBox.textColor);
    jumpingReachBox.box.texture = renderText(r, jumpingReachBox.font, jumpingReachBox.text, jumpingReachBox.textColor);
    naturalFitnessBox.box.texture = renderText(r, naturalFitnessBox.font, naturalFitnessBox.text, naturalFitnessBox.textColor);
    paceBox.box.texture = renderText(r, paceBox.font, paceBox.text, paceBox.textColor);
    staminaBox.box.texture = renderText(r, staminaBox.font, staminaBox.text, staminaBox.textColor);
    strengthBox.box.texture = renderText(r, strengthBox.font, strengthBox.text, strengthBox.textColor);
    stabilityBox.box.texture = renderText(r, stabilityBox.font, stabilityBox.text, stabilityBox.textColor);
    foulBox.box.texture = renderText(r, foulBox.font, foulBox.text, foulBox.textColor);
    contestPerformanceBox.box.texture = renderText(r, contestPerformanceBox.font, contestPerformanceBox.text, contestPerformanceBox.textColor);
    injuryBox.box.texture = renderText(r, injuryBox.font, injuryBox.text, injuryBox.textColor);
    diversityBox.box.texture = renderText(r, diversityBox.font, diversityBox.text, diversityBox.textColor);
    aerialReachBox.box.texture = renderText(r, aerialReachBox.font, aerialReachBox.text, aerialReachBox.textColor);
    commandOfAreaBox.box.texture = renderText(r, commandOfAreaBox.font, commandOfAreaBox.text, commandOfAreaBox.textColor);
    communicationBox.box.texture = renderText(r, communicationBox.font, communicationBox.text, communicationBox.textColor);
    eccentricityBox.box.texture = renderText(r, eccentricityBox.font, eccentricityBox.text, eccentricityBox.textColor);
    handlingBox.box.texture = renderText(r, handlingBox.font, handlingBox.text, handlingBox.textColor);
    kickingBox.box.texture = renderText(r, kickingBox.font, kickingBox.text, kickingBox.textColor);
    oneOnOnesBox.box.texture = renderText(r, oneOnOnesBox.font, oneOnOnesBox.text, oneOnOnesBox.textColor);
    reflexesBox.box.texture = renderText(r, reflexesBox.font, reflexesBox.text, reflexesBox.textColor);
    rushingOutBox.box.texture = renderText(r, rushingOutBox.font, rushingOutBox.text, rushingOutBox.textColor);
    throwingBox.box.texture = renderText(r, throwingBox.font, throwingBox.text, throwingBox.textColor);
    adaptationBox.box.texture = renderText(r, adaptationBox.font, adaptationBox.text, adaptationBox.textColor);
    ambitionBox.box.texture = renderText(r, ambitionBox.font, ambitionBox.text, ambitionBox.textColor);
    argueBox.box.texture = renderText(r, argueBox.font, argueBox.text, argueBox.textColor);
    loyalBox.box.texture = renderText(r, loyalBox.font, loyalBox.text, loyalBox.textColor);
    professionalBox.box.texture = renderText(r, professionalBox.font, professionalBox.text, professionalBox.textColor);
    sportmanshipBox.box.texture = renderText(r, sportmanshipBox.font, sportmanshipBox.text, sportmanshipBox.textColor);
    emotionalControlBox.box.texture = renderText(r, emotionalControlBox.font, emotionalControlBox.text, emotionalControlBox.textColor);
    punchingBox.box.texture = renderText(r, punchingBox.font, punchingBox.text, punchingBox.textColor);
    resistantToStressBox.box.texture = renderText(r, resistantToStressBox.font, resistantToStressBox.text, resistantToStressBox.textColor);

    // Position Boxes
    GKBox.box.texture = renderText(r, GKBox.font, GKBox.text, GKBox.textColor);
    DLBox.box.texture = renderText(r, DLBox.font, DLBox.text, DLBox.textColor);
    DCBox.box.texture = renderText(r, DCBox.font, DCBox.text, DCBox.textColor);
    DRBox.box.texture = renderText(r, DRBox.font, DRBox.text, DRBox.textColor);
    WBLBox.box.texture = renderText(r, WBLBox.font, WBLBox.text, WBLBox.textColor);
    WBRBox.box.texture = renderText(r, WBRBox.font, WBRBox.text, WBRBox.textColor);
    DMBox.box.texture = renderText(r, DMBox.font, DMBox.text, DMBox.textColor);
    MLBox.box.texture = renderText(r, MLBox.font, MLBox.text, MLBox.textColor);
    MCBox.box.texture = renderText(r, MCBox.font, MCBox.text, MCBox.textColor);
    MRBox.box.texture = renderText(r, MRBox.font, MRBox.text, MRBox.textColor);
    AMLBox.box.texture = renderText(r, AMLBox.font, AMLBox.text, AMLBox.textColor);
    AMCBox.box.texture = renderText(r, AMCBox.font, AMCBox.text, AMCBox.textColor);
    AMRBox.box.texture = renderText(r, AMRBox.font, AMRBox.text, AMRBox.textColor);
    STBox.box.texture = renderText(r, STBox.font, STBox.text, STBox.textColor);

    // Additional Data Fields

    nameBox.box.texture = renderText(r, nameBox.font, nameBox.text, nameBox.textColor);
    where_positionBox.box.texture = renderText(r, where_positionBox.font, where_positionBox.text, where_positionBox.textColor);
    ageBox.box.texture = renderText(r, ageBox.font, ageBox.text, ageBox.textColor);
    caBox.box.texture = renderText(r, caBox.font, caBox.text, caBox.textColor);
    paBox.box.texture = renderText(r, paBox.font, paBox.text, paBox.textColor);
    nationalityBox.box.texture = renderText(r, nationalityBox.font, nationalityBox.text, nationalityBox.textColor);
    clubBox.box.texture = renderText(r, clubBox.font, clubBox.text, clubBox.textColor);
    heightBox.box.texture = renderText(r, heightBox.font, heightBox.text, heightBox.textColor);
    weightBox.box.texture = renderText(r, weightBox.font, weightBox.text, weightBox.textColor);
    leftBox.box.texture = renderText(r, leftBox.font, leftBox.text, leftBox.textColor);
    rightBox.box.texture = renderText(r, rightBox.font, rightBox.text, rightBox.textColor);
    valuesBox.box.texture = renderText(r, valuesBox.font, valuesBox.text, valuesBox.textColor);
    currentReputationBox.box.texture = renderText(r, currentReputationBox.font, currentReputationBox.text, currentReputationBox.textColor);
    domesticReputationBox.box.texture = renderText(r, domesticReputationBox.font, domesticReputationBox.text, domesticReputationBox.textColor);
    worldReputationBox.box.texture = renderText(r, worldReputationBox.font, worldReputationBox.text, worldReputationBox.textColor);
    RCABBox.box.texture = renderText(r, RCABBox.font, RCABBox.text, RCABBox.textColor);
    birthBox.box.texture = renderText(r, birthBox.font, birthBox.text, birthBox.textColor);
    numberOfNationalTeamAppearanceBox.box.texture = renderText(r, numberOfNationalTeamAppearanceBox.font, numberOfNationalTeamAppearanceBox.text, numberOfNationalTeamAppearanceBox.textColor);
    goalsScoredForNationalTeamBox.box.texture = renderText(r, goalsScoredForNationalTeamBox.font, goalsScoredForNationalTeamBox.text, goalsScoredForNationalTeamBox.textColor);
    salaryBox.box.texture = renderText(r, salaryBox.font, salaryBox.text, salaryBox.textColor);
    UIDBox.box.texture = renderText(r, UIDBox.font, UIDBox.text, UIDBox.textColor);


    corners.box.texture = renderText(r, corners.font, corners.text, corners.textColor);
    marking.box.texture = renderText(r, marking.font, marking.text, marking.textColor);
    crossing.box.texture = renderText(r, crossing.font, crossing.text, crossing.textColor);
    dribbling.box.texture = renderText(r, dribbling.font, dribbling.text, dribbling.textColor);
    finishing.box.texture = renderText(r, finishing.font, finishing.text, finishing.textColor);
    firstTouch.box.texture = renderText(r, firstTouch.font, firstTouch.text, firstTouch.textColor);
    freeKickTaking.box.texture = renderText(r, freeKickTaking.font, freeKickTaking.text, freeKickTaking.textColor);
    heading.box.texture = renderText(r, heading.font, heading.text, heading.textColor);
    longShot.box.texture = renderText(r, longShot.font, longShot.text, longShot.textColor);
    longThrows.box.texture = renderText(r, longThrows.font, longThrows.text, longThrows.textColor);
    marking.box.texture = renderText(r, marking.font, marking.text, marking.textColor);
    passing.box.texture = renderText(r, passing.font, passing.text, passing.textColor);
    penaltyTaking.box.texture = renderText(r, penaltyTaking.font, penaltyTaking.text, penaltyTaking.textColor);
    tackling.box.texture = renderText(r, tackling.font, tackling.text, tackling.textColor);
    technique.box.texture = renderText(r, technique.font, technique.text, technique.textColor);
    aggression.box.texture = renderText(r, aggression.font, aggression.text, aggression.textColor);
    anticipation.box.texture = renderText(r, anticipation.font, anticipation.text, anticipation.textColor);
    bravery.box.texture = renderText(r, bravery.font, bravery.text, bravery.textColor);
    composure.box.texture = renderText(r, composure.font, composure.text, composure.textColor);
    concentration.box.texture = renderText(r, concentration.font, concentration.text, concentration.textColor);
    vision.box.texture = renderText(r, vision.font, vision.text, vision.textColor);
    decision.box.texture = renderText(r, decision.font, decision.text, decision.textColor);
    determination.box.texture = renderText(r, determination.font, determination.text, determination.textColor);
    flair.box.texture = renderText(r, flair.font, flair.text, flair.textColor);
    leadership.box.texture = renderText(r, leadership.font, leadership.text, leadership.textColor);
    offTheBall.box.texture = renderText(r, offTheBall.font, offTheBall.text, offTheBall.textColor);
    position.box.texture = renderText(r, position.font, position.text, position.textColor);
    teamwork.box.texture = renderText(r, teamwork.font, teamwork.text, teamwork.textColor);
    workRate.box.texture = renderText(r, workRate.font, workRate.text, workRate.textColor);
    acceleration.box.texture = renderText(r, acceleration.font, acceleration.text, acceleration.textColor);
    agility.box.texture = renderText(r, agility.font, agility.text, agility.textColor);
    balance.box.texture = renderText(r, balance.font, balance.text, balance.textColor);
    jumpingReach.box.texture = renderText(r, jumpingReach.font, jumpingReach.text, jumpingReach.textColor);
    naturalFitness.box.texture = renderText(r, naturalFitness.font, naturalFitness.text, naturalFitness.textColor);
    pace.box.texture = renderText(r, pace.font, pace.text, pace.textColor);
    stamina.box.texture = renderText(r, stamina.font, stamina.text, stamina.textColor);
    strength.box.texture = renderText(r, strength.font, strength.text, strength.textColor);
    stability.box.texture = renderText(r, stability.font, stability.text, stability.textColor);
    foul.box.texture = renderText(r, foul.font, foul.text, foul.textColor);
    contestPerformance.box.texture = renderText(r, contestPerformance.font, contestPerformance.text, contestPerformance.textColor);
    injury.box.texture = renderText(r, injury.font, injury.text, injury.textColor);
    diversity.box.texture = renderText(r, diversity.font, diversity.text, diversity.textColor);
    aerialReach.box.texture = renderText(r, aerialReach.font, aerialReach.text, aerialReach.textColor);
    commandOfArea.box.texture = renderText(r, commandOfArea.font, commandOfArea.text, commandOfArea.textColor);
    communication.box.texture = renderText(r, communication.font, communication.text, communication.textColor);
    eccentricity.box.texture = renderText(r, eccentricity.font, eccentricity.text, eccentricity.textColor);
    handling.box.texture = renderText(r, handling.font, handling.text, handling.textColor);
    kicking.box.texture = renderText(r, kicking.font, kicking.text, kicking.textColor);
    oneOnOnes.box.texture = renderText(r, oneOnOnes.font, oneOnOnes.text, oneOnOnes.textColor);
    reflexes.box.texture = renderText(r, reflexes.font, reflexes.text, reflexes.textColor);
    rushingOut.box.texture = renderText(r, rushingOut.font, rushingOut.text, rushingOut.textColor);
    throwing.box.texture = renderText(r, throwing.font, throwing.text, throwing.textColor);
    adaptation.box.texture = renderText(r, adaptation.font, adaptation.text, adaptation.textColor);
    ambition.box.texture = renderText(r, ambition.font, ambition.text, ambition.textColor);
    argue.box.texture = renderText(r, argue.font, argue.text, argue.textColor);
    loyal.box.texture = renderText(r, loyal.font, loyal.text, loyal.textColor);
    professional.box.texture = renderText(r, professional.font, professional.text, professional.textColor);
    sportmanship.box.texture = renderText(r, sportmanship.font, sportmanship.text, sportmanship.textColor);
    emotionalControl.box.texture = renderText(r, emotionalControl.font, emotionalControl.text, emotionalControl.textColor);
    punching.box.texture = renderText(r, punching.font, punching.text, punching.textColor);
    resistantToStress.box.texture = renderText(r, resistantToStress.font, resistantToStress.text, resistantToStress.textColor);

    // Position Boxes
    GK.box.texture = renderText(r, GK.font, GK.text, GK.textColor);
    DL.box.texture = renderText(r, DL.font, DL.text, DL.textColor);
    DC.box.texture = renderText(r, DC.font, DC.text, DC.textColor);
    DR.box.texture = renderText(r, DR.font, DR.text, DR.textColor);
    WBL.box.texture = renderText(r, WBL.font, WBL.text, WBL.textColor);
    WBR.box.texture = renderText(r, WBR.font, WBR.text, WBR.textColor);
    DM.box.texture = renderText(r, DM.font, DM.text, DM.textColor);
    ML.box.texture = renderText(r, ML.font, ML.text, ML.textColor);
    MC.box.texture = renderText(r, MC.font, MC.text, MC.textColor);
    MR.box.texture = renderText(r, MR.font, MR.text, MR.textColor);
    AML.box.texture = renderText(r, AML.font, AML.text, AML.textColor);
    AMC.box.texture = renderText(r, AMC.font, AMC.text, AMC.textColor);
    AMR.box.texture = renderText(r, AMR.font, AMR.text, AMR.textColor);
    ST.box.texture = renderText(r, ST.font, ST.text, ST.textColor);

    // Additional Data Fields

    name.box.texture = renderText(r, name.font, name.text, name.textColor);
    where_position.box.texture = renderText(r, where_position.font, where_position.text, where_position.textColor);
    age.box.texture = renderText(r, age.font, age.text, age.textColor);
    ca.box.texture = renderText(r, ca.font, ca.text, ca.textColor);
    pa.box.texture = renderText(r, pa.font, pa.text, pa.textColor);
    nationality.box.texture = renderText(r, nationality.font, nationality.text, nationality.textColor);
    club.box.texture = renderText(r, club.font, club.text, club.textColor);
    height.box.texture = renderText(r, height.font, height.text, height.textColor);
    weight.box.texture = renderText(r, weight.font, weight.text, weight.textColor);
    left.box.texture = renderText(r, left.font, left.text, left.textColor);
    right.box.texture = renderText(r, right.font, right.text, right.textColor);
    values.box.texture = renderText(r, values.font, values.text, values.textColor);
    currentReputation.box.texture = renderText(r, currentReputation.font, currentReputation.text, currentReputation.textColor);
    domesticReputation.box.texture = renderText(r, domesticReputation.font, domesticReputation.text, domesticReputation.textColor);
    worldReputation.box.texture = renderText(r, worldReputation.font, worldReputation.text, worldReputation.textColor);
    RCAB.box.texture = renderText(r, RCAB.font, RCAB.text, RCAB.textColor);
    birth.box.texture = renderText(r, birth.font, birth.text, birth.textColor);
    numberOfNationalTeamAppearance.box.texture = renderText(r, numberOfNationalTeamAppearance.font, numberOfNationalTeamAppearance.text, numberOfNationalTeamAppearance.textColor);
    goalsScoredForNationalTeam.box.texture = renderText(r, goalsScoredForNationalTeam.font, goalsScoredForNationalTeam.text, goalsScoredForNationalTeam.textColor);
    salary.box.texture = renderText(r, salary.font, salary.text, salary.textColor);
    UID.box.texture = renderText(r, UID.font, UID.text, UID.textColor);


    prevButton.box.texture = renderText(r,prevButton.font,prevButton.text,prevButton.textColor);
    nextButton.box.texture = renderText(r, nextButton.font, nextButton.text,nextButton.textColor);
    homeButton.box.texture = renderText(r, homeButton.font, homeButton.text, homeButton.textColor);

    bool quit = false;
    while(!quit) {
        while(SDL_PollEvent(e)) {
            if (e->type == SDL_KEYDOWN) {
                if (e->key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                    *home_quit = true;
                }
            }
            else if (e->type == SDL_QUIT) {
                quit = true;
                *home_quit = true;
            }
            else if (e->type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (isMouseOverButton(homeButton, x, y)) {
                    quit = true;
                    *home_quit = false;
                }
                else if (isMouseOverButton(prevButton, x, y)) {
                   
                    current = current->prev;

                    IdBox.text = to_char(current->id);
                    IdBox.box.texture = renderText(r, IdBox.font, IdBox.text, IdBox.textColor);

                    corners.text = to_char(current->player.ability_stat.corners);
                    corners.box.texture = renderText(r, corners.font, corners.text, corners.textColor);

                    marking.text = to_char(current->player.ability_stat.marking);
                    marking.box.texture = renderText(r, marking.font, marking.text, marking.textColor);

                    crossing.text = to_char(current->player.ability_stat.crossing);
                    crossing.box.texture = renderText(r, crossing.font, crossing.text, crossing.textColor);

                    dribbling.text = to_char(current->player.ability_stat.dribbling);
                    dribbling.box.texture = renderText(r, dribbling.font, dribbling.text, dribbling.textColor);

                    finishing.text = to_char(current->player.ability_stat.finishing);
                    finishing.box.texture = renderText(r, finishing.font, finishing.text, finishing.textColor);

                    firstTouch.text = to_char(current->player.ability_stat.first_touch);
                    firstTouch.box.texture = renderText(r, firstTouch.font, firstTouch.text, firstTouch.textColor);

                    freeKickTaking.text = to_char(current->player.ability_stat.free_kick_taking);
                    freeKickTaking.box.texture = renderText(r, freeKickTaking.font, freeKickTaking.text, freeKickTaking.textColor);

                    heading.text = to_char(current->player.ability_stat.heading);
                    heading.box.texture = renderText(r, heading.font, heading.text, heading.textColor);

                    longShot.text = to_char(current->player.ability_stat.long_shot);
                    longShot.box.texture = renderText(r, longShot.font, longShot.text, longShot.textColor);

                    longThrows.text = to_char(current->player.ability_stat.long_throws);
                    longThrows.box.texture = renderText(r, longThrows.font, longThrows.text, longThrows.textColor);

                    passing.text = to_char(current->player.ability_stat.passing);
                    passing.box.texture = renderText(r, passing.font, passing.text, passing.textColor);

                    penaltyTaking.text = to_char(current->player.ability_stat.penalty_taking);
                    penaltyTaking.box.texture = renderText(r, penaltyTaking.font, penaltyTaking.text, penaltyTaking.textColor);

                    tackling.text = to_char(current->player.ability_stat.tackling);
                    tackling.box.texture = renderText(r, tackling.font, tackling.text, tackling.textColor);

                    technique.text = to_char(current->player.ability_stat.technique);
                    technique.box.texture = renderText(r, technique.font, technique.text, technique.textColor);

                    aggression.text = to_char(current->player.ability_stat.aggressiion);
                    aggression.box.texture = renderText(r, aggression.font, aggression.text, aggression.textColor);

                    anticipation.text = to_char(current->player.ability_stat.anticipation);
                    anticipation.box.texture = renderText(r, anticipation.font, anticipation.text, anticipation.textColor);

                    bravery.text = to_char(current->player.ability_stat.bravery);
                    bravery.box.texture = renderText(r, bravery.font, bravery.text, bravery.textColor);

                    composure.text = to_char(current->player.ability_stat.composure);
                    composure.box.texture = renderText(r, composure.font, composure.text, composure.textColor);

                    concentration.text = to_char(current->player.ability_stat.concentration);
                    concentration.box.texture = renderText(r, concentration.font, concentration.text, concentration.textColor);

                    vision.text = to_char(current->player.ability_stat.vision);
                    vision.box.texture = renderText(r, vision.font, vision.text, vision.textColor);

                    decision.text = to_char(current->player.ability_stat.decision);
                    decision.box.texture = renderText(r, decision.font, decision.text, decision.textColor);

                    determination.text = to_char(current->player.ability_stat.determination);
                    determination.box.texture = renderText(r, determination.font, determination.text, determination.textColor);

                    flair.text = to_char(current->player.ability_stat.flair);
                    flair.box.texture = renderText(r, flair.font, flair.text, flair.textColor);

                    leadership.text = to_char(current->player.ability_stat.leadership);
                    leadership.box.texture = renderText(r, leadership.font, leadership.text, leadership.textColor);

                    offTheBall.text = to_char(current->player.ability_stat.off_the_ball);
                    offTheBall.box.texture = renderText(r, offTheBall.font, offTheBall.text, offTheBall.textColor);

                    position.text = to_char(current->player.ability_stat.position);
                    position.box.texture = renderText(r, position.font, position.text, position.textColor);

                    teamwork.text = to_char(current->player.ability_stat.teamwork);
                    teamwork.box.texture = renderText(r, teamwork.font, teamwork.text, teamwork.textColor);

                    workRate.text = to_char(current->player.ability_stat.work_rate);
                    workRate.box.texture = renderText(r, workRate.font, workRate.text, workRate.textColor);

                    acceleration.text = to_char(current->player.ability_stat.acceleration);
                    acceleration.box.texture = renderText(r, acceleration.font, acceleration.text, acceleration.textColor);

                    agility.text = to_char(current->player.ability_stat.agility);
                    agility.box.texture = renderText(r, agility.font, agility.text, agility.textColor);

                    balance.text = to_char(current->player.ability_stat.balance);
                    balance.box.texture = renderText(r, balance.font, balance.text, balance.textColor);

                    jumpingReach.text = to_char(current->player.ability_stat.jumping_reach);
                    jumpingReach.box.texture = renderText(r, jumpingReach.font, jumpingReach.text, jumpingReach.textColor);

                    naturalFitness.text = to_char(current->player.ability_stat.natural_fitness);
                    naturalFitness.box.texture = renderText(r, naturalFitness.font, naturalFitness.text, naturalFitness.textColor);

                    pace.text = to_char(current->player.ability_stat.pace);
                    pace.box.texture = renderText(r, pace.font, pace.text, pace.textColor);

                    stamina.text = to_char(current->player.ability_stat.stamina);
                    stamina.box.texture = renderText(r, stamina.font, stamina.text, stamina.textColor);

                    strength.text = to_char(current->player.ability_stat.strength);
                    strength.box.texture = renderText(r, strength.font, strength.text, strength.textColor);

                    stability.text = to_char(current->player.ability_stat.stability);
                    stability.box.texture = renderText(r, stability.font, stability.text, stability.textColor);

                    foul.text = to_char(current->player.ability_stat.foul);
                    foul.box.texture = renderText(r, foul.font, foul.text, foul.textColor);

                    contestPerformance.text = to_char(current->player.ability_stat.contest_performance);
                    contestPerformance.box.texture = renderText(r, contestPerformance.font, contestPerformance.text, contestPerformance.textColor);

                    injury.text = to_char(current->player.ability_stat.injury);
                    injury.box.texture = renderText(r, injury.font, injury.text, injury.textColor);

                    diversity.text = to_char(current->player.ability_stat.diversity);
                    diversity.box.texture = renderText(r, diversity.font, diversity.text, diversity.textColor);

                    aerialReach.text = to_char(current->player.ability_stat.aerial_reach);
                    aerialReach.box.texture = renderText(r, aerialReach.font, aerialReach.text, aerialReach.textColor);

                    commandOfArea.text = to_char(current->player.ability_stat.command_of_area);
                    commandOfArea.box.texture = renderText(r, commandOfArea.font, commandOfArea.text, commandOfArea.textColor);

                    communication.text = to_char(current->player.ability_stat.communication);
                    communication.box.texture = renderText(r, communication.font, communication.text, communication.textColor);

                    eccentricity.text = to_char(current->player.ability_stat.eccentricity);
                    eccentricity.box.texture = renderText(r, eccentricity.font, eccentricity.text, eccentricity.textColor);

                    handling.text = to_char(current->player.ability_stat.handling);
                    handling.box.texture = renderText(r, handling.font, handling.text, handling.textColor);

                    kicking.text = to_char(current->player.ability_stat.kicking);
                    kicking.box.texture = renderText(r, kicking.font, kicking.text, kicking.textColor);

                    oneOnOnes.text = to_char(current->player.ability_stat.one_on_ones);
                    oneOnOnes.box.texture = renderText(r, oneOnOnes.font, oneOnOnes.text, oneOnOnes.textColor);

                    reflexes.text = to_char(current->player.ability_stat.reflexes);
                    reflexes.box.texture = renderText(r, reflexes.font, reflexes.text, reflexes.textColor);

                    rushingOut.text = to_char(current->player.ability_stat.rushing_out);
                    rushingOut.box.texture = renderText(r, rushingOut.font, rushingOut.text, rushingOut.textColor);

                    throwing.text = to_char(current->player.ability_stat.throwing);
                    throwing.box.texture = renderText(r, throwing.font, throwing.text, throwing.textColor);

                    adaptation.text = to_char(current->player.ability_stat.adaptation);
                    adaptation.box.texture = renderText(r, adaptation.font, adaptation.text, adaptation.textColor);

                    ambition.text = to_char(current->player.ability_stat.ambition);
                    ambition.box.texture = renderText(r, ambition.font, ambition.text, ambition.textColor);

                    argue.text = to_char(current->player.ability_stat.argue);
                    argue.box.texture = renderText(r, argue.font, argue.text, argue.textColor);

                    loyal.text = to_char(current->player.ability_stat.loyal);
                    loyal.box.texture = renderText(r, loyal.font, loyal.text, loyal.textColor);

                    professional.text = to_char(current->player.ability_stat.professional);
                    professional.box.texture = renderText(r, professional.font, professional.text, professional.textColor);

                    sportmanship.text = to_char(current->player.ability_stat.sportmanship);
                    sportmanship.box.texture = renderText(r, sportmanship.font, sportmanship.text, sportmanship.textColor);

                    emotionalControl.text = to_char(current->player.ability_stat.emotional_control);
                    emotionalControl.box.texture = renderText(r, emotionalControl.font, emotionalControl.text, emotionalControl.textColor);

                    punching.text = to_char(current->player.ability_stat.punching);
                    punching.box.texture = renderText(r, punching.font, punching.text, punching.textColor);

                    resistantToStress.text = to_char(current->player.ability_stat.resistant_to_stress);
                    resistantToStress.box.texture = renderText(r, resistantToStress.font, resistantToStress.text, resistantToStress.textColor);

                    // Position Boxes
                    GK.text = to_char(current->player.position_stat.GK);
                    GK.box.texture = renderText(r, GK.font, GK.text, GK.textColor);

                    DL.text = to_char(current->player.position_stat.DL);
                    DL.box.texture = renderText(r, DL.font, DL.text, DL.textColor);

                    DC.text = to_char(current->player.position_stat.DC);
                    DC.box.texture = renderText(r, DC.font, DC.text, DC.textColor);

                    DR.text = to_char(current->player.position_stat.DR);
                    DR.box.texture = renderText(r, DR.font, DR.text, DR.textColor);

                    WBL.text = to_char(current->player.position_stat.WBL);
                    WBL.box.texture = renderText(r, WBL.font, WBL.text, WBL.textColor);

                    WBR.text = to_char(current->player.position_stat.WBR);
                    WBR.box.texture = renderText(r, WBR.font, WBR.text, WBR.textColor);

                    DM.text = to_char(current->player.position_stat.DM);
                    DM.box.texture = renderText(r, DM.font, DM.text, DM.textColor);

                    ML.text = to_char(current->player.position_stat.ML);
                    ML.box.texture = renderText(r, ML.font, ML.text, ML.textColor);

                    MC.text = to_char(current->player.position_stat.MC);
                    MC.box.texture = renderText(r, MC.font, MC.text, MC.textColor);

                    MR.text = to_char(current->player.position_stat.MR);
                    MR.box.texture = renderText(r, MR.font, MR.text, MR.textColor);

                    AML.text = to_char(current->player.position_stat.AML);
                    AML.box.texture = renderText(r, AML.font, AML.text, AML.textColor);

                    AMC.text = to_char(current->player.position_stat.AMC);
                    AMC.box.texture = renderText(r, AMC.font, AMC.text, AMC.textColor);

                    AMR.text = to_char(current->player.position_stat.AMR);
                    AMR.box.texture = renderText(r, AMR.font, AMR.text, AMR.textColor);

                    ST.text = to_char(current->player.position_stat.ST);
                    ST.box.texture = renderText(r, ST.font, ST.text, ST.textColor);

                    // Additional Data Fields
                    name.text = current->player.name;
                    name.box.texture = renderText(r, name.font, name.text, name.textColor);

                    where_position.text = current->player.position;
                    where_position.box.texture = renderText(r, where_position.font, where_position.text, where_position.textColor);

                    age.text = to_char(current->player.age);
                    age.box.texture = renderText(r, age.font, age.text, age.textColor);

                    ca.text = to_char(current->player.ca);
                    ca.box.texture = renderText(r, ca.font, ca.text, ca.textColor);

                    pa.text = to_char(current->player.pa);
                    pa.box.texture = renderText(r, pa.font, pa.text, pa.textColor);

                    nationality.text = current->player.nationality;
                    nationality.box.texture = renderText(r, nationality.font, nationality.text, nationality.textColor);

                    club.text = current->player.club;
                    club.box.texture = renderText(r, club.font, club.text, club.textColor);

                    height.text = to_char(current->player.body.height);
                    height.box.texture = renderText(r, height.font, height.text, height.textColor);

                    weight.text = to_char(current->player.body.weight);
                    weight.box.texture = renderText(r, weight.font, weight.text, weight.textColor);

                    left.text = to_char(current->player.foot_ability.left);
                    left.box.texture = renderText(r, left.font, left.text, left.textColor);

                    right.text = to_char(current->player.foot_ability.right);
                    right.box.texture = renderText(r, right.font, right.text, right.textColor);

                    values.text = to_char(current->player.values);
                    values.box.texture = renderText(r, values.font, values.text, values.textColor);

                    currentReputation.text = to_char(current->player.current_reputation);
                    currentReputation.box.texture = renderText(r, currentReputation.font, currentReputation.text, currentReputation.textColor);

                    domesticReputation.text = to_char(current->player.domestic_reputation);
                    domesticReputation.box.texture = renderText(r, domesticReputation.font, domesticReputation.text, domesticReputation.textColor);

                    worldReputation.text = to_char(current->player.world_reputation);
                    worldReputation.box.texture = renderText(r, worldReputation.font, worldReputation.text, worldReputation.textColor);

                    RCAB.text = to_char(current->player.RCA);
                    RCAB.box.texture = renderText(r, RCAB.font, RCAB.text, RCAB.textColor);

                    birth.text = current->player.birth;
                    birth.box.texture = renderText(r, birth.font, birth.text, birth.textColor);

                    numberOfNationalTeamAppearance.text = to_char(current->player.number_of_national_team_appearance);
                    numberOfNationalTeamAppearance.box.texture = renderText(r, numberOfNationalTeamAppearance.font, numberOfNationalTeamAppearance.text, numberOfNationalTeamAppearance.textColor);

                    goalsScoredForNationalTeam.text = to_char(current->player.goals_scored_for_the_national_team);
                    goalsScoredForNationalTeam.box.texture = renderText(r, goalsScoredForNationalTeam.font, goalsScoredForNationalTeam.text, goalsScoredForNationalTeam.textColor);

                    salary.text = to_char(current->player.salary);
                    salary.box.texture = renderText(r, salary.font, salary.text, salary.textColor);

                    UID.text = to_char(current->player.UID);
                    UID.box.texture = renderText(r, UID.font, UID.text, UID.textColor);

                }
                else if (isMouseOverButton(nextButton,x,y)) {
                    
                    current = current->next;

                    IdBox.text = to_char(current->id);
                    IdBox.box.texture = renderText(r, IdBox.font, IdBox.text, IdBox.textColor);

                    corners.text = to_char(current->player.ability_stat.corners);
                    corners.box.texture = renderText(r, corners.font, corners.text, corners.textColor);

                    marking.text = to_char(current->player.ability_stat.marking);
                    marking.box.texture = renderText(r, marking.font, marking.text, marking.textColor);

                    crossing.text = to_char(current->player.ability_stat.crossing);
                    crossing.box.texture = renderText(r, crossing.font, crossing.text, crossing.textColor);

                    dribbling.text = to_char(current->player.ability_stat.dribbling);
                    dribbling.box.texture = renderText(r, dribbling.font, dribbling.text, dribbling.textColor);

                    finishing.text = to_char(current->player.ability_stat.finishing);
                    finishing.box.texture = renderText(r, finishing.font, finishing.text, finishing.textColor);

                    firstTouch.text = to_char(current->player.ability_stat.first_touch);
                    firstTouch.box.texture = renderText(r, firstTouch.font, firstTouch.text, firstTouch.textColor);

                    freeKickTaking.text = to_char(current->player.ability_stat.free_kick_taking);
                    freeKickTaking.box.texture = renderText(r, freeKickTaking.font, freeKickTaking.text, freeKickTaking.textColor);

                    heading.text = to_char(current->player.ability_stat.heading);
                    heading.box.texture = renderText(r, heading.font, heading.text, heading.textColor);

                    longShot.text = to_char(current->player.ability_stat.long_shot);
                    longShot.box.texture = renderText(r, longShot.font, longShot.text, longShot.textColor);

                    longThrows.text = to_char(current->player.ability_stat.long_throws);
                    longThrows.box.texture = renderText(r, longThrows.font, longThrows.text, longThrows.textColor);

                    passing.text = to_char(current->player.ability_stat.passing);
                    passing.box.texture = renderText(r, passing.font, passing.text, passing.textColor);

                    penaltyTaking.text = to_char(current->player.ability_stat.penalty_taking);
                    penaltyTaking.box.texture = renderText(r, penaltyTaking.font, penaltyTaking.text, penaltyTaking.textColor);

                    tackling.text = to_char(current->player.ability_stat.tackling);
                    tackling.box.texture = renderText(r, tackling.font, tackling.text, tackling.textColor);

                    technique.text = to_char(current->player.ability_stat.technique);
                    technique.box.texture = renderText(r, technique.font, technique.text, technique.textColor);

                    aggression.text = to_char(current->player.ability_stat.aggressiion);
                    aggression.box.texture = renderText(r, aggression.font, aggression.text, aggression.textColor);

                    anticipation.text = to_char(current->player.ability_stat.anticipation);
                    anticipation.box.texture = renderText(r, anticipation.font, anticipation.text, anticipation.textColor);

                    bravery.text = to_char(current->player.ability_stat.bravery);
                    bravery.box.texture = renderText(r, bravery.font, bravery.text, bravery.textColor);

                    composure.text = to_char(current->player.ability_stat.composure);
                    composure.box.texture = renderText(r, composure.font, composure.text, composure.textColor);

                    concentration.text = to_char(current->player.ability_stat.concentration);
                    concentration.box.texture = renderText(r, concentration.font, concentration.text, concentration.textColor);

                    vision.text = to_char(current->player.ability_stat.vision);
                    vision.box.texture = renderText(r, vision.font, vision.text, vision.textColor);

                    decision.text = to_char(current->player.ability_stat.decision);
                    decision.box.texture = renderText(r, decision.font, decision.text, decision.textColor);

                    determination.text = to_char(current->player.ability_stat.determination);
                    determination.box.texture = renderText(r, determination.font, determination.text, determination.textColor);

                    flair.text = to_char(current->player.ability_stat.flair);
                    flair.box.texture = renderText(r, flair.font, flair.text, flair.textColor);

                    leadership.text = to_char(current->player.ability_stat.leadership);
                    leadership.box.texture = renderText(r, leadership.font, leadership.text, leadership.textColor);

                    offTheBall.text = to_char(current->player.ability_stat.off_the_ball);
                    offTheBall.box.texture = renderText(r, offTheBall.font, offTheBall.text, offTheBall.textColor);

                    position.text = to_char(current->player.ability_stat.position);
                    position.box.texture = renderText(r, position.font, position.text, position.textColor);

                    teamwork.text = to_char(current->player.ability_stat.teamwork);
                    teamwork.box.texture = renderText(r, teamwork.font, teamwork.text, teamwork.textColor);

                    workRate.text = to_char(current->player.ability_stat.work_rate);
                    workRate.box.texture = renderText(r, workRate.font, workRate.text, workRate.textColor);

                    acceleration.text = to_char(current->player.ability_stat.acceleration);
                    acceleration.box.texture = renderText(r, acceleration.font, acceleration.text, acceleration.textColor);

                    agility.text = to_char(current->player.ability_stat.agility);
                    agility.box.texture = renderText(r, agility.font, agility.text, agility.textColor);

                    balance.text = to_char(current->player.ability_stat.balance);
                    balance.box.texture = renderText(r, balance.font, balance.text, balance.textColor);

                    jumpingReach.text = to_char(current->player.ability_stat.jumping_reach);
                    jumpingReach.box.texture = renderText(r, jumpingReach.font, jumpingReach.text, jumpingReach.textColor);

                    naturalFitness.text = to_char(current->player.ability_stat.natural_fitness);
                    naturalFitness.box.texture = renderText(r, naturalFitness.font, naturalFitness.text, naturalFitness.textColor);

                    pace.text = to_char(current->player.ability_stat.pace);
                    pace.box.texture = renderText(r, pace.font, pace.text, pace.textColor);

                    stamina.text = to_char(current->player.ability_stat.stamina);
                    stamina.box.texture = renderText(r, stamina.font, stamina.text, stamina.textColor);

                    strength.text = to_char(current->player.ability_stat.strength);
                    strength.box.texture = renderText(r, strength.font, strength.text, strength.textColor);

                    stability.text = to_char(current->player.ability_stat.stability);
                    stability.box.texture = renderText(r, stability.font, stability.text, stability.textColor);

                    foul.text = to_char(current->player.ability_stat.foul);
                    foul.box.texture = renderText(r, foul.font, foul.text, foul.textColor);

                    contestPerformance.text = to_char(current->player.ability_stat.contest_performance);
                    contestPerformance.box.texture = renderText(r, contestPerformance.font, contestPerformance.text, contestPerformance.textColor);

                    injury.text = to_char(current->player.ability_stat.injury);
                    injury.box.texture = renderText(r, injury.font, injury.text, injury.textColor);

                    diversity.text = to_char(current->player.ability_stat.diversity);
                    diversity.box.texture = renderText(r, diversity.font, diversity.text, diversity.textColor);

                    aerialReach.text = to_char(current->player.ability_stat.aerial_reach);
                    aerialReach.box.texture = renderText(r, aerialReach.font, aerialReach.text, aerialReach.textColor);

                    commandOfArea.text = to_char(current->player.ability_stat.command_of_area);
                    commandOfArea.box.texture = renderText(r, commandOfArea.font, commandOfArea.text, commandOfArea.textColor);

                    communication.text = to_char(current->player.ability_stat.communication);
                    communication.box.texture = renderText(r, communication.font, communication.text, communication.textColor);

                    eccentricity.text = to_char(current->player.ability_stat.eccentricity);
                    eccentricity.box.texture = renderText(r, eccentricity.font, eccentricity.text, eccentricity.textColor);

                    handling.text = to_char(current->player.ability_stat.handling);
                    handling.box.texture = renderText(r, handling.font, handling.text, handling.textColor);

                    kicking.text = to_char(current->player.ability_stat.kicking);
                    kicking.box.texture = renderText(r, kicking.font, kicking.text, kicking.textColor);

                    oneOnOnes.text = to_char(current->player.ability_stat.one_on_ones);
                    oneOnOnes.box.texture = renderText(r, oneOnOnes.font, oneOnOnes.text, oneOnOnes.textColor);

                    reflexes.text = to_char(current->player.ability_stat.reflexes);
                    reflexes.box.texture = renderText(r, reflexes.font, reflexes.text, reflexes.textColor);

                    rushingOut.text = to_char(current->player.ability_stat.rushing_out);
                    rushingOut.box.texture = renderText(r, rushingOut.font, rushingOut.text, rushingOut.textColor);

                    throwing.text = to_char(current->player.ability_stat.throwing);
                    throwing.box.texture = renderText(r, throwing.font, throwing.text, throwing.textColor);

                    adaptation.text = to_char(current->player.ability_stat.adaptation);
                    adaptation.box.texture = renderText(r, adaptation.font, adaptation.text, adaptation.textColor);

                    ambition.text = to_char(current->player.ability_stat.ambition);
                    ambition.box.texture = renderText(r, ambition.font, ambition.text, ambition.textColor);

                    argue.text = to_char(current->player.ability_stat.argue);
                    argue.box.texture = renderText(r, argue.font, argue.text, argue.textColor);

                    loyal.text = to_char(current->player.ability_stat.loyal);
                    loyal.box.texture = renderText(r, loyal.font, loyal.text, loyal.textColor);

                    professional.text = to_char(current->player.ability_stat.professional);
                    professional.box.texture = renderText(r, professional.font, professional.text, professional.textColor);

                    sportmanship.text = to_char(current->player.ability_stat.sportmanship);
                    sportmanship.box.texture = renderText(r, sportmanship.font, sportmanship.text, sportmanship.textColor);

                    emotionalControl.text = to_char(current->player.ability_stat.emotional_control);
                    emotionalControl.box.texture = renderText(r, emotionalControl.font, emotionalControl.text, emotionalControl.textColor);

                    punching.text = to_char(current->player.ability_stat.punching);
                    punching.box.texture = renderText(r, punching.font, punching.text, punching.textColor);

                    resistantToStress.text = to_char(current->player.ability_stat.resistant_to_stress);
                    resistantToStress.box.texture = renderText(r, resistantToStress.font, resistantToStress.text, resistantToStress.textColor);

                    // Position Boxes
                    GK.text = to_char(current->player.position_stat.GK);
                    GK.box.texture = renderText(r, GK.font, GK.text, GK.textColor);

                    DL.text = to_char(current->player.position_stat.DL);
                    DL.box.texture = renderText(r, DL.font, DL.text, DL.textColor);

                    DC.text = to_char(current->player.position_stat.DC);
                    DC.box.texture = renderText(r, DC.font, DC.text, DC.textColor);

                    DR.text = to_char(current->player.position_stat.DR);
                    DR.box.texture = renderText(r, DR.font, DR.text, DR.textColor);

                    WBL.text = to_char(current->player.position_stat.WBL);
                    WBL.box.texture = renderText(r, WBL.font, WBL.text, WBL.textColor);

                    WBR.text = to_char(current->player.position_stat.WBR);
                    WBR.box.texture = renderText(r, WBR.font, WBR.text, WBR.textColor);

                    DM.text = to_char(current->player.position_stat.DM);
                    DM.box.texture = renderText(r, DM.font, DM.text, DM.textColor);

                    ML.text = to_char(current->player.position_stat.ML);
                    ML.box.texture = renderText(r, ML.font, ML.text, ML.textColor);

                    MC.text = to_char(current->player.position_stat.MC);
                    MC.box.texture = renderText(r, MC.font, MC.text, MC.textColor);

                    MR.text = to_char(current->player.position_stat.MR);
                    MR.box.texture = renderText(r, MR.font, MR.text, MR.textColor);

                    AML.text = to_char(current->player.position_stat.AML);
                    AML.box.texture = renderText(r, AML.font, AML.text, AML.textColor);

                    AMC.text = to_char(current->player.position_stat.AMC);
                    AMC.box.texture = renderText(r, AMC.font, AMC.text, AMC.textColor);

                    AMR.text = to_char(current->player.position_stat.AMR);
                    AMR.box.texture = renderText(r, AMR.font, AMR.text, AMR.textColor);

                    ST.text = to_char(current->player.position_stat.ST);
                    ST.box.texture = renderText(r, ST.font, ST.text, ST.textColor);

                    // Additional Data Fields
                    name.text = current->player.name;
                    name.box.texture = renderText(r, name.font, name.text, name.textColor);

                    where_position.text = current->player.position;
                    where_position.box.texture = renderText(r, where_position.font, where_position.text, where_position.textColor);

                    age.text = to_char(current->player.age);
                    age.box.texture = renderText(r, age.font, age.text, age.textColor);

                    ca.text = to_char(current->player.ca);
                    ca.box.texture = renderText(r, ca.font, ca.text, ca.textColor);

                    pa.text = to_char(current->player.pa);
                    pa.box.texture = renderText(r, pa.font, pa.text, pa.textColor);

                    nationality.text = current->player.nationality;
                    nationality.box.texture = renderText(r, nationality.font, nationality.text, nationality.textColor);

                    club.text = current->player.club;
                    club.box.texture = renderText(r, club.font, club.text, club.textColor);

                    height.text = to_char(current->player.body.height);
                    height.box.texture = renderText(r, height.font, height.text, height.textColor);

                    weight.text = to_char(current->player.body.weight);
                    weight.box.texture = renderText(r, weight.font, weight.text, weight.textColor);

                    left.text = to_char(current->player.foot_ability.left);
                    left.box.texture = renderText(r, left.font, left.text, left.textColor);

                    right.text = to_char(current->player.foot_ability.right);
                    right.box.texture = renderText(r, right.font, right.text, right.textColor);

                    values.text = to_char(current->player.values);
                    values.box.texture = renderText(r, values.font, values.text, values.textColor);

                    currentReputation.text = to_char(current->player.current_reputation);
                    currentReputation.box.texture = renderText(r, currentReputation.font, currentReputation.text, currentReputation.textColor);

                    domesticReputation.text = to_char(current->player.domestic_reputation);
                    domesticReputation.box.texture = renderText(r, domesticReputation.font, domesticReputation.text, domesticReputation.textColor);

                    worldReputation.text = to_char(current->player.world_reputation);
                    worldReputation.box.texture = renderText(r, worldReputation.font, worldReputation.text, worldReputation.textColor);

                    RCAB.text = to_char(current->player.RCA);
                    RCAB.box.texture = renderText(r, RCAB.font, RCAB.text, RCAB.textColor);

                    birth.text = current->player.birth;
                    birth.box.texture = renderText(r, birth.font, birth.text, birth.textColor);

                    numberOfNationalTeamAppearance.text = to_char(current->player.number_of_national_team_appearance);
                    numberOfNationalTeamAppearance.box.texture = renderText(r, numberOfNationalTeamAppearance.font, numberOfNationalTeamAppearance.text, numberOfNationalTeamAppearance.textColor);

                    goalsScoredForNationalTeam.text = to_char(current->player.goals_scored_for_the_national_team);
                    goalsScoredForNationalTeam.box.texture = renderText(r, goalsScoredForNationalTeam.font, goalsScoredForNationalTeam.text, goalsScoredForNationalTeam.textColor);

                    salary.text = to_char(current->player.salary);
                    salary.box.texture = renderText(r, salary.font, salary.text, salary.textColor);

                    UID.text = to_char(current->player.UID);
                    UID.box.texture = renderText(r, UID.font, UID.text, UID.textColor);
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
        if (isMouseOverButton(homeButton, x, y)) {
            homeButton.box.background_color = button_hover_color;
        } else {
            homeButton.box.background_color = button_color;
        }

        SDL_SetRenderDrawColor(r, 255,255,255,255);
        SDL_RenderClear(r);
        SDL_RenderCopy(r, texture_football, NULL, &football_img);
        renderBox(r,data_box.rect,data_box.background_color);
        renderBox(r,blue_box.rect,blue_box.background_color);
        renderButton(r, cornersBox); renderButton(r, crossingBox); renderButton(r, dribblingBox); renderButton(r, finishingBox);
        renderButton(r, firstTouchBox); renderButton(r, freeKickTakingBox); renderButton(r, headingBox); renderButton(r, longShotBox); renderButton(r, longThrowsBox);
        renderButton(r, markingBox); renderButton(r, passingBox); renderButton(r, penaltyTakingBox); renderButton(r, tacklingBox); renderButton(r, techniqueBox);
        renderButton(r, aggressionBox); renderButton(r, anticipationBox); renderButton(r, braveryBox); renderButton(r, composureBox); renderButton(r, concentrationBox);
        renderButton(r, visionBox); renderButton(r, decisionBox); renderButton(r, determinationBox); renderButton(r, flairBox); renderButton(r, leadershipBox);
        renderButton(r, offTheBallBox); renderButton(r, positionBox); renderButton(r, teamworkBox); renderButton(r, workRateBox); renderButton(r, accelerationBox);
        renderButton(r, agilityBox); renderButton(r, balanceBox); renderButton(r, jumpingReachBox); renderButton(r, naturalFitnessBox); renderButton(r, paceBox);
        renderButton(r, staminaBox); renderButton(r, strengthBox); renderButton(r, stabilityBox); renderButton(r, foulBox); renderButton(r, contestPerformanceBox);
        renderButton(r, injuryBox); renderButton(r, diversityBox); renderButton(r, aerialReachBox); renderButton(r, commandOfAreaBox);renderButton(r, communicationBox);
        renderButton(r, eccentricityBox); renderButton(r, handlingBox); renderButton(r, kickingBox); renderButton(r, oneOnOnesBox); renderButton(r, reflexesBox);
        renderButton(r, rushingOutBox); renderButton(r, throwingBox); renderButton(r, adaptationBox); renderButton(r, ambitionBox); renderButton(r, argueBox);
        renderButton(r, loyalBox); renderButton(r, professionalBox); renderButton(r, sportmanshipBox); renderButton(r, emotionalControlBox); renderButton(r, punchingBox);
        renderButton(r, resistantToStressBox);

        // Position Boxes
        renderButton(r, GKBox); renderButton(r, DLBox); renderButton(r, DCBox); renderButton(r, DRBox); renderButton(r, WBLBox);
        renderButton(r, WBRBox); renderButton(r, DMBox); renderButton(r, MLBox); renderButton(r, MCBox); renderButton(r, MRBox);
        renderButton(r, AMLBox); renderButton(r, AMCBox); renderButton(r, AMRBox); renderButton(r, STBox);

        // Additional Data Fields
        renderButton(r,IdBox);
        renderButton(r, numIdBox); renderButton(r, nameBox); renderButton(r, where_positionBox); renderButton(r, ageBox); renderButton(r, caBox);
        renderButton(r, paBox); renderButton(r, nationalityBox); renderButton(r, clubBox); renderButton(r, heightBox); renderButton(r, weightBox);
        renderButton(r, leftBox); renderButton(r, rightBox); renderButton(r, valuesBox); renderButton(r, currentReputationBox); renderButton(r, domesticReputationBox);
        renderButton(r, worldReputationBox); renderButton(r, RCABBox); renderButton(r, birthBox); renderButton(r, numberOfNationalTeamAppearanceBox); renderButton(r, goalsScoredForNationalTeamBox);
        renderButton(r, salaryBox); renderButton(r, UIDBox);


        renderButton(r, corners); renderButton(r, crossing); renderButton(r, dribbling); renderButton(r, finishing);
        renderButton(r, firstTouch); renderButton(r, freeKickTaking); renderButton(r, heading); renderButton(r, longShot); renderButton(r, longThrows);
        renderButton(r, marking); renderButton(r, passing); renderButton(r, penaltyTaking); renderButton(r, tackling); renderButton(r, technique);
        renderButton(r, aggression); renderButton(r, anticipation); renderButton(r, bravery); renderButton(r, composure); renderButton(r, concentration);
        renderButton(r, vision); renderButton(r, decision); renderButton(r, determination); renderButton(r, flair); renderButton(r, leadership);
        renderButton(r, offTheBall); renderButton(r, position); renderButton(r, teamwork); renderButton(r, workRate); renderButton(r, acceleration);
        renderButton(r, agility); renderButton(r, balance); renderButton(r, jumpingReach); renderButton(r, naturalFitness); renderButton(r, pace);
        renderButton(r, stamina); renderButton(r, strength); renderButton(r, stability); renderButton(r, foul); renderButton(r, contestPerformance);
        renderButton(r, injury); renderButton(r, diversity); renderButton(r, aerialReach); renderButton(r, commandOfArea);renderButton(r, communication);
        renderButton(r, eccentricity); renderButton(r, handling); renderButton(r, kicking); renderButton(r, oneOnOnes); renderButton(r, reflexes);
        renderButton(r, rushingOut); renderButton(r, throwing); renderButton(r, adaptation); renderButton(r, ambition); renderButton(r, argue);
        renderButton(r, loyal); renderButton(r, professional); renderButton(r, sportmanship); renderButton(r, emotionalControl); renderButton(r, punching);
        renderButton(r, resistantToStress);

        // Position es
        renderButton(r, GK); renderButton(r, DL); renderButton(r, DC); renderButton(r, DR); renderButton(r, WBL);
        renderButton(r, WBR); renderButton(r, DM); renderButton(r, ML); renderButton(r, MC); renderButton(r, MR);
        renderButton(r, AML); renderButton(r, AMC); renderButton(r, AMR); renderButton(r, ST);

        // Additional Data Fields
        renderButton(r, name); renderButton(r, where_position); renderButton(r, age); renderButton(r, ca);
        renderButton(r, pa); renderButton(r, nationality); renderButton(r, club); renderButton(r, height); renderButton(r, weight);
        renderButton(r, left); renderButton(r, right); renderButton(r, values); renderButton(r, currentReputation); renderButton(r, domesticReputation);
        renderButton(r, worldReputation); renderButton(r, RCAB); renderButton(r, birth); renderButton(r, numberOfNationalTeamAppearance); renderButton(r, goalsScoredForNationalTeam);
        renderButton(r, salary); renderButton(r, UID);

        renderButton(r, prevButton); renderButton(r,nextButton); renderButton(r, homeButton);

        SDL_RenderPresent(r);
    }
    SDL_DestroyTexture(cornersBox.box.texture); SDL_DestroyTexture(crossingBox.box.texture); SDL_DestroyTexture(dribblingBox.box.texture); SDL_DestroyTexture(finishingBox.box.texture);
    SDL_DestroyTexture(firstTouchBox.box.texture); SDL_DestroyTexture(freeKickTakingBox.box.texture); SDL_DestroyTexture(headingBox.box.texture); SDL_DestroyTexture(longShotBox.box.texture); SDL_DestroyTexture(longThrowsBox.box.texture);
    SDL_DestroyTexture(markingBox.box.texture); SDL_DestroyTexture(passingBox.box.texture); SDL_DestroyTexture(penaltyTakingBox.box.texture); SDL_DestroyTexture(tacklingBox.box.texture); SDL_DestroyTexture(techniqueBox.box.texture);
    SDL_DestroyTexture(aggressionBox.box.texture); SDL_DestroyTexture(anticipationBox.box.texture); SDL_DestroyTexture(braveryBox.box.texture); SDL_DestroyTexture(composureBox.box.texture); SDL_DestroyTexture(concentrationBox.box.texture);
    SDL_DestroyTexture(visionBox.box.texture); SDL_DestroyTexture(decisionBox.box.texture); SDL_DestroyTexture(determinationBox.box.texture); SDL_DestroyTexture(flairBox.box.texture); SDL_DestroyTexture(leadershipBox.box.texture);
    SDL_DestroyTexture(offTheBallBox.box.texture); SDL_DestroyTexture(positionBox.box.texture); SDL_DestroyTexture(teamworkBox.box.texture); SDL_DestroyTexture(workRateBox.box.texture); SDL_DestroyTexture(accelerationBox.box.texture);
    SDL_DestroyTexture(agilityBox.box.texture); SDL_DestroyTexture(balanceBox.box.texture); SDL_DestroyTexture(jumpingReachBox.box.texture); SDL_DestroyTexture(naturalFitnessBox.box.texture); SDL_DestroyTexture(paceBox.box.texture);
    SDL_DestroyTexture(staminaBox.box.texture); SDL_DestroyTexture(strengthBox.box.texture); SDL_DestroyTexture(stabilityBox.box.texture); SDL_DestroyTexture(foulBox.box.texture); SDL_DestroyTexture(contestPerformanceBox.box.texture);
    SDL_DestroyTexture(injuryBox.box.texture); SDL_DestroyTexture(diversityBox.box.texture); SDL_DestroyTexture(aerialReachBox.box.texture); SDL_DestroyTexture(commandOfAreaBox.box.texture); SDL_DestroyTexture(communicationBox.box.texture);
    SDL_DestroyTexture(eccentricityBox.box.texture); SDL_DestroyTexture(handlingBox.box.texture); SDL_DestroyTexture(kickingBox.box.texture); SDL_DestroyTexture(oneOnOnesBox.box.texture); SDL_DestroyTexture(reflexesBox.box.texture);
    SDL_DestroyTexture(rushingOutBox.box.texture); SDL_DestroyTexture(throwingBox.box.texture); SDL_DestroyTexture(adaptationBox.box.texture); SDL_DestroyTexture(ambitionBox.box.texture); SDL_DestroyTexture(argueBox.box.texture);
    SDL_DestroyTexture(loyalBox.box.texture); SDL_DestroyTexture(professionalBox.box.texture); SDL_DestroyTexture(sportmanshipBox.box.texture); SDL_DestroyTexture(emotionalControlBox.box.texture); SDL_DestroyTexture(punchingBox.box.texture);
    SDL_DestroyTexture(resistantToStressBox.box.texture);

    // Position Boxes
    SDL_DestroyTexture(GKBox.box.texture); SDL_DestroyTexture(DLBox.box.texture); SDL_DestroyTexture(DCBox.box.texture); SDL_DestroyTexture(DRBox.box.texture); SDL_DestroyTexture(WBLBox.box.texture);
    SDL_DestroyTexture(WBRBox.box.texture); SDL_DestroyTexture(DMBox.box.texture); SDL_DestroyTexture(MLBox.box.texture); SDL_DestroyTexture(MCBox.box.texture); SDL_DestroyTexture(MRBox.box.texture);
    SDL_DestroyTexture(AMLBox.box.texture); SDL_DestroyTexture(AMCBox.box.texture); SDL_DestroyTexture(AMRBox.box.texture); SDL_DestroyTexture(STBox.box.texture);

    // Additional Data Fields
    SDL_DestroyTexture(IdBox.box.texture);
    SDL_DestroyTexture(numIdBox.box.texture); SDL_DestroyTexture(nameBox.box.texture); SDL_DestroyTexture(where_positionBox.box.texture); SDL_DestroyTexture(ageBox.box.texture); SDL_DestroyTexture(caBox.box.texture);
    SDL_DestroyTexture(paBox.box.texture); SDL_DestroyTexture(nationalityBox.box.texture); SDL_DestroyTexture(clubBox.box.texture); SDL_DestroyTexture(heightBox.box.texture); SDL_DestroyTexture(weightBox.box.texture);
    SDL_DestroyTexture(leftBox.box.texture); SDL_DestroyTexture(rightBox.box.texture); SDL_DestroyTexture(valuesBox.box.texture); SDL_DestroyTexture(currentReputationBox.box.texture); SDL_DestroyTexture(domesticReputationBox.box.texture);
    SDL_DestroyTexture(worldReputationBox.box.texture); SDL_DestroyTexture(RCABBox.box.texture); SDL_DestroyTexture(birthBox.box.texture); SDL_DestroyTexture(numberOfNationalTeamAppearanceBox.box.texture); SDL_DestroyTexture(goalsScoredForNationalTeamBox.box.texture);
    SDL_DestroyTexture(salaryBox.box.texture); SDL_DestroyTexture(UIDBox.box.texture);

    SDL_DestroyTexture(corners.box.texture); SDL_DestroyTexture(crossing.box.texture); SDL_DestroyTexture(dribbling.box.texture); SDL_DestroyTexture(finishing.box.texture);
    SDL_DestroyTexture(firstTouch.box.texture); SDL_DestroyTexture(freeKickTaking.box.texture); SDL_DestroyTexture(heading.box.texture); SDL_DestroyTexture(longShot.box.texture); SDL_DestroyTexture(longThrows.box.texture);
    SDL_DestroyTexture(marking.box.texture); SDL_DestroyTexture(passing.box.texture); SDL_DestroyTexture(penaltyTaking.box.texture); SDL_DestroyTexture(tackling.box.texture); SDL_DestroyTexture(technique.box.texture);
    SDL_DestroyTexture(aggression.box.texture); SDL_DestroyTexture(anticipation.box.texture); SDL_DestroyTexture(bravery.box.texture); SDL_DestroyTexture(composure.box.texture); SDL_DestroyTexture(concentration.box.texture);
    SDL_DestroyTexture(vision.box.texture); SDL_DestroyTexture(decision.box.texture); SDL_DestroyTexture(determination.box.texture); SDL_DestroyTexture(flair.box.texture); SDL_DestroyTexture(leadership.box.texture);
    SDL_DestroyTexture(offTheBall.box.texture); SDL_DestroyTexture(position.box.texture); SDL_DestroyTexture(teamwork.box.texture); SDL_DestroyTexture(workRate.box.texture); SDL_DestroyTexture(acceleration.box.texture);
    SDL_DestroyTexture(agility.box.texture); SDL_DestroyTexture(balance.box.texture); SDL_DestroyTexture(jumpingReach.box.texture); SDL_DestroyTexture(naturalFitness.box.texture); SDL_DestroyTexture(pace.box.texture);
    SDL_DestroyTexture(stamina.box.texture); SDL_DestroyTexture(strength.box.texture); SDL_DestroyTexture(stability.box.texture); SDL_DestroyTexture(foul.box.texture); SDL_DestroyTexture(contestPerformance.box.texture);
    SDL_DestroyTexture(injury.box.texture); SDL_DestroyTexture(diversity.box.texture); SDL_DestroyTexture(aerialReach.box.texture); SDL_DestroyTexture(commandOfArea.box.texture); SDL_DestroyTexture(communication.box.texture);
    SDL_DestroyTexture(eccentricity.box.texture); SDL_DestroyTexture(handling.box.texture); SDL_DestroyTexture(kicking.box.texture); SDL_DestroyTexture(oneOnOnes.box.texture); SDL_DestroyTexture(reflexes.box.texture);
    SDL_DestroyTexture(rushingOut.box.texture); SDL_DestroyTexture(throwing.box.texture); SDL_DestroyTexture(adaptation.box.texture); SDL_DestroyTexture(ambition.box.texture); SDL_DestroyTexture(argue.box.texture);
    SDL_DestroyTexture(loyal.box.texture); SDL_DestroyTexture(professional.box.texture); SDL_DestroyTexture(sportmanship.box.texture); SDL_DestroyTexture(emotionalControl.box.texture); SDL_DestroyTexture(punching.box.texture);
    SDL_DestroyTexture(resistantToStress.box.texture);

    // Position Boxes
    SDL_DestroyTexture(GK.box.texture); SDL_DestroyTexture(DL.box.texture); SDL_DestroyTexture(DC.box.texture); SDL_DestroyTexture(DR.box.texture); SDL_DestroyTexture(WBL.box.texture);
    SDL_DestroyTexture(WBR.box.texture); SDL_DestroyTexture(DM.box.texture); SDL_DestroyTexture(ML.box.texture); SDL_DestroyTexture(MC.box.texture); SDL_DestroyTexture(MR.box.texture);
    SDL_DestroyTexture(AML.box.texture); SDL_DestroyTexture(AMC.box.texture); SDL_DestroyTexture(AMR.box.texture); SDL_DestroyTexture(ST.box.texture);

    // Additional Data Fields
    SDL_DestroyTexture(name.box.texture); SDL_DestroyTexture(where_position.box.texture); SDL_DestroyTexture(age.box.texture); SDL_DestroyTexture(ca.box.texture);
    SDL_DestroyTexture(pa.box.texture); SDL_DestroyTexture(nationality.box.texture); SDL_DestroyTexture(club.box.texture); SDL_DestroyTexture(height.box.texture); SDL_DestroyTexture(weight.box.texture);
    SDL_DestroyTexture(left.box.texture); SDL_DestroyTexture(right.box.texture); SDL_DestroyTexture(values.box.texture); SDL_DestroyTexture(currentReputation.box.texture); SDL_DestroyTexture(domesticReputation.box.texture);
    SDL_DestroyTexture(worldReputation.box.texture); SDL_DestroyTexture(RCAB.box.texture); SDL_DestroyTexture(birth.box.texture); SDL_DestroyTexture(numberOfNationalTeamAppearance.box.texture); SDL_DestroyTexture(goalsScoredForNationalTeam.box.texture);
    SDL_DestroyTexture(salary.box.texture); SDL_DestroyTexture(UID.box.texture);


}

// 메인 함수
int main() {
    int w = 1480;
    int h = 800;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        logSDLError("SDL_Init");
        return 1;
    }

    if (TTF_Init() == -1) {
        return 0;
    }
    SDL_Window* window= SDL_CreateWindow("FOOTBALL AGENCY", 10, 10, w, h, SDL_WINDOW_SHOWN);;
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
    initList(&player_list);

    
    int select;
    int what_choice;

    bool home_quit = false;

    int all_idCounter = 1;
    readCSV("FM2023.csv", &player_list, &all_idCounter);

    SDL_Event e;
    startMain(renderer,&e,&font_collection);
    while(!home_quit) {
        Circularlist search_list;
        initList(&search_list);
        int search_idCounter = 1;
        where_position_recommended_screen(renderer,&e,&font_collection, &select);
        select_position_algorithm(renderer, &e, &font_collection, &select, &what_choice);
        if (what_choice == 0) {
            printf("find strikers");
            findStrikers(&player_list, &search_list, &search_idCounter);
        }
        else if (what_choice == 1) {
            printf("find wingers");
            findWingers(&player_list,&search_list,&search_idCounter);
        }
        else if (what_choice == 2) {
            printf("find wide mid");
            findWideMidfielders(&player_list,&search_list,&search_idCounter);
        }
        else if (what_choice == 3) {
            printf("find central mid");
            findCentralMidfielders(&player_list,&search_list,&search_idCounter);
        }
        else if (what_choice == 4) {
            printf("find attack mid");
            findAttackingMidfielders(&player_list,&search_list,&search_idCounter);
        }
        else if (what_choice == 5) {
            printf("find defense mid");
            findDefensiveMidfielders(&player_list,&search_list,&search_idCounter);
        }
        else if (what_choice == 6) {
            printf("find full back");
            findFullbacks(&player_list, &search_list, &search_idCounter);
        }
        else if (what_choice == 7) {
            printf("find wing back");
            findWingBacks(&player_list,&search_list, &search_idCounter);
        }
        else if (what_choice == 8) {
            printf("find center back");
            findCenterBacks(&player_list, &search_list, &search_idCounter);
        }
        else if (what_choice == 9) {
            printf("find goalkeeper");
            findGoalkeepers(&player_list, &search_list, &search_idCounter);
        }
        player_data_screen(renderer,&e,&font_collection,&search_list,&search_idCounter, &home_quit);

    }
    
    
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
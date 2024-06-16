#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE_LENGTH 1024

typedef struct {
    int height;
    int weight;
} Body;

typedef struct 
{
    int left;
    int right;
} Foot_ability;

typedef struct {
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

typedef struct {
    //GK
    int GK;
    //Defender
    int DL; int DC; int DR; int WBL; int WBR;
    //Midfielder
    int DM; int ML; int MC; int MR; 
    //attacker
    int AML; int AMC; int AMR; int ST;
} Position_stat;

typedef struct {
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

Node* createNode(Player player) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->player = player;
    newNode->next = NULL;
    return newNode;
}

void appendNode(Node** head, Player player) {
    Node* newNode = createNode(player);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    Node* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = newNode;
    newNode->prev = temp;
    
}
//특정 UID를 가진 노드 삭제
void deleteNode(Node** head, int UID) {
    Node* temp = *head;

    while(temp != NULL && temp->player.UID != UID) {
        temp = temp->next;
    }
}
int readCSV_line(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        return -1;
    }

    int count = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        count++;
    }
    fclose(file);

    return count;
}

void readCSV(const char* filename, Player* players) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Unable to open file");
        return;
    }

    int player_num = 0;
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file)) {

        char* token = strtok(line, ",");
        if (token != NULL) strcpy(players[player_num].name, token);
        
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(players[player_num].position, token);

        token = strtok(NULL, ",");
        if (token != NULL) players[player_num].age = atoi(token);

        token = strtok(NULL, ",");
        if (token != NULL) players[player_num].ca = atoi(token);

        token = strtok(NULL,",");
        if (token != NULL) players[player_num].pa = atoi(token);

        token = strtok(NULL,",");
        if (token != NULL) strcpy(players[player_num].nationality, token);

        token = strtok(NULL,",");
        if (token != NULL) strcpy(players[player_num].club, token);
        //능력치 데이터
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.corners = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.crossing = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.dribbling = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.finishing = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.first_touch = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.free_kick_taking = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.heading = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.long_shot = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.long_throws = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.marking = atoi(token);
        //10
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.passing = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.penalty_taking = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.tackling =atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.technique = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.aggressiion = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.anticipation = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.bravery = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.composure = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.concentration = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.vision = atoi(token);
        //20
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.decision = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.determination = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.flair = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.leadership = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.off_the_ball = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.position = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.teamwork = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.work_rate = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.acceleration = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.agility = atoi(token);
        //30
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.balance = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.jumping_reach = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.natural_fitness = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.pace = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.stamina = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.strength = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.stability = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.foul = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.contest_performance = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.injury = atoi(token);
        //40
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.diversity = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.aerial_reach = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.command_of_area = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.communication = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.eccentricity = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.handling = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.kicking = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.one_on_ones = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.reflexes = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.rushing_out = atoi(token);
        //50
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.punching = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.throwing = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.adaptation = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.ambition = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.argue = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.loyal = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.resistant_to_stress = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.professional = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.sportmanship = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].abiltiy_stat.emotional_control = atoi(token);
        //60
        //포지션 스탯
        token = strtok(NULL,","); if (token != NULL) players[player_num].position_stat.GK = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].position_stat.DL = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].position_stat.DC = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].position_stat.DR = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].position_stat.WBL = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].position_stat.WBR = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].position_stat.DM = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].position_stat.ML = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].position_stat.MC = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].position_stat.MR = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].position_stat.AML = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].position_stat.AMC = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].position_stat.AMR = atoi(token);
        token = strtok(NULL,","); if (token != NULL) players[player_num].position_stat.ST = atoi(token);

        //키 몸무게
        token = strtok(NULL,",");
        if (token != NULL) players[player_num].body.height = atoi(token);
        token = strtok(NULL,",");
        if (token != NULL) players[player_num].body.weight = atoi(token);
        //발 능력치
        token = strtok(NULL,",");
        if (token != NULL) players[player_num].foot_ability.left = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) players[player_num].foot_ability.right = atoi(token);
        //선수 가치
        token = strtok(NULL, ",");
        if (token != NULL) players[player_num].values = atoi(token);
        //current reputation
        token = strtok(NULL, ",");
        if (token != NULL) players[player_num].current_reputation = atoi(token);
        //domestic reputation
        token = strtok(NULL, ",");
        if (token != NULL) players[player_num].domestic_reputation = atoi(token);
        //world reputation 
        token = strtok(NULL, ",");
        if (token != NULL) players[player_num].world_reputation = atoi(token);
        //RCA
        token = strtok(NULL, ",");
        if (token != NULL) players[player_num].RCA = atoi(token);
        //date of birth
        token = strtok(NULL, ",");
        if (token != NULL) strcpy(players[player_num].birth, token);
        //number of national team appearance
        token = strtok(NULL, ",");
        if (token != NULL) players[player_num].number_of_national_team_appearance = atoi(token);
        //goals scored for the national team
        token = strtok(NULL, ",");
        if (token != NULL) players[player_num].goals_scored_for_the_national_team = atoi(token);
        //salary
        token = strtok(NULL, ",");
        if (token != NULL) players[player_num].salary = atoi(token);
        //UID
        token = strtok(NULL, ",");
        if (token != NULL) players[player_num].UID = atoi(token);

        player_num++;
    }
    fclose(file);
}
int main() {
    int player_num = readCSV_line("FM2023.csv");

    printf("%d", player_num);
    Player* players = (Player*)malloc(player_num * sizeof(Player));
    if (players == NULL) {
        perror("Failed to allocate memory for buttons");
        exit(1);
    }
    readCSV("FM2023.csv", players);

    for (int i = 0; i < player_num; i++) {
        if (players[i].abiltiy_stat.balance >= 15) {
            printf("Name: %s Balance : %d UID : %d \n", players[i].name, players[i].abiltiy_stat.balance, players[i].UID);
        }
    }

    free(players);
    return 0;
}
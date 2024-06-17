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

    if (temp == NULL) return;

    if (temp->next != NULL) {
        //임시에 다음 것이 있다면 다음 노드의 이전 노드 주소를 지금 노드의 이전과 같게 해줘야함
        temp->next->prev = temp->prev;
    }
    if (temp->prev != NULL) {
        temp->prev->next = temp->next;
    }
    else {
        *head = temp->next;
    }
    free(temp);
}

void printList(Node* head) {
    Node* temp = head;
    if (temp == NULL) {
        printf("empty");
    }
    while(temp!=NULL) {
        printf("Name: %s, Position: %s, Age: %d, UID: %d\n", temp->player.name, temp->player.position, temp->player.age, temp->player.UID);
        temp = temp->next;
    }
}

int countNodes(Node* head) {
    int count = 0;
    Node* temp = head;
    while(temp!= NULL) {
        count++;
        temp = temp->next;
    }
    return count;
}

void readCSV(const char* filename, Node** node) {
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

        appendNode(node, player);
    }
    fclose(file);
}

void searchPlayer(Node* player_head, Node** search_head) {
    Node* temp = player_head;
    while(temp!= NULL) {
        if (temp->player.pa > 170) {
            //printf("Name: %s Position: %s Age: %d pa: %d\n", temp->player.name, temp->player.position, temp->player.age, temp->player.pa);
            appendNode(search_head,temp->player);
        }
        temp = temp->next;
    }
}

int main() {

    Node* player_head = NULL;
    Node* search_head = NULL;
    readCSV("FM2023.csv", &player_head);
    
    //printList(head);
    searchPlayer(player_head, &search_head);
    printList(search_head);
    
    return 0;
}
# Football Agency 프로그램

## 코드 설명

- Fonts 구조체 : font 글자 크기별 모음집

- Button 구조체 : Button에 필요한 변수명 모음

- Player 구조체
  (선수 신체 능력치) (선수 발 능력치) (선수 세부 스탯)
  - 이름
  - 포지션
  - 나이
  - 현재 능력치
  - 잠재 능력치
  - 국가
  - 소속 클럽
  - 선수 가치
  - 현재 평판
  - 국내 평판
  - 세계 평판
  - RCA
  - 생일
  - 국가대표 출전 횟수
  - 국가대표 득점 횟수
  - 급여
  - UID

- 이중 연결 리스트 순환 구조
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

- readCSV()
  엑셀 파일에서 선수 데이터 불러오기

- 선수 찾기
  findStrikers()
  findWingers()
  findWideMidfielders()
  findCentralMidfielders()
  findAttackingMidfielders()
  findDefensiveMidfielders()
  findFullbacks()
  findWingBacks()
  findCenterBacks()
  findGoalkeepers()

- startMain()
  초기 시작화면
- where_position_recommended_screen()
  포지션 선택
- select_position_algorithm()
  포지션 세부적인 알고리즘 선택
- player_data_screen()
  찾은 선수 화면 출력

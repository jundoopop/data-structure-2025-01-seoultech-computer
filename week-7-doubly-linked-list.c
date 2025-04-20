#include <stdio.h>
#include <stdlib.h>

// 연결 리스트를 구성하는 노드를 구조체로 정의
typedef struct Node {
  int data;
  struct Node *next;
  struct Node *prev;
} Node;

/**
 * @brief 새로운 노드를 생성함
 * @param data: 노드에 저장할 정수 값
 * @return 생성된 노드의 포인터
 */
Node *createNode(int data);

/**
 * @brief 리스트의 맨 앞에 노드를 삽입함
 * @param head: 리스트의 헤드(시작 노드)를 가리키는 포인터의 주소
 * @param data: 삽입할 정수 값
 */
void insertBegin(Node **head, int data);

/**
 * @brief 지정한 위치(0부터 시작)에 노드를 삽입함
 * @param head: 리스트의 헤드(시작 노드)를 가리키는 포인터의 주소
 * @param data: 삽입할 정수 값
 * @param index: 삽입할 위치(0부터 시작)
 * @return 성공 시 1, 실패(잘못된 인덱스) 시 0 반환
 */
int insertWhere(Node **head, int data, int index);

/**
 * @brief 리스트의 맨 뒤에 노드를 삽입함
 * @param head: 리스트의 헤드(시작 노드)를 가리키는 포인터의 주소
 * @param data: 삽입할 정수 값
 */
void insertEnd(Node **head, int data);

/**
 * @brief 리스트 전체를 처음부터 출력함
 * @param head: 리스트의 헤드(시작 노드) 포인터
 */
void printList(Node *head);

/**
 * @brief 새로운 노드를 생성함
 * @param data: 노드에 저장할 정수 값
 * @return 생성된 노드의 포인터
 */
Node *createNode(int data) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->data = data;
  newNode->next = NULL;
  newNode->prev = NULL;
  return newNode;
}

/**
 * @brief 리스트의 맨 앞에 노드를 삽입함
 * @param head: 리스트의 헤드(시작 노드)를 가리키는 포인터의 주소
 * @param data: 삽입할 정수 값
 */
void insertBegin(Node **head, int data) {
  Node *newNode = createNode(data);
  newNode->next = *head;
  if (*head != NULL) {
    (*head)->prev = newNode;
  }
  *head = newNode;
}

/**
 * @brief 지정한 위치(0부터 시작)에 노드를 삽입함
 * @param head: 리스트의 헤드(시작 노드)를 가리키는 포인터의 주소
 * @param data: 삽입할 정수 값
 * @param index: 삽입할 위치(0부터 시작)
 * @return 성공 시 1, 실패(잘못된 인덱스) 시 0 반환
 */
int insertWhere(Node **head, int data, int index) {
  // 인덱스가 0보다 작으면 실패
  if (index < 0)
    return 0;

  if (index == 0) {
    insertBegin(head, data);
    return 1;
  }
  Node *temp = *head;
  int i = 0;
  // 삽입할 위치 바로 앞 노드로 이동
  while (temp != NULL && i < index - 1) {
    temp = temp->next;
    i++;
  }
  if (temp == NULL)
    return 0; // 잘못된 인덱스 입력으로 실패
  Node *newNode = createNode(data);
  newNode->next = temp->next;
  newNode->prev = temp;
  if (temp->next != NULL) {
    temp->next->prev = newNode;
  }
  temp->next = newNode;
  return 1;
}

/**
 * @brief 리스트의 맨 뒤에 노드를 삽입함
 * @param head: 리스트의 헤드(시작 노드)를 가리키는 포인터의 주소
 * @param data: 삽입할 정수 값
 */
void insertEnd(Node **head, int data) {
  Node *newNode = createNode(data);
  if (*head == NULL) {
    *head = newNode;
    return;
  }
  Node *temp = *head;
  while (temp->next != NULL) {
    temp = temp->next;
  }
  temp->next = newNode;
  newNode->prev = temp;
}

/**
 * @brief 리스트 전체를 처음부터 출력함
 * @param head: 리스트의 헤드(시작 노드) 포인터
 */
void printList(Node *head) {
  Node *temp = head;
  while (temp != NULL) {
    printf("%d ", temp->data);
    temp = temp->next;
  }
  printf("\n");
}

/**
 * @brief 리스트의 모든 노드를 메모리에서 해제함
 * @param head: 리스트의 헤드(시작 노드) 포인터
 */
void freeList(Node *head) {
  Node *temp;
  while (head != NULL) {
    temp = head;
    head = head->next;
    free(temp);
  }
}

/**
 * @brief 지정한 위치(0부터 시작)의 노드를 삭제함
 * @param head: 리스트의 헤드(시작 노드)를 가리키는 포인터의 주소
 * @param index: 삭제할 위치(0부터 시작)
 * @return 성공 시 1, 잘못된 인덱스 입력으로 실패시 0 반환
 */
int deleteWhere(Node **head, int index) {
  if (*head == NULL || index < 0)
    return 0;
  Node *temp = *head;
  int i = 0;
  // 첫 번째 노드인 헤드 삭제
  if (index == 0) {
    *head = temp->next;
    if (*head != NULL)
      (*head)->prev = NULL;
    free(temp);
    return 1;
  }
  // 삭제할 위치까지 이동
  while (temp != NULL && i < index) {
    temp = temp->next;
    i++;
  }
  if (temp == NULL)
    return 0; // 잘못된 인덱스 입력으로 실패했을 때 반환값
  if (temp->prev != NULL)
    temp->prev->next = temp->next;
  if (temp->next != NULL)
    temp->next->prev = temp->prev;
  free(temp);
  return 1;
}

int main() {
  Node *head = NULL;
  int choice, value, index;

  // 초기값 설정: 10, 20, 30을 순서대로 삽입
  insertEnd(&head, 10);
  insertEnd(&head, 20);
  insertEnd(&head, 30);

  printf("\n[초기 리스트] ");
  printList(head);

  while (1) {
    // 각 기능 실행 후 리스트 출력
    printf("\n[현재 리스트] ");
    printList(head);
    printf("\n");

    printf("\n원하는 작업을 선택하세요:\n");
    printf("1: 맨 앞에 삽입\n");
    printf("2: 원하는 위치에 삽입\n");
    printf("3: 맨 뒤에 삽입\n");
    printf("4: 원하는 위치 삭제\n");
    printf("0: 종료\n");
    printf("번호를 입력하세요: ");
    if (scanf("%d", &choice) != 1) {
      while (getchar() != '\n')
        ;
      printf("잘못된 입력입니다.\n");
      break;
    }
    if (choice == 0)
      break;
    switch (choice) {
    case 1:
      printf("맨 앞에 삽입할 값을 입력하세요: ");
      scanf("%d", &value);
      insertBegin(&head, value);
      break;
    case 2:
      printf("삽입할 값을 입력하세요: ");
      scanf("%d", &value);
      printf("삽입할 위치(인덱스)를 입력하세요: ");
      scanf("%d", &index);
      if (!insertWhere(&head, value, index))
        printf("잘못된 인덱스입니다.\n");
      break;
    case 3:
      printf("맨 뒤에 삽입할 값을 입력하세요: ");
      scanf("%d", &value);
      insertEnd(&head, value);
      break;
    case 4:
      printf("삭제할 위치(인덱스)를 입력하세요: ");
      scanf("%d", &index);
      if (!deleteWhere(&head, index))
        printf("잘못된 인덱스입니다.\n");
      break;
    default:
      printf("잘못된 선택입니다.\n");
      break;
    }
  }
  freeList(head);
  return 0;
}

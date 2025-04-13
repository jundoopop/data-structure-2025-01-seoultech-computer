#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 연결 리스트 노드 구조체 정의
typedef struct Node {
  char name[20];     // 친구 이름 (충분한 크기로 설정)
  int count;         // 카톡 횟수
  struct Node *next; // 다음 노드를 가리키는 포인터
} Node;

// 함수 프로토타입 선언
void insertNode(Node **headRef, const char *newName, int newCount);
int deleteNode(
    Node **headRef,
    const char *nameToDelete); // 특정 친구 삭제 함수 (반환값: 0=실패, 1=성공)
void printList(Node *head);
void freeList(Node **headRef); // 전체 삭제 및 메모리 해제 함수 (head 포인터도
                               // NULL로 만듦)

int main() {
  Node *head = NULL; // 연결 리스트의 시작을 가리키는 포인터 (초기값 NULL)

  // --- 초기 친구 정보 추가 ---
  insertNode(&head, "다현", 200);
  insertNode(&head, "정연", 150);
  insertNode(&head, "쯔위", 90);
  insertNode(&head, "사나", 30);
  insertNode(&head, "지효", 15);

  printf("초기 친구 목록:\n");
  printList(head);
  printf("\n");

  // --- 새 친구 추가 (미나 40) ---
  printf("추가할 친구 --> 미나\n");
  printf("카톡 횟수 --> 40\n");
  insertNode(&head, "미나", 40);
  printList(head);
  printf("\n");

  // --- 새 친구 추가 (혜리 200) ---
  printf("추가할 친구 --> 혜리\n");
  printf("카톡 횟수 --> 200\n");
  insertNode(&head, "혜리", 200);
  printList(head);
  printf("\n");

  // --- 특정 친구 삭제 (사나) ---
  printf("삭제할 친구 --> 사나\n");
  if (deleteNode(&head, "사나")) {
    printf("'사나' 삭제 성공.\n");
  } else {
    printf("'사나'를 찾을 수 없습니다.\n");
  }
  printList(head);
  printf("\n");

  // --- 특정 친구 삭제 (헤드 노드: 혜리) ---
  printf("삭제할 친구 --> 혜리\n");
  if (deleteNode(&head, "혜리")) {
    printf("'혜리' 삭제 성공.\n");
  } else {
    printf("'혜리'를 찾을 수 없습니다.\n");
  }
  printList(head);
  printf("\n");

  // --- 존재하지 않는 친구 삭제 시도 ---
  printf("삭제할 친구 --> 없는친구\n");
  if (deleteNode(&head, "없는친구")) {
    printf("'없는친구' 삭제 성공.\n");
  } else {
    printf("'없는친구'를 찾을 수 없습니다.\n");
  }
  printList(head);
  printf("\n");

  // --- 전체 친구 삭제 ---
  printf("--- 전체 친구 목록 삭제 ---\n");
  freeList(&head); // head 포인터의 주소를 넘겨줌
  printf("전체 삭제 후 목록:\n");
  printList(head); // NULL이 출력될 것임 (빈 리스트)
  printf("\n");

  // 전체 삭제 후 다시 추가 가능
  printf("전체 삭제 후 다시 추가:\n");
  insertNode(&head, "다현", 10);
  printList(head);
  printf("\n");

  // --- 최종 메모리 해제 (프로그램 종료 전) ---
  // 만약 위에서 전체 삭제를 하지 않았다면 여기서 해제
  freeList(&head);

  return 0;
}

/**
 * @brief 연결 리스트에 새 노드를 정렬된 위치에 삽입합니다. (기존 함수와 동일)
 */
void insertNode(Node **headRef, const char *newName, int newCount) {
  // 1. 새 노드 생성 및 데이터 할당
  Node *newNode = (Node *)malloc(sizeof(Node));
  if (newNode == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }
  strncpy(newNode->name, newName, sizeof(newNode->name) - 1);
  newNode->name[sizeof(newNode->name) - 1] = '\0';
  newNode->count = newCount;
  newNode->next = NULL;

  // 2. 리스트가 비어있거나 새 노드가 맨 앞에 와야 하는 경우
  if (*headRef == NULL || newCount >= (*headRef)->count) {
    newNode->next = *headRef;
    *headRef = newNode;
  } else {
    // 3. 리스트를 순회하며 삽입할 위치 찾기
    Node *current = *headRef;
    while (current->next != NULL && newCount < current->next->count) {
      current = current->next;
    }
    // 4. 찾은 위치(current 다음)에 새 노드 삽입
    newNode->next = current->next;
    current->next = newNode;
  }
}

/**
 * @brief 특정 이름을 가진 노드를 찾아 삭제합니다.
 * @param headRef 리스트의 헤드 포인터에 대한 포인터
 * @param nameToDelete 삭제할 친구의 이름
 * @return int 삭제 성공 시 1, 실패(노드 없음) 시 0 반환
 */
int deleteNode(Node **headRef, const char *nameToDelete) {
  Node *temp = *headRef, *prev = NULL;

  // 1. 리스트가 비어있는 경우
  if (temp == NULL) {
    return 0; // 삭제 실패
  }

  // 2. 삭제할 노드가 헤드 노드인 경우
  if (strcmp(temp->name, nameToDelete) == 0) {
    *headRef = temp->next; // 헤드를 다음 노드로 변경
    free(temp);            // 이전 헤드 노드 메모리 해제
    return 1;              // 삭제 성공
  }

  // 3. 리스트를 순회하며 삭제할 노드 검색 (헤드가 아닌 경우)
  //    삭제할 노드의 *이전* 노드(prev)를 찾아야 함
  while (temp != NULL && strcmp(temp->name, nameToDelete) != 0) {
    prev = temp;
    temp = temp->next;
  }

  // 4. 리스트 끝까지 갔는데 찾지 못한 경우
  if (temp == NULL) {
    return 0; // 삭제 실패 (해당 이름 없음)
  }

  // 5. 찾은 경우: 이전 노드(prev)의 next를 삭제할 노드(temp)의 다음 노드로 연결
  prev->next = temp->next;
  free(temp); // 삭제할 노드 메모리 해제
  return 1;   // 삭제 성공
}

/**
 * @brief 연결 리스트의 모든 노드를 출력합니다. (기존 함수와 동일)
 */
void printList(Node *head) {
  Node *temp = head;
  printf("[ ");
  while (temp != NULL) {
    printf("{ %s %d } ", temp->name, temp->count);
    temp = temp->next;
  }
  printf("]\n");
}

/**
 * @brief 연결 리스트의 모든 노드 메모리를 해제하고, head 포인터를 NULL로
 * 설정합니다. (전체 삭제)
 * @param headRef 리스트의 헤드 포인터에 대한 포인터
 */
void freeList(Node **headRef) {
  Node *current = *headRef; // 현재 노드
  Node *nextNode;           // 다음 노드를 임시 저장할 포인터

  while (current != NULL) {
    nextNode = current->next; // 다음 노드 주소 저장
    free(current);            // 현재 노드 메모리 해제
    current = nextNode;       // 다음 노드로 이동
  }

  *headRef = NULL; // main 함수의 head 포인터 자체를 NULL로 변경
}
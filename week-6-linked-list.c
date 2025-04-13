#include <stdio.h>
#include <stdlib.h>
#include <string.h> // strcmp, strncpy 사용

// ----------------------------------------------------------------------------
// 1. 프로그램에서 사용할 데이터 구조체 (친구 연락처 정보)
// ----------------------------------------------------------------------------
typedef struct Contact {
  char name[20]; // 친구 이름 (20자 이내)
  int count;     // 카톡 횟수
} Contact;

// ----------------------------------------------------------------------------
// 2. 제네릭(범용) 연결 리스트 구조체 및 타입 정의
// ----------------------------------------------------------------------------

// 연결 리스트 노드 구조체
typedef struct Node {
  void *data;        // 실제 데이터를 가리키는 void 포인터
  struct Node *next; // 다음 노드를 가리키는 포인터
} Node;

// 연산을 위한 함수 포인터 타입 정의
typedef void (*PrintDataFunc)(const void *data); // 데이터 출력 함수
typedef void (*FreeDataFunc)(
    void *data); // data 포인터가 가리키는 메모리 해제 함수

// 연결 리스트 관리 구조체
typedef struct LinkedList {
  Node *head;            // 리스트의 시작 노드를 가리키는 포인터
  PrintDataFunc print;   // 데이터 출력 함수 포인터
  FreeDataFunc freeData; // 데이터 메모리 해제 함수 포인터 - 리스트 전체 삭제
} LinkedList;

// Forward declaration for helper
int getListSize(const LinkedList *list);

// ----------------------------------------------------------------------------
// 3. 연결 리스트 함수 프로토타입 (구현은 main 함수 아래에)
// ----------------------------------------------------------------------------

/**
 * @brief 새로운 연결 리스트를 생성 및 초기화
 * @param printFunc 데이터 출력 함수의 포인터
 * @param freeFunc 데이터 메모리 해제 함수의 포인터 (리스트 전체 삭제)
 * @return 성공 시 LinkedList 포인터 반환, 실패 시 NULL 반환
 */
LinkedList *createLinkedList(PrintDataFunc printFunc, FreeDataFunc freeFunc);

/**
 * @brief 리스트의 끝에 새 데이터 추가
 * @param list 대상 연결 리스트 포인터
 * @param newData 삽입할 데이터를 가리키는 포인터 (호출자가 메모리 할당)
 */
void insertNodeAtEnd(LinkedList *list, void *newData);

/**
 * @brief 리스트의 지정된 위치에 새 데이터를 삽입
 * @param list 대상 연결 리스트 포인터
 * @param newData 삽입할 데이터를 가리키는 포인터 (호출자가 메모리 할당)
 * @param position 삽입할 위치 (0부터 시작)
 * @return 성공 시 1, 실패(잘못된 위치 등) 시 0
 */
int insertNodeAtPosition(LinkedList *list, void *newData, int position);

/**
 * @brief 이름을 기준으로 노드를 찾아 삭제하는 함수 (Contact 데이터용)
 * @param list 대상 연결 리스트 포인터
 * @param nameToDelete 삭제할 친구의 이름
 * @return 해당하는 노드를 찾아서 삭제 성공 시 1, 실패(찾지 못함) 시 0
 */
int deleteContactByName(LinkedList *list, const char *nameToDelete);

/**
 * @brief 리스트의 모든 데이터를 출력
 * @param list 출력할 연결 리스트 포인터
 */
void printList(const LinkedList *list);

/**
 * @brief 리스트의 모든 노드와 각 노드가 가리키는 데이터를 해제하고, 리스트
 * 구조체 자체도 해제
 * @param list 해제할 연결 리스트 포인터. 포인터 자체는 NULL로 설정되지 않으므로
 * 호출 후 NULL 할당 권장
 */
void freeList(LinkedList **listPtr);

// Function prototype for the safe line reader
int readLineSafe(char *buffer, int bufferSize);

// ----------------------------------------------------------------------------
// 4. Contact 구조체용 콜백 함수 구현
// ----------------------------------------------------------------------------

/**
 * @brief Contact 구조체 데이터를 "{ 이름 횟수 }" 형식으로 출력
 * @param data Contact 구조체 포인터
 */
void printContact(const void *data);

/**
 * @brief Contact 구조체 포인터가 가리키는 메모리를 해제
 */
void freeContactData(void *data);

// ----------------------------------------------------------------------------
// 5. 메인 함수 - 사용자 인터페이스 및 기능 호출
// ----------------------------------------------------------------------------

int main() {
  int choice = -1;    // 사용자 선택 저장 변수
  char nameInput[20]; // 이름 입력 버퍼
  int countInput;     // 횟수 입력 변수
  int positionInput;  // 위치 입력 변수

  printf("친구 연락처 관리 프로그램 (연결 리스트 버전 - 순서 유지)\n");

  // Contact 데이터용 콜백 함수들로 연결 리스트 생성
  LinkedList *friendList = createLinkedList(printContact, freeContactData);
  if (friendList == NULL) {
    fprintf(stderr, "오류: 리스트 생성 실패!\n");
    return 1;
  }

  // --- 초기 친구 정보 추가 (선택 사항, 시작 시 빈 리스트로 시작 가능) ---
  const char *initial_names[] = {"다현", "정연", "쯔위", "사나", "지효"};
  int initial_counts[] = {200, 150, 90, 30, 15};
  int num_initial = sizeof(initial_counts) / sizeof(initial_counts[0]);
  for (int i = 0; i < num_initial; ++i) {
    Contact *initialFriend = (Contact *)malloc(sizeof(Contact));
    if (initialFriend) {
      strncpy_s(initialFriend->name, sizeof(initialFriend->name),
                initial_names[i], sizeof(initialFriend->name) - 1);
      initialFriend->name[sizeof(initialFriend->name) - 1] = '\0';
      initialFriend->count = initial_counts[i];
      insertNodeAtEnd(friendList, initialFriend);
    }
  }
  printf("초기 데이터 \n");
  printList(friendList);
  printf("-------------------------------\n");

  // --- 메인 메뉴 루프 ---
  while (choice != 0) {
    printf("\n--- 메뉴 ---\n");
    printf("1: 친구 추가 (끝에)\n");
    printf("2: 친구 삽입 (위치 지정)\n");
    printf("3: 친구 삭제 (이름으로)\n");
    printf("4: 전체 목록 삭제\n");
    printf("0: 프로그램 종료\n");
    printf("------------\n");
    printf("선택: ");

    // 사용자 입력 받기 (정수만)
    if (scanf_s("%d", &choice) != 1) {
      printf("잘못된 입력입니다. 숫자를 입력해주세요.\n");
      // 입력 버퍼 비우기
      while (getchar() != '\n')
        ;
      choice = -1; // 잘못된 입력 처리
      continue;
    }
    // scanf_s 후 남아있는 개행 문자 제거
    while (getchar() != '\n')
      ;

    switch (choice) {
    case 1: // 친구 추가 (끝에)
      printf("추가할 친구 이름: ");
      if (!readLineSafe(nameInput, sizeof(nameInput))) {
        printf("이름 입력 중 오류 발생.\n");
        break;
      }

      printf("카톡 횟수: ");
      if (scanf_s("%d", &countInput) != 1) {
        printf("횟수 입력 오류! 숫자를 입력하세요.\n");
        // Clear buffer in case of non-numeric input
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
          ;
        break;
      }
      int c_after_count;
      while ((c_after_count = getchar()) != '\n' && c_after_count != EOF)
        ;

      // 새 Contact 동적 할당
      Contact *newFriend = (Contact *)malloc(sizeof(Contact));
      if (newFriend == NULL) {
        fprintf(stderr, "오류: 친구 데이터 메모리 할당 실패!\n");
        break;
      }
      strncpy_s(newFriend->name, sizeof(newFriend->name), nameInput,
                sizeof(newFriend->name) - 1);
      newFriend->name[sizeof(newFriend->name) - 1] = '\0'; // NULL 종단 보장
      newFriend->count = countInput;

      // 리스트 끝에 노드 추가
      insertNodeAtEnd(friendList, newFriend);
      printf("친구 '%s'를(을) 리스트 끝에 추가했습니다.\n", nameInput);
      printList(friendList);
      break;

    case 2: // 친구 삽입 (위치 지정)
      printf("삽입할 친구 이름: ");
      if (!readLineSafe(nameInput, sizeof(nameInput))) {
        printf("이름 입력 중 오류 발생.\n");
        break;
      }

      printf("카톡 횟수: ");
      if (scanf_s("%d", &countInput) != 1) {
        printf("횟수 입력 오류! 숫자를 입력하세요.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
          ;
        break;
      }
      int c_after_count_ins;
      while ((c_after_count_ins = getchar()) != '\n' &&
             c_after_count_ins != EOF)
        ;

      printf("삽입할 위치 (0부터 시작, 현재 크기: %d): ",
             getListSize(friendList));
      if (scanf_s("%d", &positionInput) != 1) {
        printf("위치 입력 오류! 숫자를 입력하세요.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
          ;
        break;
      }
      int c_after_pos;
      while ((c_after_pos = getchar()) != '\n' && c_after_pos != EOF)
        ;

      // 새 Contact 동적 할당
      Contact *insertFriend = (Contact *)malloc(sizeof(Contact));
      if (insertFriend == NULL) {
        fprintf(stderr, "오류: 친구 데이터 메모리 할당 실패!\n");
        break; // 메뉴로 돌아감
      }
      strncpy_s(insertFriend->name, sizeof(insertFriend->name), nameInput,
                sizeof(insertFriend->name) - 1);
      insertFriend->name[sizeof(insertFriend->name) - 1] =
          '\0'; // Null-terminate
      insertFriend->count = countInput;

      // 지정된 위치에 노드 삽입 시도
      if (insertNodeAtPosition(friendList, insertFriend, positionInput)) {
        printf("친구 '%s'를(을) 위치 %d에 삽입했습니다.\n", nameInput,
               positionInput);
        printList(friendList);
      } else {
        printf("삽입 실패. 위치가 잘못되었거나 메모리 오류일 수 있습니다.\n");
        freeContactData(insertFriend); // 삽입 실패 시 할당된 메모리 해제
      }
      break;

    case 3: // 친구 삭제 (이름으로)
      printf("삭제할 친구 이름: ");
      if (!readLineSafe(nameInput, sizeof(nameInput))) {
        printf("이름 입력 중 오류 발생.\n");
        break;
      }

      printf("삭제 중... '%s'\n", nameInput);
      int deleted = deleteContactByName(friendList, nameInput);
      if (deleted) {
        printf("'%s' 삭제 성공.\n", nameInput);
      } else {
        printf("'%s'를(을) 찾지 못했습니다.\n", nameInput);
      }
      printList(friendList); // 결과 출력
      break;

    case 4: // 전체 목록 삭제
      printf("전체 목록을 삭제합니다...\n");
      freeList(&friendList); // 리스트의 모든 노드와 데이터 해제
      printf("목록이 비워졌습니다. 새 리스트를 생성합니다.\n");
      // 리스트 재생성
      friendList = createLinkedList(printContact, freeContactData);
      if (friendList == NULL) { // 재생성 실패 시 처리
        fprintf(stderr, "오류: 리스트 재생성 실패! 프로그램을 종료합니다.\n");
        return 1; // 또는 exit(1);
      }
      printList(friendList); // 빈 리스트 출력 (확인용)
      break;

    case 0: // 프로그램 종료
      printf("프로그램을 종료합니다.\n");
      break;

    default: // 잘못된 선택
      printf("잘못된 선택입니다. 메뉴에서 0-4 사이의 숫자를 입력하세요.\n");
      break;
    } // switch 끝
  } // while 끝

  // --- 최종 정리 ---
  // 루프 종료 시 남아있는 리스트 데이터 모두 해제
  printf("최종 리스트 정리...\n");
  freeList(&friendList); // friendList 포인터는 내부에서 NULL로 설정됨
  printf("메모리 해제 완료.\n");

  return 0;
}

// ----------------------------------------------------------------------------
// 6. Contact 이름으로 삭제하는 함수 구현
// ----------------------------------------------------------------------------
int deleteContactByName(LinkedList *list, const char *nameToDelete) {
  // 리스트가 비어있거나 필수 함수가 없는 경우
  if (list == NULL || list->head == NULL || nameToDelete == NULL) {
    return 0;
  }

  Node *temp = list->head;
  Node *prev = NULL;
  Contact *contactData = NULL;

  // 첫 번째 노드(head) 확인
  contactData = (Contact *)temp->data;
  if (contactData != NULL && strcmp(contactData->name, nameToDelete) == 0) {
    list->head = temp->next; // 헤드 업데이트
    if (list->freeData) {
      list->freeData(temp->data); // Contact 데이터 해제
    }
    free(temp); // 노드 자체 해제
    return 1;   // 삭제 성공
  }

  // 나머지 리스트 탐색
  prev = temp;
  temp = temp->next;
  while (temp != NULL) {
    contactData = (Contact *)temp->data;
    if (contactData != NULL && strcmp(contactData->name, nameToDelete) == 0) {
      // 찾았으면 이전 노드의 next를 현재 노드의 next로 연결
      prev->next = temp->next;
      // 데이터 해제 및 노드 해제
      if (list->freeData) {
        list->freeData(temp->data);
      }
      free(temp);
      return 1; // 삭제 성공
    }
    // 못 찾았으면 다음 노드로 이동
    prev = temp;
    temp = temp->next;
  }

  return 0; // 리스트 끝까지 탐색했지만 찾지 못함
}

// ----------------------------------------------------------------------------
// 7. 제네릭 연결 리스트 함수 구현
// ----------------------------------------------------------------------------

LinkedList *createLinkedList(PrintDataFunc printFunc, FreeDataFunc freeFunc) {
  LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
  if (list == NULL) {
    perror("리스트 구조체 메모리 할당 실패");
    return NULL;
  }
  list->head = NULL;
  list->print = printFunc;
  list->freeData = freeFunc;
  return list;
}

// --- 리스트 사이즈 반환 ---
int getListSize(const LinkedList *list) {
  int count = 0;
  Node *current = list->head;
  while (current != NULL) {
    count++;
    current = current->next;
  }
  return count;
}

// --- 리스트 끝에 노드 추가 ---
void insertNodeAtEnd(LinkedList *list, void *newData) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  if (!newNode) {
    fprintf(stderr, "Error: Failed to allocate memory for new node.\n");
    return;
  }
  newNode->data = newData;
  newNode->next = NULL;

  if (list->head == NULL) {
    // 빈 리스트에서의 초기화
    list->head = newNode;
  } else {
    // 리스트 끝까지 이동
    Node *current = list->head;
    while (current->next != NULL) {
      current = current->next;
    }
    // 리스트 끝에 새로운 노드 연결
    current->next = newNode;
  }
}

// --- 리스트의 지정된 위치에 노드 삽입 ---
int insertNodeAtPosition(LinkedList *list, void *newData, int position) {
  int size = getListSize(list);
  if (position < 0 || position > size) {
    fprintf(stderr,
            "Error: Invalid position %d. List size is %d. Position must be "
            "between 0 and %d.\n",
            position, size, size);
    return 0; // Indicate failure
  }

  Node *newNode = (Node *)malloc(sizeof(Node));
  if (!newNode) {
    fprintf(stderr, "Error: Failed to allocate memory for new node.\n");
    return 0; // Indicate failure
  }
  newNode->data = newData;
  newNode->next = NULL;

  if (position == 0) {
    // 리스트의 시작 부분에 삽입
    newNode->next = list->head;
    list->head = newNode;
  } else {
    // 이전 노드를 찾기
    Node *current = list->head;
    // position-1번 이전 노드 찾기
    for (int i = 0; i < position - 1; ++i) {
      if (current == NULL) {
        fprintf(stderr,
                "Error: Traversal failed unexpectedly at position %d.\n", i);
        free(newNode); // Clean up allocated node
        return 0;
      }
      current = current->next;
    }
    if (current == NULL) {
      fprintf(stderr,
              "Error: Cannot insert at position %d, previous node not found.\n",
              position);
      free(newNode);
      return 0;
    }
    // 삽입
    newNode->next = current->next;
    current->next = newNode;
  }
  return 1; // 성공
}

/**
 * @brief 리스트의 모든 데이터를 출력.
 * @param list 출력할 연결 리스트 포인터.
 */
void printList(const LinkedList *list) {
  if (list == NULL) {
    printf("[ 리스트가 NULL 입니다 ]\n");
    return;
  }
  if (list->print == NULL) {
    printf("[ 출력 함수가 설정되지 않았습니다 ]\n");
    return;
  }
  if (list->head == NULL) {
    printf("[]\n"); // 빈 리스트
    return;
  }

  Node *temp = list->head;
  printf("[ ");
  while (temp != NULL) {
    list->print(temp->data);
    if (temp->next != NULL) {
      printf(" ");
    }
    temp = temp->next;
  }
  printf(" ]\n");
}

/**
 * @brief 리스트의 모든 노드와 각 노드가 가리키는 데이터를 해제하고, 리스트
 * 구조체 자체도 해제.
 * @param list 해제할 연결 리스트 포인터. 포인터 자체는 NULL로 설정되지 않으므로
 * 호출 후 NULL 할당 권장.
 */
void freeList(LinkedList **listPtr) {
  if (listPtr == NULL || *listPtr == NULL) {
    return; // 해제할 리스트가 없을 때
  }

  LinkedList *list = *listPtr; // 작업 편의를 위해 역참조

  Node *current = list->head;
  Node *nextNode;

  while (current != NULL) {
    nextNode = current->next;
    if (list->freeData) {
      list->freeData(current->data); // 사용자 데이터 해제
    }
    free(current); // 노드 해제
    current = nextNode;
  }

  list->head = NULL; // 헤드 초기화
  free(list);        // 리스트 관리 구조체 해제
  *listPtr = NULL;   // 호출자 포인터를 NULL로 설정
}

// --- 안전한 라인 읽기 ---
int readLineSafe(char *buffer, int bufferSize) {
  if (fgets(buffer, bufferSize, stdin) == NULL) {
    // EOF 또는 읽기 오류
    fprintf(stderr, "Input error or EOF detected.\n");
    clearerr(stdin);
    // 버퍼를 소비 (예: Ctrl+Z 후 Enter)
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
    return 0; // 실패
  }

  // newline 확인 (입력이 enter로 끝났는지 확인)
  char *newline_pos = strchr(buffer, '\n');
  if (newline_pos != NULL) {
    // newline 발견, 제거
    *newline_pos = '\0';
  } else {
    // newline 없음. 입력이 버퍼를 초과했을 가능성이 높음.
    // stdin 버퍼를 소비
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
  }
  return 1; // 성공
}

// ----------------------------------------------------------------------------
// 4. Contact 구조체용 콜백 함수 구현
// ----------------------------------------------------------------------------

/**
 * @brief Contact 구조체 데이터를 "{ 이름 횟수 }" 형식으로 출력.
 */
void printContact(const void *data) {
  const Contact *c = (const Contact *)data;
  if (c != NULL) {
    printf("{ %s %d }", c->name, c->count);
  }
}
/**
 * @brief Contact 구조체 포인터가 가리키는 메모리를 해제.
 */
void freeContactData(void *data) {
  free(data); // Contact 구조체 자체가 malloc으로 할당되었다고 가정
} //
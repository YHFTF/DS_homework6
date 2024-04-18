/**
 * circularQ.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 4				//큐의 최대 크기를 4로 정의

typedef char element;					//큐의 데이터 타입을 char로 정의	
typedef struct {
	element queue[MAX_QUEUE_SIZE];
	int front, rear;
}QueueType;


QueueType *createQueue();
int freeQueue(QueueType *cQ);
int isEmpty(QueueType *cQ);
int isFull(QueueType *cQ);
void enQueue(QueueType *cQ, element item);
void deQueue(QueueType *cQ, element* item);
void printQ(QueueType *cQ);
void debugQ(QueueType *cQ);
element getElement();

int main(void)
{
	QueueType *cQ = createQueue();
	element data;
	char command;
	printf("-----[2021041019] [Park younghyeon]-----\n");

	do{
		printf("\n-----------------------------------------------------\n");
		printf("                     Circular Q                   \n");
		printf("------------------------------------------------------\n");
		printf(" Insert=i,  Delete=d,   PrintQ=p,   Debug=b,   Quit=q \n");
		printf("------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {	//입력받은 command에 따라 다음의 case문 실행
		case 'i': case 'I':
			data = getElement();
			enQueue(cQ, data);
			break;
		case 'd': case 'D':
			deQueue(cQ, &data);
			break;
		case 'p': case 'P':
			printQ(cQ);
			break;
		case 'b': case 'B':
			debugQ(cQ);
			break;
		case 'q': case 'Q':
   	        freeQueue(cQ);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');


	return 1;
}

QueueType *createQueue()
{
	QueueType *cQ;	//위에서 정의한 QueueType으로 큐의 포인터 변수 선언
	cQ = (QueueType *)malloc(sizeof(QueueType));	//큐의 메모리를 동적 할당
	cQ->front = 0;	//큐의 front와 rear를 0으로 초기화
	cQ->rear = 0;
	return cQ;
}

int freeQueue(QueueType *cQ)
{
    if(cQ == NULL) return 1;									//큐가 비어있으면 1을 반환
    free(cQ);													//큐의 메모리를 해제			
    return 1;
}

element getElement()	//큐에 넣을 데이터를 입력받는 함수,char형 데이터를 입력받음
{
	element item;
	printf("Input element = ");
	scanf(" %c", &item);
	return item;
}


/* complete the function */
int isEmpty(QueueType *cQ)	//큐가 비어있는지 확인하는 함수, frint와 rear가 같으면 비어있는 상태
{
	return cQ->front == cQ->rear;
}

/* complete the function */
int isFull(QueueType *cQ)	//큐가 가득 차있는지 확인하는 함수, rear의 다음 위치가 front와 같으면 가득 찬 상태
{
	return (cQ->rear + 1) % MAX_QUEUE_SIZE == cQ->front;

}


/* complete the function */
void enQueue(QueueType *cQ, element item)  //큐에 데이터를 넣는 함수
{
	if(isFull(cQ))	//큐가 가득 차있으면 가득 차있다는 메시지 출력
	{
		printf("큐가 가득 찼습니다.\n");
		return;
	}
	cQ->rear = (cQ->rear + 1) % MAX_QUEUE_SIZE;	//원형 큐이기 때문에 %연산을 사용하여 rear의 위치를 변경
	cQ->queue[cQ->rear] = item;	//rear의 위치에 입력 받은 데이터를 넣음
	return;
}

/* complete the function */
void deQueue(QueueType *cQ, element *item)	//큐에서 데이터를 빼는 함수
{
	if(isEmpty(cQ))
	{
		printf("큐가 비어있습니다.\n");
		return;
	}
	cQ->front = (cQ->front + 1) % MAX_QUEUE_SIZE;	//원형 큐이기 때문에 %연산을 사용하여 front의 위치를 변경
	*item = cQ->queue[cQ->front];	//front의 위치에 있는 데이터를 item에 저장 (프로그램에서 사용하지는 않지만 큐에서 데이터가 빠져나갈 때 데이터를 반환하기 위해 item에 저장)
	return;
}


void printQ(QueueType *cQ)	//큐의 데이터를 출력하는 함수
{
	int i, first, last;

	first = (cQ->front + 1)%MAX_QUEUE_SIZE;
	last = (cQ->rear + 1)%MAX_QUEUE_SIZE;

	printf("Circular Queue : [");

	i = first;
	while(i != last){
		printf("%3c", cQ->queue[i]);	//%3c를 사용하여 데이터를 3칸씩 띄워서 출력
		i = (i+1)%MAX_QUEUE_SIZE;

	}
	printf(" ]\n");
}


void debugQ(QueueType *cQ)	//큐의 데이터를 디버깅하는 함수
{

	printf("\n---DEBUG\n");
	for(int i = 0; i < MAX_QUEUE_SIZE; i++)
	{
		if(i == cQ->front) {	//front의 위치를 출력
			printf("  [%d] = front\n", i);
			continue;
		}
		printf("  [%d] = %c\n", i, cQ->queue[i]);	//front의 위치가 아닌 나머지 위치에 있는 데이터 출력

	}
	printf("front = %d, rear = %d\n", cQ->front, cQ->rear);	//front와 rear의 위치 출력
}



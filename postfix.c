/** 
 * postfix.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixpush(char x);	//postfixStack에 push
char postfixPop();	//postfixStack에서 pop
void evalPush(int x);	//evalStack에 push
int evalPop();	//evalStack에서 pop
void getInfix();	//infixExp를 입력받는 함수
precedence getToken(char symbol);	//문자를 받아서 우선순위를 반환하는 함수
precedence getPriority(char x);	//문자를 받아서 우선순위를 반환하는 함수
void charCat(char* c);	//문자를 받아서 postfixExp에 추가하는 함수
void toPostfix();	//infixExp를 postfixExp로 변환하는 함수
void debug();	//디버깅을 위한 함수
void reset();	//초기화를 위한 함수
void evaluation();	//postfixExp를 계산하는 함수

int main()
{
	char command;

	do{
		printf("[----- [Park younghyeon] [2021041019] -----]");
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;
}

char postfixPop()
{
    char x;
    if(postfixStackTop == -1)
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--];
    }
    return x;
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
}

int evalPop()
{
    if(evalStackTop == -1)
        return -1;
    else
        return evalStack[evalStackTop--];
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x)
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */

void toPostfix()
{
    // infixExp의 문자를 하나씩 읽기 위한 포인터
    char *exp = infixExp;
    char x; // 문자 하나를 임시로 저장하기 위한 변수

    // exp를 증가시켜가면서, 문자를 읽고 postfix로 변경
    while (*exp != '\0')
    {
        // 피연산자인 경우
        if (getToken(*exp) == operand)
        {
            charCat(exp);
        }
        // 오른쪽 괄호인 경우
        else if (getToken(*exp) == rparen)
        {
            while (postfixStackTop != -1 && postfixStack[postfixStackTop] != '(')
            {
                x = postfixPop();
                charCat(&x); // 스택에서 pop한 문자를 postfixExp에 추가
            }
            postfixPop(); // 왼쪽 괄호는 pop하여 제거
        }
        // 왼쪽 괄호, 연산자인 경우
        else
        {
            //왼쪽 괄호를 우선적으로 push 하지않으면 우선순위가 높은 연산자가 스택에 들어가게 되어 오류가 발생
            if (*exp == '(')
            {
                postfixPush(*exp);
            }
            else
            {
                // 스택이 비어있거나 현재 연산자의 우선순위가 스택의 가장 위 연산자보다 높은 경우
                if (postfixStackTop == -1 || getPriority(postfixStack[postfixStackTop]) < getPriority(*exp))
                {
                    postfixPush(*exp);
                }
                else
                {
                    while (postfixStackTop != -1 && getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp))
                    {
                        x = postfixPop();
                        charCat(&x); // 스택에서 pop한 문자를 postfixExp에 추가
                    }
                    postfixPush(*exp);
                }
            }
        }
        exp++;
    }

    // 스택에 남아있는 연산자들을 모두 postfixExp에 추가
    while (postfixStackTop != -1)
    {
        x = postfixPop();
        charCat(&x);
    }
}

void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset()
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()
{
	/* postfixExp, evalStack을 이용한 계산 */
	char *exp = postfixExp;
	char c;
	int x, y;

	while(*exp != '\0')
	{
		//피연산자인 경우
		if(getToken(*exp) == operand)
		{
			evalPush(*exp - '0'); //evalStack 은 int 형이고 postfixExp는 char 형이기 때문에 문자를 숫자로 변환하여 스택에 push ('0'을 빼주는 이유는 아스키코드값을 숫자로 변환하기 위함)
		}
		//연산자인 경우
		else
		{
			x = evalPop();	//스택에서 pop한 값을 x에 저장
			y = evalPop();	//스택에서 pop한 값을 y에 저장
			switch(*exp)	//연산자에 따라 계산
			{
				case '-':
					evalPush(y - x);
					break;
				case '+':
					evalPush(y + x);
					break;
				case '*':
					evalPush(y * x);
					break;
				case '/':
					evalPush(y / x);
					break;
			}
		}
		exp++;
	}
	evalResult = evalPop();
}
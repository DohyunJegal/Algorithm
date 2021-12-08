#include <iostream>
#include <string>
using namespace std;

#define MAXLEN 100	// 입력받을 문자열의 최대 길이
#define ALPHALEN 60 // 입력받을 문자의 아스키코드 범위를 조절

typedef struct node{	// 구조체를 통한 트리의 노드 
	struct node *leftChild;	// 왼쪽, 오른쪽 자식을 가리키는 포인터 
	struct node *rightChild;
	char alphabet;
	int freq;
}node;

node *newNode(char, int, struct node *, struct node *);
void newTable(node *, char[], int, char*[]);
node *huffmanTree(char[]);
void decode(char *, node *);

node nodeArray[ALPHALEN] = { NULL };	// 허프만 트리 제작시 사용되는 노드배열
int charnum = 0; // 문자의 갯수를 저장할 변수

void main(){
	node *root;	// 허프만 트리의 루트노드
	char *code[ALPHALEN];	// 각 알파벳의 인코딩된 코드 저장용 배열
	char input[MAXLEN], str[MAXLEN], encode[MAXLEN] = "";	// 입력받을 문자열, 테이블 생성에 이용, 변환 결과를 저장할 배열
	int i;

	for (i = 0; i < ALPHALEN; i++) // 인코딩된 코드 저장용 배열 생성
		code[i] = new char;

	cout << "put the string to encode : ";
	cin >> input;
	cout << "encoding..." << endl << endl;

	root = huffmanTree(input);	// 허프만 트리 제작
	newTable(root, str, 0, code);	// 테이블 생성

	for(i = 0; input[i]; i++)
		strcat(encode, code[input[i] - 'A']);	// 인코딩된 코드를 모아서 저장

	cout << "Encoded result : " << encode << endl << "encoding..." << endl << endl << "Decoded result : ";
	decode(encode, root);	// 인코딩된 코드를 디코드

	system("pause");
}

node *newNode(char alphabet, int freq, struct node *leftChild, struct node *rightChild) {
	node* temp = new node;	// 노드를 생성한 후 입력받은 값으로 초기화

	temp->leftChild = leftChild;
	temp->rightChild = rightChild;
	temp->alphabet = alphabet;
	temp->freq = freq;

	return temp;
}

void newTable(node* node, char str[], int len, char *table[]) {
	if (!node->leftChild && !node->rightChild) {	// 리프노드인 경우 '\0'을 삽입해 끝을 알려줌
		str[len] = '\0';
		strcpy(table[(node->alphabet) - 'A'], str);	// 테이블에 해당 문자에 맞춰 저장된 코드를 넣어줌
	}
	else {
		if (node->leftChild) {	// 왼쪽 자식이 있으면 0을 삽입
			str[len] = '0';
			newTable(node->leftChild, str, len + 1, table);
		}
		if (node->rightChild) {	// 오른쪽 자식이 있으면 1을 삽입
			str[len] = '1';
			newTable(node->rightChild, str, len + 1, table);
		}
	}
}

node *huffmanTree(char arr[]) {
	int i = 0, j = 0, min = 0, lessmin = 0;	// 제일 빈도수가 낮은 min과 2번째로 작은 lessmin
	int freq[ALPHALEN] = { 0 }, isMerged[ALPHALEN] = { 0 };	// 알파벳의 빈도, 노드가 합쳐졌는지 확인하는 배열들
	node *tree[ALPHALEN] = { NULL }; // 비교할 노드 배열

	while (arr[i] != NULL)	// 알파벳의 빈도수를 구하여 저장
		freq[arr[i++] - 'A']++;

	for (i = 0; i < ALPHALEN; i++) {
		if (freq[i]) {	// 문장에 해당 알파벳이 존재한다면 노드배열과 비교 노드배열에 저장
			nodeArray[charnum] = *newNode(i + 'A', freq[i], NULL, NULL);
			tree[charnum++] = newNode(i + 'A', freq[i], NULL, NULL);
		}
	}

	for (i = 0; i < charnum - 1; i++) {
		j = 0;
		while (isMerged[j] == -1)	// 합쳐진 노드 제외하고 배열중 가장 앞 인덱스를 찾음
			j++;
		min = j;	// 최소값으로 가정

		for (j = min; j < charnum - 1; j++) {
			if (isMerged[j] != -1) {	// 노드가 합쳐진 경우가 아닐 때 빈도수를 비교하여 값을 변경
				if (tree[min]->freq > tree[j]->freq)
					min = j;
			}
		}

		j = 0;
		while (isMerged[j] == -1 || j == min)	// 합쳐진 노드와 최소값을 가진 노드를 제외하고 가장 앞 인덱스를 찾음
			j++;
		lessmin = j;	// 두번째 최소값으로 가정

		for (j = lessmin; j < charnum; j++) {
			if (isMerged[j] != -1) {	// 노드가 합쳐진 경우가 아닐 때 빈도수를 비교하여 값을 변경
				if (tree[lessmin]->freq > tree[j]->freq) {
					if (j != min)
						lessmin = j;
				}
			}	
		}

		tree[min] = newNode(NULL, tree[min]->freq + tree[lessmin]->freq, tree[min], tree[lessmin]);	// 최소값과 두번째 최소값의 인덱스 노드의 빈도수를 합친 빈도수를 가진 노드를 만들고 새 노드를 최소값 인덱스에 삽입
		isMerged[lessmin] = -1; // 합쳐졌기 때문에 -1로 체크
	}
	return tree[min];
}

void decode(char *str, node *root) {
	int i = 0;
	node* temp = root;

	while (str[i] != '\0') { // 삽입해준 '\0'을 찾을 때 까지 반복
		if (str[i] == '0')	// 노드의 값이 0인 경우 왼쪽 자식으로 이동
			temp = temp->leftChild;
		else if (str[i] == '1')	// 노드의 값이 1인 경우 오른쪽 자식으로 이동
			temp = temp->rightChild;

		if (!temp->leftChild && !temp->rightChild) {	// 리프노드인 경우 저장된 알파벳을 출력
			cout << temp->alphabet;
			temp = root;
		}
		i++;
	}
	cout << endl;
}
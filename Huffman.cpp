#include <iostream>
#include <string>
using namespace std;

#define MAXLEN 100	// �Է¹��� ���ڿ��� �ִ� ����
#define ALPHALEN 60 // �Է¹��� ������ �ƽ�Ű�ڵ� ������ ����

typedef struct node{	// ����ü�� ���� Ʈ���� ��� 
	struct node *leftChild;	// ����, ������ �ڽ��� ����Ű�� ������ 
	struct node *rightChild;
	char alphabet;
	int freq;
}node;

node *newNode(char, int, struct node *, struct node *);
void newTable(node *, char[], int, char*[]);
node *huffmanTree(char[]);
void decode(char *, node *);

node nodeArray[ALPHALEN] = { NULL };	// ������ Ʈ�� ���۽� ���Ǵ� ���迭
int charnum = 0; // ������ ������ ������ ����

void main(){
	node *root;	// ������ Ʈ���� ��Ʈ���
	char *code[ALPHALEN];	// �� ���ĺ��� ���ڵ��� �ڵ� ����� �迭
	char input[MAXLEN], str[MAXLEN], encode[MAXLEN] = "";	// �Է¹��� ���ڿ�, ���̺� ������ �̿�, ��ȯ ����� ������ �迭
	int i;

	for (i = 0; i < ALPHALEN; i++) // ���ڵ��� �ڵ� ����� �迭 ����
		code[i] = new char;

	cout << "put the string to encode : ";
	cin >> input;
	cout << "encoding..." << endl << endl;

	root = huffmanTree(input);	// ������ Ʈ�� ����
	newTable(root, str, 0, code);	// ���̺� ����

	for(i = 0; input[i]; i++)
		strcat(encode, code[input[i] - 'A']);	// ���ڵ��� �ڵ带 ��Ƽ� ����

	cout << "Encoded result : " << encode << endl << "encoding..." << endl << endl << "Decoded result : ";
	decode(encode, root);	// ���ڵ��� �ڵ带 ���ڵ�

	system("pause");
}

node *newNode(char alphabet, int freq, struct node *leftChild, struct node *rightChild) {
	node* temp = new node;	// ��带 ������ �� �Է¹��� ������ �ʱ�ȭ

	temp->leftChild = leftChild;
	temp->rightChild = rightChild;
	temp->alphabet = alphabet;
	temp->freq = freq;

	return temp;
}

void newTable(node* node, char str[], int len, char *table[]) {
	if (!node->leftChild && !node->rightChild) {	// ��������� ��� '\0'�� ������ ���� �˷���
		str[len] = '\0';
		strcpy(table[(node->alphabet) - 'A'], str);	// ���̺� �ش� ���ڿ� ���� ����� �ڵ带 �־���
	}
	else {
		if (node->leftChild) {	// ���� �ڽ��� ������ 0�� ����
			str[len] = '0';
			newTable(node->leftChild, str, len + 1, table);
		}
		if (node->rightChild) {	// ������ �ڽ��� ������ 1�� ����
			str[len] = '1';
			newTable(node->rightChild, str, len + 1, table);
		}
	}
}

node *huffmanTree(char arr[]) {
	int i = 0, j = 0, min = 0, lessmin = 0;	// ���� �󵵼��� ���� min�� 2��°�� ���� lessmin
	int freq[ALPHALEN] = { 0 }, isMerged[ALPHALEN] = { 0 };	// ���ĺ��� ��, ��尡 ���������� Ȯ���ϴ� �迭��
	node *tree[ALPHALEN] = { NULL }; // ���� ��� �迭

	while (arr[i] != NULL)	// ���ĺ��� �󵵼��� ���Ͽ� ����
		freq[arr[i++] - 'A']++;

	for (i = 0; i < ALPHALEN; i++) {
		if (freq[i]) {	// ���忡 �ش� ���ĺ��� �����Ѵٸ� ���迭�� �� ���迭�� ����
			nodeArray[charnum] = *newNode(i + 'A', freq[i], NULL, NULL);
			tree[charnum++] = newNode(i + 'A', freq[i], NULL, NULL);
		}
	}

	for (i = 0; i < charnum - 1; i++) {
		j = 0;
		while (isMerged[j] == -1)	// ������ ��� �����ϰ� �迭�� ���� �� �ε����� ã��
			j++;
		min = j;	// �ּҰ����� ����

		for (j = min; j < charnum - 1; j++) {
			if (isMerged[j] != -1) {	// ��尡 ������ ��찡 �ƴ� �� �󵵼��� ���Ͽ� ���� ����
				if (tree[min]->freq > tree[j]->freq)
					min = j;
			}
		}

		j = 0;
		while (isMerged[j] == -1 || j == min)	// ������ ���� �ּҰ��� ���� ��带 �����ϰ� ���� �� �ε����� ã��
			j++;
		lessmin = j;	// �ι�° �ּҰ����� ����

		for (j = lessmin; j < charnum; j++) {
			if (isMerged[j] != -1) {	// ��尡 ������ ��찡 �ƴ� �� �󵵼��� ���Ͽ� ���� ����
				if (tree[lessmin]->freq > tree[j]->freq) {
					if (j != min)
						lessmin = j;
				}
			}	
		}

		tree[min] = newNode(NULL, tree[min]->freq + tree[lessmin]->freq, tree[min], tree[lessmin]);	// �ּҰ��� �ι�° �ּҰ��� �ε��� ����� �󵵼��� ��ģ �󵵼��� ���� ��带 ����� �� ��带 �ּҰ� �ε����� ����
		isMerged[lessmin] = -1; // �������� ������ -1�� üũ
	}
	return tree[min];
}

void decode(char *str, node *root) {
	int i = 0;
	node* temp = root;

	while (str[i] != '\0') { // �������� '\0'�� ã�� �� ���� �ݺ�
		if (str[i] == '0')	// ����� ���� 0�� ��� ���� �ڽ����� �̵�
			temp = temp->leftChild;
		else if (str[i] == '1')	// ����� ���� 1�� ��� ������ �ڽ����� �̵�
			temp = temp->rightChild;

		if (!temp->leftChild && !temp->rightChild) {	// ��������� ��� ����� ���ĺ��� ���
			cout << temp->alphabet;
			temp = root;
		}
		i++;
	}
	cout << endl;
}
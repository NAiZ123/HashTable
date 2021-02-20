#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "hash-table.h"
#include <stdbool.h>

#define SIZE 20

/* �����N���ꂽ���X�g�ɍ��ڂ��i�[���邽�߂̃m�[�h�̒�` */
struct node
{
	int key;
	int data;
	struct node* next;
};
/* �n�b�V���e�[�u���̊e�C���f�b�N�X�Ƀ����N���ꂽ���X�g���i�[����ꍇ */
struct arrayitem
{
	struct node* head;  //�����N���ꂽ���X�g�̐擪�v�f���n�b�V���e�[�u���̃C���f�b�N�X�Ɏw��
	struct node* tail;  //�n�b�V���e�[�u���̃C���f�b�N�X�Ƀ����N���ꂽ���X�g�̍Ō�̗v�f���w��
};

struct arrayitem* HashArray;
int size = 0;   //�n�b�V���e�[�u���v�f��
int max = SIZE; //�n�b�V���e�[�u���ő�v�f��

//struct node *item; //�����p
struct node* item;

/* �n�b�V���֐��̒�` */
int HashCode(int key)
{
	return key % SIZE;
}

struct node* get_element(struct node* list, int find_index);
void remove_element(int key);
void rehash();
void init_array();

/* �}���̂��߂̊֐� chaining */
void insert(int key, int data)
{
	float n = 0.0; //���n�b�V���̗L��

	int HashIndex = HashCode(key);

	/* �w�肳�ꂽ�C���f�b�N�X���烊���N���ꂽ���X�g�𒊏o���� */
	struct node* list = (struct node*)HashArray[HashIndex].head;

	/*�n�b�V���e�[�u���ɑ}�����鍀�ڂ̍쐬*/
	struct node* item = (struct node*)malloc(sizeof(struct node) * max);
	item->key = key;
	item->data = data;
	item->next = NULL;

	if (list == NULL)
	{
		/* �n�b�V���e�[�u���̎w��C���f�b�N�X�Ƀ����N���ꂽ���X�g�����݂��Ȃ��ꍇ */
		HashArray[HashIndex].head = item;
		HashArray[HashIndex].tail = item;
		size++;
	}
	else {
		/* �n�b�V���e�[�u���̎w��C���f�b�N�X�Ƀ����N���ꂽ���X�g�����݂���ꍇ�@*/
		int find_index = find(list, key);
		if (find_index == -1) {
			// �����̃����N�惊�X�g��key���Ȃ��ꍇ�A�����N�惊�X�g�̍Ō�ɃL�[��ǉ�����
			HashArray[HashIndex].tail->next = item;
			HashArray[HashIndex].tail = item;
			size++;
		}
		else {
			// �����N��̃��X�g�Ɋ��ɃL�[�����݂���ꍇ�A�����̃L�[�̒l���X�V����
			struct node* element = get_element(list, find_index);
			element->data = data;
		}

	}
	n = (1.0 * size) / max;
	if (n >= 0.85) {
		//rehashing
		printf("going to rehashing\n");
		rehash();
	}
	free(item);
}


void rehash()
{
	struct arrayitem* temp = HashArray;
	/* temp pointing to the current Hash Table array */

	int i = 0, n = max;
	size = 0;
	max = 2 * max;

	/*
	 *array variable is assigned with newly created Hash Table
	 *with double of previous array size
	*/
	HashArray = (struct arrayitem*)malloc(max * sizeof(struct node));
	init_array();

	for (i = 0; i < n; i++)
	{

		/* Extracting the Linked List at position i of Hash Table array */
		struct node* list = (struct node*)temp[i].head;

		if (list == NULL)
		{

			/* if there is no Linked List, then continue */
			continue;

		}
		else
		{
			/*
			 *Presence of Linked List at i
			 *Keep moving and accessing the Linked List item until the end.
			 *Get one key and value at a time and add it to new Hash Table array.
			*/

			while (list != NULL)
			{
				insert(list->key, list->data);
				list = list->next;
			}
		}
	}
	temp = NULL;
	free(HashArray);
}


int find(struct node* list, int key)
{
	int retval = 0;
	struct node* temp = list;
	while (temp != NULL)
	{
		if (temp->key == key)
		{
			return retval;
		}
		temp = temp->next;
		retval++;
	}
	return -1;

}

/* �w�肳�ꂽfind_index�ɂ���m�[�h�i�����N���ꂽ���X�g���ځj��Ԃ��@*/
struct node* get_element(struct node* list, int find_index)
{
	int i = 0;
	struct node* temp = list;
	while (i != find_index)
	{
		temp = temp->next;
		i++;
	}
	return temp;
}


/* To remove an element from Hash Table */
void remove_element(int key)
{
	int index = HashCode(key);
	struct node* list = (struct node*)HashArray[index].head;

	if (list == NULL)
	{
		printf("This key does not exists\n");

	}
	else
	{
		int find_index = find(list, key);

		if (find_index == -1)
		{
			printf("This key does not exists\n");

		}
		else
		{
			struct node* temp = list;
			if (temp->key == key)
			{

				HashArray[index].head = temp->next;
				printf("This key has been removed\n");
				return;
			}

			while (temp->next->key != key)
			{
				temp = temp->next;
			}

			if (HashArray[index].tail == temp->next)
			{
				temp->next = NULL;
				HashArray[index].tail = temp;
			}
			else
			{
				temp->next = temp->next->next;
			}
			printf("This key has been removed\n");
		}
	}
}

/* �����̂��߂̊֐� */
/* liner Probing���g���Ă���̂ł���ɍ��킹�������������ł��B */
/*
struct DataItem* search(int key)
{
	//get the hash
	int hashIndex = hashCode(key);

	//move in array until an empty
	while (hashArray[hashIndex] != NULL)
	{

		if (hashArray[hashIndex]->key == key)
			return hashArray[hashIndex];

		//go to next cell
		++hashIndex;

		//wrap around the table
		hashIndex %= SIZE;
	}

	return NULL;
}
*/



/* �n�b�V���e�[�u���\���o�� */
void display()
{
	int i = 0;
	for (i = 0; i < max; i++)
	{
		struct node* temp = HashArray[i].head;
		if (temp == NULL)
		{
			printf("array[%d] -- \n", i);

		}
		else
		{
			printf("array[%d] has elements-: ", i);
			while (temp != NULL)
			{
				printf("key= %d  value= %d\t", temp->key, temp->data);
				temp = temp->next;
			}
			printf("\n");

		}
	}
}

/*  �n�b�V���e�[�u�������� */
void init_array()
{
	int i = 0;
	for (i = 0; i < max; i++)
	{
		HashArray[i].head = NULL;
		HashArray[i].tail = NULL;
	}

}

int main()
{
	int choice, key, data, c = 0;

	HashArray = (struct arrayitem*)malloc(sizeof(struct arrayitem*) * max);
	init_array();

	insert(1, 20);
	insert(2, 70);
	insert(42, 80);
	insert(4, 25);
	insert(12, 44);
	insert(14, 32);
	insert(17, 11);
	insert(13, 78);
	insert(37, 97);
	insert(5, 44);

	printf("MENU-: \n1.Inserting item in the Hash Table"
		"\n2.Removing item from the Hash Table"
		"\n3.search Hash Table"
		"\n4.To display a Hash Table"
		"\n\n Please enter your choice -: ");

	scanf("%d", &choice);

	switch (choice)
	{
	case 1:

		printf("Inserting element in Hash Table\n");
		printf("Enter key and value-:\t");
		scanf("%d %d", &key, &data);
		insert(key, data);

		break;

	case 2:

		printf("Deleting in Hash Table \nEnter the key to delete-:");
		scanf("%d", &key);
		remove_element(key);

		break;

	case 3:
		printf("input search : ");
		scanf("%d", &key);
		//  = search(key);
		// printf(" Element found : %d\n", );

		break;

	case 4:

		display();
		break;

	default:

		printf("Wrong Input\n");

	}

	free(HashArray);
	getch();

}
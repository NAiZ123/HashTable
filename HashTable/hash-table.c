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

struct arrayitem *HashArray;
int size = 0;   //�n�b�V���e�[�u���v�f��
int max = SIZE; //�n�b�V���e�[�u���ő�v�f��

//struct node *item; //�����p

/* �n�b�V���֐��̒�` */
int hashCode(int key)
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

    /*�n�b�V���e�[�u���ɑ}�����鍀�ڂ̍쐬*/
    struct node* item = (struct node*)malloc(sizeof(struct node));
    item->key = key;
    item->data = data;
    item->next = NULL;

    int HashIndex = hashcode(key);

    /* �w�肳�ꂽ�C���f�b�N�X���烊���N���ꂽ���X�g�𒊏o���� */
    struct node* list = (struct node*)HashArray[HashIndex].head;

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





/* �����̂��߂̊֐� */
/* liner Probing���g���Ă���̂ł���ɍ��킹�������������ł��B */
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




struct DataItem* delete (struct DataItem* item)
{
    int key = item->key;

    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty
    while (hashArray[hashIndex] != NULL)
    {

        if (hashArray[hashIndex]->key == key)
        {
            struct DataItem* temp = hashArray[hashIndex];

            //assign a dummy item at deleted position
            hashArray[hashIndex] = dummyItem;
            return temp;
        }

        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    return NULL;
}

void display()
{
    int i = 0;

    for (i = 0; i < SIZE; i++)
    {

        if (hashArray[i] != NULL)
            printf(" hashArray[%d]-> (%d,%d) \n", i, hashArray[i]->key, hashArray[i]->data);
        else
            printf(" ~~ \n");
    }

    printf("\n");
}

int main()
{
    dummyItem = (struct DataItem*)malloc(sizeof(struct DataItem));
    dummyItem->data = -1;
    dummyItem->key = -1;

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

    display();
    item = search(37);

    if (item != NULL)
    {
        printf("Element found: %d\n", item->data);
    }
    else
    {
        printf("Element not found\n");
    }

    delete (item);
    item = search(13);

    if (item != NULL)
    {
        printf("Element found: %d\n", item->data);
    }
    else
    {
        printf("Element not found\n");
    }
}
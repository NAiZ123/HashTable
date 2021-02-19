#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "hash-table.h"
#include <stdbool.h>

#define SIZE 20

/* リンクされたリストに項目を格納するためのノードの定義 */
struct node
{
    int key;
    int data;
    struct node* next;
};
/* ハッシュテーブルの各インデックスにリンクされたリストを格納する場合 */
struct arrayitem
{
    struct node* head;  //リンクされたリストの先頭要素をハッシュテーブルのインデックスに指す
    struct node* tail;  //ハッシュテーブルのインデックスにリンクされたリストの最後の要素を指す
};

struct arrayitem *HashArray;
int size = 0;   //ハッシュテーブル要素数
int max = SIZE; //ハッシュテーブル最大要素数

//struct node *item; //検索用

/* ハッシュ関数の定義 */
int hashCode(int key)
{
    return key % SIZE;
}

struct node* get_element(struct node* list, int find_index);
void remove_element(int key);
void rehash();
void init_array();

/* 挿入のための関数 chaining */
void insert(int key, int data)
{
    float n = 0.0; //リハッシュの有無

    /*ハッシュテーブルに挿入する項目の作成*/
    struct node* item = (struct node*)malloc(sizeof(struct node));
    item->key = key;
    item->data = data;
    item->next = NULL;

    int HashIndex = hashcode(key);

    /* 指定されたインデックスからリンクされたリストを抽出する */
    struct node* list = (struct node*)HashArray[HashIndex].head;

    if (list == NULL)
    {
        /* ハッシュテーブルの指定インデックスにリンクされたリストが存在しない場合 */
        HashArray[HashIndex].head = item;
        HashArray[HashIndex].tail = item;
        size++;
    }
    else {
        /* ハッシュテーブルの指定インデックスにリンクされたリストが存在する場合　*/
        int find_index = find(list, key);
        if (find_index == -1) {
            // 既存のリンク先リストにkeyがない場合、リンク先リストの最後にキーを追加する
            HashArray[HashIndex].tail->next = item;
            HashArray[HashIndex].tail = item;
            size++;
        }
        else {
            // リンク先のリストに既にキーが存在する場合、既存のキーの値を更新する
            struct node* element = get_element(list, find_index);
            element->data = data;
        }

    }
    
}

/* 指定されたfind_indexにあるノード（リンクされたリスト項目）を返す　*/
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





/* 検索のための関数 */
/* liner Probingを使っているのでそれに合わせた検索を実装です。 */
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
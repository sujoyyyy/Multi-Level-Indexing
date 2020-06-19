#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#define Order 5   //You can change the order of the B-Tree from here.

typedef struct _node {
    int    n; 
    int  keys[Order - 1]; 
    struct _node* p[Order]; 
} node;
node* root = NULL;

typedef enum KeyStatus {
    dupl,
    sf ,
    Suc ,
    insit ,
    LK ,
} KeyStatus;

void ins(int k);
void Del(int x);
void inor(node* root);
void display(node* root, int);
KeyStatus ins(node* r, int x, int* y, node** u);
int searchPos(int x, int* key_arr, int n);
KeyStatus del(node* r, int x);
void eatline(void);

int main() {
    int key;
    int choice;
    printf("Creation of B tree for M=%d (if you want to change the order you can change it from line 4 in the code)\n", Order);
    while (1) {
        printf("1.Insert\n");
        printf("2.Delete\n");
        printf("3.Display\n");
        printf("4.Quit\n");
       
        printf("Enter your choice : ");
        scanf("%d", &choice); eatline();

        switch (choice) {
        case 1:
            printf("Enter the key : ");
            scanf("%d", &key); eatline();
            ins(key);
            break;
        case 2:
            printf("Enter the key : ");
            scanf("%d", &key); eatline();
            Del(key);
            break;
        
        case 3:
            printf("Btree is :\n");
            display(root,Order);
            break;
        case 4:
            exit(1);
        
        default:
            printf("Wrong choice\n");
            break;
        }
    }
    return 0;
}

int searchPos(int key, int* key_arr, int n) 
{
    int pos = 0;
    while (pos < n && key > key_arr[pos])
        pos++;
    return pos;
}

void Del(int key) { //Initiating deletion
    node* uproot;
    KeyStatus value;
    value = del(root, key);
    switch (value) {
    case sf:
        printf("Key %d is not available\n", key);
        break;
    case LK:
        uproot = root;
        root = root->p[0];
        free(uproot);
        break;
    default:
        return;
    }
}
KeyStatus ins(node* ptr, int key, int* upKey, node** newnode) { //Insertion
    node* newPtr, * lastPtr;
    int pos, i, n, splitPos;
    int newKey, lastKey;
    KeyStatus value;
    if (ptr == NULL) {
        *newnode = NULL;
        *upKey = key;
        return insit;
    }
    n = ptr->n;
    pos = searchPos(key, ptr->keys, n);
    if (pos < n && key == ptr->keys[pos])
        return dupl;
    value = ins(ptr->p[pos], key, &newKey, &newPtr);
    if (value != insit)
        return value;

    if (n < Order - 1) {
        pos = searchPos(newKey, ptr->keys, n);

        for (i = n; i > pos; i--) 
		{
            ptr->keys[i] = ptr->keys[i - 1];
            ptr->p[i + 1] = ptr->p[i];
        }
        ptr->keys[pos] = newKey;
        ptr->p[pos + 1] = newPtr;
        ++ptr->n; 
        return Suc;
    }
    if (pos == Order - 1) {
        lastKey = newKey;
        lastPtr = newPtr;
    }
    else { 
        lastKey = ptr->keys[Order - 2];
        lastPtr = ptr->p[Order - 1];
        for (i = Order - 2; i > pos; i--) {
            ptr->keys[i] = ptr->keys[i - 1];
            ptr->p[i + 1] = ptr->p[i];
        }
        ptr->keys[pos] = newKey;
        ptr->p[pos + 1] = newPtr;
    }
    splitPos = (Order - 1) / 2;
    (*upKey) = ptr->keys[splitPos];

    (*newnode) = (node*)malloc(sizeof(node));
    ptr->n = splitPos; 
    (*newnode)->n = Order - 1 - splitPos;
    for (i = 0; i < (*newnode)->n; i++) {
        (*newnode)->p[i] = ptr->p[i + splitPos + 1];
        if (i < (*newnode)->n - 1)
            (*newnode)->keys[i] = ptr->keys[i + splitPos + 1];
        else
            (*newnode)->keys[i] = lastKey;
    }
    (*newnode)->p[(*newnode)->n] = lastPtr;
    return insit;
}

KeyStatus del(node* ptr, int key) //Deletion
{
    int pos, i, pivot, n, min;
    int* key_arr;
    KeyStatus value;
    node** p, * lptr, * rptr;

    if (ptr == NULL)
        return sf;
    
    n = ptr->n;
    key_arr = ptr->keys;
    p = ptr->p;
    min = (Order - 1) / 2;
    pos = searchPos(key, key_arr, n);
    if (p[0] == NULL) {
        if (pos == n || key < key_arr[pos])
            return sf;
        for (i = pos + 1; i < n; i++)
        {
            key_arr[i - 1] = key_arr[i];
            p[i] = p[i + 1];
        }
        return --ptr->n >= (ptr == root ? 1 : min) ? Suc : LK;
    }
    if (pos < n && key == key_arr[pos]) {
        node* qp = p[pos], * qp1;
        int nkey;
        while (1) {
            nkey = qp->n;
            qp1 = qp->p[nkey];
            if (qp1 == NULL)
                break;
            qp = qp1;
        }
        key_arr[pos] = qp->keys[nkey - 1];
        qp->keys[nkey - 1] = key;
    }
    value = del(p[pos], key);
    if (value != LK)
        return value;

    if (pos > 0 && p[pos - 1]->n > min) {
        pivot = pos - 1; 
        lptr = p[pivot];
        rptr = p[pos];
        rptr->p[rptr->n + 1] = rptr->p[rptr->n];
        for (i = rptr->n; i > 0; i--) {
            rptr->keys[i] = rptr->keys[i - 1];
            rptr->p[i] = rptr->p[i - 1];
        }
        rptr->n++;
        rptr->keys[0] = key_arr[pivot];
        rptr->p[0] = lptr->p[lptr->n];
        key_arr[pivot] = lptr->keys[--lptr->n];
        return Suc;
    }
    if (pos < n && p[pos + 1]->n > min) {
        pivot = pos; 
        lptr = p[pivot];
        rptr = p[pivot + 1];
        lptr->keys[lptr->n] = key_arr[pivot];
        lptr->p[lptr->n + 1] = rptr->p[0];
        key_arr[pivot] = rptr->keys[0];
        lptr->n++;
        rptr->n--;
        for (i = 0; i < rptr->n; i++) {
            rptr->keys[i] = rptr->keys[i + 1];
            rptr->p[i] = rptr->p[i + 1];
        }
        rptr->p[rptr->n] = rptr->p[rptr->n + 1];
        return Suc;
    }

    if (pos == n)
        pivot = pos - 1;
    else
        pivot = pos;

    lptr = p[pivot];
    rptr = p[pivot + 1];
    lptr->keys[lptr->n] = key_arr[pivot];
    lptr->p[lptr->n + 1] = rptr->p[0];
    for (i = 0; i < rptr->n; i++) {
        lptr->keys[lptr->n + 1 + i] = rptr->keys[i];
        lptr->p[lptr->n + 2 + i] = rptr->p[i + 1];
    }
    lptr->n = lptr->n + rptr->n + 1;
    free(rptr);
    for (i = pos + 1; i < n; i++) {
        key_arr[i - 1] = key_arr[i];
        p[i] = p[i + 1];
    }
    return --ptr->n >= (ptr == root ? 1 : min) ? Suc : LK;
}
void ins(int key) //Initiating Insertion
{
    node* nnode;
    int up;
    KeyStatus value;
    value = ins(root, key, &up, &nnode);
    if (value == dupl )
        printf("Duplicate\n");
    if (value == insit) {
        node* upr = root;
        root = (node*)malloc(sizeof(node));
        root->n = 1;
        root->keys[0] = up;
        root->p[0] = upr;
        root->p[1] = nnode;
    }
}
void eatline(void) {
    char c;
    while ((c = getchar()) != '\n');
}

void display(node* ptr, int blanks) {
    if (ptr) {
        int i;
        for (i = 1; i <= blanks; i++)
            printf(" ");
        for (i = 0; i < ptr->n; i++)
            printf("%d ", ptr->keys[i]);
        printf("\n");
        for (i = 0; i <= ptr->n; i++)
            display(ptr->p[i], blanks + 10);
    }
}



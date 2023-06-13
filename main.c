
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_LENGTH 100
#define MAX_CARS 512

/***************************************
 *    CARS STRUCTURE AND FUNCTIONS
 ***************************************/
//create bucket car for hash table
typedef struct node_car {
    int autonomy;
    struct node_car *next;
} CarNode;

typedef struct HashTable {
    CarNode *table[MAX_CARS];
} HashTable;

//initialize hash table
void init_hash_table(HashTable *hash_table) {
    for (int i = 0; i < MAX_CARS; i++) {
        hash_table->table[i] = NULL;
    }
}

unsigned hash(int autonomy) {
    unsigned int a = 2654435769;
    unsigned int hash_value = autonomy * a;
    hash_value = hash_value >> (32 - (int)log2(MAX_CARS));
    return hash_value;
}

//insert car in hash table
void insert_car_hash_table(HashTable *hash_table, int autonomy) {
    unsigned int hash_value = hash(autonomy);
    CarNode *new_node = (CarNode *)malloc(sizeof(CarNode));
    new_node->autonomy = autonomy;
    new_node->next = NULL;

    if (hash_table->table[hash_value] == NULL) {
        hash_table->table[hash_value] = new_node;
    } else {
        CarNode *current = hash_table->table[hash_value];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_node;
    }
    printf("autonomia: %d\n", autonomy);
}

//delete car from hash table
void delete_car_hash_table(HashTable *hash_table, int autonomy) {
    unsigned int hash_value = hash(autonomy);
    CarNode *current = hash_table->table[hash_value];
    CarNode *previous = NULL;

    while (current != NULL && current->autonomy != autonomy) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        puts("non rottamata\n");
    } else {
        if (previous == NULL) {
            hash_table->table[hash_value] = current->next;
        } else {
            previous->next = current->next;
        }
        puts("rottamata\n");
        free(current);
    }
}

//return the maximum autonomy of the cars in the hash table
int max_autonomy(HashTable *hash_table) {
    int max = 0;
    for (int i = 0; i < MAX_CARS; i++) {
        CarNode *current = hash_table->table[i];
        while (current != NULL) {
            if (current->autonomy > max) {
                max = current->autonomy;
            }
            current = current->next;
        }
    }
    return max;
}

//print hash table
void print_hash_table(HashTable *hash_table) {
    for (int i = 0; i < MAX_CARS; i++) {
        CarNode *current = hash_table->table[i];
        printf("Bucket %d: ", i);
        while (current != NULL) {
            printf("%d ", current->autonomy);
            current = current->next;
        }
        printf("\n");
    }
}

//delete hash table
void delete_hash_table(HashTable *hash_table) {
    for (int i = 0; i < MAX_CARS; i++) {
        CarNode *current = hash_table->table[i];
        while (current != NULL) {
            CarNode *temp = current;
            current = current->next;
            free(temp);
        }
    }
}

/***************************************
 *    STATIONS STRUCTURE AND FUNCTIONS
 ***************************************/
typedef struct node_station {
    int distance;
    int max_autonomy;
    HashTable *cars;
    struct node_station *left;
    struct node_station *right;
} StationNode;

//insert station in binary search tree
void * insert_station(StationNode **root, int distance, StationNode **station) {
    if (*root == NULL) {
        StationNode *new_node = (StationNode *)malloc(sizeof(StationNode));
        new_node->distance = distance;
        new_node->max_autonomy = 0;
        new_node->cars = (HashTable *)malloc(sizeof(HashTable));
        init_hash_table(new_node->cars);
        new_node->left = NULL;
        new_node->right = NULL;
        *root = new_node;
        printf("aggiunta\n");
        *station = new_node;
    } else {
        if (distance < (*root)->distance) {
            insert_station(&(*root)->left, distance, station);
        } else if (distance > (*root)->distance) {
            insert_station(&(*root)->right, distance, station);
        } else {
            printf("non aggiunta\n");
            *station = *root;
        }
    }
}

//delete station from binary search tree
void delete_station(StationNode **root, int distance) {
    if (*root == NULL) {
        printf("non demolita\n");
    } else {
        if (distance < (*root)->distance) {
            delete_station(&(*root)->left, distance);
        } else if (distance > (*root)->distance) {
            delete_station(&(*root)->right, distance);
        } else {
            if ((*root)->left == NULL && (*root)->right == NULL) {
                free(*root);
                *root = NULL;
            } else if ((*root)->left == NULL) {
                StationNode *temp = *root;
                *root = (*root)->right;
                free(temp);
            } else if ((*root)->right == NULL) {
                StationNode *temp = *root;
                *root = (*root)->left;
                free(temp);
            } else {
                StationNode *temp = (*root)->right;
                while (temp->left != NULL) {
                    temp = temp->left;
                }
                (*root)->distance = temp->distance;
                delete_station(&(*root)->right, temp->distance);
            }
            printf("demolita\n");
        }
    }
}

//search station in binary search tree
StationNode * search_station(StationNode *root, int distance) {
    if (root == NULL) {
        return NULL;
    } else {
        if (distance < root->distance) {
            return search_station(root->left, distance);
        } else if (distance > root->distance) {
            return search_station(root->right, distance);
        } else {
            return root;
        }
    }
}

//delete binary search tree
void delete_tree(StationNode **root) {
    if (*root != NULL) {
        delete_tree(&(*root)->left);
        delete_tree(&(*root)->right);
        if((*root)->cars != NULL) {
            delete_hash_table((*root)->cars);
            free((*root)->cars);
        }
        free(*root);
        *root = NULL;
    }
}

//print binary search tree
void print_tree(StationNode *root) {
    if (root != NULL) {
        print_tree(root->left);
        printf("%d ", root->distance);
        print_tree(root->right);
    }
}

//delete car from hash table
void delete_car(StationNode *root, int distance, int autonomy) {
    StationNode *station = search_station(root, distance);
    if (station != NULL) {
        delete_car_hash_table(station->cars, autonomy);
    }
}

//print the maximum autonomy for each station
void print_max_autonomy(StationNode *root) {
    if (root != NULL) {
        print_max_autonomy(root->left);
        printf("\nStazione %d, max autonomy %d", root->distance, root->max_autonomy);
        print_max_autonomy(root->right);
    }
}

/***************************************
 *    MAIN
 ***************************************/
int main() {
    char *input = (char *)malloc(sizeof(char) * MAX_LENGTH);
    char *token;
    StationNode *root = NULL;
    StationNode *station = NULL;
    int *path = NULL;

    //Read input file
    while(fgets(input, MAX_LENGTH, stdin) != NULL) {
        //read input line
        input[strcspn(input, "\n")] = '\0';
        //tokenize input line
        token = strtok(input, " ");

        while (token != NULL) {
            //Switch based on first word of input line
            if (strcmp(token, "aggiungi-stazione") == 0) {
                puts("Esegui azione: aggiungi-stazione\n");
                //read the distance
                char * distance = strtok(NULL, " ");
                //insert station
                insert_station(&root, atoi(distance), &station);
                printf("Stazione distanza %d \n", station->distance);
                //read the number of cars
                token = strtok(NULL, " ");
                if(atoi(token) == 0) break;
                //read the autonomy of each car
                char * autonomy = strtok(NULL, " ");
                while (autonomy != NULL) {
                    //insert_car(root, atoi(distance), atoi(token));
                    insert_car_hash_table(station->cars, atoi(autonomy));
                    if(station->max_autonomy < atoi(autonomy)) {
                        station->max_autonomy = atoi(autonomy);
                    }
                    autonomy = strtok(NULL, " ");
                }
                //print_hash_table(station->cars);
            } else if (strcmp(token, "demolisci-stazione") == 0) {
                printf("Esegui azione: demolisci-stazione\n");
                token = strtok(NULL, " ");
                //delete station
                delete_car_hash_table(station->cars, atoi(token));
                delete_station(&root, atoi(token));
            } else if (strcmp(token, "aggiungi-auto") == 0) {
                printf("Esegui azione: aggiungi-auto\n");
                //read the distance
                char * distance = strtok(NULL, " ");
                //search station
                station = search_station(root, atoi(distance));
                if(station != NULL) {
                    printf("Stazione distanza %d \n", station->distance);
                    //read the autonomy
                    token = strtok(NULL, " ");
                    //insert car
                    insert_car_hash_table(station->cars, atoi(token));
                    if(station->max_autonomy < atoi(token)) {
                        station->max_autonomy = atoi(token);
                    }
                    puts("aggiunta");
                } else {
                    puts("non aggiunta");
                    token = strtok(NULL, " ");
                    break;
                }
            } else if (strcmp(token, "rottama-auto") == 0) {
                printf("Esegui azione: rottama-auto\n");
                //read the distance
                char * distance = strtok(NULL, " ");
                //search station
                station = search_station(root, atoi(distance));
                if(station != NULL) {
                    //read the autonomy
                    token = strtok(NULL, " ");
                    //delete car
                    delete_car_hash_table(station->cars, atoi(token));
                    if(station->max_autonomy == atoi(token)){
                        station->max_autonomy = max_autonomy(station->cars);
                    }
                } else {
                    puts("non rottamata");
                    token = strtok(NULL, " ");
                    break;
                }
            } else if (strcmp(token, "pianifica-percorso") == 0) {
                printf("Esegui azione: pianifica-percorso\n");
                int start = atoi(strtok(NULL, " "));
                int end = atoi(strtok(NULL, " "));
                if(start == end) {
                    printf("%d", start);
                    break;
                }
                //search path from begin to end
                if(start > end) {
                    StationNode *start_node = search_station(root, start);
                    int max_distance = start_node->distance + start_node->max_autonomy;

                }
                //search path from end to begin
                else {

                }
            }
            token = strtok(NULL, " ");
        } //end of line
    } //end of file

    //print_tree(root);
    //print_max_autonomy(root);

    //free memory
    free(input);
    free(token);
    delete_tree(&root);
    return 0;
}
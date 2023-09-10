
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 10000

/***************************************
 *    STATIONS STRUCTURE AND FUNCTIONS
 ***************************************/
typedef struct node_station {
    int distance;
    int max_autonomy;
    int *cars;
    int size;
    int capacity;
    struct node_station *left;
    struct node_station *right;
} StationNode;

//insert station in binary search tree
void * insert_station(StationNode **root, int distance, StationNode **station) {
    if (*root == NULL) {
        StationNode *new_node = (StationNode *)malloc(sizeof(StationNode));
        new_node->distance = distance;
        new_node->max_autonomy = 0;
        new_node->size = 0;
        new_node->capacity = 0;
        new_node->cars = NULL;
        new_node->left = NULL;
        new_node->right = NULL;
        *root = new_node;
        puts("aggiunta");
        *station = new_node;
    } else {
        if (distance < (*root)->distance) {
            insert_station(&(*root)->left, distance, station);
        } else if (distance > (*root)->distance) {
            insert_station(&(*root)->right, distance, station);
        } else {
            *station = *root;
        }
    }
    return NULL;
}

//find min function for binary search tree
StationNode * find_min(StationNode *root) {
    if (root == NULL) {
        return NULL;
    } else if (root->left == NULL) {
        return root;
    } else {
        return find_min(root->left);
    }
}

//delete a station from binary search tree
void delete_station(StationNode **root, int distance) {
    if (*root != NULL) {
        if (distance < (*root)->distance) {
            delete_station(&(*root)->left, distance);
        } else if (distance > (*root)->distance) {
            delete_station(&(*root)->right, distance);
        } else {
            if ((*root)->left == NULL && (*root)->right == NULL) {
                free((*root)->cars);
                (*root)->cars = NULL;
                (*root)->size = 0;
                (*root)->capacity = 0;
                free(*root);
                *root = NULL;
            } else if ((*root)->left == NULL) {
                StationNode *temp = *root;
                *root = (*root)->right;
                free(temp->cars);
                temp->cars = NULL;
                temp->size = 0;
                temp->capacity = 0;
                free(temp);
            } else if ((*root)->right == NULL) {
                StationNode *temp = *root;
                *root = (*root)->left;
                free(temp->cars);
                temp->cars = NULL;
                temp->size = 0;
                temp->capacity = 0;
                free(temp);
            } else {
                StationNode *temp = find_min((*root)->right);
                free((*root)->cars);
                (*root)->cars = NULL;
                (*root)->size = 0;
                (*root)->capacity = 0;
                (*root)->distance = temp->distance;
                (*root)->max_autonomy = temp->max_autonomy;
                (*root)->size = temp->size;
                (*root)->capacity = temp->capacity;
                (*root)->cars = (int *)malloc(sizeof(int) * (*root)->capacity);
                for (int i = 0; i < temp->size; i++) {
                    (*root)->cars[i] = temp->cars[i];
                }
                delete_station(&(*root)->right, temp->distance);
            }
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
        free((*root)->cars);
        (*root)->cars = NULL;
        (*root)->size = 0;
        (*root)->capacity = 0;
        free(*root);
        *root = NULL;
    }
}

StationNode* get_next_station(StationNode* root, StationNode* station) {
    if (root == NULL) {
        return NULL;
    } else {
        if (station->distance < root->distance) {
            StationNode* temp = get_next_station(root->left, station);
            if (temp == NULL) {
                return root;
            } else {
                return temp;
            }
        } else {
            return get_next_station(root->right, station);
        }
    }
}

StationNode * get_previous_station(StationNode *root, StationNode *station) {
    if (root == NULL) {
        return NULL;
    } else {
        if (station->distance <= root->distance) {
            return get_previous_station(root->left, station);
        } else if (station->distance > root->distance) {
            StationNode *temp = get_previous_station(root->right, station);
            if (temp == NULL) {
                return root;
            } else {
                return temp;
            }
        } else {
            return root;
        }
    }
}

StationNode * get_previous_station_based_on_distance(StationNode *root, int distance) {
    if (root == NULL) {
        return NULL;
    } else {
        if (distance < root->distance) {
            return get_previous_station_based_on_distance(root->left, distance);
        } else if (distance > root->distance) {
            StationNode *temp = get_previous_station_based_on_distance(root->right, distance);
            if (temp == NULL) {
                return root;
            } else {
                return temp;
            }
        } else {
            return root;
        }
    }
}

StationNode * get_next_station_based_on_max_autonomy(StationNode *root, StationNode *station) {
    if (root == NULL) {
        return NULL;
    } else {
        if (station->distance - station->max_autonomy < root->distance) {
            StationNode *temp = get_next_station_based_on_max_autonomy(root->left, station);
            if (temp == NULL) {
                return root;
            } else {
                return temp;
            }
        } else if (station->distance - station->max_autonomy > root->distance) {
            return get_next_station_based_on_max_autonomy(root->right, station);
        } else {
            return root;
        }
    }
}

StationNode * get_next_station_generic(StationNode *root, int distance) {
    StationNode *next_station = NULL;

    while (root != NULL) {
        if (distance < root->distance) {
            next_station = root;
            root = root->left;
        } else if (distance > root->distance) {
            root = root->right;
        } else {
            next_station = root;
            break;
        }
    }
    return next_station;
}

StationNode * check_min_station(StationNode * root, int max, StationNode * current, int end) {
    StationNode * min = current;
    int minDistance = current->distance - current->max_autonomy;
    StationNode * next = get_next_station_generic(root, minDistance);

    current = get_next_station(root, current);
    while(current != NULL && current->distance < max) {
        if(current->distance - current->max_autonomy < minDistance && current->distance - current->max_autonomy <= end) {
            return current;
        }
        if(current->distance - current->max_autonomy > minDistance && current->distance - current->max_autonomy > end) {
            current = get_next_station(root, current);
        }
        else {
            StationNode *next_check = get_next_station_generic(root, current->distance - current->max_autonomy);
            if(next_check->distance < next->distance) {
                min = current;
                minDistance = current->distance - current->max_autonomy;
                next = next_check;
            }
            current = get_next_station(root, current);
        }
    }
    return min;
}

//search the station with the distance + max autonomy closest to the distance between two distances
StationNode * check_best_station(StationNode *root, StationNode *start, int end) {
    StationNode * best = start;
    int target = start->distance;

    StationNode * current = start;
    while(current != NULL && current->distance >= end) {
        current = get_previous_station(root, current);
        if(current != NULL && current-> distance >= end && current->distance + current->max_autonomy >= target) {
                best = current;
            }
    }
    if(best->distance == start->distance)
        return NULL;
    else return best;
}

//print the binary search tree
void print_tree(StationNode *root) {
    if (root != NULL) {
        print_tree(root->left);
        printf("\nStazione %d, max autonomy %d, max distance %d, min distance %d", root->distance, root->max_autonomy, root->distance + root->max_autonomy, root->distance - root->max_autonomy);
        print_tree(root->right);
    }
}

//add a car in the station in order from the biggest to the smallest
void add_car(StationNode *s, int car) {
    int i = 0;
    int j = s->size - 1;
    int m = 0;
    while(i <= j) {
        m = (i + j) / 2;
        if(s->cars[m] < car) {
            j = m - 1;
        }
        else if(s->cars[m] > car) {
            i = m + 1;
        }
        else {
            return;
        }
    }
    for(int k = s->size; k > i; k--) {
        s->cars[k] = s->cars[k - 1];
    }
    s->cars[i] = car;
    s->size++;
}

//search and remove a car in the station
void remove_car(StationNode *station, int car) {
    int i = 0;
    int j = station->size - 1;
    int m = 0;
    while(i <= j) {
        m = (i + j) / 2;
        if(station->cars[m] < car) {
            j = m - 1;
        }
        else if(station->cars[m] > car) {
            i = m + 1;
        }
        else {
            for(int k = m; k < station->size - 1; k++) {
                station->cars[k] = station->cars[k + 1];
            }
            station->size--;
            puts("rottamata");
            return;
        }
    }
    puts("non rottamata");
}

//replace a station in an array of stations
void replace_station(StationNode *station, StationNode **stations, int index) {
    (*stations)[index] = *station;
}

/***************************************
 *    MAIN
 ***************************************/
int main() {
    char *input = (char *)malloc(sizeof(char) * MAX_LENGTH);
    char *token = NULL;
    StationNode *root = NULL;
    StationNode *station = NULL;
    StationNode *add = (StationNode *)malloc(sizeof(StationNode));
    int *path = NULL;
    StationNode *path2 = NULL;
    int size = 0;
    int capacity = 0;

    while(fgets(input, MAX_LENGTH, stdin) != NULL) {
        input[strcspn(input, "\n")] = '\0';
        token = strtok(input, " ");

        while (token != NULL) {
            if (strcmp(token, "aggiungi-stazione") == 0) {
                char * distance = strtok(NULL, " ");
                if(search_station(root, atoi(distance)) == NULL) {
                    insert_station(&root, atoi(distance), &station);
                    token = strtok(NULL, " ");
                    if(atoi(token) == 0) break;
                    char * autonomy = strtok(NULL, " ");
                    while (autonomy != NULL) {
                        if(station->size >= station->capacity) {
                            station->capacity = (station->capacity == 0) ? 1 : station->capacity * 2;
                            int *temp = (int *)realloc(station->cars, sizeof(int) * station->capacity);
                            station->cars = temp;
                        }
                        add_car(station, atoi(autonomy));
                        if(station->max_autonomy < atoi(autonomy)) {
                            station->max_autonomy = atoi(autonomy);
                        }
                        autonomy = strtok(NULL, " ");
                    }
                }
                else {
                    puts("non aggiunta");
                    break;
                }
            } else if (strcmp(token, "demolisci-stazione") == 0) {
                token = strtok(NULL, " ");
                station = search_station(root, atoi(token));
                if(station != NULL) {
                    delete_station(&root, atoi(token));
                    puts("demolita");
                }
                else {
                    puts("non demolita");
                    break;
                }
            } else if (strcmp(token, "aggiungi-auto") == 0) {
                char * distance = strtok(NULL, " ");
                station = search_station(root, atoi(distance));
                if(station != NULL) {
                    token = strtok(NULL, " ");
                    if(station->size >= station->capacity) {
                        station->capacity = (station->capacity == 0) ? 1 : station->capacity * 2;
                        int *temp = (int *)realloc(station->cars, sizeof(int) * station->capacity);
                        station->cars = temp;
                    }
                    add_car(station, atoi(token));
                    if(station->max_autonomy < atoi(token)) {
                        station->max_autonomy = atoi(token);
                    }
                    puts("aggiunta");
                } else {
                    puts("non aggiunta");
                    break;
                }
            } else if (strcmp(token, "rottama-auto") == 0) {
                char * distance = strtok(NULL, " ");
                station = search_station(root, atoi(distance));
                if(station != NULL) {
                    token = strtok(NULL, " ");
                    remove_car(station, atoi(token));
                    if(station->max_autonomy == atoi(token)){
                        if(station->size != 0)
                            station->max_autonomy = station->cars[0];
                        else station->max_autonomy = 0;
                    }
                } else {
                    puts("non rottamata");
                    break;
                }
            } else if (strcmp(token, "pianifica-percorso") == 0) {
                int start = atoi(strtok(NULL, " "));
                int end = atoi(strtok(NULL, " "));
                if(start == end) {
                    printf("%d", start);
                    break;
                }
                if(start < end) {
                    if(size >= capacity) {
                        capacity = (capacity == 0) ? 1 : capacity * 2;
                        int *temp = (int *)realloc(path, sizeof(int) * capacity);
                        path = temp;
                    }
                    path[size] = end;
                    size++;

                    StationNode *current = NULL;
                    current = search_station(root, end);
                    current = check_best_station(root, current, start);
                    if(current == NULL) {
                        puts("nessun percorso");
                        size = 0;
                        capacity = 0;
                        break;
                    }
                    while(current != NULL && current->distance > start) {
                        if (current == NULL) {
                            puts("nessun percorso");
                            break;
                        }
                        if(size >= capacity) {
                            capacity = (capacity == 0) ? 1 : capacity * 2;
                            int *temp = (int *)realloc(path, sizeof(int) * capacity);
                            path = temp;
                        }
                        path[size] = current->distance;
                        size++;
                        current = check_best_station(root, current, start);
                    }
                    if(current == NULL) {
                        puts("nessun percorso");
                        size = 0;
                        capacity = 0;
                        break;
                    }
                    if(size >= capacity) {
                        capacity = (capacity == 0) ? 1 : capacity * 2;
                        int *temp = (int *)realloc(path, sizeof(int) * capacity);
                        path = temp;
                    }
                    path[size] = start;
                    size++;

                    for(int i = size - 1; i >= 0; i--) {
                        if(i == 0) printf("%d\n", path[i]);
                        else printf("%d ", path[i]);
                    }
                    size = 0;
                    capacity = 0;
                }
                //search path from end to begin
                else {
                    StationNode *start_node = search_station(root, start);
                    if(size >= capacity) {
                        capacity = (capacity == 0) ? 1 : capacity * 2;
                        StationNode *temp = (StationNode *)realloc(path2, sizeof(StationNode) * capacity);
                        path2 = temp;
                    }
                    add->distance = start_node->distance;
                    add->max_autonomy = start_node->max_autonomy;
                    path2[size] = *add;
                    (size)++;

                    StationNode *current = get_next_station_based_on_max_autonomy(root, start_node);
                    StationNode *prev = current;

                    if(current->distance == start_node->distance) {
                        puts("nessun percorso");
                        size = 0;
                        capacity = 0;
                        break;
                    }
                    current = check_min_station(root, start, current, end);

                    if(size >= capacity) {
                        capacity = (capacity == 0) ? 1 : capacity * 2;
                        StationNode *temp = (StationNode *)realloc(path2, sizeof(StationNode) * capacity);
                        path2 = temp;
                    }
                    add->distance = current->distance;
                    add->max_autonomy = current->max_autonomy;
                    path2[size] = *add;
                    (size)++;

                    StationNode *current1 = NULL;
                    while(current->distance > end) {
                        int max = current->distance;
                        current = get_next_station_based_on_max_autonomy(root, current);

                        if(current1 != NULL && current1->distance == current->distance) {
                            puts("nessun percorso");
                            size = 0;
                            capacity = 0;
                            break;
                        }
                        else if(current->distance < end) {
                            if(size >= capacity) {
                                capacity = (capacity == 0) ? 1 : capacity * 2;
                                StationNode *temp = (StationNode *)realloc(path2, sizeof(StationNode) * capacity);
                                path2 = temp;
                            }
                            add->distance = end;
                            add->max_autonomy = 0;
                            path2[size] = *add;
                            (size)++;
                            break;
                        }
                        else if(current->distance - current->max_autonomy < end) {
                            if(size >= capacity) {
                                capacity = (capacity == 0) ? 1 : capacity * 2;
                                StationNode *temp = (StationNode *)realloc(path2, sizeof(StationNode) * capacity);
                                path2 = temp;
                            }
                            add->distance = current->distance;
                            add->max_autonomy = current->max_autonomy;
                            path2[size] = *add;
                            (size)++;

                            if(current->distance != end) {
                                if(size == capacity) {
                                    capacity = (capacity == 0) ? 1 : capacity * 2;
                                    StationNode *temp = (StationNode *)realloc(path2, sizeof(StationNode) * capacity);
                                    path2 = temp;
                                }
                                add->distance = end;
                                add->max_autonomy = 0;
                                path2[size] = *add;
                                (size)++;
                            }
                            break;
                        }

                        StationNode * cur = check_min_station(root, max, current, end);

                        if(prev->distance-prev->max_autonomy < cur->distance) {
                            replace_station(prev, &path2, size-1);
                        }
                        prev = current;
                        current = cur;

                        if(size >= capacity) {
                            capacity = (capacity == 0) ? 1 : capacity * 2;
                            StationNode *temp = (StationNode *)realloc(path2, sizeof(StationNode) * capacity);
                            path2 = temp;
                        }
                        add->distance = current->distance;
                        add->max_autonomy = current->max_autonomy;
                        path2[size] = *current;
                        (size)++;
                        current1 = current;
                    }

                    for(int i = 0; i < size; i++) {
                        if(i >= 1 && i <= size - 2) {
                            StationNode * max = get_next_station_generic(root, path2[i-1].distance-path2[i-1].max_autonomy);
                            StationNode * next = get_next_station(root, max);
                            while(next != NULL && next->distance < path2[i].distance) {
                                if(next->distance - next->max_autonomy <= path2[i+1].distance) {
                                    replace_station(next, &path2, i);
                                    break;
                                }
                                else next = get_next_station(root, next);
                            }
                        }
                        if(i == size - 1) printf("%d\n", path2[i].distance);
                        else printf("%d ", path2[i].distance);
                    }
                    size = 0;
                    capacity = 0;
                }
            }
            token = strtok(NULL, " ");
        }
    }

    free(path);
    free(add);
    free(path2);
    free(input);
    free(token);
    delete_tree(&root);
    return 0;
}
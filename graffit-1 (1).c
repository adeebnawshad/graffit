/**
 * CSC A48 - Intro to Computer Science II
 * 
 * Assignment 3 - Graffit 
 * 
 * This is the file where you will implement your solution for
 * Assignment 3. Please make sure you read through this file carefully
 * and that you understand what is provided and what you need to complete.
 * 
 * You will also need to read the handout carefully. 
 * 
 * Parts where you have to implement functionality are clearly labeled TODO.
 * 
 * Be sure to test your work thoroughly, our testing will be extensive
 * and will check that your solution is *correct*, not only that it
 * provides functionality.
 * 
 * (c) William Song, Mustafa Quraish
 * (c) Charles Xu
 **/

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include<limits.h>
#define MAX_STR_LEN 1024

#ifndef MAT_SIZE
#define MAT_SIZE 3 // A small graph
#endif

typedef struct user_struct
{
  char name[MAX_STR_LEN];
  struct friend_node_struct *friends;
  struct brand_node_struct *brands;
  bool visited;
} User;

typedef struct friend_node_struct
{
  User *user;
  struct friend_node_struct *next;
} FriendNode;

typedef struct brand_node_struct
{
  char brand_name[MAX_STR_LEN];
  struct brand_node_struct *next;
  bool visited;
  int similarity;
} BrandNode;

FriendNode *allUsers = NULL;

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Given the head to a FriendNode linked list, returns true if a
 * given user's name exists in the list. Returns false otherwise.
 */
bool in_friend_list(FriendNode *head, User *node)
{
  for (FriendNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->user->name, node->name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a BrandNode linked list, returns true if a
 * given brand's name exists in the list. Returns false otherwise.
 */
bool in_brand_list(BrandNode *head, char *name)
{
  for (BrandNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->brand_name, name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a FriendNode linked list, inserts a given user
 * into the linked list and returns the head of the new linked list.
 * The insertion is done in alphabetical order. If the user already
 * exists, no modifications are made and the list is returned as is.
 */
FriendNode *insert_into_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (in_friend_list(head, node))
  {
    printf("User already in list\n");
    return head;
  }

  FriendNode *fn = calloc(1, sizeof(FriendNode));
  fn->user = node;
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->user->name, node->name) > 0)
  {
    fn->next = head;
    return fn;
  }

  FriendNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a BrandNode linked list, inserts a given brand into
 * the linked list and returns the head of the new linked list. The
 * insertion is done in alphabetical order. If the brand already exists,
 * no modifications are made and the list is returned as is.
 */
BrandNode *insert_into_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (in_brand_list(head, node))
  {
    printf("Brand already in list\n");
    return head;
  }

  BrandNode *fn = calloc(1, sizeof(BrandNode));
  strcpy(fn->brand_name, node);
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->brand_name, node) > 0)
  {
    fn->next = head;
    return fn;
  }

  BrandNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a FriendNode linked list, removes a given user from
 * the linked list and returns the head of the new linked list. If the user
 * does not exist, no modifications are made and the list is returned as is.
 */
FriendNode *delete_from_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (!in_friend_list(head, node))
  {
    printf("User not in list\n");
    return head;
  }

  if (strcmp(head->user->name, node->name) == 0)
  {
    FriendNode *temp = head->next;
    free(head);
    return temp;
  }

  FriendNode *cur;
  for (cur = head; cur->next->user != node; cur = cur->next)
    ;

  FriendNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given the head to a BrandNode linked list, removes a given brand from
 * the linked list and returns the head of the new linked list. If the brand
 * does not exist, no modifications are made and the list is returned as is.
 */
BrandNode *delete_from_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (!in_brand_list(head, node))
  {
    printf("Brand not in list\n");
    return head;
  }

  if (strcmp(head->brand_name, node) == 0)
  {
    BrandNode *temp = head->next;
    free(head);
    return temp;
  }

  BrandNode *cur;
  for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
    ;

  BrandNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given a user, prints their name, friends, and liked brands.
 */
void print_user_data(User *user)
{
  printf("User name: %s\n", user->name);

  printf("Friends:\n");
  for (FriendNode *f = user->friends; f != NULL; f = f->next)
  {
    printf("   %s\n", f->user->name);
  }

  printf("Brands:\n");
  for (BrandNode *b = user->brands; b != NULL; b = b->next)
  {
    printf("   %s\n", b->brand_name);
  }
}

/**
 * Given a brand, returns the index of the brand inside the brand_names array.
 * If it doesn't exist in the array, return -1
 */
int get_brand_index(char *name)
{
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (strcmp(brand_names[i], name) == 0)
    {
      return i;
    }
  }

  printf("Brand '%s' not found\n", name);
  return -1; // Not found
}

/**
 * Given a brand, prints their name, index (inside the brand_names
 * array), and the names of other similar brands.
 */
void print_brand_data(char *brand_name)
{
  int idx = get_brand_index(brand_name);
  if (idx < 0)
  {
    printf("Brand '%s' not in the list.\n", brand_name);
    return;
  }

  printf("Brand name: %s\n", brand_name);
  printf("Brand idx: %d\n", idx);

  printf("Similar brands:\n");
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0)
    {
      printf("   %s\n", brand_names[i]);
    }
  }
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char *file_name)
{
  // Read the file
  char buff[MAX_STR_LEN];
  FILE *f = fopen(file_name, "r");
  fscanf(f, "%s", buff);
  char *line = buff;
  // Load up the brand_names matrix
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (i == MAT_SIZE - 1)
    {
      strcpy(brand_names[i], line);
      break;
    }
    int index = strchr(line, ',') - line;
    strncpy(brand_names[i], line, index);
    line = strchr(line, ',') + sizeof(char);
  }
  // Load up the brand_adjacency_matrix
  for (int x = 0; x < MAT_SIZE; x++)
  {
    fscanf(f, "%s", buff);
    for (int y = 0; y < MAT_SIZE; y++)
    {
      int value = (int)buff[y * 2];
      if (value == 48)
      {
        value = 0;
      }
      else
      {
        value = 1;
      }
      brand_adjacency_matrix[x][y] = value;
    }
  }
}

/**
 * TODO: Complete this function
 * Creates and returns a user.
 */
User *create_user(char *name)
{
  for (FriendNode *current = allUsers; current != NULL; current = current->next){
    if (strcmp((current->user->name), name) == 0) return NULL;
  } 
  User *new_user = (User *)calloc(1, sizeof(User));
  if (new_user == NULL) return NULL;
  strcpy(new_user->name, name);
  new_user->friends = NULL;
  new_user->brands = NULL;
  new_user->visited = false;

  allUsers = insert_into_friend_list(allUsers, new_user);
  return new_user;
}

/**
 * TODO: Complete this function
 * Deletes a given user.
 */
int delete_user(User *user)
{
  if (user == NULL){
    return -1;
  }
    
  // Check if the user exists in the allUsers list
  if (!in_friend_list(allUsers, user)) {
    return -1;
  }

  allUsers = delete_from_friend_list(allUsers, user);

  FriendNode *temp = user->friends; // friend list of user
  while (temp != NULL){ // traverse friend list of user and remove user from each friend's friend list
    temp->user->friends = delete_from_friend_list(temp->user->friends, user);
    temp = temp->next;
  }
  free(user);
  return 0;
}

/**
 * TODO: Complete this function
 * Create a friendship between user and friend.
 */
int add_friend(User *user, User *friend)
{
  if (user == NULL || friend == NULL) return -1;
  // Check if the users are already friends
  if (in_friend_list(user->friends, friend)) return -1;
  user->friends = insert_into_friend_list(user->friends, friend);
  friend->friends = insert_into_friend_list(friend->friends, user);
  return 0;
}

/**
 * TODO: Complete this function
 * Removes a friendship between user and friend.
 */
int remove_friend(User *user, User *friend)
{
  if (user == NULL || friend == NULL) return -1;
  // Check if the user and friend are not friends
  if (!in_friend_list(user->friends, friend)){
    return -1;
  }
  user->friends = delete_from_friend_list(user->friends, friend);
  friend->friends = delete_from_friend_list(friend->friends, user);
  return 0;
}

bool in_brand_names_array(char *brand_name){
  for (int i = 0; i < MAT_SIZE; i++){
    if (strcmp(brand_names[i], brand_name) == 0) return true;
  }
  return false;
}
/**
 * TODO: Complete this function
 * Creates a follow relationship, the user follows the brand.
 */
int follow_brand(User *user, char *brand_name)
{
  // Check if link already existed
  if (in_brand_list(user->brands, brand_name) == 1) return -1;
  // Check if the brand name is invalid
  if (user == NULL || brand_name == NULL) return -1;
  if (in_brand_names_array(brand_name) == false) return -1;
  // Add brand to user's brands list
  user->brands = insert_into_brand_list(user->brands, brand_name);
  return 0;
}

/**
 * TODO: Complete this function
 * Removes a follow relationship, the user unfollows the brand.
 */
int unfollow_brand(User *user, char *brand_name)
{
  if (in_brand_list(user->brands, brand_name) == 0) return -1;
  if (user == NULL || brand_name == NULL) return -1;
  if (in_brand_names_array(brand_name) == false) return -1;
  user->brands = delete_from_brand_list(user->brands, brand_name);
  return 0;
}

/**
 * TODO: Complete this function
 * Return the number of mutual friends between two users.
 */
int get_mutual_friends(User *a, User *b)
{
  if (a == NULL || b == NULL) return 0;
  if (!in_friend_list(allUsers, a) || !in_friend_list(allUsers, b)) return 0;
  int mutual_friends = 0;
  for (FriendNode *currentA = a->friends; currentA != NULL; currentA = currentA->next){
    if (in_friend_list(b->friends, currentA->user) == true) mutual_friends++;
  }
  return mutual_friends;
}

int get_degrees_of_connection_helper(User *a, User *b, int partial_degree, int min_degree){
  if (a == b && partial_degree < min_degree) return partial_degree;
  a->visited = true;
  for (FriendNode *friend = a->friends; friend != NULL; friend = friend->next){
    if (!friend->user->visited){
      int new_degree = get_degrees_of_connection_helper(friend->user, b, partial_degree + 1, min_degree);
      if (new_degree < min_degree) min_degree = new_degree;
    }
  }
  a->visited = false;
  return min_degree;
}
/**
 * TODO: Complete this function
 * A degree of connection is the number of steps it takes to get from
 * one user to another. Returns a non-negative integer representing
 * the degrees of connection between two users.
 */
int get_degrees_of_connection(User *a, User *b)
{
  if (a == NULL || b == NULL || !in_friend_list(allUsers, a) || !in_friend_list(allUsers, b)) return -1;
  if (a == b) return 0;

  int degrees = get_degrees_of_connection_helper(a, b, 0, INT_MAX);
  if (degrees == INT_MAX) return -1;
  return degrees;
}

/**
 * TODO: Complete this function
 * Marks two brands as similar.
 */
void connect_similar_brands(char *brandNameA, char *brandNameB)
{
  if (brandNameA == NULL || brandNameB == NULL) return; 
  if (!in_brand_names_array(brandNameA)) return;
  if (!in_brand_names_array(brandNameB)) return;
  int indexA = get_brand_index(brandNameA);
  int indexB = get_brand_index(brandNameB);
  if (indexA == -1 || indexB == -1) return;
  brand_adjacency_matrix[indexA][indexB] = 1;
  brand_adjacency_matrix[indexB][indexA] = 1;
}

int count_mutual_brands(User *user1, User *user2) {
  int count = 0;
  BrandNode *brands1 = user1->brands;
  BrandNode *brands2 = user2->brands;

  // Iterate over the brands liked by user1
  while (brands1 != NULL) {
  // Check if user2 also likes this brand
    if (in_brand_list(brands2, brands1->brand_name)) {
      count++;
    }
    brands1 = brands1->next;
  }
  return count;
}

/**
 * TODO: Complete this function
 * Returns a suggested friend for the given user.
 */
User *get_suggested_friend(User *user)
{
  if (user == NULL) return NULL;
  int max_similarity = -1;
  User *suggested_friend = NULL;
  for (FriendNode *candidate = allUsers; candidate != NULL; candidate = candidate->next){
    if (candidate->user != user && !in_friend_list(user->friends, candidate->user)){
      int similarity = count_mutual_brands(user, candidate->user);
      if (similarity > max_similarity) {
        max_similarity = similarity;
        suggested_friend = candidate->user;
      }
      else if (similarity == max_similarity){
        if (strcmp(candidate->user->name, suggested_friend->name) > 0) suggested_friend = candidate->user;
      }
    }
  }
  return suggested_friend;
}

/**
 * TODO: Complete this function
 * Adds n suggested friends for the given user.
 * Returns how many friends were successfully followed.
 */
int add_suggested_friends(User *user, int n)
{
  if (user == NULL) return 0;
  int friends_added = 0;
  if (get_suggested_friend(user) == NULL) return 0;
  while (friends_added < n){
    User *suggested_friend = get_suggested_friend(user);
    if (suggested_friend == NULL) return friends_added;
    add_friend(user, suggested_friend);
    friends_added++;
  }
  return friends_added;
}

int calculate_similarity_score(User *user, char *brand_name){
  int brand_index = get_brand_index(brand_name);
  if (brand_index == -1) return 0;
  int similarity_score = 0;
  for (BrandNode *current = user->brands; current != NULL; current = current->next) {
    int user_brand_index = get_brand_index(current->brand_name);
    if (user_brand_index != -1 && brand_adjacency_matrix[user_brand_index][brand_index] == 1) similarity_score++;
  }
  return similarity_score; 
}
/**
 * TODO: Complete this function
 * Follows n suggested brands for the given user.
 * Returns how many brands were successfully followed.
 */
int follow_suggested_brands(User *user, int n)
{
  if (user == NULL) return 0;
  char *available_brands[MAT_SIZE]; // array to store the names of brands that the user does not already follow.
  int similarity_scores[MAT_SIZE];
  int available_count = 0;
  for (int i = 0; i < MAT_SIZE; i++) {
    if (!in_brand_list(user->brands, brand_names[i])) {
      available_brands[available_count] = brand_names[i];
      similarity_scores[available_count] = calculate_similarity_score(user, brand_names[i]);
      available_count++;
    }
  }
  if (available_count == 0) return 0; // no available brands

  // Sort available brands based on similarity scores and reverse-alphanumeric order
  for (int i = 0; i < available_count - 1; i++) {
    for (int j = 0; j < available_count - 1 - i; j++) {
      if (similarity_scores[j] < similarity_scores[j+1] || (similarity_scores[j] == similarity_scores[j+1] && strcmp(available_brands[j], available_brands[j+1]) < 0)) {
        char *temp_brand = available_brands[j];
        int temp_score = similarity_scores[j];
        available_brands[j] = available_brands[j+1];
        similarity_scores[j] = similarity_scores[j+1];
        available_brands[j+1] = temp_brand;
        similarity_scores[j+1] = temp_score;
      }
    }
  }

  // Take the top n brands in terms of similarity and insert them into the user's brand list
  int followed_count = 0;
  for (int i = 0; i < n && i < available_count; i++) {
    follow_brand(user, available_brands[i]);
    followed_count++;
  }

  return followed_count;
}

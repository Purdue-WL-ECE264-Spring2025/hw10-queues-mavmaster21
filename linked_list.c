#include "linked_list.h"

#include <stdlib.h>

/////MODIFY THIS FILE

struct list_node *new_node(size_t value) //creates a node with an empty pointer to next and param value
{ 
  struct list_node * new = malloc(sizeof(struct list_node));
  new->value = value;
  new->next = NULL;
  return new;
}

void insert_at_head(struct linked_list *list, size_t value) 
{
  struct list_node * new = new_node(value);
  new->next = list->head;
  list->head = new;
}

void insert_at_tail(struct linked_list *list, size_t value) 
{
  struct list_node *new = new_node(value);
  
  if (list->head == NULL) {
    list->head = new;
    return;
  }

  struct list_node * temp = list->head;

  while(temp->next != NULL)
  {
    temp = temp->next;
  }
  temp->next = new;
}

size_t remove_from_head(struct linked_list *list) 
{ 
  if (list->head == NULL) {return 0;} // avoid segfault
  struct list_node * temp = list->head; //stores pointer to current first entry
  size_t val = temp->value; //stores current first value
  list->head = temp->next; //makes head point to second entry
  free(temp); //frees former first entry
  return val; //returns original first value ig
}

size_t remove_from_tail(struct linked_list *list) //I HIGHLY doubt TS works
{ 

  if (list->head == NULL) {return 0;} //list is empty

  struct list_node * temp = list->head;

  if (temp->next == NULL)  //if only one node
  {
    size_t val = temp->value;
    free(temp);
    list->head = NULL;
    return val;
  }

  while(temp->next->next != NULL)
  {
    temp = temp->next;
  }
  size_t val = temp->next->value;
  free(temp->next);
  temp->next = NULL;
  return val; 
}

void free_list(struct linked_list list) 
{
  struct list_node * temp = list.head;
  while (temp != NULL)
  {
    struct list_node *next = temp->next;
    free(temp);
    temp = next;
  }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}

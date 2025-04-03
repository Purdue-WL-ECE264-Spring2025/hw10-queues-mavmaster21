#include "linked_list.h"

#include <stdlib.h>

/////MODIFY THIS FILE

struct list_node *new_node(size_t value) //creates a node with an empty pointer to next and param value
{ 
  struct list_node * new = malloc(sizeof(struct list_node));
  new->value = value;
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
  struct list_node * temp = list->head;
  while(temp->next != NULL)
  {
    temp = temp->next;
  }
  temp->next = new_node(value);
  temp->next->next = NULL;
}

size_t remove_from_head(struct linked_list *list) 
{ 
  size_t val = list->head->value; //stores current first value
  struct list_node * temp = list->head; //stores pointer to current first entry
  list->head = list->head->next; //makes head point to second entry
  free(list->head); //frees former first entry
  return val; //returns original first value ig
}

size_t remove_from_tail(struct linked_list *list) //I HIGHLY doubt TS works
{ 
  struct list_node * temp = list->head;
  while(temp->next->next != NULL)
  {
    temp = temp->next;
  }
  size_t val = temp->next->value;
  free(temp->next);
  temp = NULL;
  return val; 
}

void free_list(struct linked_list list) 
{
  struct list_node * temp = list.head->next;
  while (list.head->next != NULL)
  {
    temp = list.head->next;
    free(list.head);
    list.head = temp;
  }
  free(list.head);
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}

#include "CAN_list.H"
#include "CAN_driver.h"

typedef struct CAN_message_node_struct{
	CAN_message * msg;
	struct CAN_message_node_struct * next;
} CAN_message_node;

static CAN_message_node * head = NULL;
/* TODO: Add tail to prevent linear runtime of add to queue */

int CAN_list_push(CAN_message * msg)
{
	CAN_message_node * node = malloc(sizeof(CAN_message_node));
	if (node == NULL)
			return -1; /* TODO: Add proper error code */
	
	node->msg = msg;

	CAN_message_node * tmp = head;
	
	while(tmp != NULL)
		tmp = tmp->next;
	
	tmp->next = node;
	node.next = NULL;
	return 1;
}

static void CAN_list_transmit(void)
{
	if (head == NULL)
		return;
	
	CAN_send(head);
	
	CAN_message_node * tmp = head;
	head = head->next;
	free(tmp);
}
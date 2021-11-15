/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Spring 2014                              *
 *  Authors: Ulrich Kremer                   *
 *           Hans Christian Woithe           *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"

static void ini(Instruction * head);
static void idMarker(Instruction * head, int id);
static void regMarker(Instruction * head, int reg);
static void del(Instruction * head);

//initial with 0, which means non-critical
static void ini(Instruction * head){
	if (head->next==NULL) {
		head->critical='0';
		return;
	}
	head->critical='0';
	ini(head->next);
}

//id-marker
static void idMarker(Instruction * head, int id){
	if (head->prev==NULL) {
		switch (head->opcode) {
		case STORE:
				if(id==head->field1){
					head->critical='1';
					regMarker(head,head->field2);
				}
			break;
		case READ:
			head->critical='1';
			break;
		case WRITE:
			head->critical='1';
			break;
		default:
			break;
		}
		return;
	}
	switch (head->opcode) {
	case STORE:
			if(id==head->field1){
				head->critical='1';
				regMarker(head,head->field2);
			}
			else{
				idMarker(head->prev,id);
			}
		break;
	case READ:
		head->critical='1';
		idMarker(head->prev,id);
		break;
	case WRITE:
		head->critical='1';
		idMarker(head->prev,id);
		idMarker(head->prev,head->field1);
		break;
	default:
		idMarker(head->prev,id);
	}
}

//reg-marker
static void regMarker(Instruction * head, int reg){
	if (head->prev==NULL) {
		switch (head->opcode) {
		case LOAD:
			if(reg==head->field1){
				head->critical='1';
				idMarker(head,head->field2);
			}
			break;
		case LOADI:
			if(reg==head->field1){
				head->critical='1';
			}
			break;
		case ADD:
			if(reg==head->field1){
				head->critical='1';
			}
			break;
		case SUB:
			if(reg==head->field1){
				head->critical='1';
			}
			break;
		case MUL:
			if(reg==head->field1){
				head->critical='1';
			}
			break;
		case AND:
			if(reg==head->field1){
				head->critical='1';
			}
			break;
		case OR:
			if(reg==head->field1){
				head->critical='1';
			}
			break;
			default:
				break;
		}
		return;
	}
	switch (head->opcode) {
	case LOAD:
		if(reg==head->field1){
			head->critical='1';
			idMarker(head,head->field2);
		}
		else{
			regMarker(head->prev,reg);
		}
		break;
	case LOADI:
		if(reg==head->field1){
			head->critical='1';
		}
		else{
			regMarker(head->prev,reg);
		}
		break;
	case ADD:
		if(reg==head->field1){
			head->critical='1';
			regMarker(head->prev,head->field2);
			regMarker(head->prev,head->field3);
		}
		else{
			regMarker(head->prev,reg);
		}
		break;
	case SUB:
		if(reg==head->field1){
			head->critical='1';
			regMarker(head->prev,head->field2);
			regMarker(head->prev,head->field3);
		}
		else{
			regMarker(head->prev,reg);
		}
		break;
	case MUL:
		if(reg==head->field1){
			head->critical='1';
			regMarker(head->prev,head->field2);
			regMarker(head->prev,head->field3);
		}
		else{
			regMarker(head->prev,reg);
		}
		break;
	case AND:
		if(reg==head->field1){
			head->critical='1';
			regMarker(head->prev,head->field2);
			regMarker(head->prev,head->field3);
		}
		else{
			regMarker(head->prev,reg);
		}
		break;
	case OR:
		if(reg==head->field1){
			head->critical='1';
			regMarker(head->prev,head->field2);
			regMarker(head->prev,head->field3);
		}
		else{
			regMarker(head->prev,reg);
		}
		break;
		default:
			regMarker(head->prev,reg);
	}
}

//del
static void del(Instruction * head){
	if (head->next==NULL) {
		if(head->critical=='0'){
			head->prev->next=NULL;
		}
		return;
	}
	if (head->prev==NULL) {
		if(head->critical=='0'){
			head=head->next;
			head->next->prev=NULL;
		}
		del(head->next);
	}
	if(head->critical=='0'){
		head->prev->next=head->next;
		head->next->prev=head->prev;
	}
	del(head->next);
}

int main()
{
	Instruction *head;


	head = ReadInstructionList(stdin);
	if (!head) {
		WARNING("No instructions\n");
		exit(EXIT_FAILURE);
	}
	/* YOUR CODE GOES HERE */
	ini(head);
	idMarker(LastInstruction(head),0);
	del(head);
	if (head) {
		PrintInstructionList(stdout, head);
		DestroyInstructionList(head);
	}
	return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

/* Search the list of groups for a group with matching group_name
* If group_name is not found, return NULL, otherwise return a pointer to the
* matching group list node.
*/
Group *find_group(Group *group_list, const char *group_name) {
    Group* current = group_list;
    int found = 0; 
    Group *group_found = NULL;
       //if the list passed in is empty, return null 
       //if the list is not empty, go through each group until the group name is found
       while ((current != NULL) && (found == 0)) {
             // if the group name is found
             if (strcmp(current->name,group_name) == 0) {
                found = 1;
                group_found = current;
            // if its not found, move to the next group
             } else {
                current = current->next; 
             }
             
       }
    return group_found;
}

/* Add a group with name group_name to the group_list referred to by
* group_list_ptr. The groups are ordered by the time that the group was
* added to the list with new groups added to the end of the list.
*
* Returns 0 on success and -1 if a group with this name already exists.
*
* (I.e, allocate and initialize a Group struct, and insert it
* into the group_list. Note that the head of the group list might change
* which is why the first argument is a double pointer.)
*/

int add_group(Group **group_list, char *group_name) {
 	Group* new_node = (Group*) malloc(sizeof(Group));
 
    Group *last = *group_list;  /* used in step 5*/
    Group *found = find_group(*group_list, group_name);
  	int stat = -1;
  	//if successfully malloced..
  	if (new_node == NULL) {
  		exit(-1); //"check the return value from malloc, if its NULL, terminate the program"
  	} else {
  		//if successfully malloced..
	    new_node->name = malloc(sizeof(group_name) + 1);
	    if (new_node->name == NULL) {
	    	exit(-1); //"check the return value from malloc, if its NULL, terminate the program"
	    } else {
		    strcpy(new_node->name, group_name);
		    new_node->users = NULL;
		    new_node->xcts = NULL;
		 
		    /* 3. This new node is going to be the last node, so make next 
		          of it as NULL*/
		    new_node->next = NULL;
		 
		    /* 4. If the Linked List is empty, then make the new node as head */
		    if (*group_list == NULL)
		    {
		       *group_list = new_node;
		       stat = 0;
		       return stat;
		    }  
		    if (found == NULL) {
		    /* 5. Else traverse till the last node */
		    while (last->next != NULL)
		        last = last->next;
		  
		    /* 6. Change the next of last node */
		    last->next = new_node;
		    stat = 0;
		    return stat;
			}
		    return stat;
		}
	}  
}
 


/* Print to standard output the names of all groups in group_list, one name
*  per line. Output is in the same order as group_list.
*/
void list_groups(Group *group_list) {
   Group* current = group_list;
   while (current != NULL) {
         printf("%s\n",current->name);
         current = current->next;
   }
}

/* Add a new user with the specified user name to the specified group. Return zero
* on success and -1 if the group already has a user with that name.
* (allocate and initialize a User data structure and insert it into the
* appropriate group list)
*/
int add_user(Group *group, const char *user_name) {
	int job = -1;
    if (group != NULL) {
        //find the group, i.e returns a pointer to the found node
        Group *group_ptr = group;
        User *find_prev = find_prev_user(group_ptr, user_name);
        if (find_prev == NULL) {
	        //creating User...
	        User *new = (User *)malloc(sizeof(User));
	        if (new == NULL) {
	        	exit(-1); //"check the return value from malloc, if its NULL, terminate the program"
	        } else {
		        new->name = malloc(sizeof(user_name) + 1);
		        if (new->name == NULL) {
		        	exit(-1); //"check the return value from malloc, if its NULL, terminate the program"
		        } else {
			        strcpy(new->name, user_name);
			        new->balance = 0;
			        //adding User to the front of the User list of Group found, since balance is set to 0
			        User *tmp = group_ptr->users;
			        group_ptr->users = new;
			        new->next = tmp;
			        job = 0;
			    }
		    }
	    }
        return job;
    } else {
    	return job;
    }
}

/* Remove the user with matching user and group name and
* remove all her transactions from the transaction list.
* Return 0 on success, and -1 if no matching user exists.
* Remember to free memory no longer needed.
* (Wait on implementing the removal of the user's transactions until you
* get to Part III below, when you will implement transactions.)
*/
int remove_user(Group *group, const char *user_name) {
    Group *group_ptr = group;
    int job_status = -1;
    //if there are set of users
    if (group_ptr->users != NULL) {
    	User *find_prev = find_prev_user(group_ptr, user_name);
        //if the user is found, follow the process to remove it
        if (find_prev != NULL) {
        	User *curr = find_prev->next;
        	//if the previous user has the username, means it is the first user in the list to be removed
	        if (strcmp(find_prev->name, user_name) == 0) {
	            User *temp = find_prev;
	            group_ptr->users = find_prev->next;
	            free(temp);
	           	job_status = 0;
	        //otherwise it is not the first user to be removed
	        } else {

	            User *temp = curr;
	            find_prev->next = curr->next;
	            free(temp);
	            job_status = 0;
	        }
	        
	    }
	    if (group_ptr->xcts != NULL) {
				//remove the users transactions
	    		remove_xct(group_ptr, user_name);
		} 
	    
	} 
	
    return job_status;
}

/* Print to standard output the names of all the users in group, one
* per line, and in the order that users are stored in the list, namely
* lowest payer first.
*/
void list_users(Group *group) {
	Group *q = group;
	//if the group isnt null, check for users 
    if (q != NULL) {
        User *p = q->users;
        // if the group contains users, print them
        while(p != NULL) {
            printf("%s\n", p->name);
            p = p->next;
        }
    }
}

/* Print to standard output the balance of the specified user. Return 0
* on success, or -1 if the user with the given name is not in the group.
*/
int user_balance(Group *group, const char *user_name) {
    int job = -1;
    User *find_prev = find_prev_user(group, user_name);
    if (find_prev != NULL) {
    	User *curr = find_prev->next;
    	//if the previous user has the username, means it is the first user in the list, and that balance should be displayed
        if (strcmp(find_prev->name, user_name) == 0) {
        	printf("%.2f", find_prev->balance);
        	printf("\n");
        	job = 0;
        //otherwise it is the next user
        } else {
        	printf("%.2f", curr->balance);
        	printf("\n");
        	job = 0;
       
   		}
    }
    return job;
}

/* Print to standard output the name of the user who has paid the least
* If there are several users with equal least amounts, all names are output.
* Returns 0 on success, and -1 if the list of users is empty.
* (This should be easy, since your list is sorted by balance).
*/
int under_paid(Group *group) {
	if (group != NULL) {
        //pointer to group
        Group *group_ptr = group;
        //check if the group contains users
        if (group_ptr->users != NULL) {
            User *curr = group_ptr->users->next;
            User *prev = group_ptr->users;
            //if there is only one user, that user has the lowest balance, therefore least paid
            if (curr == NULL) {
                printf("%s", prev->name);
            }
            //if there are more than one user...
            if (curr != NULL) {
            	//print the first user
            	printf("%s\n", prev->name);
            	//print all the users that have the lowest balance
                while (prev->balance == curr->balance) {
                	printf("%s\n", curr->name);
                	prev = curr;
                	curr = curr->next;
                }
            }
        }    
    }
    return 0;
}

/* Return a pointer to the user prior to the one in group with user_name. If
* the matching user is the first in the list (i.e. there is no prior user in
* the list), return a pointer to the matching user itself. If no matching user
* exists, return NULL.
*
* The reason for returning the prior user is that returning the matching user
* itself does not allow us to change the user that occurs before the
* matching user, and some of the functions you will implement require that
* we be able to do this.
*/
User *find_prev_user(Group *group, const char *user_name) {
	Group *group_ptr = group;
	int found = 0;
	User *userfound = NULL;
	//if the group contains users..
    if (group_ptr->users != NULL) {
    	// two pointers to keep track of previous and current user
        User *curr = group_ptr->users->next;
        User *prev = group_ptr->users;
        // if the first user is the given user
        if (strcmp(prev->name, user_name) == 0) {
        	found = 1;
        	userfound = prev;
        }
        //otherwise traverse the list of users until the given user is found
        while ((curr != NULL) && (found == 0)) {
        	//when the given user is reached, return the node
        	if (strcmp(curr->name, user_name) == 0) {
        		User *temp = prev;
        		prev = curr;
        		curr = curr->next;
        		//since the user is found, assign found as 1 to stop traversing the list
        		found = 1;
        		userfound = temp;
        	}
        	//otherwise continue traversing the list to look for the given user
        	else {
        		prev = curr;
        		curr = curr->next;
        	}

       	}
    }
    return userfound;
}

/* Swap the two given nodes, where A->B becomes B->A
* This is a helper function used in bubbleSort to swap the elements in Users list for the given group
*/
void swap(User *a, User *b)
{
    //store the elements of first node
    double temp = a->balance;
    char* temp2 = a->name;
    //Assign A to contain the elements of B
    a->balance = b->balance;
    a->name = b->name;
    //Assign B to contain the stored elements of A
    b->balance = temp;
    b->name = temp2;
}

/* Sort the given linked list from lowest to highest balance
* PREREQ: the Group passed in must not be NULL
* This is a helper function used in add_xct to sort the Users list for the given group
*/
void bubbleSort(Group *group) {
	Group *group_ptr = group;
	User *start = group_ptr->users; 
    int swapped;
    //assign two pointers
    User *user_begin;
    User *user_end = NULL;
 
    // Checking for empty list 
    if (start == NULL)
        return;
 	
    do
    {
        //set ptr1 to point at the start of the USers list
        swapped = 0;
        user_begin = start;
 		//traverse the list until the end is reached
        while (user_begin->next != user_end)
        {
         	//compare the balances of current and next user
            if (user_begin->balance > user_begin->next->balance)
            { 
            	//call helper function to swap the elements, so that the lower balance comes before
                swap(user_begin, user_begin->next);
                swapped = 1;
            }
            user_begin = user_begin->next;
        }
        user_end = user_begin;
    }
    while (swapped);
}

/* Add the transaction represented by user_name and amount to the appropriate
* transaction list, and update the balances of the corresponding user and group.
* Note that updating a user's balance might require the user to be moved to a
* different position in the list to keep the list in sorted order. Returns 0 on
* success, and -1 if the specified user does not exist.
*/
int add_xct(Group *group, const char *user_name, double amount) {
	int job = -1;
	if (group != NULL) {
		
        //find the User in the Users list and update its balance
        User *prev_user = find_prev_user(group, user_name);
        if (prev_user != NULL ) { //a user cant have a transcation without it being a user. so it must not ever be null?
            if (strcmp(prev_user->name, user_name) == 0) {
            	prev_user->balance += amount;
            	//set a pointer to the group
			    Group *group_ptr = group;
			    //creating a transaction ...
			    Xct *new = (Xct *)malloc(sizeof(Xct));
			    if (new == NULL) {
			    	exit(-1);
			    } else {
			    	new->name = malloc(sizeof(user_name) + 1);
			    	if (new->name == NULL) {
			    		exit(-1);
			    	} else {
					    strcpy(new->name, user_name);
					    new->amount = amount;
					    //adding Xct to the front of the Xct list of Group
					    Xct *tmp = group_ptr->xcts;
					    group_ptr->xcts = new;
					    new->next = tmp;
					}
			    }
			    job = 0;
			    
	        }
	        else {
	        	prev_user->next->balance += amount;
	        	//set a pointer to the group
	        	Group *group_ptr = group;
	        	//creating a transaction...
			    Xct *new = (Xct *)malloc(sizeof(Xct));
			    if (new == NULL) {
			    	exit(-1);
			    } else {
			    	new->name = malloc(sizeof(user_name) + 1);
			    	if (new->name == NULL) {
			    		exit(-1);
			    	} else {
					    strcpy(new->name, user_name);
					    new->amount = amount;
					    //adding Xct to the front of the Xct list of Group
					    Xct *tmp = group_ptr->xcts;
					    group_ptr->xcts = new;
					    new->next = tmp;
					}
					job = 0;
			    }
	        }
        }
        //sort the Users list every time the balance is updated
        bubbleSort(group); 
    }
    return job;
}

/* Print to standard output the num_xct most recent transactions for the
* specified group (or fewer transactions if there are less than num_xct
* transactions posted for this group). The output should have one line per
* transaction that prints the name and the amount of the transaction. If
* there are no transactions, this function will print nothing.
*/
void recent_xct(Group *group, long nu_xct) {
	Group *q = group;
	long count = 0; //set counter to keep track of printed xcts
	//traverse the list if there are groups
    if (q != NULL) {
        Xct *p = q->xcts;
        //traverse until there are no xcts for the given group
        //traverse until given amount of xcts are printed
        while((p != NULL) && (count < nu_xct)) {
            printf("%s ", p->name);
            printf("%.2f ", p->amount);
            printf("\n");
            count += 1;
            p = p->next;
            
        }
    }
}

/* Remove all transactions that belong to the user_name from the group's
* transaction list. This helper function should be called by remove_user.
* If there are no transactions for this user, the function should do nothing.
* Remember to free memory no longer needed.
*/
void remove_xct(Group *group, const char *user_name) {
	Group *q = group;
    if (q != NULL) {
        Xct *prev = q->xcts;
        Xct *curr = q->xcts->next;
        if (prev != NULL) {
        	//if the user to be removed is the first transaction
             if (strcmp(prev->name, user_name) == 0) {
             	//store the transaction to be removed
                Xct *temp = prev;
                prev = curr;
                //point to the next transaction, causing the removal of the first transaction
                group->xcts = prev;
                free(temp);
            }
            //if its not the first transaction
            while (curr != NULL) {
            	//if the transaction is of the given user...
                if (strcmp(curr->name,user_name) == 0) {
                	//store the transaction to be removed
                    Xct *temp = curr;
                    //remove the transaction
                    prev->next = curr->next;
                    free(temp);
                    curr = curr->next;
              
                //otherwise, move to the next transaction
                } else {
                		prev = curr;
                		curr = curr ->next;
               	}
            }
            
        }
    }
}
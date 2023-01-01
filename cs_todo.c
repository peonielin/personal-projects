//This program is written by Peonie Lin (z5454933)
//written on 31/10/2022
//this program is a to do list 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define INVALID_PRIORITY -1

#define MAX_TASK_LENGTH 200
#define MAX_CATEGORY_LENGTH 40
#define MAX_STRING_LENGTH 1024
#define IS_REPEATABLE -1
#define IS_NOT_REPEATABLE 1

// You *should* #define each command
#define COMMAND_ADD_TASK 'a'
#define COMMAND_PRINT_TASKS 'p'
#define COMMAND_CHANGE_PRIORITY 'i'
#define COMMAND_COUNT_TASKS 'n'
#define COMMAND_ADD_COMPLETED_TASK 'c'
#define COMMAND_PRINT_COMPLETED_TASKS 'P'
#define COMMAND_EXPECTED_COMPLETION_TIME 'e'
#define COMMAND_DELETE_TASK 'd'
#define COMMAND_FINISH_DAY 'f'
#define COMMAND_REPEATABLE_TASK 'r'

enum priority { LOW = 0, MEDIUM, HIGH };

struct task {
    char task_name[MAX_TASK_LENGTH];
    char category[MAX_CATEGORY_LENGTH];
    enum priority priority;
    struct task *next;
    int repeatable;
};

struct completed_task {
    struct task *task;
    int start_time;
    int finish_time;
    struct completed_task *next;
};


struct todo_list {
    struct task *tasks;
    struct completed_task *completed_tasks;
};

////////////////////////////////////////////////////////////////////////////////
///////////////////// YOUR FUNCTION PROTOTYPES GO HERE /////////////////////////
////////////////////////////////////////////////////////////////////////////////

void add_task(struct todo_list *task);
struct task *create_node(char *task_name, char *category,enum priority priority);
void command_loop(struct todo_list *todo);
void print_tasks(struct todo_list *task);
void change_priority (struct todo_list *todo);
int size_of_list(struct todo_list *todo);
struct completed_task *create_completed_node(char *task_name, char *category, int start_time, int finish_time);
void add_completed_tasks(struct todo_list *todo);
void print_complete_task_list(struct todo_list *todo);
void print_estimated_time (struct todo_list *todo);
int estimate_time (struct todo_list *todo, struct task *task);
struct task *delete_tasks(struct todo_list *todo);
void finish_day(struct todo_list *todo);
void add_last_uncompleted(struct todo_list *todo, struct task *new_node);
void add_last_completed(struct todo_list *todo, struct completed_task *new_node);
void repeat_task(struct todo_list *todo);


////////////////////////////////////////////////////////////////////////////////
//////////////////////// PROVIDED HELPER PROTOTYPES ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void parse_add_task_line(
    char buffer[MAX_STRING_LENGTH], char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH], enum priority *prio
);
void parse_task_category_line(
    char buffer[MAX_STRING_LENGTH], char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH]
);
void parse_complete_task_line(
    char buffer[MAX_STRING_LENGTH], char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH], int *start_time, int *finish_time
);

enum priority string_to_priority(char priority[MAX_STRING_LENGTH]);
void remove_newline(char input[MAX_STRING_LENGTH]);
void trim_whitespace(char input[MAX_STRING_LENGTH]);
void print_one_task(int task_num, struct task *task);
void print_completed_task(struct completed_task *completed_task);
struct task *delete_node(struct todo_list *todo);

int task_compare(struct task *t1, struct task *t2);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int main(void) {
    // Stage 1.1
    // You should initialize the `todo` variable below. You will need
    // to use the malloc() function to allocate memory for it!
    struct todo_list *todo = malloc(sizeof(struct todo_list));
    todo->tasks = NULL;
    todo->completed_tasks = NULL;
    printf("Welcome to CS ToDo!\n");

    
    command_loop(todo);
    printf("All done!\n");
    return 0;
}

/**
 * The central loop that executes commands until the program is completed.
 *
 * Parameters:
 *     todo - The todo list to execute commands on.
 *
 * Returns:
 *     Nothing
 */
void command_loop(struct todo_list *todo) {
    printf("Enter Command: ");
    char command;
    while (scanf(" %c", &command) == 1) {
        // TODO: Add to this loop as you complete the assignment
        if (command == COMMAND_ADD_TASK) {
            add_task(todo);
        } else if (command == COMMAND_PRINT_TASKS) {
            print_tasks(todo);
        } else if (command == COMMAND_CHANGE_PRIORITY) {
            change_priority(todo);
        } else if (command == COMMAND_COUNT_TASKS) {
            printf("There are %d items on your list!\n", size_of_list(todo));
        } else if (command == COMMAND_ADD_COMPLETED_TASK) {
            add_completed_tasks(todo);
        } else if (command == COMMAND_PRINT_COMPLETED_TASKS) {
            print_complete_task_list(todo);
        } else if (command == COMMAND_EXPECTED_COMPLETION_TIME) {
            print_estimated_time(todo);
        } else if (command == COMMAND_DELETE_TASK) {
            delete_tasks(todo);
        } else if (command == COMMAND_FINISH_DAY) {
            finish_day(todo);
        } else if (command == COMMAND_REPEATABLE_TASK) {
            //printf("work in progress");
            repeat_task(todo);
        }
        printf("Enter Command: ");
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////// YOUR HELPER FUNCTIONS ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//this function designates a repeatable task
void repeat_task(struct todo_list *todo) {
    //mark task as repeatable
    char buffer[MAX_STRING_LENGTH];
    fgets(buffer, MAX_STRING_LENGTH, stdin);
    char task[MAX_TASK_LENGTH];
    char category[MAX_CATEGORY_LENGTH];
    parse_task_category_line(buffer, task, category);

    struct task *current = todo->tasks;
    if (current == NULL) {
        printf("Could not find task '%s' in category '%s'.\n", task, category);
        return;
    }

    int flag = 0;

    while (current != NULL) {
        //current->repeatable = IS_NOT_REPEATABLE;
        if (strcmp(current->task_name, task) == 0 && 
            strcmp(current->category, category) == 0) {
            current->repeatable *= -1;
            flag++;
            return;
        } 
        current = current->next;
    }

    if (flag == 0) {
        printf("Could not find task '%s' in category '%s'.\n", task, category);
    }

}

//this function deletes the all of the completed list
void finish_day(struct todo_list *todo) {
    struct completed_task *current = todo->completed_tasks;
    struct completed_task *previous = todo->completed_tasks;

    if (current == NULL) {
        return;
    }
    
    while (current != NULL) {
        // printf("\n%d\n", current->task->repeatable);
        if (current->task->repeatable == IS_REPEATABLE) { 
            add_last_uncompleted(todo, current->task);
            previous->next = current->next;
            current->next = NULL;

            
        }
        previous = current;
        current = current->next;
    }
    
    struct completed_task *temp = todo->completed_tasks;
    while(todo->completed_tasks != NULL) {
        temp = todo->completed_tasks;
        todo->completed_tasks = todo->completed_tasks->next;
        free(current);
    }
}

struct task *delete_tasks(struct todo_list *todo) { 
    char buffer[MAX_STRING_LENGTH];
    fgets(buffer, MAX_STRING_LENGTH, stdin);
    char task[MAX_TASK_LENGTH];
    char category[MAX_CATEGORY_LENGTH];
    parse_task_category_line(buffer, task, category);

    struct task *current = todo->tasks;
    struct task *previous = NULL;
    //if list is empty
    if (current == NULL) {
        return current;
    }
    //if data is at head
    if (strcmp(current->task_name, task) == 0 && 
        strcmp(current->category, category) == 0) {
        todo->tasks = current->next;
        free(current);
        return todo->tasks;
    }

    while (current != NULL) {
        if (strcmp(current->task_name, task) == 0 && 
            strcmp(current->category, category) == 0) {
            previous->next = current->next;
            free(current);
            return todo->tasks;
        }
        previous = current;
        current = current->next;
    }
    // doesnt exist in list
    if (current == NULL) {
        printf("Could not find task '%s' in category '%s'.\n", task, category);
        return todo->tasks;
    }

    return todo->tasks;

}
//this function creates nodes for the completed list
struct completed_task *create_completed_node(char *task_name, char *category, 
    int start_time, int finish_time) {
    struct completed_task *new_node = malloc(sizeof(struct completed_task));
    new_node->task = NULL;
    new_node->start_time = start_time;
    new_node->finish_time = finish_time;
    new_node->next = NULL;
    return new_node;
}


// // // this function adds node to compeleted list
void add_completed_tasks(struct todo_list *todo) {
    char buffer[MAX_STRING_LENGTH];
    fgets(buffer, MAX_STRING_LENGTH, stdin);
    char task[MAX_TASK_LENGTH];
    char category[MAX_CATEGORY_LENGTH];
    int start_time;
    int finish_time;
    parse_complete_task_line(buffer, task, category, &start_time, &finish_time);
    //create completed node but task in completed needs to point 
    //to a tasks struct which points to null
    struct completed_task *new_node = create_completed_node(task, category, 
        start_time, finish_time);
    struct task *current_task = todo->tasks;
    struct task *previous_task = todo->tasks;
    int flag = 0;
    //case previous = null?


    if (current_task == NULL) {
        printf("Could not find task '%s' in category '%s'.\n", task, category);
        return;
    }
    
    //only one thing in the list
    if (strcmp(current_task->task_name, task) == 0 && 
        strcmp(current_task->category, category) == 0
        && previous_task->next == NULL) {

        todo->tasks = NULL;
        current_task->next = NULL;
        new_node->task = current_task;
        add_last_completed(todo, new_node);
        return;
    } 

    //if item to complete is the first in the list
    if (strcmp(current_task->task_name, task) == 0 && 
        strcmp(current_task->category, category) == 0 
        && previous_task->next != NULL) {

        todo->tasks = current_task->next;
        new_node->task = current_task;
        add_last_completed(todo, new_node);
        return;
    }
    
    while (current_task != NULL) {
        
        if (strcmp(current_task->task_name, task) == 0 && 
            strcmp(current_task->category, category) == 0) {

            new_node->task = current_task;
            previous_task->next = current_task->next;
            current_task->next = NULL;
            flag++;
            //create node if correct tasks exist
            add_last_completed(todo, new_node);
        }
        previous_task = current_task;
        current_task = current_task->next;
    }

    if (flag == 0) {
        printf("Could not find task '%s' in category '%s'.\n", task, category);
    }



}



//this function estimates completion time of non-completed tasks
int estimate_time (struct todo_list *todo, struct task *task) {
    int time = 100;
    int avg = 0;
    int counter = 0;
    int task_time = 0;
    //no completed tasks
    if (todo->completed_tasks == NULL) {
        return time;
    } 
    //one or more completed tasks
    struct completed_task *current = todo->completed_tasks;
    while (current != NULL) {
        if (strcmp(current->task->category, task->category ) == 0) {
            //if they match add time
            counter++;
            task_time += current->finish_time - current->start_time; 
            avg = task_time/counter;
        }
        current = current->next; 
    }
    if (counter == 0) {
        avg = 100;
    }
    return avg;
 
}

// //function prints the estimated completion time
void print_estimated_time (struct todo_list *todo) {
    int task_num = 1;
    struct task *current = todo->tasks;
    printf("Expected completion time for remaining tasks:\n\n");

    while (current != NULL) {
        
        print_one_task(task_num, current);
        printf ("Expected completion time: %d minutes\n\n", estimate_time(todo, current));
        task_num++;
        current = current->next;

    }
}

// //this function creates nodes
// //INPUT the data 
struct task *create_node(char *task_name, char *category, enum priority priority) {
    struct task *new_node = malloc(sizeof(struct task));
    strcpy(new_node->task_name, task_name);
    strcpy(new_node->category, category);
    new_node->priority = priority;
    new_node->next = NULL;
    new_node->repeatable = IS_NOT_REPEATABLE;
    return new_node;
}


int size_of_list(struct todo_list *todo) {
    int counter = 0;
    struct task *current = todo->tasks;
    if (todo->tasks == NULL) {
        return 0;
    }
    while (current != NULL) {
        counter++;
        current = current->next;
    }
    return counter;
}


void change_priority (struct todo_list *todo) {
    char buffer[MAX_STRING_LENGTH];
    fgets(buffer, MAX_STRING_LENGTH, stdin);
    char task[MAX_TASK_LENGTH];
    char category[MAX_CATEGORY_LENGTH];
    parse_task_category_line(buffer, task, category);
    struct task *current = todo->tasks;
    int flag = 0;
    while (current != NULL) {
        int result_task;
        int result_catergory;
        result_task = strcmp(current->task_name,task);
        result_catergory = strcmp(current->category, category);
        
        if (result_catergory == 0 && result_task == 0  ) {   
            if (current->priority < 2) {
                current->priority++;
            } else if (current->priority == 2) {
                current->priority = 0;
            }
            flag = flag+1;
        } 
        current = current->next;
    }
    if (flag == 0) {         
        printf("Could not find task '%s' in category '%s'.\n", task, category);
    }

}





void print_tasks(struct todo_list *todo) {
    printf("==== Your ToDo List ====\n");
    int task_num = 1;
    struct task *current = todo->tasks;
    if (todo->tasks == NULL) {
        printf("All tasks completed, you smashed it!\n");
    } 
    while (current != NULL) {
        
        print_one_task(task_num, current);
        task_num++;
        current = current->next;

    }
    
    printf("====   That's it!   ====\n");
    
}

void print_complete_task_list(struct todo_list *todo) {
    
    printf("==== Completed Tasks ====\n");
    struct completed_task *current = todo->completed_tasks;
    
    if (current == NULL) {
        printf("No tasks have been completed today!\n");
    }

    while (current != NULL) {
        print_completed_task(current);
        current = current->next;

    }
    printf("=========================\n");
    
}
void add_task(struct todo_list *todo) {
    char buffer[MAX_STRING_LENGTH];
    fgets(buffer, MAX_STRING_LENGTH, stdin);
    char task_name[MAX_TASK_LENGTH];
    char task_category[MAX_CATEGORY_LENGTH];
    enum priority task_priority;
    parse_add_task_line(buffer, task_name, task_category, &task_priority);
    struct task *new_node = create_node(task_name, task_category, task_priority);
    add_last_uncompleted(todo, new_node);
}

void add_last_uncompleted(struct todo_list *todo, struct task *new_node) {
    if (todo->tasks == NULL) {
        todo->tasks = new_node;
       // new_node->repeatable = IS_NOT_REPEATABLE;
        return;
    }
    //list not empty add to the end of list
    struct task *current = todo->tasks;
    while (current->next != NULL) {
        //current->repeatable = IS_NOT_REPEATABLE;
        printf("r u the issue");
        current = current->next;
    } 
    //exit out of the loop where current->next = null
    //curernt node to point to the new nodes address
    current->next = new_node;
    //new_node->repeatable = IS_NOT_REPEATABLE;
}

void add_last_completed(struct todo_list *todo, struct completed_task *new_node) {
    if (todo->completed_tasks == NULL) {
        todo->completed_tasks = new_node;
    
    } else {
        struct completed_task *current = todo->completed_tasks;
        new_node->next = current;
        todo->completed_tasks = new_node;

    }
}

//todo->tasks first task (header of the list)
//todo holds both completed and non compelted takses


////////////////////////////////////////////////////////////////////////////////
/////////////////////// PROVIDED HELPER FUNCTIONS //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/**
 * Helper Function
 * You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
 *
 * Given a raw string in the format: [string1] [string2] [enum priority]
 * This function will extract the relevant values into the given variables.
 * The function will also remove any newline characters.
 *
 * For example, if given: "finish_assignment_2 assignment2 high"
 * The function will copy the string:
 *     "finish_assignment_2" into the `task_name` array
 * Then copy the string:
 *     "assignment2" into the `task_category` array
 * And finally, copy over the enum:
 *     "high" into the memory that `prio` is pointing at.
 *
 * Parameters:
 *     buffer        - A null terminated string in the following format
 *                     [string1] [string2] [enum priority]
 *     task_name     - A character array for the [string1] to be copied into
 *     task_category - A character array for the [string2] to be copied into
 *     prio          - A pointer to where [enum priority] should be stored
 *
 * Returns:
 *     None
 */
void parse_add_task_line(
    char buffer[MAX_STRING_LENGTH],
    char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH],
    enum priority *prio
) {
    remove_newline(buffer);

    // Extract value 1 as string
    char *name_str = strtok(buffer, " ");
    if (name_str != NULL) {
        strcpy(task_name, name_str);
    }

    // Extract value 2 as string
    char *category_str = strtok(NULL, " ");
    if (category_str != NULL) {
        strcpy(task_category, category_str);
    }
    
    // Extract value 3 as string
    char *prio_str = strtok(NULL, " ");
    if (prio_str != NULL) {
        *prio = string_to_priority(prio_str);
    }

    if (
        name_str == NULL ||
        category_str == NULL ||
        prio_str == NULL ||
        *prio == INVALID_PRIORITY
    ) {
        // If any of these are null, there were not enough words.
        printf("Could not properly parse line: '%s'.\n", buffer);
    }
}

/*
 * Helper Function
 * You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
 *
 * See `parse_add_task_line` for explanation - This function is very similar,
 * with only the exclusion of an `enum priority`.
 */
void parse_task_category_line(
    char buffer[MAX_STRING_LENGTH],
    char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH]
) {
    remove_newline(buffer);

    // Extract value 1 as string
    char *name_str = strtok(buffer, " ");
    if (name_str != NULL) {
        strcpy(task_name, name_str);
    }

    // Extract value 2 as string
    char *category_str = strtok(NULL, " ");
    if (category_str != NULL) {
        strcpy(task_category, category_str);
    }

    if (name_str == NULL || category_str == NULL) {
        // If any of these are null, there were not enough words.
        printf("Could not properly parse line: '%s'.\n", buffer);
    }
}

/*
 * Helper Function
 * You DO NOT NEED TO UNDERSTAND THIS FUNCTION, and will not need to change it.
 *
 * See `parse_add_task_line` for explanation - This function is very similar,
 * with only the exclusion of an `enum priority` and addition of start/end times
 */
void parse_complete_task_line(
    char buffer[MAX_STRING_LENGTH],
    char task_name[MAX_TASK_LENGTH],
    char task_category[MAX_CATEGORY_LENGTH],
    int *start_time,
    int *finish_time
) {
    remove_newline(buffer);

    // Extract value 1 as string
    char *name_str = strtok(buffer, " ");
    if (name_str != NULL) {
        strcpy(task_name, name_str);
    }

    // Extract value 2 as string
    char *category_str = strtok(NULL, " ");
    if (category_str != NULL) {
        strcpy(task_category, category_str);
    }
    
    // Extract value 2 as string
    char *start_str = strtok(NULL, " ");
    if (start_str != NULL) {
        *start_time = atoi(start_str);
    }
    
    // Extract value 2 as string
    char *finish_str = strtok(NULL, " ");
    if (finish_str != NULL) {
        *finish_time = atoi(finish_str);
    }

    if (
        name_str == NULL ||
        category_str == NULL ||
        start_str == NULL ||
        finish_str == NULL
    ) {
        // If any of these are null, there were not enough words.
        printf("Could not properly parse line: '%s'.\n", buffer);
    }
}

/**
 * Helper Function
 * You should not need to change this function.
 *
 * Given a raw string, will return the corresponding `enum priority`,
 * or INVALID_PRIORITY if the string doesn't correspond with the enums.
 *
 * Parameters:
 *     priority - string representing the corresponding `enum priority` value
 * Returns:
 *     enum priority
 */
enum priority string_to_priority(char priority[MAX_STRING_LENGTH]) {
    if (strcmp(priority, "low") == 0) {
        return LOW;
    } else if (strcmp(priority, "medium") == 0) {
        return MEDIUM;
    } else if (strcmp(priority, "high") == 0) {
        return HIGH;
    }

    return INVALID_PRIORITY;
}

/**
 * Helper Function
 * You should not need to change this function.
 *
 * Given an priority and a character array, fills the array with the
 * corresponding string version of the priority.
 *
 * Parameters:
 *     prio - the `enum priority` to convert from
 *     out  - the array to populate with the string version of `prio`.
 * Returns:
 *     Nothing
 */
void priority_to_string(enum priority prio, char out[MAX_STRING_LENGTH]) {
    if (prio == LOW) {
        strcpy(out, "LOW");
    } else if (prio == MEDIUM) {
        strcpy(out, "MEDIUM");
    } else if (prio == HIGH) {
        strcpy(out, "HIGH");
    } else {
        strcpy(out, "Provided priority was invalid");
    }
}

/*
 * Helper Function
 * You should not need to change this function.
 *
 * Given a raw string will remove and first newline it sees.
 * The newline character wil be replaced with a null terminator ('\0')
 *
 * Parameters:
 *     input - The string to remove the newline from
 *
 * Returns:
 *     Nothing
 */
void remove_newline(char input[MAX_STRING_LENGTH]) {
    // Find the newline or end of string
    int index = 0;
    while (input[index] != '\n' && input[index] != '\0') {
        index++;
    }
    // Goto the last position in the array and replace with '\0'
    // Note: will have no effect if already at null terminator
    input[index] = '\0';
}

/*
 * Helper Function
 * You likely do not need to change this function.
 *
 * Given a raw string, will remove any whitespace that appears at the start or
 * end of said string.
 *
 * Parameters:
 *     input - The string to trim whitespace from
 *
 * Returns:
 *     Nothing
 */
void trim_whitespace(char input[MAX_STRING_LENGTH]) {
    remove_newline(input);
    
    int lower;
    for (lower = 0; input[lower] == ' '; ++lower);
    
    int upper;
    for (upper = strlen(input) - 1; input[upper] == ' '; --upper);
    
    for (int base = lower; base <= upper; ++base) {
        input[base - lower] = input[base];
    }
}

/**
 * Helper Function
 * You SHOULD NOT change this function.
 *
 * Given a task, prints it out in the format specified in the assignment.
 *
 * Parameters:
 *     task_num - The position of the task within a todo list
 *     task     - The task in question to print
 *
 * Returns:
 *     Nothing
 */
void print_one_task(int task_num, struct task *task) {
    char prio_str[MAX_STRING_LENGTH];
    priority_to_string(task->priority, prio_str);

    printf(
        "  %02d. %-30.30s [ %s ] %s\n",
        task_num, task->task_name, task->category, prio_str
    );

    int i = 30;
    while (i < strlen(task->task_name)) {
        printf("      %.30s\n", task->task_name + i);
        i += 30;
    }
}

/**
 * Helper Function
 * You SHOULD NOT change this function.
 *
 * Given a completed task, prints it out in the format specified in the
 * assignment.
 *
 * Parameters:
 *     completed_task - The task in question to print
 *
 * Returns:
 *     Nothing
 */
void print_completed_task(struct completed_task *completed_task) {
    int start_hour = completed_task->start_time / 60;
    int start_minute = completed_task->start_time % 60;
    int finish_hour = completed_task->finish_time / 60;
    int finish_minute = completed_task->finish_time % 60;
    
    printf(
        "  %02d:%02d-%02d:%02d | %-30.30s [ %s ]\n",
        start_hour, start_minute, finish_hour, finish_minute,
        completed_task->task->task_name, completed_task->task->category
    );

    int i = 30;
    while (i < strlen(completed_task->task->task_name)) {
        printf("      %.30s\n", (completed_task->task->task_name + i));
        i += 30;
    }
}

/**
 * Compares two tasks by precedence of category -> priority -> name and returns
 * an integer referring to their relative ordering
 * 
 * Parameters:
 *     t1 - The first task to compare
 *     t2 - The second task to compare
 *
 * Returns:
 *     a negative integer if t1 belongs before t2
 *     a positive integer if t1 belongs after t2
 *     0 if the tasks are identical (This should never happen in your program)
 */
int task_compare(struct task *t1, struct task *t2) {
    int category_diff = strcmp(t1->category, t2->category);
    if (category_diff != 0) {
        return category_diff;
    }
    
    int priority_diff = t2->priority - t1->priority;
    if (priority_diff != 0) {
        return priority_diff;
    }
    
    return strcmp(t1->task_name, t2->task_name);
}

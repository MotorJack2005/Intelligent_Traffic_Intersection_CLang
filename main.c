#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "stack.h"
#include "priority_queue.h"
#include "linkedlist.h"


// ░██████╗███████╗███╗░░██╗░█████╗░██████╗░░█████╗░░██████╗
// ██╔════╝██╔════╝████╗░██║██╔══██╗██╔══██╗██╔══██╗██╔════╝
// ╚█████╗░█████╗░░██╔██╗██║███████║██║░░██║██║░░██║╚█████╗░
// ░╚═══██╗██╔══╝░░██║╚████║██╔══██║██║░░██║██║░░██║░╚═══██╗
// ██████╔╝███████╗██║░╚███║██║░░██║██████╔╝╚█████╔╝██████╔╝
// ╚═════╝░╚══════╝╚═╝░░╚══╝╚═╝░░╚═╝╚═════╝░░╚════╝░╚═════╝░

// ███████╗███╗░░██╗████████╗███████╗██████╗░██████╗░██████╗░██╗░██████╗███████╗
// ██╔════╝████╗░██║╚══██╔══╝██╔════╝██╔══██╗██╔══██╗██╔══██╗██║██╔════╝██╔════╝
// █████╗░░██╔██╗██║░░░██║░░░█████╗░░██████╔╝██████╔╝██████╔╝██║╚█████╗░█████╗░░
// ██╔══╝░░██║╚████║░░░██║░░░██╔══╝░░██╔══██╗██╔═══╝░██╔══██╗██║░╚═══██╗██╔══╝░░
// ███████╗██║░╚███║░░░██║░░░███████╗██║░░██║██║░░░░░██║░░██║██║██████╔╝███████╗
// ╚══════╝╚═╝░░╚══╝░░░╚═╝░░░╚══════╝╚═╝░░╚═╝╚═╝░░░░░╚═╝░░╚═╝╚═╝╚═════╝░╚══════╝


enum COMMANDS {
    ARRIVE,
    SET_SIGNAL_STRATEGY,
    TICK,
    EMERGENCY_PREEMPT,
    STATUS,
    END
};

enum OUTPUT_PROMPT {
    PASS,
    EMERGENCY_PASS
};



struct _VEHICLE_ {
    char *id;
    unsigned int lane;
    unsigned int arrival_t;
    unsigned int final_t;

    enum DIRECTION{
        LEFT,
        STRAIGHT,
        RIGHT
    }direction;

    enum VEHICLE_TYPE {
        CAR,
        BUS,
        EMERGENCY
    }type;
};

struct _LANE_ {

    struct _QUEUE_ *queue;
};

struct _INTERSECTION_ {
    int lane_x;
    struct _LANE_ *lanes[4];
    struct _STACK_ *emergency_lane;
    struct _LINKEDLIST_ *logs;
    enum SIGNAL_STRATEGY {
        ROUND_ROBIN,
        LENGTH_BASED,
    }strat;
};


struct _VEHICLE_ *create_vehicle(unsigned int lane, char *id, enum VEHICLE_TYPE type, unsigned int a_t) {
    struct _VEHICLE_ *v = (struct _VEHICLE_*) malloc(sizeof(struct _VEHICLE_));
    v->id = id;
    v->lane = lane;
    v->arrival_t = a_t;
    v->type = type;
    return v;
}


struct _LANE_ *create_lane() {
    struct _LANE_ *l = (struct _LANE_*) malloc(sizeof(struct _LANE_));
    l->queue = create_queue();
    return l;
}


struct _INTERSECTION_ *create_intersection() {
    struct _INTERSECTION_ *s = (struct _INTERSECTION_*) malloc(sizeof(struct _INTERSECTION_));

    for(int i=0;i<4;i++) {
        s->lanes[i] = create_lane();
    }
    s->emergency_lane = create_stack();
    s->lane_x = 1;
    s->logs = create_list();
    return s;
}


enum COMMANDS str_to_command(char *str) {
    char *upper_str = strupr(str);
         if(strcmp(upper_str, "ARRIVE")              == 0) return ARRIVE;
    else if(strcmp(upper_str, "SET_SIGNAL_STRATEGY") == 0) return SET_SIGNAL_STRATEGY;
    else if(strcmp(upper_str, "TICK")                == 0) return TICK;
    else if(strcmp(upper_str, "EMERGENCY_PREEMPT")   == 0) return EMERGENCY_PREEMPT;
    else if(strcmp(upper_str, "STATUS")              == 0) return STATUS;
    else if(strcmp(upper_str, "END")                 == 0) return END;
}

enum VEHICLE_TYPE str_to_vehicle(char *str) {
    char *upper_str = strupr(str);
         if(strcmp(upper_str, "CAR")        == 0) return CAR;
    else if(strcmp(upper_str, "BUS")        == 0) return BUS;
    else if(strcmp(upper_str, "EMERGENCY")  == 0) return EMERGENCY;
}

enum SIGNAL_STRATEGY str_to_strategy(char *str) {
    char *upper_str = strupr(str);
         if(strcmp(upper_str, "ROUND_ROBIN")      == 0) return ROUND_ROBIN;
    else if(strcmp(upper_str, "LENGTH_BASED")     == 0) return LENGTH_BASED;
}

char *strategy_to_str(enum SIGNAL_STRATEGY strat) {
    switch(strat) {
        case ROUND_ROBIN:       return "ROUND_ROBIN";
        case LENGTH_BASED:      return "LENGTH_BASED";
    }
    return "";
}

char **verify_input(char *rgstr, int s, char *error_msg) {
    char **inputs = (char**) malloc(s * sizeof(char*));
    for(int i=0;i<s;i++) {
        rgstr = strtok(NULL, " ");
        if(rgstr) {
            inputs[i] = rgstr;
        }
        else {
            printf("%s\n", error_msg);
            return NULL;
        }
    }
    return inputs;
}


//PANG DEBUG PURPOSES RANI DELETE NI KUNG HUMANA !!
void display(char *cmd, char **registers, int s) {
    printf("COMMAND: %s\n", cmd);

    for(int i=0; i<s;i++) {
        printf("%s\n", registers[i]);
    }
    printf("\n");
}

void display_queue(struct _QUEUE_ *q) {
    struct _NODE_QUEUE_ *ptr = q->first;
    while(ptr!=NULL) {
        struct _VEHICLE_ *v = (struct _VEHICLE_*) ptr->data;
        printf("%s -> ", v->id);
        ptr=ptr->next;
    }
    printf("NULL\n");

}


void display_vehicle(struct _VEHICLE_ *v) {
    printf("%s %d %d\n", v->id, v->lane, v->final_t);
}

void round_robin(struct _INTERSECTION_ *junction, int tick) {

    int *x= &(junction->lane_x);

    struct _QUEUE_ *lane = junction->lanes[(*x)-1]->queue;
    struct _VEHICLE_ *v = (struct _VEHICLE_*) peek_q(lane);
    v->final_t = tick;

    add_element(junction->logs, v);
    dequeue(lane);

    *x %= 4;
}

void length_based(struct _INTERSECTION_ *junction, int tick) {
    struct _PRIORITY_QUEUE_ *p_q = create_priority_queue();
    for(int i=0; i<4;i++) {
        if(junction->lanes[i]->queue->first != NULL)
            push_pq(p_q, junction->lanes[i]->queue, junction->lanes[i]->queue->total_size);
    }
    if(peek_pq(p_q)!=NULL) {

        struct _QUEUE_ *q = (struct _QUEUE_*) peek_pq(p_q);

        if (peek_q(q)!=NULL) {
            struct _VEHICLE_ *v = (struct _VEHICLE_*) peek_q(q);
            v->final_t = tick;
            add_element(junction->logs, v);
            dequeue(q);

        }
        else {            
            pop_pq(p_q);
        }        
    }
    
}

void display_logs(struct _LINKEDLIST_ *l) {
    struct _LIST_NODE_ *ptr = l->head;
    while(ptr!=NULL) {
        printf("PASS ");
        display_vehicle((struct _VEHICLE_*)ptr->data);
        ptr=ptr->next;
    }
}

void simulate(struct _INTERSECTION_ *junction, enum SIGNAL_STRATEGY strategy, int tick) {
    switch(strategy) {

        case ROUND_ROBIN: {
            round_robin(junction,tick);
            break;
        }
        case LENGTH_BASED: {
            length_based(junction, tick);
            break;
        }

    }
}

void initiate(struct _INTERSECTION_ *junction) {
    char input[256];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    char *string_register = strtok(input," ");
    if(!string_register) return initiate(junction);

    enum COMMANDS cmd = str_to_command(string_register);

    switch(cmd) {
        case ARRIVE: {
            char **arrive_registers = verify_input(string_register, 4, "ARRIVE <Lane> <VehicleID> <Type> <t>");

            if(arrive_registers != NULL) {
                int lane = atoi(arrive_registers[0]);
                //RUN COMMAND IF INPUT CORRECT !
                struct _VEHICLE_ *new_vehicle = create_vehicle(lane,
                                                               strupr(arrive_registers[1]),
                                                               str_to_vehicle(arrive_registers[2]),
                                                               atoi(arrive_registers[3]));
                if(new_vehicle->type == EMERGENCY) {
                    push(junction->emergency_lane, new_vehicle);
                }
                else {
                    enqueue(junction->lanes[lane-1]->queue, new_vehicle);
                }
                
            }
            break;
        }
        case SET_SIGNAL_STRATEGY:{
            char **strategy_registers = verify_input(string_register, 1, "SET_SIGNAL_STRATEGY <strategy>");

            if(strategy_registers != NULL) {
                //RUN COMMAND IF INPUT CORRECT !
                junction->strat = str_to_strategy(strategy_registers[0]);
                
            }
            break;
        }

        case TICK: {
            char **tick_registers = verify_input(string_register, 1, "TICK <t>");

            if(tick_registers != NULL) {
                // RUN COMMAND IF INPUT CORRECT !
                int tick = atoi(tick_registers[0]);
                simulate(junction, junction->strat, tick);
            }
            break;
        }

        case EMERGENCY_PREEMPT: {
            char **preempt_registers = verify_input(string_register, 2, "EMERGENCY_PREEMPT <VehicleID> <t>");

            if(preempt_registers != NULL) {
                // RUN COMMAND IF INPUT CORRECT !
                int t = atoi(preempt_registers[1]);
                struct _VEHICLE_ *v = (struct _VEHICLE_*) peek(junction->emergency_lane);
                v->final_t = t;
                add_element(junction->logs, v);
                pop(junction->emergency_lane);
            }
            break;
        }
        case STATUS: {
            // RUN COMMAND IF INPUT CORRECT !
            printf("STRATEGY: %s\n",strategy_to_str(junction->strat));
            display_queue(junction->lanes[0]->queue);
            display_queue(junction->lanes[1]->queue);
            display_queue(junction->lanes[2]->queue);
            display_queue(junction->lanes[3]->queue);
        }
        case END: {
            display_logs(junction->logs);
            return;
        }
    }
    initiate(junction);

}



// ⠀⠀⢀⣤⣶⣶⣤⣄⡀
// ⠀⢀⣿⣿⣿⣿⣿⣿⣿⡆
// ⠀⠸⣿⣿⣿⣿⣿⡟⡟⡗ ⣿⠉⣿⠉⣿⡏⠹⡏⢹⡏⢹⣿⣿⠉⣿⠉⣿⡟⢋⠛⣿⠉⡟⢉⡏⠹⠏⣹⣿
// ⠀⠀⠙⠏⠯⠛⣉⢲⣧⠟ ⣿⠄⣿⠄⣿⡇⡄⠁⢸⡇⢸⣿⣿⠄⣿⠄⣿⠄⣿⣿⣿⠄⡀⢻⣿⡄⢠⣿⣿
// ⠀⠀⠠⢭⣝⣾⠿⣴⣿⠇ ⣿⣦⣤⣴⣿⣧⣿⣤⣼⣧⣬⣭⣿⣦⣤⣴⣿⣧⣤⣤⣿⣤⣷⣤⣿⣧⣼⣿⣿
// ⠀⠀⢐⣺⡿⠁⠀⠈⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀ ⣶⣶⣶⣶⣶⣶⠀
// ⠀⠀⣚⣿⠃ ⣶⣶⣶⣶
// ⢀⣿⣿⣿⣷⢒⣢⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⣶⣶⣄⠄
// ⢰⣿⣿⡿⣿⣦⠬⢝⡄⠀⠀⠀⠀⠀⠀⢠⣿⠿⠿⠟⠛⠋⠁
// ⠠⢿⣿⣷⠺⣿⣗⠒⠜⡄⠀⠀⠀⠀⣴⠟⠁
// ⠀⣰⣿⣷⣍⡛⣯⣯⣙⡁⠀⠀⣠⡾⠁
// ⠀⠨⢽⣿⣷⢍⣛⣶⢷⣼⣠⣾⠋
// ⠀⠀⠘⢿⣿⣖⠬⣹⣶⣿⠟⠁
// ⠀⠀⠀⠚⠿⠿⡒⠨⠛⠋
// ⠀⠀⠀⠐⢒⣛⣷
// ⠀⠀⠀⢘⣻⣭⣭
// ⠀⠀⠀⡰⢚⣺⣿
// ⠀⠀⢠⣿⣿⣿⣿⣦⡄
// ⠀⠀⢸⡿⢿⣿⢿⡿⠃
// ⠀⠀⠘⡇⣸⣿⣿⣿⣆
// ⠀⠀⠀⠀⠸⣿⡿⠉⠁
// ⠀⠀⠀⠀⠀⢿⡟

int main() {

    initiate(create_intersection());

    return 0;
}
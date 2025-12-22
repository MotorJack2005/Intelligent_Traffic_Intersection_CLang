#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "stack.h"
#include "priority_queue.h"


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
    END,
    INVALID
};

enum OUTPUT_PROMPT {
    PASS,
    EMERGENCY_PASS
};



struct _VEHICLE_ {
    char *id;
    unsigned int lane;
    unsigned int t;

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

    enum LANE_TYPE {
        STANDARD_LANE,
        EMERGENCY_LANE
    }type;

    struct _QUEUE_ *queue;
    struct _STACK_ *stack;
};

struct _INTERSECTION_ {
    unsigned int tick;
    struct _LANE_ *lanes[4];
    struct _LANE_ *emergency_lane;
    enum SIGNAL_STRATEGY {
        ROUND_ROBIN,
        LENGTH_BASED,
        EMERGENCY_FIRST
    }strat;
};


struct _VEHICLE_ *create_vehicle(unsigned int lane, char *id, unsigned int t, enum VEHICLE_TYPE type) {
    struct _VEHICLE_ *v = (struct _VEHICLE_*) malloc(sizeof(struct _VEHICLE_));
    v->id = id;
    v->lane = lane;
    v->t = t;
    v->type = type;
    return v;
}


struct _LANE_ *create_lane(enum LANE_TYPE type) {
    struct _LANE_ *l = (struct _LANE_*) malloc(sizeof(struct _LANE_));
    switch(type) {
        case STANDARD_LANE: {
            l->stack = create_stack();
            l->queue = NULL;
            break;
        }
        case EMERGENCY_LANE: {
            l->queue = create_queue();
            l->stack = NULL;
            break;
        }
            
    }
    return l;
}


struct _INTERSECTION_ *create_intersection() {
    struct _INTERSECTION_ *s = (struct _INTERSECTION_*) malloc(sizeof(struct _INTERSECTION_));

    for(int i=0;i<4;i++) {
        s->lanes[i] = create_lane(STANDARD_LANE);
    }
    s->emergency_lane = create_lane(EMERGENCY_LANE);
    s->tick = 0;
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
    else                                                   return INVALID;
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
    else if(strcmp(upper_str, "EMERGENCY_FIRST")  == 0) return EMERGENCY_FIRST;
}

char *strategy_to_str(enum SIGNAL_STRATEGY strat) {
    switch(strat) {
        case ROUND_ROBIN:       return "ROUND_ROBIN";
        case LENGTH_BASED:      return "LENGTH_BASED";
        case EMERGENCY_FIRST:   return "EMERGENCY_FIRST";
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
                                                               atoi(arrive_registers[2]),
                                                               str_to_vehicle(arrive_registers[3])
                                                              );
                enqueue(junction->lanes[lane-1]->queue, new_vehicle);
                display("ARRIVE", arrive_registers, 4);
            }
            break;
        }
        case SET_SIGNAL_STRATEGY:{
            char **strategy_registers = verify_input(string_register, 1, "SET_SIGNAL_STRATEGY <strategy>");

            if(strategy_registers != NULL) {
                //RUN COMMAND IF INPUT CORRECT !
                junction->strat = str_to_strategy(strategy_registers[0]);
                display("SET_SIGNAL_STRATEGY", strategy_registers, 1);
            }
            break;
        }

        case TICK: {
            char **tick_registers = verify_input(string_register, 1, "TICK <t>");

            if(tick_registers != NULL) {
                // RUN COMMAND IF INPUT CORRECT !
                display("TICK", tick_registers, 1);
            }
            break;
        }

        case EMERGENCY_PREEMPT: {
            char **preempt_registers = verify_input(string_register, 1, "EMERGENCY_PREEMPT <VehicleID> <t>");

            if(preempt_registers != NULL) {
                // RUN COMMAND IF INPUT CORRECT !
                display("EMERGENCY_PREEMPT", preempt_registers, 1);
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
    struct _INTERSECTION_ *new_junction = create_intersection();
    initiate(new_junction);

    return 0;
}
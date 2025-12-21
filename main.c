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
    unsigned int lane;
    unsigned int id;
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
    struct _QUEUE_ *queue;
    struct _VEHICLE_ *first_vehicle;
};

struct _INTERSECTION_ {
    unsigned int tick;
    struct _LANE_ *lanes[4];

    enum SIGNAL_STRATEGY {
        ROUND_ROBIN,
        LENGTH_BASED,
        EMERGENCY_FIRST
    }strat;
};


struct _VEHICLE_ *create_vehicle(unsigned int lane, unsigned int id, unsigned int t, enum VEHICLE_TYPE type) {
    struct _VEHICLE_ *v = (struct _VEHICLE_*) malloc(sizeof(struct _VEHICLE_));
    v->id = id;
    v->lane = lane;
    v->t = t;
    v->type = type;
    return v;
}


struct _LANE_ *create_lane() {
    struct _LANE_ *l = (struct _LANE_*) malloc(sizeof(struct _LANE_));
    l->queue = create_queue();
    l->first_vehicle = NULL;
    return l;
}


struct _INTERSECTION_ *create_intersection(unsigned int tick, struct _LANE_ **lanes, enum SIGNAL_STRATEGY strat) {
    struct _INTERSECTION_ *s = (struct _INTERSECTION_*) malloc(sizeof(struct _INTERSECTION_));

    for(int i=0;i<4;i++) {
        s->lanes[i] = lanes[i];
    }
    s->tick = tick;
    s->strat = strat;
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


void initiate() {
    char input[256];
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    char *string_register = strtok(input," ");
    if(!string_register) return initiate();

    enum COMMANDS cmd = str_to_command(string_register);

    switch(cmd) {
        case ARRIVE: {
            char **arrive_registers = verify_input(string_register, 4, "ARRIVE <Lane> <VehicleID> <Type> <t>");

            if(arrive_registers != NULL) {
                //RUN COMMAND IF INPUT CORRECT !
                display("ARRIVE", arrive_registers, 4);
            }
            break;
        }
        case SET_SIGNAL_STRATEGY:{
            char **strategy_registers = verify_input(string_register, 1, "SET_SIGNAL_STRATEGY <strategy>");

            if(strategy_registers != NULL) {
                //RUN COMMAND IF INPUT CORRECT !
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
        }
        case END: {
            return;
        }
    }
    initiate();

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
    initiate();

    return 0;
}
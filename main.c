#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "stack.h"


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


    return 0;
}
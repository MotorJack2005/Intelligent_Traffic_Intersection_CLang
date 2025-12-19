#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


//   _________                         .___             _________            .___.__                 ___________       __                            .__               
//  /   _____/ ____   ____ _____     __| _/____  ______ \_   ___ \  ____   __| _/|__| ____    ____   \_   _____/ _____/  |_  ________________________|__| ______ ____  
//  \_____  \_/ __ \ /    \\__  \   / __ |/  _ \/  ___/ /    \  \/ /  _ \ / __ | |  |/    \  / ___\   |    __)_ /    \   __\/ __ \_  __ \____ \_  __ \  |/  ___// __ \ 
//  /        \  ___/|   |  \/ __ \_/ /_/ (  <_> )___ \  \     \___(  <_> ) /_/ | |  |   |  \/ /_/  >  |        \   |  \  | \  ___/|  | \/  |_> >  | \/  |\___ \\  ___/ 
// /_______  /\___  >___|  (____  /\____ |\____/____  >  \______  /\____/\____ | |__|___|  /\___  /  /_______  /___|  /__|  \___  >__|  |   __/|__|  |__/____  >\___  >
//         \/     \/     \/     \/      \/          \/          \/            \/         \//_____/           \/     \/          \/      |__|                 \/     \/ 


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

enum SIGNAL_STRATEGY {
    ROUND_ROBIN,
    LENGTH_BASED
};

enum VEHICLE_TYPE {
    CAR,
    BUS,
    EMERGENCY
};


struct _VEHICLE_ {
    unsigned int lane;
    unsigned int id;
    unsigned int t;
    enum VEHICLE_TYPE type;
};


struct _LANE_ {
    struct _QUEUE_ *queue;
    struct _VEHICLE_ *first_vehicle;
};

struct _INTERSECTION_ {
    unsigned int tick;
    struct _LANE_ *lanes[4];
    enum SIGNAL_STRATEGY strat;
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



int main(int argc, char **argv) {


    return 0;
}
#ifndef OVERWORLD_H
#define OVERWORLD_H

struct TrainerSpotted {
    /* NPC state id */
    u8 id;

    /* Distance from trainer to player */
    u8 distance;

    /* The script the on the trainer NPC. */
    void *script;
};

struct TrainerSpotState {
    u8 count;
    struct TrainerSpotted trainers[2];
};

extern struct TrainerSpotState trainers_spotted;

#endif /* OVERWORLD_H */

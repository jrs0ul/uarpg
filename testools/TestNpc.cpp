#include "../src/npc.h"

int main(){

    NpcArray npc;

    npc.load("../base/maps/throneroom.npc", "");

    npc.destroy();

}

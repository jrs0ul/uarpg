#include "../src/SpellData.h"

int main(){

    SpellData spells;

    if (!spells.load("../baseLT/data/spells.xml"))
        return -1;


    for (unsigned long i = 0; i < spells.spells.count(); i++){
        puts(spells.spells[i].name);
    }

    spells.destroy();

}

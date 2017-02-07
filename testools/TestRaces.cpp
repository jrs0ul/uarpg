
#include "../src/Races.h"


RaceList Races;

int main(){
    Races.load("../baseLT/data/races.xml");
    printf("Total races: %u\n", Races.count());
    puts("---------------------------------");
    for (unsigned i = 0; i < Races.count(); i++){

        Xrace * rc = 0;
        rc = Races.get(i);

        puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
        printf("Name: %12s\n", rc->name);
        printf("Playble ? : %s\n", (rc->isPlayable) ? "yes" : "no");
        printf("Evolution speed %d\n", rc->espeed);
        for (unsigned a = 0; a < rc->maxsex; a++){
            if (a == 0)
                puts("male");
            else
                puts("female");

            puts("BasicPoints----------------------------------------------------------------------");
            puts("|    hp    |    mp   |   atack   |  defence  |  agility  |  magpower |   spirit  |");
            puts("---------------------------------------------------------------------------------");
            printf("|%4u/%5u|%4u/%4u|%5u/%5u|%5u/%5u|%5u/%5u|%5u/%5u|%5u/%5u|\n",
                    rc->startp[a].hp.min, rc->startp[a].hp.max,
                    rc->startp[a].mp.min, rc->startp[a].mp.max,
                    rc->startp[a].atck.min, rc->startp[a].atck.max,
                    rc->startp[a].dfns.min, rc->startp[a].dfns.max,
                    rc->startp[a].agil.min, rc->startp[a].agil.max,
                    rc->startp[a].magp.min, rc->startp[a].magp.max,
                    rc->startp[a].sprt.min, rc->startp[a].sprt.max);
            puts("---------------------------------------------------------------------------------");
            puts("BonusPoints----------------------------------------------------------------------");
            puts("|    hp    |    mp   |   atack   |  defence  |  agility  |  magpower |   spirit  |");
            puts("---------------------------------------------------------------------------------");
            printf("|%10u|%9u|%11u|%11u|%11u|%11u|%11u|\n",
                    rc->bonusp[a].hp,
                    rc->bonusp[a].mp,
                    rc->bonusp[a].atck,
                    rc->bonusp[a].dfns,
                    rc->bonusp[a].agil,
                    rc->bonusp[a].magp,
                    rc->bonusp[a].sprt);

            puts("---------------------------------------------------------------------------------");

            puts("Resistence-----------------------------------------------------------------------");
            puts("|       fire       |         ice         |        bolt        |      poison      |");
            puts("---------------------------------------------------------------------------------");
            printf("|%18d|%21d|%20d|%18d|\n",
                    rc->resistense.fire, rc->resistense.ice,
                    rc->resistense.bolt, rc->resistense.poison);

            puts("---------------------------------------------------------------------------------");

        }
    }
    Races.destroy();
}

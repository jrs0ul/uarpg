
#include "../src/Klasses.cpp"


KlassList klasses;

int main(){

    if (!klasses.load("../base/data/classes.xml")){
        puts("failed to load stuff");
        return -1;
    }

    for (unsigned i = 0; i < klasses.count(); i++){

        XKlass * kl = 0;
        kl = klasses.get(i);

        if (kl){
            puts(   "-----------------------------------------");
            printf( "| %15s            playble: %c |\n", kl->name, (kl->isPlayable)? 'y' : 'n');
            printf( "| hp: %10d         mp: %10d |\n", kl->hp, kl->mp);
            printf( "| attack: %6d         deffense: %4d |\n", kl->atck, kl->dfns);
            printf( "| agility: %5d         spirit: %6d |\n", kl->agil, kl->sprt);
            printf( "| magpower: %4d                        |\n", kl->magp);
            if (kl->spells.count()){
                puts(   "| Spells:                               |");
                for (unsigned a = 0; a< kl->spells.count(); a++){
                    printf(" %u", kl->spells[a]);
                }
                puts("|");
            }
        }
        
    }
    puts(   "-----------------------------------------");

    klasses.destroy();


}

    /*Player Gfx list test*/
    
    #include "../src/PlayerGfxList.h"
    

    
    int main(){

        PlayerGfxList thelist;
        RaceList races;
        KlassList klasses;

        races.load("../baseLT/data/races.xml");
        klasses.load("../baseLT/data/classes.xml");
        thelist.load("../baseLT/data/playergfx.xml", races, klasses );
        
        for (unsigned i = 0 ; i < thelist.races.count(); i++){

            printf("======%s=======\n", races.get(i)->name);

            printf("class count %lu\n", thelist.races[i].clas.count());
            for (unsigned a = 0; a < thelist.races[i].clas.count(); a++){

                printf("=====[[%s]]====\n", klasses.get(a)->name);

                for (unsigned z = 0; z < thelist.races[i].clas[a].sex.count(); z++){
                    puts(thelist.races[i].clas[a].sex[z].m);
                    puts(thelist.races[i].clas[a].sex[z].t);
                    puts(thelist.races[i].clas[a].sex[z].s);
                    puts("~~~~~~~~~~~~~~~~~~~~~~~~");
                }
                puts("==========================");
            }
        }


        races.destroy();
        klasses.destroy();
        thelist.destroy();

    }

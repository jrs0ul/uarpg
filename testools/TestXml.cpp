#include "../src/Xml.h"

int main(){

    Xml data;
    data.load("../base/arenas/dungeon.arn");
    XmlNode * intro = data.root.getNode(L"Arena")->getNode(L"IntroPath");
    if (intro){
        XmlNode * c = 0;
        c = intro->getNode(L"Node")->getNode(L"Pos");
        if (c){
            for (unsigned i = 0; i < c->attributeCount(); i++){
                printf("name:%ls value:%ls\n", 
                c->getAttribute(i)->getName(),
                c->getAttribute(i)->getValue());
            }
        }
    }
    data.destroy();
}

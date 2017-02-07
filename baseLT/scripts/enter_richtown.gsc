NoJoy
if(Has_Spit=true)
    GoTo("11")
Say("100","50","Korn","Čia gyvena turtingi žmones","party.tga","0")
if(conversation=active)
 GoTo("5")
Say("100","50","Korn","Geriau keliaukime ten kur keliavome.","party.tga","0")
if(conversation=active)
 GoTo("8")
GoTo("12")
LoadMap("13","36","richtown")
if(Advice_Taken=true)
MovNpc("0","10","5")
NoJoy

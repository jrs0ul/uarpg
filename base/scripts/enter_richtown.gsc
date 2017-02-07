NoJoy
if(Has_Spit=true)
    GoTo("11")
Say("100","50","Korn","Rich people live here","party.tga","0")
if(conversation=active)
 GoTo("5")
Say("100","50","Korn","We'd better return to our way.","party.tga","0")
if(conversation=active)
 GoTo("8")
GoTo("12")
LoadMap("13","36","richtown")
if(Advice_Taken=true)
MovNpc("0","10","5")
NoJoy

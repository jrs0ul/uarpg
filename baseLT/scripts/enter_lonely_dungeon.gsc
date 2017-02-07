NoJoy
if(Got_Task=true)
    GoTo("6")
Say("100","50","Korn","Kodėl turėtume čia eiti ?","party.tga","0")    
GoTo("9")
LoadMap("5","1","lonely_dungeon")
if(Has_Dara=true)
    GoTo("10")
GoTo("12")
RemoveNpc("1")
RemoveNpc("2")
NoJoy

NoJoy
if(tradersknown=true)
    GoTo("11")
Let("tradersknown","true")
Say("100","50","Korn","I see the traders town ahead !","party.tga","0")
if(conversation=active)
 GoTo("6")
Say("100","50","Korn","We should visit them and get some stuff.","party.tga","0")
if(conversation=active)
 GoTo("9")
NoJoy

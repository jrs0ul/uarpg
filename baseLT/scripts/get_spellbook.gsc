NoJoy
if(Got_SpellBook=true)
    GoTo("12")
Say("100","50","Korn","E, man rodos radau tą burtų knygą!","party.tga","0")
if(conversation=active)
    GoTo("5")
Say("100","50","Spit","Jėga! Grįžtam pas tą PeeWee bičą.","party.tga","1")
if(conversation=active)
    GoTo("8")
Let("Got_SpellBook","true")
OpenBox("-1","-1","0","55","2")
NoJoy

NoJoy
if(Got_SpellBook=true)
    GoTo("12")
Say("100","50","Korn","Hey, it seams we found a spellbook","party.tga","0")
if(conversation=active)
    GoTo("5")
Say("100","50","Spit","Allright! Let's get back to that PeeWee guy.","party.tga","1")
if(conversation=active)
    GoTo("8")
Let("Got_SpellBook","true")
OpenBox("-1","-1","0","55","2")
NoJoy

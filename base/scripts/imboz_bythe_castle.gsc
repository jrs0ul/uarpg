NoJoy
if(talked_with_king=true)
GoTo("5")
GoTo("41")
if(escape=true)
GoTo("41")
Let("escape","true")
RemoveNpc("4")
RemoveNpc("3")
Say("100","50",hero0name,"Damn, I must get out of this town!",hero0face,hero0faceID)
if(conversation=active)
 GoTo("11")
PushNpc("2","1","16")
PushCam("0","1","12")
if(dude2=active)
 GoTo("15")
PushNpc("1","4","1")
if(dude1=active)
 GoTo("18")
PushNpc("1","2","3")
if(dude1=active)
 GoTo("21")
Say("100","50","GuardB","Stop killer! For the majesty !!!","citizens.tga","1")
if(conversation=active)
 GoTo("24")
AddNpc("24","18","255","horn.tga")
AddPartyMember("data/horn.chr")
PushNpc("3","1","3")
if(dude3=active)
 GoTo("29")
PushNpc("3","4","6")
if(dude3=active)
 GoTo("32")
Say("100","50","Korn","You killed that stupid king?","party.tga","0")
if(conversation=active)
 GoTo("35")
Say("100","50","Korn","I'll help you stranger.","party.tga","0")
if(conversation=active)
 GoTo("38")
fight("0")
NoJoy

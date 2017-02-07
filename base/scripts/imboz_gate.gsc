NoJoy
if(talked_with_king=true)
 GoTo("9")
SetCollision("17","26","0")
Say("100","50","GuardA","Go, talk to the king first!","citizens.tga","1")
if(conversation=active)
 GoTo("6")
MovNpc("4","17","24")
NoJoy

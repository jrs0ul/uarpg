NoJoy
if(talked_with_king=true)
 GoTo("9")
SetCollision("17","26","0")
Say("100","50","Sargybinis A","Eik pirmą pasišnekėt su karalium !","citizens.tga","1")
if(conversation=active)
 GoTo("6")
MovNpc("4","17","24")
NoJoy

NoJoy
Say("150","10","Zombių Karalius","STAIGMENA!...","citizens.tga","3")
if(conversation=active)
 GoTo("3")
SetCamPos("63.17","7.00","61.78");
SetCamDir("0.46","-0.36","-0.81");
SetCamRight("0.87","0.00","0.49");
SetCamUp("0.18","0.93","-0.32");
Say("150","10","Zombių Karalius","SUZOMBĖKIT...","citizens.tga","3")
if(conversation=active)
 GoTo("10")
Let("Victory","false")
fight("0")
if(Victory=false)
 GoTo("16")
if(Victory=true)
 GoTo("19")
GoTo("14")
Say("150","10","Zombių Karalius","JŪS MANE NUGALĖJOT...","citizens.tga","3")
if(conversation=active)
 GoTo("20")
Say("150","10","Zombių Karalius","ŠŪDAS.","citizens.tga","3")
if(conversation=active)
 GoTo("23")
Let("EndGame","true")
NoJoy

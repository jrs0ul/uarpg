NoJoy
if(talkedbyimboz=true)
 GoTo("20")
Say("100","50","Korn","Nuo šiol tu karalystės priešas...","party.tga","0")
if(conversation=active)
 GoTo("5")
Say("100","50",hero0name,"Bet aš ne....",hero0face,hero0faceID)
if(conversation=active)
 GoTo("8")
Say("100","50","Korn","Užteks pezėt. Einam į mūsų būstinę.","party.tga","0")
if(conversation=active)
 GoTo("11")
Say("100","50","Korn","Ji yra pietiniame šios salos krante.","party.tga","0")
if(conversation=active)
 GoTo("14")
Say("100","50",hero0name,"Na gerai... Eime.",hero0face,hero0faceID)
if(conversation=active)
 GoTo("17")
Let("talkedbyimboz","true")
NoJoy

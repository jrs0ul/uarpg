if(Has_Spit=true)
    GoTo("67")
PushNpc("4","3","1")
if(dude4=active)
    GoTo("4")
PushNpc("4","2","4")
if(dude4=active)
    GoTo("7")
PushNpc("4","4","1")
if(dude4=active)
    GoTo("10")
SetCamPos("73.54","6.32","54.14")
SetCamDir("0.79","-0.41","-0.45")
SetCamRight("-0.49","0.0","-0.87")
SetCamUp("0.36","0.91","-0.20")
Say("150","300","Korn","Ei, Spit, tu nepatikėsi.","party.tga","0")
if(conversation=active)
 GoTo("17")
Say("150","300","Korn","Karalius negyvas!","party.tga","0")
Redir("2","1","0")
if(conversation=active)
 GoTo("21")
SetCamPos("103.47","11.81","59.78")
SetCamDir("-0.53","-0.51","-0.68")
SetCamRight("-0.79","0.0","0.62")
SetCamUp("-0.31","0.86","-0.40")
Say("100","50","Spit","Ką?! Negali būt!","fhdeemon.tga","0")
if(conversation=active)
 GoTo("28")
Say("100","50","Korn","Susipažink su šituo veikėju.","party.tga","0")
SetCamPos("98.23","12.61","81.03")
SetCamDir("-0.27","-0.49","-0.83")
SetCamRight("-0.95","0.0","0.31")
SetCamUp("-0.15","0.87","-0.47")
if(conversation=active)
 GoTo("35")
PushNpc("3","2","3")
if(dude3=active)
    GoTo("38")
Say("100","50","Korn","Jis nužudė karalių!","party.tga","0")
SetCamPos("84.34","7.91","63.38")
SetCamDir("0.35","-0.41","-0.84")
SetCamRight("-0.93","0.00","-0.38")
SetCamUp("0.16","0.91","-0.38")
if(conversation=active)
 GoTo("45")
Say("100","50",hero0name,"Bet aš ne...",hero0face,hero0faceID)
if(conversation=active)
 GoTo("48")
Say("100","50","Spit","Pala. Sugalvojau. Davai apiplešiam Turčmiestį","fhdeemon.tga","0")
if(conversation=active)
 GoTo("51")
Say("100","50","Korn","Jo davai! Dabar karalystėje chaosas","party.tga","0")
if(conversation=active)
 GoTo("54")
Say("100","50","Korn","ir niekas nesaugos aukso !","party.tga","0")
if(conversation=active)
 GoTo("57")
Say("100","50",hero0name,"O kur tas 'Turčmiestis'?",hero0face,hero0faceID)
if(conversation=active)
 GoTo("60")
Say("100","50","Spit","Jis yra truputi šiauriau nuo čia. Varom!","fhdeemon.tga","0")
if(conversation=active)
 GoTo("63")
Exec("scripts/add_spit.gsc")
NoJoy
//lolzor

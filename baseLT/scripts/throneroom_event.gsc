if(dude7=active)
    GoTo("1")
NoJoy
if(talked_with_king=true)
    GoTo("79")
SetCamPos("50.47","12.09","51.79")
SetCamDir("0.81","-0.56","0.16")
SetCamRight("-0.20","0.00","0.98")
SetCamUp("0.55","0.83","0.11")
Say("150","10","Karalius","Sveikas jaunasis nuotykių ieškotojau","citizens.tga","0")
if(conversation=active)
    GoTo("11")
Say("150","300",hero0name,"Jūsų didenybe... ",hero0face,hero0faceID)
if(conversation=active)
    GoTo("14")
Say("150","10","Karalius","Aš turiu tau užduo..*kost*","citizens.tga","0")
if(conversation=active)
    GoTo("17")
NpcFrame("6","0")
PushNpc("6","1","1")
if(dude6=active)
    GoTo("21")
Say("150","10","Karalius","*kost*...*kost*..*kost*...arghh!!!!...","citizens.tga","0")
if(conversation=active)
    GoTo("24")
NpcFrame("6","13")
PushNpc("2","2","2")
PushNpc("0","2","7")
PushNpc("1","2","7")
PushNpc("5","4","1")
PushNpc("4","3","1")
if(dude5=active)
    GoTo("32")
PushNpc("5","2","7")
PushNpc("4","2","7")
PushNpc("3","2","2")
if(dude0=active)
    GoTo("37")
Redir("2","1","0")
Redir("3","-1","0")
PushNpc("0","3","1")
PushNpc("1","4","1")
if(dude1=active)
    GoTo("43")
if(dude0=active)
    GoTo("45")
PushNpc("0","2","7")
PushNpc("1","2","7")
SetCamPos("34.23","36.87","84.17")
SetCamDir("0.60","-0.80","-0.02")
SetCamRight("0.03","0.00","1.00")
SetCamUp("0.80","0.60","-0.03")
if(dude5=active)
    GoTo("53")
if(dude4=active)
    GoTo("55")
Say("90","10","Sargybinis","O ble!!! Karalius negyvas! Čia svetimšalio kaltė!","citizens.tga","1")
if(conversation=active)
    GoTo("58")
PushNpc("7","1","1")
if(dude7=active)
    GoTo("61")
Redir("7","0","-1")
Redir("2","0","1")
if(dude0=active)
    GoTo("65")
Say("90","10","Sargybinis B","Griebkit jį!","citizens.tga","1")
if(conversation=active)
    GoTo("68")
Redir("0","0","1")
Redir("1","0","1")
Redir("5","0","1")
Redir("4","0","1")
PushNpc("7","1","18")
PushCam("0","1","10")
Let("talked_with_king","true")
if(dude7=active)
    GoTo("77")
NoJoy
NoJoy
if(Advice_Taken=true)
    GoTo(15)
Say("100","50",hero0name,"Šudas, užrakinta!",hero0face,hero0faceID)
if(conversation=active)
    GoTo(5)
PushNpc("0","4","1")
if(dude0=active)
    GoTo(8)
PushNpc("0","1","3")
if(dude0=active)
    GoTo(11)
Say("100","50","PeeWee","Pssst!...Ateikit čia...","fhdeemon.tga",1)
Let("Advice_Taken","true")
NoJoy

NoJoy
if(Heard_first_eek=true)
    GoTo("11")
Let("Heard_first_eek","true")
Say("100","50","Somebody","Eeeeek!","party.tga","2")
if(conversation=active)
    GoTo("6")
Say("150","300",hero0name,"What was THAT ?!",hero0face,hero0faceID)
if(conversation=active)
    GoTo("9")
NoJoy

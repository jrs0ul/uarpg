NoJoy
if(Heard_second_eek=true)
    GoTo("17")
Let("Heard_second_eek","true")
Say("100","50","Kažkas","Aaaaa! Padėkit!","party.tga","2")
if(conversation=active)
    GoTo("6")
Say("150","300","Korn","Atrodo kad jai reiki pagalbos","party.tga","0")
if(conversation=active)
    GoTo("9")
Say("150","300",hero0name,"Mes turim jai padėti, paskubėkim!",hero0face,hero0faceID)
if(conversation=active)
    GoTo("12")
Say("150","300","Spit","Ane? O ar tikrai ?","party.tga","1")
if(conversation=active)
    GoTo("15")
NoJoy

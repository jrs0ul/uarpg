NoJoy
if(tradersknown=true)
    GoTo("11")
Let("tradersknown","true")
Say("100","50","Korn","Matau prekeivių miestelį !","party.tga","0")
if(conversation=active)
 GoTo("6")
Say("100","50","Korn","Būtų neprošal užsukti ir šio bei to nusipirkti.","party.tga","0")
if(conversation=active)
 GoTo("9")
NoJoy

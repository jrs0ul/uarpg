Let("index","0")
inject(index,"1000")
if(index=3)
GoTo("6")
Let("index",index+"1")
GoTo("1")
Write("Done.#")
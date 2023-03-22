
count = 0
size = 99
map_txt = "./src/comp/conf/map.txt"


open(map_txt, "w").close()
with open(map_txt, "a") as f:
    for i in range(size):
        for l in range(size):
            if(i == 0 or l == 0 or i == size-1 or l == size-1):
                f.write("#")
            elif(l > size/2 - count and l < size/2 + count):
                f.write("#")
            else:
                f.write(" ")
        f.write("""\n""")
        count += 1
        if(count*2 + 1 == size):
            exit(0)
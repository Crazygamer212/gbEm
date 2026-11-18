#basic assembler for testing pourpouses

DEBUG = True
OUTPUT = "bin" # txt, bin

START_OFFSET = 0x100
BUFFER_SIZE = 32 * 1024



textB=""
with open("./Opcodes.json", "r") as f:
    textB = f.read()

exec("opcodes = " + textB.replace("true", "True").replace("false", "False"))

with open("./program.asm", "r") as f:
    asmB = f.read()

def trim(string):
    trimList = [
        ["  ", " "], 
        ["\n\n", "\n"],
        [" \n", "\n"]
    ]

    trims = 1
    while trims:
        trims=0
        for toTrim in trimList:
            if toTrim[0] in string: trims+=1
            string = string.replace(*toTrim)
    return string

lexes = trim(asmB).split("\n")
func = []
args = []
gptrs = {}
offset = 0
for i,lex in enumerate(lexes):
    lex = lex.split(";")[0] # strip comments
    if lex.endswith(":"): 
        gptrs[lex.replace(":", "")] = offset
        continue
    if lex.startswith("."):
        gptrs[lex] = offset
        continue
    while lex.startswith(" "): lex=lex[1:]
    func.append(lex.split(" ")[0])
    args.append([ a for a in trim(lex[len(func[offset])+1:]).split(", ") if a])
    offset+=1

if (DEBUG):
    for i, (fun,arg) in enumerate(zip(func, args)):
        buf = f"{i:>3}    "
        buf+= f"{fun:<4}  {str(arg):<9}"
        if (i in gptrs.values()):
            buf+= "  <<<<   " + list(gptrs.keys())[list(gptrs.values()).index(i)]
        print(buf)


R8s = ["A", "B", "C", "D", "E", "H", "L", "(HL)"]
R16s = ["AF", "BC", "DE", "HL", "SP"]

types = []
for pc,arg in enumerate(args):
    v = []
    for a in arg:
         # [ type | value ]
        if a.startswith("$") and len(a) == 5:
            v.append(["a16", int(a[1:], 16)])
        elif a.startswith("e$") and len(a) == 6:
            v.append(["e8", 0x80-int(a[2:], 16)]) #TODO must use JR for e8
        elif a.startswith("$") and len(a) == 3:
            v.append(["a8", int(a[1:], 16)])
        elif a in R8s:
            v.append([a, None])
        elif a in R16s:
            v.append([a, None])
        elif a in gptrs.keys():
            v.append(["a16", gptrs[a]+START_OFFSET])
        elif a in []:    
            offset = lptrs[a] - (pc + 2)
            if not -128 <= offset <= 127:  raise Exception("e8 jump out of range: " + a)
            v.append(["e8", offset & 0xFF])
            #TODO add a8 jumps
        else:
            raise Exception(f"unknown type : {arg}  :  {pc}")

    types.append(v)



def get_bin(fun, typ, debugstr:str = ""):
    data = bytearray()
    for operand, info in opcodes["unprefixed"].items():
        if info["mnemonic"] == fun:
            if len(info["operands"]) == len(typ):
                for i in range(len(typ)):
                    if info["operands"][i]["name"] != typ[i][0]:  break
                else:
                    data.append(int(operand, 16))
                    for ty, oper in zip(typ,info["operands"]):
                        if "bytes" in oper:
                            data.extend(ty[1].to_bytes(oper["bytes"], "little"))
                    break
    else:
        raise Exception(f"NotFound {fun}  :  {typ} " + debugstr)
    
    if DEBUG:
        print(fun, typ, data.hex())
    return data
        



if OUTPUT == "bin":
    buffer = bytearray([0 for i in range(START_OFFSET)])
    for fun, typ in zip(func, types):
        buffer.extend(get_bin(fun, typ))
    while len(buffer) != BUFFER_SIZE: buffer.append(0)

    with open("./program.bin", "wb") as f:
        f.write(buffer)
    
elif OUTPUT == "txt":
    with open("./program.txt", "w") as f:
        for fun,typ in zip(func, types):
            for byte in get_bin(fun,typ):
                f.write(f"{byte:0>2x} ")
            f.write("\n")

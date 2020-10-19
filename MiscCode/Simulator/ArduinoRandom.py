next = 1

def twos_complement(value, bitWidth):
    if value >= 2**bitWidth:
        # This catches when someone tries to give a value that is out of range
        raise ValueError("Value: {} out of range of {}-bit value.".format(value, bitWidth))
    else:
        return value - int((value << 1) & 2**bitWidth)

def do_random(ctx):
    global next
    x = twos_complement(ctx, 32)
    if (x == 0):
        x = 123459876
    hi = x // 127773
    lo = x % 127773
    x = 16807 * lo - 2836 * hi
    if (x < 0):
        x += 0x7fffffff
    next = x & 0xffffffff

    return (next % (0x7FFF0000 + 1))
		
def rando():
    global next
    return do_random(next) & 0x7fffffff
	
def srandom(seed):
    global next
    next = seed & 0xffffffff

def randomSeed(seed):
    if (seed != 0):
        srandom(seed)

def random(howbig):
    if (howbig == 0):
        return 0
    return  rando() % howbig

def random2(howsmall, howbig):
    if howsmall >= howbig:
        return howsmall
    diff = howbig - howsmall
    return random(diff) + howsmall    
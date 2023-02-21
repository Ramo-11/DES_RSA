from math import gcd
from sympy import isprime

n = 68102916241556953901301068745501609390192169871097881297
    
def modinv(e, T):
    a, b = T, e
    x, y = 0, 1
    while b:
        q, r = divmod(a, b)
        a, b, x, y = b, r, y, x - (q * y)
    if a != 1:
        raise Exception("Modular inverse does not exist")
    return x

def coprime(a, b):
    return gcd(a, b) == 1

def pollard_p_1(n, b):
    a = 2
    for j in range(2, b+1):
        a = pow(a, j, n)
        d = gcd(a-1, n)
        if d != 1 and d != n:
            return d
    return None

def gcdExtended(a, b):
    if a == 0:
        return b, 0, 1
    gcd, x1, y1 = gcdExtended(b % a, a)
    x = y1 - (b // a) * x1
    y = x1
    return gcd, x, y

p = pollard_p_1(n, 1500)
q = n // p

if p * q == n and isprime(p) and isprime(q):
    print(f"both p and q are primes\np: {p}\nq: {q}")
    
phi_n = (p-1) * (q-1)
print(f"phi_n: {phi_n}")

b = 36639088738407540894550923202224101809992059348223191165   # starting value for a
gcd, a, _ = gcdExtended(b, phi_n)

if gcd == 1:
    while a <= 0:
        a += phi_n 
    print(f"a = {a}")
    print(f"b = {b}")

# Using readlines()
file = open('RSA-ciphertext.txt', 'r')
lines = file.readlines()

decrypted_list = []
for line in lines:
    decrypted_list.append(pow(int(line.replace('\n', '')), a, n))
    
line = []
lines = []
for decrypted in decrypted_list:
    if len(str(decrypted)) % 2 != 0:
        decrypted = "0" + str(decrypted)
    else:
        decrypted = str(decrypted)
    for i in range(0, len(decrypted), 2):
        line.append(decrypted[i:i+2])
    lines.append(line)
    line = []

f = open("RSA_decrypted_numbers", "w")
for line in lines:
    for one in line:  
        f.write(one)
    f.write("\n")
f.close()
            
matrix = [
    " !\"#$%&'()",
    "*+,-./0123",
    "456789:;<=",
    ">?@ABCDEFG",
    "HIJKLMNOPQ",
    "RSTUVWXYZ[",
    "\\]^_`abcde",
    "fghijklmno",
    "pqrstuvwxy",
    "z{|}~\n\r "
]

final_plain_text = ""

for line in lines:
    for one in line:
        row = int(one) // 10
        col = (int(one) % 10)
        letter = matrix[row][col]
        final_plain_text += letter
    final_plain_text += "\n"
            
f = open("RSA_Plain_Text", "w")
f.write(final_plain_text)
f.close()
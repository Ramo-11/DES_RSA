x = 2
b = 3
n1, n2, n3 = 3, 5, 7

# Oscar section
N = n1 * n2 * n3

N1 = N // n1
N2 = N // n2
N3 = N // n3

a1 = pow(N1, -1, n1)
a2 = pow(N2, -1, n2)
a3 = pow(N3, -1, n3)

y1 = pow(x, b) % n1
y2 = pow(x, b) % n2
y3 = pow(x, b) % n3

plain_message = (y1 * N1 * a1) + (y2 * N2 * a2) + (y3 * N3 * a3)
plain_message = plain_message % N
plain_message = round(plain_message ** (1/3))
print(plain_message)
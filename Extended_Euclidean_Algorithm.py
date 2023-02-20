def extended_gcd(a, b):
    x_initial, y_initial = 0, 1
    x_last, y_last = 1, 0

    while b != 0:
        quotient = a // b
        a, b = b, a % b

        x_last, x_initial = x_initial, x_last - quotient * x_initial
        y_last, y_initial = y_initial, y_last - quotient * y_initial

    return x_last, y_last, a

result, y_last, a = extended_gcd(3125, 9987)

print(f"Multiplicative inverse of 3125 modulo 9987 is: {result}")
# # Write in input.txt the number 1 until 1000
value = 1200000

with open('input.txt', 'w') as f:
    f.write(str(value) + ' ')
    for i in range(1, value):
        f.write(str(i) + ' ')
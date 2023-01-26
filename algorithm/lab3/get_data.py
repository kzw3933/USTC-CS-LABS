import random

## 产生30个随机区间,所有区间取自区间[0,25]或[30,50]且各区间左端点互异，不要和(25,30)有重叠

a = [i for i in range(26)]
b = [30 + i for i in range(21)]
gen_sets = ['a', 'b']

data = []
data_l = []

while (len(data) != 30):
    gen_set = random.choice(gen_sets)
    if gen_set == 'a':
        l = random.choice(a)
        r = random.choice(a)
        if l == r:
            continue
        if l > r:
            l, r = r, l

        if l in data_l:
            continue
        else:
            data_l.append(l)
            data.append((l, r))
    else:
        l = random.choice(b)
        r = random.choice(b)
        if l == r:
            continue
        if l > r:
            l, r = r, l
        if l in data_l:
            continue
        else:
            data_l.append(l)
            data.append((l, r))

# with open("E:\\Savefiles\\Labs\\Algorithm\\lab3\\ex1\\input\\input.txt", "w") as f:
#     for (a,b) in data:
#         f.write(str(a))
#         f.write('\t')
#         f.write(str(b))
#         f.write('\n')
        


search_data = []
while (len(search_data) != 2):
    gen_set = random.choice(gen_sets)
    if gen_set == 'a':
        l = random.choice(a)
        r = random.choice(a)
        if l > r:
            l, r = r, l
        else:
            search_data.append((l, r))
    else:
        l = random.choice(b)
        r = random.choice(b)
        if l > r:
            l, r = r, l
        else:
            search_data.append((l, r))
            
c = [26, 27, 28, 29]
l = random.choice(c)
r = random.choice(c)

if l > r:
    l, r = r, l
search_data.append((l, r))

with open("E:\\Savefiles\\Labs\\Algorithm\\lab3\\search_data.txt", "w") as f:
    for (a,b) in search_data:
        f.write(str(a))
        f.write('\t')
        f.write(str(b))
        f.write('\n')


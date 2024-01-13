num_length = 2

def fill_incremental(arr, i1, i2, incr_val):
    global num_length
    count = 0
    end = i2 + incr_val
    while i2 < end:
        arr.append([i1, i2])
        if (i1 % num_length == 0):
            i2 += 1
        i1 += 1
        count += 1
    return i1, i2, count 

def add_with_swap(arr, i1, i2, c):
    global num_length
    new_c = int(c / num_length)

    for _ in range(new_c):
        arr.append([i1, i2])
        i1+=1
        i2+=1

    k = len(arr) - new_c
    writer = k
    k -= 1
    end = k - c

    while k > end:
        arr[k], arr[writer] = arr[writer], arr[k]
        writer+=1
        k -= num_length
    
    return i1, i2

def dump_in(arr, test_num):
    filename = "./gen/in/%s.dat" % test_num
    print("Create: %s" % filename)

    with open(filename, "w") as file:
        # количество записей (N), количество полей (K), номер поля для сортировки (X)
        file.write('{} {} {}\n'.format(len(arr), len(arr), 2))
        for r in arr:
            s = ""
            for i in r:
                s += str(i) + " "
            file.write(s + "\n")

def dump_out(arr, test_num):
    N = len(arr)
    res = ""
    for i in range(N):
        pi = i % N
        res += str(arr[i][pi]) + " "

    filename = "./gen/out/%s.dat" % test_num
    print("Create: %s" % filename)
    with open(filename, "w") as file:
        file.write(res)

def dump_sorted_in(arr, test_num):
    filename = "./gen/sorted/%s.dat" % test_num
    print("Create: %s" % filename)

    with open(filename, "w") as file:
        for r in arr:
            file.write('{} {}\n'.format(r[0], r[1]))

def sorted_key(a):
    return a[1] 

def fill_rest_records(arr):
    N = len(arr)
    c = 1
    for r in arr:
        while len(r) < N:
            r.append(c)
            c += 1
            c %= 10

def gen_out(test_num, data_count):
    res = []
    i = 1
    j = 1
    c = 0

    for count in data_count:
        i, j, c = fill_incremental(res, i, j, count)
        i, j = add_with_swap(res, i, j, c)

    fill_rest_records(res)
    dump_in(res, test_num)

    res.sort(key=sorted_key)
    dump_sorted_in(res, test_num)
    dump_out(res, test_num)

""" 
    contain onle even number
"""
data = [
    [4, 12, 4],
    [8, 4, 2],
    [16, 14],
    [8, 16, 32, 64, 32, 16, 8],
    [128, 256, 128],
    # [1024, 2048, 4096], for stress 
]

temp = []
for i in range(1000):
    # 'Yes' if fruit == 'Apple' else 'No'
    temp.append(4 if i % 2 == 0 else 8)

data.append(temp)

for i in range(len(data)):
    gen_out(i + 3, data[i])



ip = {}
with open("prueba.csv", "r") as file:
    for x in file:
        a = x.split(",")
        if a[0] not in ip:
            ip[a[0]] = a[1].strip()
        else:
            if ip[a[0]].strip() == "Broadcast" and a[1].strip() != "Broadcast":
                ip[a[0]] = a[1].strip()
        if a[2] not in ip:
            ip[a[2]] = a[3].strip()
        else:
            if ip[a[2]].strip() == "Broadcast" and a[3].strip() != "Broadcast":
                ip[a[2]] = a[3].strip()

for x in ip:
    print(x + ", " + ip[x] + ", xxx")
import ihs_bindings

a = ihs_bindings.Speed(3, 5)
b = ihs_bindings.Speed(5, 5)
print(a == b)
print(a.left)
print(a.right)
print(a)

times = 0
t = ihs_bindings.Timer(3.35)
while not t():
    times += 1
print("finished with", times, "times")

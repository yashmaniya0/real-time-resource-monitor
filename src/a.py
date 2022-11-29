import psutil
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation as FA


total = psutil.virtual_memory().total/(10**6)
print(f"Total Virtual Memory : {total} MB")
used = []
available = []
used_percent = []
time_axis = []
interval = 0.5  # plot update interval in seconds


def update(i):
    vm = psutil.virtual_memory()
    used.append(vm.used/(10**6))
    available.append(vm.available/(10**6))
    used_percent.append(vm.percent)
    time_axis.append(i*interval)


    if len(time_axis)>100:
        time_axis.pop(0)
        used.pop(0)
        available.pop(0)
        used_percent.pop(0)

    plt.cla()

    plt.plot(time_axis, used, label="Used Virtual Memory", alpha=0.8)
    plt.plot(time_axis, used, label=f"CPU Utilization {vm.percent}%", alpha=0)
    plt.fill_between(time_axis, used, alpha=0.2)
    plt.title("Virtual Memory Visualization", fontsize=13)
    plt.ylim(min(used)*0.99, max(used)*1.01)
    plt.xlabel("Time (sec)")
    plt.ylabel("Size (MB)")
    plt.legend()


anime = FA(plt.gcf(), update, interval=1000*interval)

plt.show()

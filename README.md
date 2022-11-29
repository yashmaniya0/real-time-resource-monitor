# **Real-Time Resources Monitor for Linux**

![img](./assets/resource%20monitor.png)

![img](./assets/memory_usage_graph.gif)

![img](./assets/warning.gif)

## **psutil**
[psutil](https://psutil.readthedocs.io/en/latest/#) (python system and process utilities) is a cross-platform 
library for retrieving information on running processes and system utilization (CPU, memory, disks, network, 
sensors) in Python.  It is useful mainly for system monitoring, profiling, limiting process resources and the 
management of running processes. We have implemented real time resource monitor graph using this library.

To install psutil on your PC:
```
pip install psutil
```

## **ncurses**
[ncurses](https://www.gnu.org/software/ncurses/) is a library that facilitates text-based graphical output in 
the terminal. This project relies on ncurses for display output of CPU utilization as well as main memory 
utilization data.

To install ncurses on your Linux environment: 
```
sudo apt install libncurses5-dev libncursesw5-dev
```

## **cmake**
This project uses [Make](https://www.gnu.org/software/make/). The Makefile has four targets:
* `build` compiles the source code and generates an executable
* `format` applies [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) to style the source code
* `debug` compiles the source code and generates an executable, including debugging symbols
* `clean` deletes the `build/` directory, including all of the build artifacts

```
sudo apt-get -y install cmake
sudo apt-get -y install cmake-qt-gui
```

## **How to use**
Navigate to home directory path in terminal and write following commands:

For displaying CPU utilization as well as main memory 
utilization stats :
```
rmdir ./build
make build
./build/monitor
```

For displaying real-time resource monitor (will only work for Windows):
```
python3 ./src/resource_usage_graph.py
```

> You can also run `./script.sh` directly in your Linux Terminal

> If the above steps do not work, make sure you have added **execution perms** for the `./build/monitor` and `./script.sh`

The code was tested on the following specifications

- **PC :** `ACER ASPIRE 7`
- **CPU :** `Intel Core™ i5-9300H`
- **GPU :** `Nvidia GeForce GTX 1650 / 60 Hz`
- **RAM :** `8 GB`
- **OS :** `Ubuntu 16.02.4 LTS`
- **Kernal :** `4.15.0-72-generic`
# Delay-Analysis-and-Area-Optimalization-of-Netlist-
These problems were solved by converting the graph Netlist to graph data structure. For Area optimization heuristic algorithm was employed. The Area optimization works quickly up to 500 gates. The circuits may include DFF(D-Flip-Flop).
All inputs are given using text files
The commands to run it on linux system is
# For Part A
g++ main.cpp -o main.o
./main.o A <path to circuit file> <path to gate delays file> <path to longest delay file>
# For Part B
g++ main.cpp -o main.o
./main.o B <path to circuit file> <path to gate delays file> <path to delay constraint file> <path to minimum area file>

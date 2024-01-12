## Uniprocessor Scheduler Simulation

The assignment aims to simulate a Round Robin Scheduling Algorithm with various quanta values and study the effect of varying numbers of threads on multiple workloads and the impact of quanta values on waiting time, turnaround time, and time wasted due to context switches.

### Working : 

The scheduler process S is executed first; it spawns and execs two children, P1 and P2. P1 spawns a certain number of threads to read matrices from two input files and uses IPC mechanisms such as shared memory to send those matrices to P2. P2 spawns threads for matrix multiplication computation and writes the result into an output file. S uses signals to simulate a uniprocessor scheduler. That is, it suspends P1 and lets P2 execute, and vice versa. So P1 would read some part of the matrix and send them to P2 for computation, this alternates each quanta. </br> </br>
![image](https://user-images.githubusercontent.com/98446038/209423843-48cfde48-88ae-417b-9852-558a1e6eda71.png)
</br>
</br>
#### How to run : 
1. Grant executable permission to the runner bash script using ```chmod +x runner.sh```
2. Run the bash script using with the appropriate arguments ```./runner.sh N M K matrix1.txt matrix2.txt output.txt``` where N x M is the size of first matrix and M x K is the size of second matrix.
### Analysis after Benchmarking : 
1. P1 (file input and IPC) - On varying the number of threads, we found that intially, as the number of threads increased, the execution time decreased. However, after reaching a particular no.of threads that ensure minimum execution time, increasing the number of threads further did not decrease the execution time, but increased it instead. We concluded that this is because after reaching an optimal point, the overhead of creating new threads and context switches outweighs the speedup caused by them, thus steadily increasing the time. </br>
2. P2 (matrix multiplication) - For smaller cases, the effect of system processors and context switches is amplified due to very small execution time, whereas for larger cases the same observations were reported as P1; that is, parallelization decreases execution time till a certain minimum, following which it again starts to increase. This is because of a limited number of CPU cores, which limits the parallelism that can be achieved.
3. Scheduler - We observe that time wasted due to context switches (latency due to signals) reduces considerably when the quanta is longer, that is context switch with quanta as 2ms is lower than the one with 1ms. The total waiting time and the turnaround time increases with increasing quanta (for higher load, the results might be skewed to due to system processes and limitations of our CPU).


## Contributors : 
1. Milind Jain
2. Ashwin Naveen Pugalia
3. Sriram Balasubramanian
4. Anish Kallepalli
5. Anubhav Sharma
6. Arya Pathak

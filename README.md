# network-daemon
Overview

In this last step of the assignment you will build on the previous step. The networking part of the server is done, now we are going to add some semantics, which will have a small impact on the client, but not on the server.

Task

I want you to implement the "3A+1" algorithm in the server. The algorithm works as follows:
In the beginning an arbitrary integer number greater than 0 is provided. If the number is even, it needs to be devided by 2. If the number is odd, it is going to be multiplied by 3, after which 1 is added. This algorithm keeps going until the number is finally 1, at which point it stops. Although nobody knows whether this algorithm always terminates, so far no number has been found where it doesn't. The number of steps it takes until the algorithm terminates does not follow any pattern and is not predictable. Your server is supposed to implement the algorithm, i.e. for any number supplied by the client, it returns the number of steps it takes until termination.

For example, assuming that the initial number was 7, the sequence of numbers that the algorithm touches is: 22, 11, 34, 17, 52, 26, 13, 40, 20, 10, 5, 16, 8, 4, 2, 1 - i.e. the number of steps is 16.

Now, to make the algorithm a bit more efficient, I ask you to implement memoization, i.e. you will keep track of the numbers that you already computed (up to a certain limit like 1000). If a new number needs to get computed, you will look it up first and if it got computed before you can use that result right away. E.g. assuming the client passed in 14, the next step would be 7, but we just computed 7 -> 16, which means we return 1+16.

The challenge is that the different threads accessing the data structure, which keeps track of the number->steps mapping, need to synchronize their accesses, so that the data structure does not get corrupted. It is here where the mutex mechanism of pthreads comes into play.

You need to adapt the client so that it would take an integer from the user, send it to the server, gets the response from the server and displays it. Then it would start over again.

1) A basic mutex

2) Yes it works will end up as it started, as 0

3) Bx is the thing controlling our loops so increasing it makes it
   so our loops are longer the answer will remain the same tho.

4) Any i value that is cutting into the critical section can cause problems.
   Assigning flag or reading flag

5) Does the same job as flag but now as atomically so we can no longer
   cause problems to  code's execution via just interrupts (usage of xchg)

6) Answered in q5

7) Yes the right thing will happen as it is an atomic action we cannot interupt the 
   assignign of flag so when we fetch it it will come bakc as locked

8) Peterson aims to have a mutex lik behavior on CPUs that dont support it on a hardware
   level.

9) Changing i wont affect too much as this is replicating mutex behavior wherever we cut
   inside of the critical info it will be protected

10) 

11) A FIFO scheduler  that takes turn kinda like a RR

12) The more threads we add the longer it takes for another thread to take action 

13) yield.s is just a simple atomic lock that gives up the CPU when it is locked
    almost any lock usage would benefit from giving up the CPU to a long process on the
    other end

14) Test and set will keep checking if the lock is empty and only when it is sure that 
    it is empty it will do a cpu intensivec action of xchg, where as test-set will just
    do it without being sure 

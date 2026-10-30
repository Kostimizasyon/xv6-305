1) Yes, it gives info about where potenntial race conditiouns can occur as well 
   as where thread are created.


2) On the first case it will report no errors as without two threads trying 
   to acess the variable the variable no race condition is triggered 

   On the second case a race will still be triggered as one of the acessing 
   ones will be bound by a lock but the other acess wont care about the lock 
   and try to acess anyway

   Now it is safe and nothing will be reported

3) T1 will wait for m1 to get unlocked and T0 will wait for m2 to get unlocked
   causing a deadlock

4)  It complains about lock orded

5) This obviously has no error unlike the previous one since the second thread
   will wait for g to be unlocked the other thread will roam freely, tools like
   this are great but just like AI they need supervision

6) Instead of just letting the cpu do other things with waiting on a mutex we make
   the cpu / thread keep checking whether or not the vlaue changed

7) Helgrind reports a data race.

8) Here it is both better in terms of perforamnce AND correctness however it 
   is worse on memory usage instead of the cpu constantly checking whether or 
   not the value changed we just tell it straight up which reduces poor utilization
   to practically 0

9) nO ERRORS

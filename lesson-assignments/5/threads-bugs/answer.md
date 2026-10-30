1) The output direction changes based on which thread fires first which is kinda 
// if we release late we kill paralelism
   entirely luck dependant

2) No, not always. With few loops it often complete.

3) n=1 any higher will cause a deadlock

4) Since a thread that is spawned after the other thread then its vectors lives
   infront of the other thread resulting in 

5) Both increase the time

6) A LOT faster as the threads divide the work. IN HIGHER LOOPS, on lower it will be 
   about the same / worse

7) We are unable to not lock the first thread so no reason, to try lock we can
   just lock it straight up.

   it is slower as there are more acts of locking and a jump which is kind of
   a waste, increases via thread count

8) Main problem is the addition of another lock resulting in another lock operation costing
   us valuable performance and memory, also it will kill parallelism

9) Instead of locking the vector operation it locks the current add operation, instead
   of a whole vector_add being atomic, only the current act of adding somethng is atomic
   end  result is the same BUT only for this operation but readers will see partial updates.

10) Noticably slower with both, the main problem is probably the fact that

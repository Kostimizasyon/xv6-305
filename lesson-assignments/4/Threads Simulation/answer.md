1) -1
2) -1 both
3) No it will not change the last values but it will change the CPU utilization
    it will make it worse if it keeps doing it for no reason / mid operation
4) 1
5) We use bx as the forloop so it will cycle till it runs out and we have it at 3
   so 3 cycles, val ends at 6
6) The critical section is the act of getting the value at the adress, incrementing it
    and putting it back (any act of using the atomic vlaue)
   2000 as if we were to intersect here then we would overwrite an increment
7) As we keep interupting in the critical section tihs wont work, i shold atleast be 3
    And our final value would be = 1 for i 1-2 and 2 for i-3>
8) Same as above?
9) 1
10) Thead 0 will hog the CPU till the interrupt is triggered, and thread 1 will 
    change 2000 to the val 1, chaging i wont do much for us, the program is not efficient

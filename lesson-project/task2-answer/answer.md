# Task 2 Answer

## Setup

Lets say that we have 3 jobs in our scheduler, with the tickets

J0 35
J1 25
J2 40

And now lets assume that our scheduler keeps selecting the number 10
so only `J0` runs.

## Scheduler Assesment

The scheduler in this situation is (considering all jobs are infinty long)
extremely unfair, and only  job 0 is allowed to run. This causes starvation
to all other jobs. As they are not getting any cpu time.

## Solution

We could implement some sort of 'staling' where after a Job is hogging the cpu
for X amount of cycles, it could start losing / giving away tickets to other jobs.
Leading it to eventually fall out of the schedulers favor and give its place to 
some other job. Where then as that job gets favored then another job will move to
the favor as the other job that got replaced has lower tickets than it as its been
favored before so they will iterate amongst each other making it somewhat fair.
To make sure a penalized job is never completely shut out, we also keep a ticket
floor: a job can never drop below 1 ticket, so it always keeps a range in the
number space and can win again later once the other jobs get penalized.

## Solution Example (using setups jobs)

J0 Is being favored and after lets say 15 cycles of the cpu it starts giving away 10 tickets per
cycle evenly distrubiting it to the other two jobs. so after 3 cycles:

J0 5
J1 40
J2 55

Now since the scheduler is still picking number 10: after 1 cycle
J1 wins so J0 stops being favored and J1 becomes the hog.

J0 5   
J1 40 
J2 55

After 15 cycles J1 starts giving away 10 tickets per cycle,  
After 3 cycles J1 has given away 30 tickets:

J0 20
J1 10
J2 70

The number 10 now falls in J0's range, so J0 runs again. It is the
only job that can win right now. J0 hogs until its own penalty kicks in:
after 3 cycles it gives away 30 tickets, 
J0 1    
J1 20  
J2 79  

The number 10 now falls in J1's range

At this point every job has run at least once with the same stuck number 10:
J2 will win once J1's penalty shifts the ranges so 10 lands in, which
happens as J1 and J0 keep shrinking toward the floor. Eventually J0 = 1,

J1 = 1, J2 = 98 

The number 10 falls in J2's range.

## Conclusion

With the stuck number 10, the winner went J0 -> J1 -> J2. Every job got CPU
time even though the scheduler never changed its number, so starvation is
prevented. The floor of 1 ticket keeps penalized jobs alive, and tickets are
conserved (total = 100), so a job that stops hogging can win again later.

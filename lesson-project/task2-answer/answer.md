# Task 2 Answer

## Setup

Lets say that we have 3 jobs in our scheduler, with the tickets

J0 35
J1 25
J2 40

And now lets assume that our scheduler keeps selecting the number 25
so only `J0` runs.

## Scheduler Assesment

The scheduler in this situation is (considering all jobs are infinty long)
extremely unfair, and only  job 0 is allowed to run. This causes starvation
to all other jobs. As they are not getting any cpu time.

## Solution



"""The prime factors of 13195 are 5, 7, 13 and 29.

What is the largest prime factor of the number 600851475143 ?"""
import math

def LargestPrimeFactor(n):
    factors =[]
    while n % 2 == 0:
        n = n /2
        factors.append(2)
    for i in range(3,int(math.sqrt(n))+1,2):  
        while n % i== 0:
            factors.append(i)
            n = n / i
             
    #print the greatest prime factor among all the ones calculated
    return max(factors)
n = 600851475143
print(LargestPrimeFactor(n))


 



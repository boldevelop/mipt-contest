Timsort implementation from CPython before switching
to Munro & Wild "powersort" merge strategy https://github.com/python/cpython/pull/28108

https://github.com/python/cpython/blob/v3.7.17/Objects/listobject.c#L2158

## Build & run
```
> make
> ./bin/ts < ./in/1.dat

# run measure
> make run-measure
```

## Perf
with -O2 optimization


arr : 4 equal parts of iota sorts part.  
arr/: 2 equal parts of iota sorts part.  
arr*: 4 equal parts of iota sorts part but end of every parts shuffled.  
arr!: all elems shuffled.  

```
                arr        arr/        arr*         arr!   
100
       ts:    0.000002    0.000002    0.000008    0.000011
       tg:    0.000002    0.000001    0.000007    0.000011    
       qs:    0.000003    0.000002    0.000003    0.000004    
1000
       ts:    0.000004    0.000011    0.000084    0.000174
       tg:    0.000002    0.000010    0.000068    0.000169    
       qs:    0.000022    0.000015    0.000028    0.000049    
10000
       ts:    0.000037    0.000105    0.000936    0.001846
       tg:    0.000018    0.000097    0.000752    0.001754    
       qs:    0.000169    0.000177    0.000349    0.000620    
100000
       ts:    0.000455    0.001066    0.012108    0.023884
       tg:    0.000242    0.001278    0.010705    0.023327    
       qs:    0.002161    0.002067    0.004974    0.007761    
1000000
       ts:    0.004440    0.010675    0.146274    0.295124
       tg:    0.002849    0.010786    0.129676    0.292585    
       qs:    0.024609    0.025015    0.052887    0.094190  
```

Merge tests with and without galoping, 2 iota arrais,  
diffrent tests with swapping tail or head of arrays (mostly for check correctness merge)  
w - with galop  
wo - without galop (GALLOP_MODE 200000000)  
size 1000000 

```
       wo          w
1.  0.004491s  0.000771s 
2.  0.004186s  0.000375s 
3.  0.004487s  0.000473s 
4.  0.004494s  0.000664s 
5.  0.008231s  0.000465s 
6.  0.008574s  0.000595s 
7.  0.008367s  0.000494s 
8.  0.008363s  0.000551s 
9.  0.008381s  0.000393s 
10. 0.008352s  0.000462s 
11. 0.008345s  0.000380s 
12. 0.008310s  0.000448s 
13. 0.008359s  0.000430s 
14. 0.008434s  0.000574s 
15. 0.008449s  0.000472s 
16. 0.008968s  0.000549s 
17. 0.008300s  0.000423s 
18. 0.008537s  0.000777s 
19. 0.008283s  0.000411s 
20. 0.008567s  0.000476s 
21. 0.004177s  0.000404s 
22. 0.004248s  0.000318s 
23. 0.004243s  0.000312s 
24. 0.004278s  0.000483s 
25. 0.004140s  0.000499s 
26. 0.004448s  0.000466s 
27. 0.004235s  0.000313s 
28. 0.004306s  0.000380s 
29. 0.004504s  0.000331s 
30. 0.004384s  0.000427s 
31. 0.004209s  0.000418s 
32. 0.004308s  0.000441s 
33. 0.004204s  0.000283s 
34. 0.004241s  0.000351s 
35. 0.004245s  0.000441s 
36. 0.004396s  0.000364s 
37. 0.004181s  0.000273s 
38. 0.004303s  0.000354s 
39. 0.004471s  0.000350s 
40. 0.004148s  0.000480s 
41. 0.006269s  0.000405s 
42. 0.006598s  0.000514s 
```
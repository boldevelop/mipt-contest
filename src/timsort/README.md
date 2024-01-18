Timsort implementation from CPython before switching
to Munro & Wild "powersort" merge strategy https://github.com/python/cpython/pull/28108

https://github.com/python/cpython/blob/v3.7.17/Objects/listobject.c#L2158

## Build & run
```
> make
> ./bin/ts < ./in/1.dat
```

## Perf
with -O2 optimization

arr! - all shuffled  
arr - 4 equal parts if source array sort  
arr* - 4 equal parts if source array sort but end of every parts shuffled  
ts - timsort without galoping  
tg - timsort with galoping  
qs - qsort  

|      s |    arr      |       arr* |           arr!|
|:---:|---|---|---|
|100|ts: 0.000001s<br/>tg: 0.000001s<br>qs: 0.000002s|ts: 0.000003s<br>tg:0.000002s<br/>qs: 0.000002s|ts: 0.000005s<br>tg: 0.000005s<br>qs: 0.000005s|
|10000|ts: 0.000025s<br>tg: 0.000018s<br>qs: 0.000169s|ts: 0.000398s<br>tg: 0.000338s<br>qs: 0.000343s|ts: 0.000809s<br>tg: 0.000822s<br>qs: 0.000642s|
|100000|ts: 0.000319s<br>tg: 0.000300s<br>qs: 0.002078s|ts: 0.005224s<br>tg: 0.004515s<br>qs: 0.005638s|ts: 0.011752s<br>tg: 0.010629s<br>qs: 0.008327s|
|1000000|ts: 0.003594s<br>tg: 0.002952s<br>qs: 0.026343s|ts: 0.066524s<br>tg: 0.060415s<br>qs: 0.055587s|ts: 0.132696s<br>tg: 0.133065s<br>qs: 0.095594s|

Merge tests with and without galoping, 2 iota arrais,  
diffrent tests with swapping tail or head of arrays (mostly for check correctness merge)  
w - with galop  
wo - without galop (GALLOP_MODE 200000000)  
size 1000000  

|-|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|20|21|22|23|24|25|26|27|28|29|30|31|32|33|34|35|36|37|38|39|40|41|42|
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
|wo|0.003059s|0.002728s|0.004080s|0.004154s|0.004259s|0.004088s|0.004431s|0.004134s|0.004109s|0.004380s|0.004164s|0.004084s|0.004265s|0.004063s|0.004160s|0.004145s|0.004226s|0.004227s|0.004163s|0.004079s|0.002430s|0.002372s|0.002321s|0.002431s|0.002441s|0.002282s|0.002276s|0.002466s|0.002535s|0.002276s|0.002250s|0.002355s|0.002560s|0.002342s|0.002291s|0.002381s|0.002374s|0.002361s|0.002310s|0.002322s|0.003370s|0.003392s|
|w|0.001775s|0.001769s|0.001216s|0.001262s|0.001192s|0.001393s|0.001321s|0.001285s|0.001182s|0.001477s|0.001460s|0.001645s|0.001280s|0.001487s|0.001253s|0.001318s|0.001519s|0.001124s|0.001348s|0.001180s|0.000804s|0.000949s|0.000888s|0.000860s|0.000876s|0.000823s|0.000974s|0.000808s|0.001070s|0.000852s|0.000899s|0.000866s|0.000811s|0.000856s|0.000911s|0.000902s|0.001264s|0.000894s|0.000825s|0.000852s|0.001200s|0.001163s|

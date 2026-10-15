# Smart_Pointers_Homework_MEPhI  
`c.sh` - скрипт компиляции  
`benchmark.sh` - создание 10^6 каждого вида объектов(самописные указатели, сырые указатели, stl)  
пример работы бенчмарка показан в файле `benchmark_results.csv`  
| Implementation | User_Time_s | Sys_Time_s | Wall_Clock | Max_Memory_KB |
| --- | --- | --- | --- | --- |
| STL Smart Pointers | 1.65 | 0.66 | 0:02.32 | 549824 |
| Custom Smart Pointers | 1.54 | 0.62 | 0:02.17 | 549820 |
| Raw Pointers | 1.48 | 0.62 | 0:02.11 | 549684 |

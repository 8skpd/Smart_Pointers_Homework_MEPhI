#!/bin/bash

# Скрипт для сравнения 3 реализаций умных указателей

# Цвета для вывода
RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE} Компиляция программ ${NC}"
g++ -O2 -std=c++20 bench_raw.cpp -o bench_raw
g++ -O2 -std=c++20 bench_custom.cpp -o bench_custom
g++ -O2 -std=c++20 bench_stl.cpp -o bench_stl

echo -e "${GREEN}Компиляция завершена${NC}\n"

# Функция для запуска бенчмарка и сбора результатов

run_benchmark() {
    local program=$1
    local name=$2
    
    echo -e "${BLUE} Запуск $name ${NC}"
    
    # Запускаем через /usr/bin/time -v и сохраняем вывод
    output=$(/usr/bin/time -v ./$program 2>&1)
    
    # Парсим результаты
    elapsed=$(echo "$output" | grep "Elapsed (wall clock)" | awk '{print $NF}')
    user_time=$(echo "$output" | grep "User time (seconds)" | awk '{print $NF}')
    sys_time=$(echo "$output" | grep "System time (seconds)" | awk '{print $NF}')
    max_memory=$(echo "$output" | grep "Maximum resident set size" | awk '{print $NF}')
    
    # Выводим результаты
    echo -e "${GREEN}Результаты для $name:${NC}"
    echo "  Время (wall clock): $elapsed"
    echo "  User time: ${user_time}s"
    echo "  System time: ${sys_time}s"
    echo "  Макс. память: ${max_memory} KB"
    echo ""
    
    # Сохраняем в CSV
    echo "$name,$user_time,$sys_time,$elapsed,$max_memory" >> benchmark_results.csv
}

# Основная часть

# Создаем CSV файл с заголовком
echo "Implementation,User_Time_s,Sys_Time_s,Wall_Clock,Max_Memory_KB" > benchmark_results.csv

# Запускаем бенчмарки
run_benchmark "bench_stl" "STL Smart Pointers"
run_benchmark "bench_custom" "Custom Smart Pointers"
run_benchmark "bench_raw" "Raw Pointers"


# Вывод итоговой таблицы

echo -e "\n${BLUE} ИТОГОВАЯ ТАБЛИЦА ${NC}"
column -t -s ',' benchmark_results.csv

echo -e "\n${GREEN}Результаты сохранены в benchmark_results.csv${NC}"

Данная программа выявляет минимальные различия между двумя файлами (аналог утилиты diff в linux). 
По факту программа находит минимальную последовательность операций вставки, удаления или замены строк в первом файле на строки из второго файла, чтобы в итоге получился второй файл.
Задача решена при помощи алгоритма LCS нахождения наибольшей общей подпоследовательности двух строк за время O(nlogn).  

#include <string>
#include <fstream>  // работа с файлами.
#include <iostream>
#include <cstdlib> // для получения рандомных чисел.
#include <ctime>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cstdio>
#include <sstream>


using namespace std;

void create_file(vector<string>& fileNames)
{
    int fileIndex = 1;

    for (int i = 1; i <= 10; i++)
    {
        string filename = "filename_" + std::to_string(fileIndex) + ".txt"; // Создание имени файла.
        ofstream outFile(filename); // Открываем файл для создания.

        if (!outFile.is_open()) // Проверяем открытие файла.
        {
            std::cerr << "Error open file (Func create_file): " << filename << endl;
            return;
        }
        outFile.close();
        fileNames.push_back(filename); // Записываем в вектор имя созданного файла.
        fileIndex++;
        
    }
}

void write_file(vector<string>& fileNames, vector<int>& rand_arr, const int& size_file)
{
    fstream fs; // Объект для работы с файлами.
    string filename; // Переменная для хранения имени файла.
    auto start = chrono::high_resolution_clock::now();
    // Цикл по всем именам файлов.
    for (int fileIndex = 0; fileIndex < fileNames.size(); fileIndex++)
    {
        filename = fileNames[fileIndex]; // Получаем имя текущего файла.

        fs.open(filename, fstream::out); // Открываем файл для записи.

        // Проверяем, успешно ли открылся файл.
        if (!fs.is_open())
        {
            std::cerr << "Error open file (Func read_file): " << filename << endl; // Сообщаем об ошибке.
            return; // Выходим из функции, если файл не открылся
        }

        // Определяем начальный и конечный индексы для записи в файл.
        int startIndex = fileIndex * size_file; // Начальный индекс.
        int endIndex = std::min(startIndex + size_file, static_cast<int>(rand_arr.size())); // Конечный индекс, не превышающий размер массива.

        // Цикл для записи значений из rand_arr в файл.
        for (int i = startIndex; i < endIndex; i++)
        {
            fs << rand_arr[i] << " "; // Записываем текущее значение в файл, с новой строки.
        }

        fs.close(); // Закрываем файл после записи
        auto finish = chrono::high_resolution_clock::now(); // Останавливаем таймер

        chrono::duration<double> elapsed = finish - start; // Делаем расчет

        cout << "Elapsed time: " << elapsed.count() << " sec" << endl; // Выводим результат таймера
        std::cout << "File " << filename << " read" << endl; // Сообщаем, что файл был успешно записан.
        // Проверяем, достигли ли мы конца массива
        if (endIndex >= rand_arr.size())
        {
            break; // Выходим из цикла, если все данные записаны.
        }
    }
}

void countSort(std::vector<int>& arr)
{
    // Определяем максимальное значение в векторе для создания массива count
    int max = *std::max_element(arr.begin(), arr.end());

    // Создаем вектор count размером max + 1, заполненный нулями
    std::vector<int> count(max + 1, 0);
    std::vector<int> output(arr.size()); // Создаем выходной вектор для отсортированного результата

    // Считаем количество вхождений каждого элемента в исходном массиве
    for (int i = 0; i < arr.size(); i++) {
        count[arr[i]]++;
    }

    // Преобразуем count, чтобы он содержал накопленные суммы позиций
    for (int i = 1; i <= max; i++) {
        count[i] += count[i - 1];
    }

    // Построение выходного массива, используя накопленные позиции в count
    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[arr[i]] - 1] = arr[i];
        count[arr[i]]--;  // Уменьшаем индекс для следующего вхождения
    }

    // Копируем отсортированные элементы обратно в исходный массив
    for (int i = 0; i < arr.size(); i++) {
        arr[i] = output[i];
    }
}

void sort_file(vector<string>& fileNames, const int& file_size)
{
    fstream fs; // Для записи в файл после сортировки
    string filename; // Переменная для хранения имени файла
    vector<int> arr; // Вектор для хранения значений файла

    auto start_read = chrono::high_resolution_clock::now(); // Начало отсчета времени чтения файлов

    // Цикл по всем именам файлов
    for (int fileIndex = 0; fileIndex < fileNames.size(); fileIndex++)
    {
        filename = fileNames[fileIndex]; // Получаем имя текущего файла
        ifstream read(filename); // Для чтения данных из файла

        // Проверяем, успешно ли открылся файл
        if (!read.is_open())
        {
            std::cerr << "Error open file (Func sort_file): " << filename << endl; // Сообщаем об ошибке
            return; // Выходим из функции, если файл не открылся
        }

        string line;
        while (getline(read, line)) // Перебираем значение и записываем в вектор
        {
            std::istringstream iss(line);
            int number;

            while (iss >> number)
            {
                arr.push_back(number);
            }
            
        }
        cout << "Read " << arr.size() << " numbers from file: " << filename << endl;
        read.close(); // Закрываем файл

        // Попытка удалить файл после его обработки
        if (remove(filename.c_str()) != 0)
        {
            cerr << "Error: Don't delete file: " << filename << endl; // Ошибка при удалении файла
        }
        else
        {
            cout << "File " << filename << " was successfully deleted" << endl; // Файл успешно удален
        }
    }

    auto finish_read = chrono::high_resolution_clock::now(); // Конец отсчета времени чтения файлов
    chrono::duration<double> elapsed_read = finish_read - start_read; // Расчет времени чтения
    cout << "Elapsed time for reading files: " << elapsed_read.count() << " sec" << endl; // Выводим время чтения

    auto start_sort = chrono::high_resolution_clock::now(); // Начало отсчета времени сортировки
    if (arr.empty()) {
        std::cerr << "Error: The array is empty before sorting." << std::endl;
        return; // или обработка ошибки
    }

    countSort(arr); // Сортируем только если массив не пуст

    auto finish_sort = chrono::high_resolution_clock::now(); // Конец отсчета времени сортировки
    chrono::duration<double> elapsed_sort = finish_sort - start_sort; // Расчет времени сортировки
    cout << "Elapsed time for sorting: " << elapsed_sort.count() << " sec" << endl; // Выводим время сортировки

    filename = "finalfile.txt";

    fs.open(filename, fstream::out | fstream::trunc); // Открываем финальный файл для записи отсортированного массива

    // Проверка, открыт ли файл
    if (!fs.is_open())
    {
        cerr << "Error open 'finalfile'" << endl; // Сообщение об ошибке
        return; // Выход из функции, если файл не открыт
    }

    auto start_write = chrono::high_resolution_clock::now(); // Начало отсчета времени записи
    // Записываем отсортированный массив в finalfile
    for (size_t i = 0; i < arr.size(); i++)
    {
        fs << arr[i] << " "; // Записываем каждый элемент
    }
    auto finish_write = chrono::high_resolution_clock::now(); // Конец отсчета времени записи

    fs.close(); // Закрываем файл

    chrono::duration<double> elapsed_write = finish_write - start_write; // Расчет времени записи
    cout << "Elapsed time for writing to file: " << elapsed_write.count() << " sec" << endl; // Выводим время записи
}


int main()
{
    string finalfile = "finalfile.txt";
    const int size = 800000000; // Массив размером 800 миллионов (может быть слишком большим для обычной системы)
    const int size_file = 80000000; // Размер элементов записываемых в файл
    vector<int> rand_arr(size); // Выделение памяти под массив размером 800 млн.
    vector<string> fileNames; // Хранит имена созданных файлов

    srand(time(nullptr)); // Инициализация генерации случайных чисел на основе времени

    int l = 1;
    int r = 100;

    // Генерация случайных чисел
    for (int i = 0; i < size; i++)
    {
        rand_arr[i] = l + rand() % r; // Генерация случайных чисел в диапазоне от l до r
    }

    // Создание, запись и сортировка файлов
    create_file(fileNames);
    write_file(fileNames, rand_arr, size_file);
    sort_file(fileNames, size_file);
}
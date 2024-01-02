#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

// Функция возвращает количество установленных битов в байте.
char get_number_of_set_bits(unsigned char byte) {
    int count = 0;
    // Если младший бит = 1, то count увеличивается на 1.
    // Затем происходит сдвиг вправо на 1. Таким образом проверенный бит "исчезает", а на его место встаёт следующий.
    // Слева же появляются 0, и, когда все 1 будут найдены, цикл прекратит работу, так как значение byte станет = 0.
    while (byte) {
        count += byte & 1;
        byte = byte >> 1;
    }
    return count;
}

// Функция возврацает байт с рандомной перестановкой битов относительно байта, указанного в качестве аргумента.
unsigned char get_rand_bit_swap(unsigned char byte) {
    // Устанавливаем в качестве начального числа функции rand() количество секунд, прошедших с 1 января 1970 года.
    // Таким образом обеспечивая изменчивость вычислений rand().
    srand(time(NULL));

    unsigned char result = 0;

    // Вычисляем рандомное число от 0 до 255.
    unsigned int item = rand();
    item = item % 256;

    // Определяем количество установленных битов в байте.
    char number_of_set_bits = get_number_of_set_bits(byte);

    // В цикле item будет увеличиваться, пока в результате операции (item + byte) % 256
    // не будет найден рандомный байт, с таким же как и у исходного байта количеством установленных битов.
    // Рандомность найденного байта обеспечивается рандомностью исходного значения item
    while (result == 0) {
        if (get_number_of_set_bits((item + byte) % 256) == number_of_set_bits) {
            result = (item + byte) % 256;
        }
        item += 1;
    }
    return result;
}

// Функция выводит в консоль бинарное представление переменной unsigned типа long long, разделяя его байты пробелом.
void bin_print(unsigned long value) {
    int bit_index = 31;
    int count = 1;
    unsigned long long i = 1;
    while (bit_index >= 0) {
        if (((i << bit_index) & value) != 0) { // Выясняем значение бита c помощью побитовой конъюнкции.
            printf("1");
        }
        else {
            printf("0");
        }

        if (count % 8 == 0) { // Делим вывод на отдельные байты.
            printf(" ");
            if (count == 32) {
                printf("\n");
            }
        }

        count += 1;
        bit_index -= 1;
    }
}

int main(int argc, char *argv[]) {

    unsigned long value = 0;

    if (argc == 1) { // Если argc = 1, то количество введённых аргументов = 0.
        // Устанавливаем в качестве начального числа функции rand() количество секунд, прошедших с 1 января 1970 года.
        // Таким образом обеспечивая изменчивость вычислений rand().
        srand(time(NULL));
        value = rand(); // Присваиваем в value рандомное число
    }
    else if (argc > 2) { // Если argc > 2, то количество введённых аргументов > 1.
        printf("Ошибка: введён более чем 1 аргумент.");
        exit(1); // Ошибка => завершаем работу программы.
    }
    else if (argc == 2) { // Если argc = 2, то количество введённых аргументов = 1.
        
        for (int i = 0; i < (int)strlen(argv[1]); i++) {
            if (argv[1][0] == '-') { // Если первый символ = '-', то следующая проверка укажет на ошибку.
                continue;
            }
            if (argv[1][i] < '0' || argv[1][i] > '9') { // Проверим что все символы лежат в диапазоне от '0' до '9' в кодировке ASCII.
                printf("Ошибка: '%s' не является числом.", argv[1]);
                exit(1); // Ошибка => завершаем работу программы.
            }
        }

        if (argv[1][0] == '-') {
            printf("Ошибка: '%s' отрицательное число. Введите число типа unsigned long.", argv[1]);
            exit(1); // Ошибка => завершаем работу программы.
        }

        // Перенесем значение из argv[1] в value
        int power_of_ten = 0;
        int index = strlen(argv[1]);
        unsigned long long input_value = 0;
        // Пройдём по всем элементам полученной строки и переведём их в соответствующие им числовые значения.
        while (index) {
            input_value += (pow(10, power_of_ten)) * (argv[1][index - 1] - '0'); // Прибавляем к value значение каждого символа.
            power_of_ten += 1;
            index -= 1;
        }
        if (input_value > 4294967295) {
            printf("Ошибка: '%s' больше 4294967295. Введите число типа unsigned long.", argv[1]);
            exit(1); // Ошибка => завершаем работу программы.
        }

        value = (unsigned long)input_value;

    }

    unsigned char* p_first_byte = (unsigned char*)&value; // Получаем указатель на первый байт введённого числа.
    
    bin_print(value);

    // Проходимся по всем байтам value и рандомно переставляем в них биты.
    for (char i = 0; i < 4; i++) {
        unsigned char byte = (char) *(p_first_byte + i); // Получаем текущий байт.
        // Если байт = 00000000 или 11111111, то переставить биты как-то подругому не получится, поэтому просто пропускаем.
        if (byte == 0 || byte == 255) {
            continue;
        }
        *(p_first_byte + i) = get_rand_bit_swap(byte); // Записываем в соответсвующий байт полученный байт с рандомной перестановкой битов.
    }

    bin_print(value); // Выводим результат на экран.

}

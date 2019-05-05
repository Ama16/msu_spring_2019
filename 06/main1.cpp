#include <iostream>
#include <algorithm>1
#include <atomic>
#include <thread>
using namespace std;
using type = uint64_t;
const int amount = 1<<17;
const char *name_tmp = "tmp.bin";
const char *name_input = "input.bin";
const char *name_output = "output.bin";
const char *name_output1 = "output1.bin";
const char *name_output2 = "output2.bin";
atomic<bool> ok {true};

// считываю по 2^17 чисел (по 1 МБ), сортирую их, и закидываю в файл tmp.bin
// возвращает количество таких считанных блоков
int first_read()
{
    type *q = (type*)malloc(amount*sizeof(type));
    FILE *input;
    if ((input = fopen(name_input, "rb"))==NULL)
    {
        printf("Cannot open file.\n");
        ok = false;
        free(q);
        return 0;
    }
    FILE *output;
    if ((output = fopen(name_tmp, "wb"))==NULL)
    {
        printf("Cannot open file.\n");
        ok = false;
        fclose(input);
        free(q);
        return 0;
    }
    int now = 0, i = 0;
    do
    {
        now = fread(q, sizeof(type), amount, input);
        i++;
        std::sort(q, q+now);
        fwrite(q, sizeof(type), now, output);
    }
    while (now == amount);
    free(q);
    fclose(input);
    fclose(output);
    return i;
}

// сортировка слиянием, подаю номер чанка, размер, имя файла, где числа есть, и файл, куда записывать уже отсортированное
// сортирует 2 куска по size_of_chunk МБ
void mergesort(const int beg, const int size_of_chunk, const char *name_output, FILE *output)
{
    FILE *input1;
    if ((input1 = fopen(name_output, "rb"))==NULL)
    {
        printf("Cannot open file.\n");
        ok = false;
        return;
    }
    FILE *input2;
    if ((input2 = fopen(name_output, "rb"))==NULL)
    {
        printf("Cannot open file.\n");
        fclose(input1);
        ok = false;
        return;
    }
    type *q1 = (type*)malloc(amount*sizeof(type));
    type *q2 = (type*)malloc(amount*sizeof(type));
    type *q3 = (type*)malloc(2*amount*sizeof(type));
    for (int i = 1; i < beg; i++)
    {
        fread(q1, sizeof(type), amount, input1);
        fread(q2, sizeof(type), amount, input2);
    }
    for (int i = 0; i < size_of_chunk; i++)
    {
        fread(q2, sizeof(type), amount, input2);
    }



    fread(q1, sizeof(type), amount, input1);
    int how_read = fread(q2, sizeof(type), amount, input2);
    type *r1 = q1, *r2 = q2; //r1 - указатель на текущий сравниваемый элемент в массиве q1
    int i1 = 1, i2 = 1; // количество считанных чанков
    bool who_was_end = 0;
    do
    {
        if (*r1 - *q1 == amount)
        {
            i1++;
            fread(q1, sizeof(type), amount, input1);
            r1 = q1;
        }
        if (*r2 - *q2 == how_read)
        {
            i2++;
            how_read = fread(q2, sizeof(type), amount, input2);
            r2 = q2;
        }
        int how_comp = 0, now1 = 0, now2 = 0;
        while (now1 < amount && now2 < how_read)
        {
            if (*r1 < *r2)
            {
                q3[how_comp] = *r1;
                r1++;
                now1++;
            }
            else
            {
                q3[how_comp] = *r2;
                r2++;
                now2++;
            }
        }
        if (now1 == amount)
            who_was_end = 0;
        else
            who_was_end = 1;

        fwrite(q3, sizeof(type), how_comp, output);
    }
    while (!(how_read != amount && who_was_end) && !(i1 == size_of_chunk && !who_was_end) && !(i2 == size_of_chunk && who_was_end));

    if (who_was_end)
    {
        fwrite(r1, sizeof(type), amount - (*r1 - *q1), output);
        while (i1 != size_of_chunk)
        {
            fread(q1, sizeof(type), amount, input1);
            i1++;
            fwrite(q1, sizeof(type), amount, output);
        }
    }
    else
    {
        fwrite(r2, sizeof(type), how_read - (*r2 - *q2), output);
        while (i2 != size_of_chunk && how_read == amount)
        {
            how_read = fread(q2, sizeof(type), amount, input2);
            i2++;
            fwrite(q2, sizeof(type), how_read, output);
        }
    }



    free(q1);
    free(q2);
    free(q3);
    fclose(input1);
    fclose(input2);
}

void merge_file(bool a)
{
    FILE *output1;
    if ((output1 = fopen(name_output1, "rb"))==NULL)
    {
        printf("Cannot open file.\n");
        ok = false;
        return;
    }
    FILE *output2;
    if ((output2 = fopen(name_output2, "rb"))==NULL)
    {
        printf("Cannot open file.\n");
        fclose(output1);
        ok = false;
        return;
    }
    FILE *output;
    if ((output = fopen(name_output, "wb"))==NULL)
    {
        printf("Cannot open file.\n");
        fclose(output1);
        fclose(output2);
        ok = false;
        return;
    }
    type *q = (type*)malloc(amount*sizeof(type));
    int how_read;
    if (a)
    {
        while (how_read = fread(q, sizeof(type), amount, output1))
        {
            fwrite(q, sizeof(type), how_read, output);
        }
        while (how_read = fread(q, sizeof(type), amount, output2))
        {
            fwrite(q, sizeof(type), how_read, output);
        }
    }
    else
    {
        while (how_read = fread(q, sizeof(type), amount, output2))
        {
            fwrite(q, sizeof(type), how_read, output);
        }
        while (how_read = fread(q, sizeof(type), amount, output1))
        {
            fwrite(q, sizeof(type), how_read, output);
        }
    }
    free(q);
    fclose(output);
    fclose(output2);
    fclose(output1);
}

int main()
{
    thread thread1, thread2;
    int n = first_read();
    if (!ok)
    {
        printf("Cannot open file.\n");
        return 0;
    }
    int pow = 1, i = 0;
    while (pow < n)
    {
        pow <<= 1;
        i++;
    }
    //слияния по 2 соседних блока чисел
    for (int j = 0; j < i; j++)
    {
        FILE *output1;
        FILE *output2;
        if ((output1 = fopen(name_output1, "wb"))==NULL)
        {
            printf("Cannot open file.\n");
            return 0;
        }
        if ((output2 = fopen(name_output2, "wb"))==NULL)
        {
            printf("Cannot open file.\n");
            fclose(output1);
            return 0;
        }

        for (int k = 1; k <= n/2 && ok; k++)
        {
            if (k&1)
            {
                thread1 = thread(mergesort, (k-1)*2*(1<<j), 1<<j, name_tmp, output1);
                thread1.join();
            }
            else
            {
                thread2 = thread(mergesort, (k-1)*2*(1<<j), 1<<j, name_tmp, output2);
                thread2.join();
            }
        }
        fclose(output1);
        fclose(output2);
        if (!ok)
        {
            printf("Cannot open file.\n");
            return 0;
        }
        if ((n/2)&1)
            merge_file(false);  //последним записывали в output1
        else
            merge_file(true);
        rename(name_output, name_tmp);
    }


    rename(name_tmp, name_output);
    //результат в output.bin
    return 0;
}





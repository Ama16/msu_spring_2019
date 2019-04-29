#include <iostream>
#include <atomic>
#include <thread>
using namespace std;
#define type int
const int amount = 1<<17;


// считываю по 2^17 чисел (по 1 МБ), сортирую их, и закидываю в файл tmp.bin
// возвращает количество таких считанных блоков
int first_read()
{
    type *q = (type*)malloc(amount*sizeof(type));
    FILE *input = fopen("input.bin", "rb");
    FILE *output = fopen("tmp.bin", "wb");
    int now = 0, i = 0;
    do
    {
        now = fread(q, sizeof(type), amount, input);
        i++;
        qsort(q, now, sizeof(type), [](const void* a, const void* b)
        {
            int arg1 = *static_cast<const type*>(a);
            int arg2 = *static_cast<const type*>(b);

            if(arg1 < arg2) return -1;
            if(arg1 > arg2) return 1;
            return 0;

        });
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
    FILE *input1 = fopen(name_output, "rb");
    FILE *input2 = fopen(name_output, "rb");
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
    FILE *output1 = fopen("output1.bin", "rb");
    FILE *output2 = fopen("output2.bin", "rb");
    FILE *output = fopen("output.bin", "wb");
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
    int n = first_read();
    int pow = 1, i = 0;
    while (pow < n)
    {
        pow <<= 1;
        i++;
    }
    //слияния по 2 соседних блока чисел
    for (int j = 0; j < i; j++)
    {
        FILE *output1 = fopen("output1.bin", "wb");
        FILE *output2 = fopen("output2.bin", "wb");
        for (int k = 1; k <= n/2; k++)
        {
            if (k&1)
            {
                thread thread1(mergesort, (k-1)*2*(1<<j), 1<<j, "tmp.bin", output1);
                thread1.join();
            }
            else
            {
                thread thread2(mergesort, (k-1)*2*(1<<j), 1<<j, "tmp.bin", output2);
                thread2.join();
            }
        }
        fclose(output1);
        fclose(output2);
        if ((n/2)&1)
            merge_file(false);  //последним записывали в output1
        else
            merge_file(true);
        rename("output.bin", "tmp.bin");
    }

    rename("tmp.bin", "output.bin");
    //результат в output.bin
    return 0;
}

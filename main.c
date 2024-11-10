#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
//=================================Structs=================================
typedef struct
{
    char ChunkID[4];
    unsigned int ChunkSize;
    char Format[4];
} riff;
typedef struct
{
    char SubChunk1ID[4];
    uint32_t SubChunk1Size;
    uint16_t Audio_format;
    uint16_t N_of_channels;
    uint32_t Sample_rate;
    uint32_t Byte_rate;
    uint16_t Block_align;
    uint16_t Bits_per_sample;
} fmt;
typedef struct
{
    char SubChunk2ID[4];
    uint32_t SubChunk2Size;
    unsigned int Audio_data;
} data;

typedef struct
{
    // int etiq;
    riff RIFF;
    fmt FMT;
    data Dados;
} header;

//=================================Funções========================================
// funções de escrita e leitura para facilitar a tarefa
void writeInFile(header *h, FILE *f)
{
    fread(&h->RIFF, sizeof(riff), 1, f);
    fread(&h->FMT, sizeof(fmt), 1, f);
    fread(&h->Dados, sizeof(data), 1, f);
}
void readHeader(header *h, FILE *f)
{
    fwrite(&h->RIFF, sizeof(riff), 1, f);
    fwrite(&h->FMT, sizeof(fmt), 1, f);
    fwrite(&h->Dados, sizeof(data), 1, f);
}

// Tarefa 1: escrever um programa C que imprima os valores dos dados associados ao
// cabeçalho de um determinado arquivo WAV.
void printHeader(FILE *file)
{
    header WAV;

    if (fread(&WAV, sizeof(header), 1, file) < 1)
    {
        printf("Erro ao ler o cabecalho");
        fclose(file);
        exit(1);
    }
    printf("Cabecalho do arquivo .WAV: \n\n");
    printf(" __________________________________\n");
    printf("|  ID                 |   Value    |\n");
    printf("+---------------------+------------+\n");
    printf("| Chunck ID           | %.4s\t   |\n", WAV.RIFF.ChunkID);
    printf("| Chunck Size         | %d   |\n", WAV.RIFF.ChunkSize);
    printf("| Format               | %.4s\t   |\n", WAV.RIFF.Format);
    printf("+---------------------+------------+\n");
    printf("| SubChunk1 ID        | %.4s\t   |\n", WAV.FMT.SubChunk1ID);
    printf("| SubChunk1 Size      | %d\t   |\n", WAV.FMT.SubChunk1Size);
    printf("| Audioformat         | %d\t   |\n", WAV.FMT.Audio_format);
    printf("| Numero de channels  | %d\t   |\n", WAV.FMT.N_of_channels);
    printf("| Sample rate         | %d\t   |\n", WAV.FMT.Sample_rate);
    printf("| Byte rate           | %d\t   |\n", WAV.FMT.Byte_rate);
    printf("| Block align         | %d\t   |\n", WAV.FMT.Block_align);
    printf("| Bits per sample     | %d\t   |\n", WAV.FMT.Bits_per_sample);
    printf("+---------------------+------------+\n");
    printf("| SubChunk2 ID        | %.4s\t   |\n", WAV.Dados.SubChunk2ID);
    printf("| SubChunk2 Size      | %d\t   |\n", WAV.Dados.SubChunk2Size);
    printf("| Audio data          | %u |\n", WAV.Dados.Audio_data);
    printf("====================================\n\n");
    printf("End of Header\n\n");
}

// Tarefa 2: escrever um programa C que copie um arquivo WAV em outro arquivo
// definindo um struct com os mesmos campos, tamanhos e ordem do cabeçalho de um
// arquivo .WAV, e usando a função de leitura fread.
void copyFile(header *SAMPLE, FILE *file, FILE *copy)
{
    int ch;
    readHeader(SAMPLE, file);
    writeInFile(SAMPLE, copy);

    while ((ch = fgetc(file)) != EOF)
    {
        fputc(ch, copy);
    }
    // printHeader(copy);
    printf("Arquivo Copiado Com Sucesso!!");
}

// Tarefa 3: escrever um programa C que obtenha e escreva o maior e o menor valor do
// sinal de áudio (amostra) presente num arquivo WAV.
void largest(FILE *file)
{
    // header SAMPLE;
    unsigned int aux = 0;
    uint16_t sample, max, min;

    // fread(&SAMPLE, sizeof(header), 1, file);
    while (fread(&sample, sizeof(uint16_t), 1, file) == 1)
    {
        if (aux == 0)
        {
            max = sample;
            min = sample;
            aux = 1;
        }
        if (sample > max)
        {
            max = sample;
        }
        if (sample < min)
        {
            min = sample;
        }
    }

    printf("Maior valor de amostra: %d\n", max);
    printf("Menor valor de amostra: %d\n", min);
}

// Tarefa 4: escrever um programa C que, dado um arquivo wav, gere um outro arquivo wav
// contendo as amostras invertidas do arquivo dado
void invert(header *SAMPLE, FILE *file, FILE *inv)
{
    printf("%d\n", SAMPLE->FMT.Bits_per_sample);
    uint32_t i, n_samples = SAMPLE->Dados.SubChunk2Size / (SAMPLE->FMT.Bits_per_sample / 8);
    printHeader(file);
    // uint16_t n_samples = SAMPLE->FMT.Bits_per_sample;
    printf("bits per sample: %d\n", SAMPLE->Dados.SubChunk2Size);
    uint16_t *samples = malloc(SAMPLE->Dados.SubChunk2Size);
    if (samples == NULL)
    {
        printf("Erro de memoria");
        exit(1);
    }
    fread(samples, sizeof(uint16_t), n_samples, file);

    for (i = 0; i < (n_samples / 2); i++)
    {
        uint16_t temp = samples[i];
        samples[i] = samples[n_samples - 1 - i];
        samples[n_samples - 1 - i] = temp;
    }
    writeInFile(SAMPLE, inv);
    fwrite(samples, sizeof(uint16_t), n_samples, inv);

    free(samples);
    printf("Audio invertido com sucesso!!\n");
    printHeader(inv);
}
//=================================Main()==============================================
int main()
{
    header SAMPLE;
    FILE *fil, *cpy, *inv;
    int menu;

    if ((fil = fopen("source/sample.wav", "rb")) == NULL)
    {
        printf("Arquivo 'audio.wav' nao foi aberto");
        exit(1);
    }
    // faz a leitura do header do arquivo original uma unica vez
    readHeader(&SAMPLE, fil);
    do
    {
        printf("===============Menu==============\n");
        printf(" 1 - Tarefa 1 (Imprimir Valores)\n 2 - Tarefa 2 (Copiar)\n 3 - Tarefa 3 (Maior e Menor)\n 4 - Tarefa 4 (Inverter)\n\n 0 - Sair\n");
        printf("---------------------------------\nEscolha: ");
        scanf("%d", &menu);

    } while (menu > 4 || menu < 0);
    printf("---------------------------------\n");
    switch (menu)
    {
    case 1:
        printHeader(fil);
        break;
    case 2:
        if ((cpy = fopen("source/copy.wav", "wb")) == NULL)
        {
            printf("Falha ao abrir/criar arquivo de cópia.");
            exit(1);
        }
        copyFile(&SAMPLE, fil, cpy);
        break;
    case 3:
        largest(fil);
        break;
    case 4:
        if ((inv = fopen("source/inverted.wav", "wb")) == NULL)
        {
            printf("Falha ao abrir/criar arquivo de inversão.");
            exit(1);
        }
        invert(&SAMPLE, fil, inv);
        break;
    default:
        break;
    }
    fclose(fil);
    fclose(cpy);
    fclose(inv);
    return 0;
}

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
    unsigned int SubChunk1Size;
    unsigned int Audio_format;
    unsigned int N_of_channels;
    unsigned int Sample_rate;
    unsigned int Byte_rate;
    unsigned int Block_align;
    unsigned int Bits_per_sample;
} tmf;
typedef struct
{
    char SubChunk2ID[4];
    unsigned int SubChunk2Size;
    unsigned int Audio_data;
} data;

typedef struct
{
    // int etiq;
    riff RIFF;
    tmf TMF;
    data Dados;
} header;

//=================================Funções========================================
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
    printf(" _______________________________\n");
    printf("|  RIFF\t\t|   Value\t|\n");
    printf("+---------------+---------------+\n");
    printf("|Chunck ID\t| %.4s\t\t|\n", WAV.RIFF.ChunkID);
    printf("|Chunck Size\t| %u\t|\n", WAV.RIFF.ChunkSize);
    printf("|Format\t\t| %.4s\t\t|\n\n", WAV.RIFF.Format);

    printf(" ___________________________________ \n");
    printf("|    TMF             |   Value      |\n");
    printf("+--------------------+--------------+\n");
    printf("|SubChunk1 ID        |  %.4s\t    |\n", WAV.TMF.SubChunk1ID);
    printf("|SubChunk1 Size      |  %u\t    |\n", WAV.TMF.SubChunk1Size);
    printf("|Audioformat         |  %u\t    |\n", WAV.TMF.Audio_format);
    printf("|Numero de channels  |  %u\t    |\n", WAV.TMF.N_of_channels);
    printf("|Sample rate         |  %u\t    |\n", WAV.TMF.Sample_rate);
    printf("|Byte rate           |  %u\t    |\n", WAV.TMF.Byte_rate);
    printf("|Block align         |  %u  |\n", WAV.TMF.Block_align);
    printf("|Bits per sample     |  %u\t    |\n\n", WAV.TMF.Bits_per_sample);

    printf(" _______________________________ \n");
    printf("|    DATA       |   Value       |\n");
    printf("+---------------+---------------+\n");
    printf("|SubChunk2 ID\t|  %.4s\t\t|\n", WAV.Dados.SubChunk2ID);
    printf("|SubChunk2 Size\t|  %u\t|\n", WAV.Dados.SubChunk2Size);
    printf("|Audio data\t|  %u\t\t|\n\nEnd of Header\n\n", WAV.Dados.Audio_data);
}

// Tarefa 2: escrever um programa C que copie um arquivo WAV em outro arquivo
// definindo um struct com os mesmos campos, tamanhos e ordem do cabeçalho de um
// arquivo .WAV, e usando a função de leitura fread.
void copyFile(FILE *file, FILE *copy)
{
    header SAMPLE;
    int ch;

    // fread(&SAMPLE.RIFF, sizeof(riff), 1, file);
    // fread(&SAMPLE.Dados, sizeof(data), 1, file);
    // fread(&SAMPLE.TMF, sizeof(tmf), 1, file);

    printHeader(file);

    // fwrite(&SAMPLE.RIFF, sizeof(riff), 1, copy);
    // fwrite(&SAMPLE.Dados, sizeof(data), 1, copy);
    // fwrite(&SAMPLE.TMF, sizeof(tmf), 1, copy);

    fread(&SAMPLE, sizeof(header), 1, file);
    fwrite(&SAMPLE, sizeof(header), 1, copy);

    while ((ch = fgetc(file)) != EOF)
    {
        fputc(ch, copy);
    }
    printHeader(copy);
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
void invert(FILE *f, FILE *i)
{
}
//=================================Main()==============================================
int main()
{
    FILE *fil, *cpy, *inv;
    int menu;

    if ((fil = fopen("source/sample.wav", "rb")) == NULL)
    {
        printf("Arquivo 'audio.wav' nao foi aberto");
        exit(1);
    }
    else
    {

        if ((cpy = fopen("source/copy.wav", "wb")) == NULL)
        {
            printf("Falha ao abrir/criar arquivo de cópia.");
        }

        if ((inv = fopen("source/inverted.wav", "wb")) == NULL)
        {
            printf("Falha ao abrir/criar arquivo de cópia.");
        }
    }

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
        copyFile(fil, cpy);
        break;
    case 3:
        largest(fil);
        break;
    case 4:
        invert(fil, inv);
        break;
    default:
        break;
    }
    fclose(fil);
    fclose(cpy);
    fclose(inv);
    return 0;
}

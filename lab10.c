#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
    struct RecordType* next; // Pointer to the next record for chaining
};

// Fill out this structure
struct HashType
{
    struct RecordType** records;
    int size;
};

// Compute the hash function
int hash(int x, int size)
{
    return x % size;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL; // Initialize the next pointer
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i, j;

    for (i = 0; i < hashSz; ++i)
    {
        if (pHashArray->records[i] != NULL)
        {
            printf("Index %d -> ", i);
            struct RecordType* current = pHashArray->records[i];
            while (current != NULL)
            {
                printf("%d %c %d", current->id, current->name, current->order);
                current = current->next;
                if (current != NULL)
                    printf(" -> ");
            }
            printf("\n");
        }
    }
}

// Insert a record into the hash table
void insertRecord(struct HashType* hashTable, struct RecordType record)
{
    int index = hash(record.id, hashTable->size);
    if (hashTable->records[index] == NULL)
    {
        hashTable->records[index] = (struct RecordType*)malloc(sizeof(struct RecordType));
        *hashTable->records[index] = record;
        hashTable->records[index]->next = NULL;
    }
    else
    {
        struct RecordType* current = hashTable->records[index];
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = (struct RecordType*)malloc(sizeof(struct RecordType));
        *current->next = record;
        current->next->next = NULL;
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);
    
    // Initialize the hash table
    struct HashType hashTable;
    int hashSize = 10; // Choose your hash table size
    hashTable.size = hashSize;
    hashTable.records = (struct RecordType**)malloc(sizeof(struct RecordType*) * hashSize);
    for (int i = 0; i < hashSize; ++i)
    {
        hashTable.records[i] = NULL;
    }

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i)
    {
        insertRecord(&hashTable, pRecords[i]);
    }

    // Display records in the hash table
    displayRecordsInHash(&hashTable, hashSize);

    return 0;
}

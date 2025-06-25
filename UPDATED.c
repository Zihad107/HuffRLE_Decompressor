#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Huffman Tree structure
struct TreeNode
{
    char character;
    unsigned freq;
    struct TreeNode *left, *right;
};

// MinHeap structure
struct Heap
{
    unsigned size;
    unsigned capacity;
    struct TreeNode **array;
};

// decompression structure (prefix)
struct Trie
{
    char *huffCode; // Store the Huffman code at the leaf
    struct Trie *children[10];
};

// Huffman Coding Functions
struct TreeNode *createTreeNode(char character, unsigned freq)
{
    struct TreeNode *node = (struct TreeNode *)malloc(sizeof(struct TreeNode));
    if (node==NULL)
    {
        fprintf(stderr, "Failed to allocate memory for TreeNode");
        exit(EXIT_FAILURE);
    }
    node->character = character;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

//Creates an empty heap to store huffman code
struct Heap *createHeap(unsigned capacity)
{
    struct Heap *heap = (struct Heap *)malloc(sizeof(struct Heap));
    if (heap==NULL)
    {
        fprintf(stderr, "Failed to allocate memory for Heap");
        exit(EXIT_FAILURE);
    }
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (struct TreeNode **)malloc(heap->capacity * sizeof(struct TreeNode *));
    if (heap->array=='\0')
    {
        fprintf(stderr, "Failed to allocate memory for Heap array");
        exit(EXIT_FAILURE);
    }
    return heap;
}

void swapHeapNodes(struct TreeNode **a, struct TreeNode **b)
{
    struct TreeNode *temp = *a;
    *a = *b;
    *b = temp;
}

//Maintains min heap property
void heapify(struct Heap *heap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    if (left < heap->size && heap->array[left]->freq < heap->array[smallest]->freq)
        smallest = left;
    if (right < heap->size && heap->array[right]->freq < heap->array[smallest]->freq)
        smallest = right;
    if (smallest != idx)
    {
        swapHeapNodes(&heap->array[smallest], &heap->array[idx]);
        heapify(heap, smallest);
    }
}

struct TreeNode *extractMin(struct Heap *heap)
{
    if (heap->size == 0)
        return NULL;

    struct TreeNode *temp = heap->array[0];
    heap->array[0] = heap->array[heap->size - 1];
    --heap->size;
    heapify(heap, 0);
    return temp;
}

void insertHeap(struct Heap *heap, struct TreeNode *node)
{
    if (heap->size == heap->capacity)
    {
        heap->capacity = heap->capacity * 2;
        heap->array = (struct TreeNode **)realloc(heap->array, heap->capacity * sizeof(struct TreeNode *));
        if (heap->array=='\0')
        {
            fprintf(stderr, "Memory reallocation failed for Heap array.");
            exit(EXIT_FAILURE);
        }
    }
    ++heap->size;
    int i = heap->size - 1;
    while (i && node->freq < heap->array[(i - 1) / 2]->freq)
    {
        heap->array[i] = heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->array[i] = node;
}

//Organizes an array of nodes into a valid min-heap
void buildHeap(struct Heap *heap)
{
    int n = heap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        heapify(heap, i);
}

//Combines the functionality of createHeap and buildHeap
struct Heap *createAndBuildHeap(char character[], int freq[], int size)
{
    struct Heap *heap = createHeap(size);
    for (int i = 0; i < size; ++i)
        heap->array[i] = createTreeNode(character[i], freq[i]);

    heap->size = size;
    buildHeap(heap);
    return heap;
}

//Constructs Hufman Tree
struct TreeNode *buildHuffTree(char character[], int freq[], int size)
{
    struct TreeNode *left, *right, *top;
    struct Heap *heap = createAndBuildHeap(character, freq, size);

    while (heap->size != 1)//until we get a single node in the heap which indicates the root of hufftree
    {
        left = extractMin(heap);
        right = extractMin(heap);
        top = createTreeNode('\0', left->freq + right->freq); // '\0' indicates "no character"
        top->left = left;
        top->right = right;
        insertHeap(heap, top);
    }
    struct TreeNode *root = extractMin(heap);
    free(heap->array);
    free(heap);
    return root;
}

void generateHuffCodes(struct TreeNode *root, char codes[256][256], char currentCode[], int top)
{
    if (root->left)
    {
        currentCode[top] = '0';
        currentCode[top + 1] = '\0';
        generateHuffCodes(root->left, codes, currentCode, top + 1);
    }
    if (root->right)
    {
        currentCode[top] = '1';
        currentCode[top + 1] = '\0';
        generateHuffCodes(root->right, codes, currentCode, top + 1);
    }
    if ((root->left)==NULL && (root->right)==NULL)
    {
        strcpy(codes[(unsigned char)root->character], currentCode);
    }
}

// Frequency Calculation
void calculateFreq(const char text[], int freq[], char character[], int *size)
{
    int len = strlen(text);
    for (int i = 0; i < len; i++)
    {
        unsigned char ch = text[i];
        int j;
        for (j = 0; j < *size; j++)
        {
            if (character[j] == ch)
            {
                freq[j]++;
                break;
            }
        }
        if (j == *size)
        {
            character[*size] = ch;
            freq[*size] = 1;
            (*size)++;
        }
    }
}

// RLE Encoding
char* applyRLE(const char *encodedText)
{
    size_t len = strlen(encodedText);

    // Worst case scenario: no repeats, so RLE length = original length
    // Plus some extra space for counts (assuming counts are small)
    size_t buffer_size = len * 2 + 1;
    char *rleEncoded = (char *)malloc(buffer_size);
    if (rleEncoded==NULL)
    {
        fprintf(stderr,  "Memory allocation failed for RLE encoded string.");
        exit(EXIT_FAILURE);
    }
    size_t j = 0;
    for (size_t i = 0; i < len; i++)
    {
        rleEncoded[j++] = encodedText[i];
        int count = 1;
        while (i + 1 < len && encodedText[i] == encodedText[i + 1])
        {
            count++;
            i++;
        }
        if (count > 1)
        {
            // Convert count to string
            char countStr[20];
            int countLen = sprintf(countStr, "%d", count);

            // Ensure buffer has enough space
            if (j + countLen >= buffer_size)
            {
                buffer_size = buffer_size * 2;

                //Retains existing data using realloc
                rleEncoded = (char *)realloc(rleEncoded, buffer_size);
                if (rleEncoded==NULL)
                {
                    fprintf(stderr, "Memory reallocation failed for RLE encoded string.");
                    exit(EXIT_FAILURE);
                }
            }
            memcpy(&rleEncoded[j], countStr, countLen);
            j = j + countLen; // Update j and append the count in the existing string using sprintf
        }
    }
    rleEncoded[j] = '\0';
    return rleEncoded;
}

// Trie Creation and Insert
struct Trie *createTrieNode()
{
    struct Trie *node = (struct Trie *)malloc(sizeof(struct Trie));
    if (node==NULL)
    {
        fprintf(stderr,  "Memory allocation failed for Trie node.\n");
        exit(EXIT_FAILURE);
    }
    node->huffCode = NULL;
    for (int i = 0; i < 10; ++i)
        node->children[i] = NULL;
    return node;
}

void insertTrie(struct Trie *root, const char *rleEncoded, const char *huffCode)
{
    struct Trie *current = root;
    for (int i = 0; rleEncoded[i] != '\0'; i++)
    {
        if (isdigit(rleEncoded[i])=='\0')
        {
            fprintf(stderr,  "RLE encoding contains non-digit characters in position %d.\n", i);
            exit(EXIT_FAILURE);
        }
        int index = rleEncoded[i] - '0';
        if (index < 0 || index > 9)
        {
            fprintf(stderr,  "RLE encoding digit out of range (0-9) at position %d.\n", i);
            exit(EXIT_FAILURE);
        }
        if (current->children[index] == NULL)
        {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
    }
    current->huffCode = strdup(huffCode);
    if (current->huffCode==NULL)
    {
        fprintf(stderr, "Memory allocation failed for Huffman code in Trie.");
        exit(EXIT_FAILURE);
    }
}

// RLE Decoding
void decodeRLE(struct Trie *root, const char *rleEncodedText, char **decodedHuff)
{
    struct Trie *current = root;
    int i = 0;
    size_t decoded_size = strlen(rleEncodedText) * 8 + 1; // Approximate size
    *decodedHuff = (char *)malloc(decoded_size);
    if ((*decodedHuff)=='\0')
    {
        fprintf(stderr,  "Memory allocation failed for decoded Huffman string.\n");
        exit(EXIT_FAILURE);
    }
    (*decodedHuff)[0] = '\0';
    while (rleEncodedText[i] != '\0')
    {
        if (isdigit(rleEncodedText[i])=='\0')
        {
            fprintf(stderr,  "RLE encoded text contains non-digit characters at position %d.\n", i);
            free(*decodedHuff);
            exit(EXIT_FAILURE);
        }
        int index = rleEncodedText[i] - '0';
        if (index < 0 || index > 9)
        {
            fprintf(stderr,  "RLE encoding digit out of range (0-9) at position %d.\n", i);
            free(*decodedHuff);
            exit(EXIT_FAILURE);
        }
        if (current->children[index])
        {
            current = current->children[index];
            i++;
            if (current->huffCode)
            {
                size_t huff_len = strlen(current->huffCode);
                size_t current_len = strlen(*decodedHuff);
                if (current_len + huff_len + 1 > decoded_size)
                {
                    decoded_size = decoded_size * 2;
                    *decodedHuff = (char *)realloc(*decodedHuff, decoded_size);
                    if ((*decodedHuff)==NULL)
                    {
                        fprintf(stderr,  "Memory reallocation failed for decoded Huffman string.\n");
                        exit(EXIT_FAILURE);
                    }
                }
                strcat(*decodedHuff, current->huffCode);
                current = root;
            }
        }

        else//when there is only one character in the array
        {
            // If current path does not exist, it might be a count
            if (isdigit(rleEncodedText[i]))
            {
                int count = 0;
                while (isdigit(rleEncodedText[i]))
                {
                    count = count * 10 + (rleEncodedText[i] - '0');
                    i++;
                }
                if (current->huffCode == '\0')
                {
                    fprintf(stderr,  "Invalid RLE encoding: count without preceding Huffman code.\n");
                    free(*decodedHuff);
                    exit(EXIT_FAILURE);
                }
                size_t huff_len = strlen(current->huffCode);
                size_t current_len = strlen(*decodedHuff);
                size_t required_size = current_len + huff_len * count + 1;
                if (required_size > decoded_size)
                {
                    while (required_size > decoded_size)
                    {
                        decoded_size = decoded_size * 2;
                    }
                    *decodedHuff = (char *)realloc(*decodedHuff, decoded_size);
                    if ((*decodedHuff)=='\0')
                    {
                        fprintf(stderr,  "Memory reallocation failed for decoded Huffman string.\n");
                        exit(EXIT_FAILURE);
                    }
                }
                for (int k = 0; k < count; k++)
                {
                    strcat(*decodedHuff, current->huffCode);
                }
                current = root;
            }
            else
            {
                fprintf(stderr,  "Invalid character in RLE encoded text at position %d.\n", i);
                free(*decodedHuff);
                exit(EXIT_FAILURE);
            }
        }
    }
}

// Huffman Decoding
void decodeHuff(struct TreeNode *root, const char *encodedText, char **originalText)
{
    struct TreeNode *current = root;
    int i = 0;
    size_t original_size = strlen(encodedText) + 1; // Max possible
    *originalText = (char *)malloc(original_size);
    if ((*originalText)==NULL)
    {
        fprintf(stderr,  "Memory allocation failed for original text.\n");
        exit(EXIT_FAILURE);
    }
    (*originalText)[0] = '\0';
    size_t j = 0;
    while (encodedText[i] != '\0')
    {
        if (encodedText[i] == '0')
            current = current->left;
        else if (encodedText[i] == '1')
            current = current->right;
        else
        {
            fprintf(stderr,  "Invalid character in Huffman encoded text at position %d.\n", i);
            free(*originalText);
            exit(EXIT_FAILURE);
        }
        if (current == NULL)
        {
            fprintf(stderr,  "Traversal reached a NULL node at position %d.\n", i);
            free(*originalText);
            exit(EXIT_FAILURE);
        }
        if (current->left == NULL && current->right == NULL)
        {
            // Ensure there is enough space
            if (j + 1 >= original_size)
            {
                original_size = original_size *2;
                *originalText = (char *)realloc(*originalText, original_size);
                if ((*originalText)==NULL)
                {
                    fprintf(stderr,  "Memory reallocation failed for original text.\n");
                    exit(EXIT_FAILURE);
                }
            }
            (*originalText)[j++] = current->character;
            (*originalText)[j] = '\0';
            current = root;
        }
        i++;
    }
}

// Function to generate random input
char* generateRandomText(int length)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const size_t charset_size = sizeof(charset) - 1;
    char *text = (char *)malloc((length + 1) * sizeof(char));
    if (text==NULL)
    {
        fprintf(stderr,  "Memory allocation failed for random text.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < length; i++)
    {
        text[i] = charset[rand() % charset_size];
    }
    text[length] = '\0';
    return text;
}

// Function to print time and space information
void printTotalTimeAndSpace(clock_t start_time, clock_t end_time, size_t total_space)
{
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("\nTime Taken: %f seconds\n", time_taken);
    printf("Total Space Consumed: %zu bytes\n", total_space);
}

int main()
{
    srand((unsigned int)time(NULL));  // random number generation
    int length;
    printf("Enter the length of random text to generate: ");
    scanf("%d", &length);

    // Generate random text
    char *text = generateRandomText(length);
    printf("Generated Random Text: %s\n", text);
    clock_t start_time = clock();

    int *freq = (int *)calloc(256, sizeof(int));
    if (freq==NULL)
    {
        fprintf(stderr,  "Memory allocation failed for frequency array.\n");
        free(text);
        return EXIT_FAILURE;
    }

    char *character = (char *)malloc(256 * sizeof(char));
    if (character==NULL)
    {
        fprintf(stderr,  "Memory allocation failed for character array.\n");
        free(text);
        free(freq);
        return EXIT_FAILURE;
    }

    int size = 0;
    calculateFreq(text, freq, character, &size);

    // Handling for single-character input
    if (size == 1)
    {
        char *rleEncoded = applyRLE(text);
        printf("Huffman Encoded String: 0\n");// Only one unique character, Huffman code is '0'
        printf("RLE Encoded String: %s\n", rleEncoded);
        printf("Original Text After Decompression: %s\n", text);
        free(text);
        free(rleEncoded);
        free(freq);
        free(character);
        return EXIT_SUCCESS;
    }

    struct TreeNode *root = buildHuffTree(character, freq, size);
    char codes[256][256] = {0};
    char currentCode[256];
    currentCode[0] = '\0';
    generateHuffCodes(root, codes, currentCode, 0);

    // Estimate the size of encodedText (each character can be up to 256 bits)
    size_t encoded_size = length * 16 + 1; // Adjust as needed
    char *encodedText = (char *)malloc(encoded_size);
    if (encodedText==NULL)
    {
        fprintf(stderr,  "Memory allocation failed for encoded text.\n");
        free(text);
        free(freq);
        free(character);

        return EXIT_FAILURE;
    }

    encodedText[0] = '\0';
    for (int i = 0; i < length; i++)
    {
        strcat(encodedText, codes[(unsigned char)text[i]]);
    }
    printf("\nHuffman Encoded String:\n%s\n", encodedText);

    char *rleEncoded = applyRLE(encodedText);
    printf("\nRLE Encoded String:\n%s\n", rleEncoded);

    struct Trie *trieRoot = createTrieNode();

    insertTrie(trieRoot, rleEncoded, encodedText);

    char *decodedHuff = NULL;
    decodeRLE(trieRoot, rleEncoded, &decodedHuff);

    char *originalText = NULL;
    decodeHuff(root, decodedHuff, &originalText);
    printf("\nOriginal Text After Decompression: %s\n", originalText);
    clock_t end_time = clock();

    // Estimate total space consumed
    size_t total_space =
        sizeof(struct TreeNode) * size +
        sizeof(struct Trie) +
        strlen(encodedText) +
        strlen(rleEncoded) +
        sizeof(char) * (length + 1) +
        sizeof(char) * (strlen(decodedHuff) + 1) +
        sizeof(char) * (strlen(originalText) + 1) +
        sizeof(char) * (256 + 256) + // freq and character arrays
        sizeof(struct Heap);
    printTotalTimeAndSpace(start_time, end_time, total_space);

    // Free allocated memory
    free(text);
    free(freq);
    free(character);
    free(encodedText);
    free(rleEncoded);
    free(decodedHuff);
    free(originalText);

    return EXIT_SUCCESS;
}



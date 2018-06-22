#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Brainfuck
{
    char data[30000];
    int pointer;
    int max_pointer;
};

struct Brainfuck *new_Brainfuck()
{
    struct Brainfuck *brainfuck = malloc(sizeof(struct Brainfuck));
    assert (brainfuck != NULL);

    brainfuck -> pointer = 0;
    brainfuck -> max_pointer = 0;

    int i = 0;
    for(;i<30000;i++)
    {
        brainfuck -> data[i]=0;
    }

    return brainfuck;
}

void Brainfuck_destroy(struct Brainfuck *brainfuck)
{
    assert (brainfuck != NULL);
    free(brainfuck);
}

void Brainfuck_increment_pointer(struct Brainfuck *brainfuck)
{
    brainfuck -> pointer += 1;
    brainfuck -> max_pointer += 1;
}

void Brainfuck_decrement_pointer(struct Brainfuck *brainfuck)
{
    brainfuck -> pointer -= 1;
}

void Brainfuck_increment_data(struct Brainfuck *brainfuck)
{
    brainfuck -> data[brainfuck->pointer] += 1;
}

void Brainfuck_decrement_data(struct Brainfuck *brainfuck)
{
    brainfuck -> data[brainfuck->pointer] -= 1;
}

char Brainfuck_peek(struct Brainfuck *brainfuck)
{
    return brainfuck -> data[brainfuck->pointer];
}

void Brainfuck_output(struct Brainfuck *brainfuck)
{
    printf("%c\n", Brainfuck_peek(brainfuck));
}

void Brainfuck_debug(struct Brainfuck *brainfuck)
{
    printf("index    = ");
    int i=0;
    for(;i<=brainfuck->max_pointer;i++)
    {
        printf("|%3d", i);
    }
    printf("|\n");

    printf("data     = ");
    i=0;
    for(;i<=brainfuck->max_pointer;i++)
    {
        printf("|%3d", brainfuck->data[i]);
    }
    printf("|\n");

    printf("pointer  = ");
    i=0;
    for(;i<=brainfuck->pointer;i++)
    {
        printf(" ");
    }
    printf("%3s", "^");

    printf("\n");
}

struct CharTokenizer
{
    char *input;
    int size;
    int position;
};

struct CharTokenizer *new_CharTokenizer(char *input)
{
    struct CharTokenizer *tokenizer = malloc(sizeof(struct CharTokenizer));
    assert (tokenizer != NULL);

    tokenizer -> input = input;
    tokenizer -> size = strlen(input);
    tokenizer -> position = -1;

    return tokenizer;
}

void CharTokenizer_destroy(struct CharTokenizer *tokenizer)
{
    assert (tokenizer != NULL);
    free(tokenizer);
}

int CharTokenizer_has_next(struct CharTokenizer *tokenizer)
{
    if((tokenizer->position + 1) < tokenizer->size)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

char CharTokenizer_next(struct CharTokenizer *tokenizer)
{
    return tokenizer->input[++(tokenizer->position)];
}

void CharTokenizer_jump_forward_to(struct CharTokenizer *tokenizer, char c)
{
    int i = tokenizer->position;
    for (; i < tokenizer->size; i++)
    {
        if (tokenizer->input[i] == c)
        {
            tokenizer->position = i;
            break;
        }
    }
}

void CharTokenizer_jump_back_to(struct CharTokenizer *tokenizer, char c)
{
    int i = tokenizer->position;
    for (; i >=0; i--)
    {
        if (tokenizer->input[i] == c)
        {
            tokenizer->position = i;
            break;
        }
    }
}

struct Brainfuck *interpret(const char *input_string)
{
    struct Brainfuck *brainfuck = new_Brainfuck();
    struct CharTokenizer *tokenizer = new_CharTokenizer("+++>++<[->+<]");

    while(CharTokenizer_has_next(tokenizer))
    {
        char command_char = CharTokenizer_next(tokenizer);
        switch(command_char)
        {
            case '>':
                Brainfuck_increment_pointer(brainfuck);
                break;
            case '<':
                Brainfuck_decrement_pointer(brainfuck);
                break;
            case '+':
                Brainfuck_increment_data(brainfuck);
                break;
            case '-':
                Brainfuck_decrement_data(brainfuck);
                break;
            case '.':
                Brainfuck_output(brainfuck);
                break;
            case '[':
                if(Brainfuck_peek(brainfuck) == 0)
                {
                    CharTokenizer_jump_forward_to(tokenizer, ']');
                }
                break;
            case ']':
                if(Brainfuck_peek(brainfuck) != 0)
                {
                    CharTokenizer_jump_back_to(tokenizer, '[');
                }
                break;
            default:
                Brainfuck_destroy(brainfuck);
                printf("Invalid char: %c", command_char);
                exit(-1);
        }
    }

    CharTokenizer_destroy(tokenizer);

    return brainfuck;
}

int main(void)
{
    struct Brainfuck *brainfuck = interpret("+++>++<[->+<]");
    Brainfuck_debug(brainfuck);
    Brainfuck_destroy(brainfuck);

    return 0;
}

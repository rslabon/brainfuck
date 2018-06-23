#!/usr/bin/env python

# >	increment the data pointer (to point to the next cell to the right).
# <	decrement the data pointer (to point to the next cell to the left).
# +	increment (increase by one) the byte at the data pointer.
# -	decrement (decrease by one) the byte at the data pointer.
# .	output the byte at the data pointer.
# ,	accept one byte of input, storing its value in the byte at the data pointer.
# [	if the byte at the data pointer is zero, then instead of moving the instruction pointer forward to the next command, jump it forward to the command after the matching ] command.
# ]	if the byte at the data pointer is nonzero, then instead of moving the instruction pointer forward to the next command, jump it back to the command after the matching [ command.


class Brainfuck:
    def __init__(self):
        self.data = [0] * 30000
        self.pointer = 0
        self.max_pointer = 0

    @staticmethod
    def interpret(input_string):
        brainfuck = Brainfuck()
        tokenizer = CharTokenizer(input_string)
        commands = {
            '>': brainfuck.increment_pointer,
            '<': brainfuck.decrement_pointer,
            '+': brainfuck.increment_data,
            '-': brainfuck.decrement_data,
            '.': brainfuck.output,
            ',': brainfuck.input,
            '[': (lambda: tokenizer.jump_forward_to(']') if brainfuck.peek() == 0 else None),
            ']': (lambda: tokenizer.jump_back_to('[') if brainfuck.peek() != 0 else None),
        }
        while tokenizer.has_next():
            command_char = tokenizer.next()
            command_handler = commands[command_char]
            if command_handler is None:
                raise Exception("Invalid char: '%s'" % command_char)
            command_handler()

        return brainfuck

    def peek(self):
        return self.data[self.pointer]

    def increment_pointer(self):
        self.pointer += 1
        self.max_pointer += 1

    def decrement_pointer(self):
        self.pointer -= 1

    def increment_data(self):
        self.data[self.pointer] += 1

    def decrement_data(self):
        self.data[self.pointer] -= 1

    def output(self):
        print(self.data[self.pointer])

    def input(self):
        self.data[self.pointer] = int(input())

    def debug(self):
        print("index     = ", end='')
        for index in range(0, self.max_pointer):
            print("| %3d" % index, end='')
        print("|")

        print("data      = ", end='')
        for index in range(0, self.max_pointer):
            print("| %3d" % (self.data[index]), end='')
        print("|")

        print("position  = ", end='')
        for index in range(0, self.pointer):
            print(" ", end='')
        print("%5s" % "^")


class CharTokenizer:
    def __init__(self, input_string):
        self.input_chars = list(input_string)
        self.position = -1

    def has_next(self):
        return self.position + 1 < len(self.input_chars)

    def next(self):
        self.position += 1
        return self.input_chars[self.position]

    def jump_forward_to(self, c):
        index = 0
        while index < len(self.input_chars):
            if self.input_chars[index] == c:
                self.position = index
                break
            index += 1

    def jump_back_to(self, c):
        index = len(self.input_chars) - 1
        while index >= 0:
            if self.input_chars[index] == c:
                self.position = index
                break
            index -= 1


Brainfuck.interpret("+++>++<[->+<]").debug()

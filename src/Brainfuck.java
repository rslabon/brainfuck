import java.io.IOException;
import java.util.HashMap;
import java.util.Map;


// >	increment the data pointer (to point to the next cell to the right).
// <	decrement the data pointer (to point to the next cell to the left).
// +	increment (increase by one) the byte at the data pointer.
// -	decrement (decrease by one) the byte at the data pointer.
// .	output the byte at the data pointer.
// ,	accept one byte of input, storing its value in the byte at the data pointer.
// [	if the byte at the data pointer is zero, then instead of moving the instruction pointer forward to the next command, jump it forward to the command after the matching ] command.
// ]	if the byte at the data pointer is nonzero, then instead of moving the instruction pointer forward to the next command, jump it back to the command after the matching [ command.

public class Brainfuck {

    public static void main(String[] args) {
        System.out.println("\n********* data[1] = 3 + 2 *********");
        Brainfuck.interpret("+++>++<[->+<]").debug();

        System.out.println("\n********* Hello World! *********");
        Brainfuck.interpret("++++++++++[>+++++++>++++++++++>+++>+++++++++<<<<-]>++.>+.+++++++..+++.>++.>---.<<.+++.------.--------.>+.").debug();
    }

    @FunctionalInterface
    interface CommandHandler {
        void handle();
    }

    private final byte[] data = new byte[30000];
    private int pointer = 0;
    private int maxPointer = 0;

    public static Brainfuck interpret(String input) {
        Brainfuck brainfuck = new Brainfuck();
        CharTokenizer tokenizer = new CharTokenizer(input);

        Map<Character, CommandHandler> commands = new HashMap<>();
        commands.put('>', brainfuck::incrementPointer);
        commands.put('<', brainfuck::decrementPointer);
        commands.put('+', brainfuck::incrementData);
        commands.put('-', brainfuck::decrementData);
        commands.put('.', brainfuck::outputByte);
        commands.put(',', brainfuck::inputByte);
        commands.put('[', () -> {
            if (brainfuck.peek() == 0) tokenizer.jumpForwardTo(']');
        });
        commands.put(']', () -> {
            if (brainfuck.peek() != 0) tokenizer.jumpBackTo('[');
        });

        while (tokenizer.hasNext()) {
            char commandChar = tokenizer.next();
            CommandHandler handler = commands.get(commandChar);
            if (handler == null) {
                throw new IllegalStateException("Invalid char: '" + commandChar +
                        "' at index " + tokenizer.getPosition() + " in: '" + input + "'");
            }
            handler.handle();
        }

        return brainfuck;
    }

    public byte peek() {
        return data[pointer];
    }

    public void incrementPointer() {
        pointer++;
        maxPointer++;
    }

    public void decrementPointer() {
        pointer--;
    }

    public void incrementData() {
        data[pointer]++;
    }

    public void decrementData() {
        data[pointer]--;
    }

    public void outputByte() {
        System.out.println(String.format("%3d[%1c]", peek(), (char) peek()));
    }

    public void inputByte() {
        try {
            data[pointer] = (byte) System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void debug() {
        System.out.print("index    = ");
        for (int i = 0; i <= maxPointer; i++) {
            System.out.print(String.format("|%3d", i));
        }
        System.out.println("|");

        System.out.print("data     = ");
        for (int i = 0; i <= maxPointer; i++) {
            System.out.print(String.format("|%3d", data[i]));
        }
        System.out.println("|");

        System.out.print("pointer  = ");
        for (int i = 0; i <= pointer; i++) {
        }
        System.out.println(String.format("%4s", "^"));
    }
}

class CharTokenizer {
    private final char[] input;
    private int position = -1;

    public CharTokenizer(String input) {
        this.input = input.toCharArray();
    }

    public boolean hasNext() {
        return position + 1 < input.length;
    }

    public char next() {
        return input[++position];
    }

    public int getPosition() {
        return position;
    }

    public void jumpForwardTo(char c) {
        for (int i = position; i < input.length; i++) {
            if (input[i] == c) {
                position = i;
                break;
            }
        }
    }

    public void jumpBackTo(char c) {
        for (int i = position; i >= 0; i--) {
            if (input[i] == c) {
                position = i;
                break;
            }
        }
    }
}

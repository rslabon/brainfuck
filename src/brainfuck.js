function charTokenizer(str) {
    let position = -1;
    return {
        hasNext: () => position + 1 < str.length,
        next: () => str.charAt(++position),
        jumpForward: (startingChar, endingChar) => {
            let count = 0;
            for (let i = position; i < str.length; i++) {
                if (str.charAt(i) === startingChar) {
                    count++;
                }
                if (str.charAt(i) === endingChar) {
                    count--;
                    if (count === 0) {
                        position = i;
                        return;
                    }
                }
            }
            throw new Error("Cannot find: " + endingChar);
        },
        jumpBack: (startingChar, endingChar) => {
            let count = 0;
            for (let i = position; i >= 0; i--) {
                if (str.charAt(i) === startingChar) {
                    count--;
                    if (count === 0) {
                        position = i;
                        return;
                    }
                }
                if (str.charAt(i) === endingChar) {
                    count++;
                }
            }
            throw new Error("Cannot find: " + startingChar);
        }
    };
}


function brainLuck(code, input) {
    const data = [];
    for (let i = 0; i < 30000; i++) {
        data.push(0);
    }
    let pointer = 0;
    let output = "";

    const codeTokenizer = charTokenizer(code);
    const inputTokenizer = charTokenizer(input);

    const handlers = {
        ">": () => {
            ++pointer;
            data[pointer] = data[pointer] || 0;
        },
        "<": () => {
            --pointer;
            if (pointer < 0) throw new Error("Pointer error: " + pointer);
        },
        "+": () => {
            data[pointer]++;
            if (data[pointer] > 255) data[pointer] = 0;
        },
        "-": () => {
            data[pointer]--;
            if (data[pointer] < 0) data[pointer] = 255;
        },
        ".": () => {
            output += String.fromCharCode(data[pointer]);
        },
        ",": () => data[pointer] = inputTokenizer.next().charCodeAt(0),
        "[": () => {
            if (data[pointer] === 0) codeTokenizer.jumpForward("[", "]")
        },
        "]": () => {
            if (data[pointer] !== 0) codeTokenizer.jumpBack("[", "]")
        }
    };

    while (codeTokenizer.hasNext()) {
        const c = codeTokenizer.next();
        const handler = handlers[c];
        if (!handler) throw new Error("Invalid char: " + c);

        handler();
    }

    return output;
}
#define  _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define LETTERS_NUMBER 26
#define MILLION 1000000

typedef struct SuperLetter {
    int *value;
    size_t occupied_space;
    size_t free_space;
} SuperLetter;

void init_letter(SuperLetter *superLetter) {
    superLetter->value = calloc(16, sizeof(int));
    superLetter->occupied_space = 1;
    superLetter->free_space = 16;
}

typedef struct SuperAlphabet {
    struct SuperLetter letters[LETTERS_NUMBER];
} SuperAlphabet;

SuperAlphabet *init_alphabet(void) {
    SuperAlphabet *new_alphabet = malloc(sizeof(struct SuperAlphabet));
    for (int i = 0; i < LETTERS_NUMBER; i++) {
        init_letter(&new_alphabet->letters[i]);
    }
    return new_alphabet;
}

void free_alphabet(SuperAlphabet *alphabet) {
    for (int i = 0; i < LETTERS_NUMBER; i++) {
        free(alphabet->letters[i].value);
    }
    free(alphabet);
}

bool is_alphabetic(char c) {
    return c >= 'a' && c <= 'z';
}

void check_letter_size(SuperLetter *letter, size_t desired_size) {

    if (letter->occupied_space == letter->free_space) {
        letter->free_space *= 2;
        letter->value = realloc(letter->value, letter->free_space * sizeof(int));
        if (letter->value == NULL) {
            exit(1);
        }
        memset(letter->value + letter->occupied_space, 0, (letter->free_space - letter->occupied_space) * sizeof(int));
    }

    while (letter->free_space <= desired_size) {
        letter->free_space *= 2;
        letter->value = realloc(letter->value, letter->free_space * sizeof(int));
        if (letter->value == NULL) {
            exit(1);
        }
        memset(letter->value + letter->occupied_space, 0, (letter->free_space - letter->occupied_space) * sizeof(int));
    }
}

void safe_inc(SuperLetter *letter) {
    size_t ind = 0;

    check_letter_size(letter, 0);

    letter->value[ind]++;

    while (letter->value[ind] >= MILLION) {
        letter->value[ind] %= MILLION;
        ind++;
        letter->value[ind] += 1;
    }
    if (letter->occupied_space < ind + 1) {
        letter->occupied_space = ind + 1;
    }
}

void safe_add(SuperLetter *letter1, SuperLetter *letter2) {
    size_t bigger_size = letter1->occupied_space;
    if (bigger_size < letter2->occupied_space) {
        bigger_size = letter2->occupied_space;
    }
    check_letter_size(letter1, bigger_size);

    size_t ind = 0;
    int following = 0;
    while (ind < bigger_size) {
        letter1->value[ind] += letter2->value[ind] + following;
        if (letter1->value[ind] >= MILLION) {
            letter1->value[ind] %= MILLION;
            following = 1;
        } else {
            following = 0;
        }
        ind++;
    }

    letter1->occupied_space = bigger_size;

    if (following == 1) {
        letter1->value[ind] = 1;
        letter1->occupied_space++;
    }
}

void safe_clear(SuperLetter *letter) {
    check_letter_size(letter, 0);
    size_t ind = 0;
    while (ind < letter->occupied_space) {
        letter->value[ind] = 0;
        ind++;
    }
    letter->occupied_space = 1;
}

bool compare_zero(SuperLetter *letter) {
    for (size_t i = 0; i < letter->occupied_space; i++) {
        if (letter->value[i] != 0) {
            return false;
        }
    }
    return true;
}

bool safe_decrement(SuperLetter *letter) {
    if (compare_zero(letter)) {
        return false;
    }

    for (size_t i = 0; i < letter->occupied_space; i++) {
        if (letter->value[i] == 0) {
            letter->value[i] = MILLION - 1;
        } else {
            letter->value[i]--;
            break;
        }
    }

    if (letter->occupied_space > 1 && letter->value[letter->occupied_space - 1] == 0) {
        letter->occupied_space -= 1;
    }

    return true;
}


size_t check_trailing_zeros(int n) {
    size_t trailing_zeros = 6;
    while (n != 0) {
        trailing_zeros--;
        n /= 10;
    }
    return trailing_zeros;
}

void safe_print(SuperLetter *letter) {
    size_t occupied = letter->occupied_space;

    printf("%d", letter->value[occupied - 1]);

    for (int i = (int) occupied - 2; i >= 0; i--) {
        size_t zeros = check_trailing_zeros(letter->value[i]);
        for (size_t j = 0; j < zeros; j++) {
            printf("0");
        }
        if (zeros != 6) {
            printf("%d", letter->value[i]);
        }
    }
    printf("\n");
}

typedef enum InstructionType {
    INC,
    ADD,
    CLR,
    JMP,
    DJZ,
    HLT,
    PRT
} InstructionType;

typedef struct Instruction {
    InstructionType instrType;
    int first_arg;
    int second_arg;
} Instruction;

typedef struct InstrContainer {
    struct Instruction *instructions;
    size_t occupied_space;
    size_t all_space;
} InstrContainer;

InstrContainer *init_container(void) {
    InstrContainer *container = malloc(sizeof(struct InstrContainer));
    container->instructions = malloc(16 * sizeof(struct Instruction));
    container->occupied_space = 0;
    container->all_space = 16;
    return container;
}

void free_container(InstrContainer *container) {
    free(container->instructions);
    free(container);
}

void set_djz_address(InstrContainer *container, int djz_own_address, int jumping_address) {
    container->instructions[djz_own_address].second_arg = jumping_address;
}

void check_resizing(InstrContainer *container) {
    if (container->occupied_space == container->all_space) {
        container->all_space *= 2;
        container->instructions =
                realloc(container->instructions, container->all_space * sizeof(struct Instruction));
        if (container->instructions == NULL) {
            exit(420);
        }
    }
}

size_t add_to_container(InstrContainer *container, InstructionType type, int arg1, int arg2) {
    Instruction new_instr = {type, arg1, arg2};

    check_resizing(container);
    container->instructions[container->occupied_space] = new_instr;
    container->occupied_space++;

    return container->occupied_space - 1;
}

/* Constraints: line has minimal length of 3. [For ex. "(a)" ] */
bool check_if_can_simplify(const char *line, size_t length) {
    char letter = line[1];
    int ind = 2;

    for (size_t i = 2; i < length && line[i] != ')'; i++) {
        if (line[ind] == '(' || line[ind] == letter)
            return false;
        ind++;
    }
    return true;
}

size_t find_length_of_loop(const char *loop_str, size_t max_length) {
    int open_brackets_count = 0;
    int close_brackets_count = 0;

    size_t i = 0;
    while (i < max_length) {
        if (loop_str[i] == '(') {
            open_brackets_count++;
        } else if (loop_str[i] == ')') {
            close_brackets_count++;
        }
        if (open_brackets_count == close_brackets_count) {
            return i + 1;
        }
        i++;
    }
    return 0;
}

/* Constraints: line has minimal length of 3. [For ex. "(a)" ] */
size_t parse_simplified_loop(InstrContainer *container, const char *line, size_t length) {

    size_t ind = 2;
    int arg1 = (int) line[1];
    size_t instr_counter = 0;

    while (ind < length && line[ind] != ')') {
        add_to_container(container, ADD, line[ind], arg1);
        instr_counter++;
        ind++;
    }

    add_to_container(container, CLR, arg1, 0);
    instr_counter++;
    ind++;

    return instr_counter;
}

size_t parse_loop(InstrContainer *container, char *line, size_t length) {
    if (length < 3) {
        return 0;
    }

    if (check_if_can_simplify(line, length)) {
        return parse_simplified_loop(container, line, length);
    }

    size_t instructions_counter = 0;
    int arg1 = (int) line[1];

    size_t djz_address = add_to_container(container, DJZ, arg1, 0);
    instructions_counter++;

    size_t ind = 2;
    while (ind < length && line[ind] != ')') {
        if (line[ind] == '(') {

            size_t loop_length = find_length_of_loop(line + ind, length - ind);
            size_t loop_instr_counter = parse_loop(container, line + ind, loop_length);
            instructions_counter += loop_instr_counter;
            ind += loop_length;

        } else if (is_alphabetic(line[ind])) {

            add_to_container(container, INC, line[ind], 0);
            instructions_counter++;
            ind++;

        }
    }

    add_to_container(container, JMP, (int) djz_address, 0);
    instructions_counter++;

    set_djz_address(container, (int) djz_address, (int) (djz_address + instructions_counter));

    return instructions_counter;
}

void parse_line(InstrContainer *container, char *line, size_t length) {
    char first_letter = line[0];

    if (first_letter == '\n') {
        return;
    }

    if (first_letter == '=') {
        add_to_container(container, PRT, line[1], 0);
        return;
    }

    size_t ind = 0;
    while (ind < length) {
        char current_letter = line[ind];
        if (is_alphabetic(current_letter)) {

            add_to_container(container, INC, current_letter, 0);
            ind++;

        } else if (current_letter == '(') {

            size_t loop_length = find_length_of_loop(line + ind, length - ind);
            parse_loop(container, line + ind, loop_length);
            ind += loop_length;

        } else {
            ind++;
        }
    }
}

void parse_data(InstrContainer *container) {
    char *line = NULL;
    size_t len = 0;
    size_t length_read;

    while ((length_read = (size_t) getline(&line, &len, stdin)) != (size_t) -1) {
        parse_line(container, line, length_read);
    }

    free(line);

    add_to_container(container, HLT, 0, 0);
}


void execute_instructions(InstrContainer *container, SuperAlphabet *superAlphabet) {

    size_t ind = 0;
    bool program_end = false;
    bool should_increment_index = true;

    while (!program_end) {
        Instruction current_instr = container->instructions[ind];
        switch (current_instr.instrType) {

            case INC: {
                safe_inc(&superAlphabet->letters[current_instr.first_arg - 97]);
                break;
            }
            case ADD: {
                safe_add(&superAlphabet->letters[current_instr.first_arg - 97],
                         &superAlphabet->letters[current_instr.second_arg - 97]);
                break;
            }
            case CLR: {
                safe_clear(&superAlphabet->letters[current_instr.first_arg - 97]);
                break;
            }
            case JMP: {
                ind = (size_t) current_instr.first_arg;
                should_increment_index = false;
                break;
            }
            case DJZ: {
                if (!safe_decrement(&superAlphabet->letters[current_instr.first_arg - 97])) {
                    ind = (size_t) current_instr.second_arg;
                    should_increment_index = false;
                }
                break;
            }
            case HLT: {
                program_end = true;
                break;
            }
            case PRT: {
                safe_print(&superAlphabet->letters[current_instr.first_arg - 97]);
                break;
            }
        }

        if (should_increment_index) {
            ind++;
        }
        should_increment_index = true;
    }
}


int main(void) {

    InstrContainer *container = init_container();
    SuperAlphabet *alphabet = init_alphabet();

    parse_data(container);

    execute_instructions(container, alphabet);

    free_alphabet(alphabet);
    free_container(container);

    return 0;
}

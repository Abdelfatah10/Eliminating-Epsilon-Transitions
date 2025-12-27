// Djaballah Abdelfatah "G01 Security"
// Derarja Mohamed "G01 Security"
// Achouri Mohamed Tahar "G01 Security"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_STATES 50
#define MAX_SYMBOLS 26

typedef struct {
    int num_states;
    int num_symbols;
    char symbols[MAX_SYMBOLS];
    int initial_state;

    int num_final_states;
    int final_states[MAX_STATES];

    int transitions[MAX_STATES][MAX_SYMBOLS][MAX_STATES];  
    int transition_count[MAX_STATES][MAX_SYMBOLS];

    int epsilon_transitions[MAX_STATES][MAX_STATES]; 
    int epsilon_count[MAX_STATES];

    int epsilon_closure[MAX_STATES][MAX_STATES];      
    int closure_count[MAX_STATES];

} NFA;

void read_nfa(NFA *nfa);
void calculate_epsilon_closure(NFA *nfa);
void compute_new_transitions(NFA *nfa, int new_transitions[MAX_STATES][MAX_SYMBOLS][MAX_STATES], 
                             int new_transition_count[MAX_STATES][MAX_SYMBOLS]);
void determine_final_states(NFA *nfa, int *new_final_states, int *num_new_final);

bool contains_state(int *array, int count, int state) {
    for (int i = 0; i < count; i++) {
        if (array[i] == state) return true;
    }
    return false;
}


// Task 1: Read NFA with Epsilon Transitions
void read_nfa(NFA *nfa) {
    printf("\t 1/ Reading NFA \n");
    
    printf("Enter number of states: ");
    scanf("%d", &nfa->num_states);
    
    printf("Enter number of symbols (excluding epsilon): ");
    scanf("%d", &nfa->num_symbols);
    
    printf("Enter the symbols (space-separated): ");
    for (int i = 0; i < nfa->num_symbols; i++) {
        scanf(" %c", &nfa->symbols[i]);
    }
    
    printf("Enter initial state: ");
    scanf("%d", &nfa->initial_state);
    
    // Validate initial state
    if (nfa->initial_state < 0 || nfa->initial_state >= nfa->num_states) {
        printf("Invalid initial state! Must be between 0 and %d.\n", nfa->num_states - 1);
        nfa->initial_state = 0;
    }
    
    printf("Enter number of final states: ");
    scanf("%d", &nfa->num_final_states);
    
    printf("Enter final states (space-separated): ");
    for (int i = 0; i < nfa->num_final_states; i++) {
        scanf("%d", &nfa->final_states[i]);
        // Validate final state
        if (nfa->final_states[i] < 0 || nfa->final_states[i] >= nfa->num_states) {
            printf("Invalid final state! Must be between 0 and %d.\n", nfa->num_states - 1);
            i--;
        }
    }
    
    // Initialize transition counters to zero
    for (int i = 0; i < nfa->num_states; i++) {
        for (int j = 0; j < nfa->num_symbols; j++) {
            nfa->transition_count[i][j] = 0;
        }
        nfa->epsilon_count[i] = 0;
    }
    
    printf("\nEnter transitions (q_from symbol q_to) or (q_from e q_to) for epsilon.\n");
    printf("Enter -1 e -1 to stop:\n");
    
    int from, to;
    char symbol;
    
    while (1) {
        scanf("%d %c %d", &from, &symbol, &to);
        
        if (from == -1) break;
        
        // Validate state indices
        if (from < 0 || from >= nfa->num_states || to < 0 || to >= nfa->num_states) {
            printf("Invalid state index! States must be between 0 and %d.\n", nfa->num_states - 1);
            continue;
        }
        
        if (symbol == 'e') {
            // Add to epsilon transitions
            if (nfa->epsilon_count[from] < MAX_STATES) {
                nfa->epsilon_transitions[from][nfa->epsilon_count[from]++] = to;
            }
        } else {
            // Add to regular transitions - find symbol index
            int found = 0;
            for (int i = 0; i < nfa->num_symbols; i++) {
                if (nfa->symbols[i] == symbol) {
                    if (nfa->transition_count[from][i] < MAX_STATES) {
                        nfa->transitions[from][i][nfa->transition_count[from][i]++] = to;
                    }
                    found = 1;
                    break;
                }
            }
            if (!found) {
                printf("Symbol '%c' not found in symbol list!\n", symbol);
            }
        }
    }
    
    printf("NFA read successfully!\n\n");
}

// Task 2: Calculate Epsilon Closure
void calculate_epsilon_closure(NFA *nfa) {
    printf("\t 2/ Calculating Epsilon Closure \n");
    
    for (int state = 0; state < nfa->num_states; state++) {
        int closure[MAX_STATES];
        int closure_count = 1;
        closure[0] = state;
        
        int changed = 1;
        while (changed) {
            changed = 0;
            int new_count = closure_count;
            
            for (int i = 0; i < closure_count; i++) {
                int current = closure[i];
                for (int j = 0; j < nfa->epsilon_count[current]; j++) {
                    int next = nfa->epsilon_transitions[current][j];
                    if (!contains_state(closure, new_count, next)) {
                        closure[new_count++] = next;
                        changed = 1;
                    }
                }
            }
            closure_count = new_count;
        }
        
        nfa->closure_count[state] = closure_count;
        memcpy(nfa->epsilon_closure[state], closure, (size_t)closure_count * sizeof(int));
        
        printf("ε-closure(%d) = { ", state);
        for (int i = 0; i < closure_count; i++) {
            printf("%d ", closure[i]);
        }
        printf("}\n");
    }
    
    printf("\n");
}

// Task 3: Compute New Transitions without Epsilon
void compute_new_transitions(NFA *nfa, int new_transitions[MAX_STATES][MAX_SYMBOLS][MAX_STATES], 
                             int new_transition_count[MAX_STATES][MAX_SYMBOLS]) {
    printf("\t 3/ Computing New Transitions (without epsilon) \n");
    
    // Initialize new transition counters
    for (int i = 0; i < nfa->num_states; i++) {
        for (int j = 0; j < nfa->num_symbols; j++) {
            new_transition_count[i][j] = 0;
        }
    }
    
    for (int state = 0; state < nfa->num_states; state++) {
        for (int sym_idx = 0; sym_idx < nfa->num_symbols; sym_idx++) {
            int next_states[MAX_STATES];
            int next_count = 0;
            
            for (int i = 0; i < nfa->closure_count[state]; i++) {
                int closure_state = nfa->epsilon_closure[state][i];
                for (int j = 0; j < nfa->transition_count[closure_state][sym_idx]; j++) {
                    int dest = nfa->transitions[closure_state][sym_idx][j];
                    if (!contains_state(next_states, next_count, dest)) {
                        next_states[next_count++] = dest;
                    }
                }
            }
            
            for (int i = 0; i < next_count; i++) {
                int next = next_states[i];
                for (int j = 0; j < nfa->closure_count[next]; j++) {
                    int closure_dest = nfa->epsilon_closure[next][j];
                    if (!contains_state(new_transitions[state][sym_idx], new_transition_count[state][sym_idx], closure_dest)) {
                        if (new_transition_count[state][sym_idx] < MAX_STATES) {
                            new_transitions[state][sym_idx][new_transition_count[state][sym_idx]++] = closure_dest;
                        }
                    }
                }
            }
        }
    }
    
    printf("New transitions computed successfully!\n\n");
}

// Task 4: Determine New Final States
void determine_final_states(NFA *nfa, int *new_final_states, int *num_new_final) {
    printf("\t 4/ Determining New Final States \n");
    
    *num_new_final = 0;
    
    for (int state = 0; state < nfa->num_states; state++) {
        for (int i = 0; i < nfa->closure_count[state]; i++) {
            if (contains_state(nfa->final_states, nfa->num_final_states, nfa->epsilon_closure[state][i])) {
                new_final_states[(*num_new_final)++] = state;
                break;
            }
        }
    }
    
    printf("New final states: { ");
    for (int i = 0; i < *num_new_final; i++) {
        printf("%d ", new_final_states[i]);
    }
    printf("}\n\n");
}

int main() {
    NFA nfa;
    int new_transitions[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
    int new_transition_count[MAX_STATES][MAX_SYMBOLS];
    int new_final_states[MAX_STATES];
    int num_new_final;

    printf("\t\t\t\t  Welcome to the NFA Epsilon Transition Elimination Program  \n");

    
    read_nfa(&nfa);
    calculate_epsilon_closure(&nfa);
    compute_new_transitions(&nfa, new_transitions, new_transition_count);
    determine_final_states(&nfa, new_final_states, &num_new_final);
    
    return 0;
}
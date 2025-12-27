// Djaballah Abdelfatah "G01 Security"
// Derarja Mohamed "G01 Security"
// Achouri Mohamed Tahar "G01 Security"

#include <stdio.h>

#define MAX_STATES 100
#define MAX_SYMBOLS 26
#define MAX_TRANSITIONS 1000

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
    
    printf("Enter number of final states: ");
    scanf("%d", &nfa->num_final_states);
    
    printf("Enter final states (space-separated): ");
    for (int i = 0; i < nfa->num_final_states; i++) {
        scanf("%d", &nfa->final_states[i]);
    }
    
    // Initialize transition counters to zero
    for (int i = 0; i < nfa->num_states; i++) {
        for (int j = 0; j < nfa->num_symbols; j++) {
            nfa->transition_count[i][j] = 0;
        }
        nfa->epsilon_count[i] = 0;
    }
    
    printf("\nEnter transitions (q_from symbol q_to) or (q_from e q_to) for epsilon.\n");
    printf("Enter -1 -1 -1 to stop:\n");
    
    int from, to;
    char symbol;
    
    while (1) {
        scanf("%d %c %d", &from, &symbol, &to);
        
        if (from == -1) break;
        
        if (symbol == 'e') {
            // Add to epsilon transitions
            if (nfa->epsilon_count[from] < MAX_STATES) {
                nfa->epsilon_transitions[from][nfa->epsilon_count[from]++] = to;
            }
        } else {
            // Add to regular transitions
            for (int i = 0; i < nfa->num_symbols; i++) {
                if (nfa->symbols[i] == symbol) {
                    if (nfa->transition_count[from][i] < MAX_STATES) {
                        nfa->transitions[from][i][nfa->transition_count[from][i]++] = to;
                    }
                    break;
                }
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
        memcpy(nfa->epsilon_closure[state], closure, closure_count * sizeof(int));
        
        printf("ε-closure(%d) = { ", state);
        for (int i = 0; i < closure_count; i++) {
            printf("%d ", closure[i]);
        }
        printf("}\n");
    }
    
    printf("\n");
}

int main() {
    NFA nfa;
    printf("\t\t\t\t  Welcome to the NFA Epsilon Transition Elimination Program  \n");
    read_nfa(&nfa);
    return 0;
}

enum{
    NOT_MACRO,
    MACRO_READ,
    MACRO_CALL
};
/* Structure to represent a macro node in the linked list */
struct MacroNode {
    char name[MAX_LABEL_LENGTH]; /* Name of the macro */
    int offset; /* offset of the macro definition in the file (from the beginning of the file) */
    int line_number; /* Line number where the macro is defined */
    struct MacroNode *next;
};

/* Structure to represent the head of the macro linked list */
typedef struct MacroNode *macro_node;

struct head_mcr{
    macro_node head_mac;
};
typedef struct head_mcr head;


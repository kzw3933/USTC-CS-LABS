import ply.lex as lex

# List of token names.   This is always required
tokens = (
    'NAME',
    'AND',
    'OR',
    'NOT',
    'LPAREN',
    'RPAREN',
)

# Regular expression rules for simple tokens

def t_LPAREN(t):
    r'\('
    return t

def t_RPAREN(t):
    r'\)'
    return t

def t_NOT(t):
    r'NOT'
    return t

def t_OR(t):
    r'OR'
    return t

def t_AND(t):
    r'AND'
    return t

def t_NAME(t):
    r'[^ \n\t()]+'
    return t


# A string containing ignored characters (spaces and tabs)
t_ignore = ' \t'


# Error handling rule
def t_error(t):
    print("Illegal character '%s'" % t.value[0])
    t.lexer.skip(1)


# Build the lexer
lexer = lex.lex()
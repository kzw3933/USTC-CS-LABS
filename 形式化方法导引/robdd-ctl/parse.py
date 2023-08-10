import ply.lex as lex
import ply.yacc as yacc

# Define the tokens
tokens = (
    'TRUE', 'FALSE', 'VAR', 
    'NOT', 'AND', 'OR',
    'IMPLIES', 'IFF', 
    'A', 'E', 'U',
    'AX', 'EX', 'AF', 'EF', 'AG', 'EG',
    'LPAREN', 'RPAREN',
    'LBRACKET', 'RBRACKET'
)

# Define the token regular expressions
t_TRUE = r'True'
t_FALSE = r'False'
t_AF = r'AF'
t_AG = r'AG'
t_AX = r'AX'
t_EF = r'EF'
t_EG = r'EG'
t_EX = r'EX'
t_A = r'A'
t_E = r'E'
t_U = r'U'
t_VAR = r'[a-z][a-z0-9]*'
t_NOT = r'~'
t_AND = r'\&'
t_OR = r'\|'
t_IMPLIES = r'->'
t_IFF = r'<->'
t_LPAREN = r'\('
t_RPAREN = r'\)'
t_LBRACKET = r'\['
t_RBRACKET = r'\]'

# Define ignored characters (spaces and tabs)
t_ignore = ' \t\n'

# Define error handling rule
def t_error(t):
    print("Illegalinput character '%s'" % t.value[0])
    t.lexer.skip(1)

# Define the parsing rules
precedence = (
    ('left', 'IFF'),
    ('left', 'IMPLIES'),
    ('right', 'AX', 'AF', 'AG', 'EX', 'EF', 'EG'),
    ('left', 'OR'),
    ('left', 'AND'),
    ('right', 'NOT'),
)

def p_expression_true(p):
    'expression : TRUE'
    p[0] = ('TRUE',)

def p_expression_false(p):
    'expression : FALSE'
    p[0] = ('FALSE',)

def p_expression_var(p):
    'expression : VAR'
    p[0] = ('VAR', p[1])

def p_expression_not(p):
    'expression : NOT expression'
    p[0] = ('NOT', p[2])

def p_expression_and(p):
    'expression : expression AND expression'
    p[0] = ('AND', p[1], p[3])

def p_expression_or(p):
    'expression : expression OR expression'
    p[0] = ('OR', p[1], p[3])

def p_expression_implies(p):
    'expression : expression IMPLIES expression'
    p[0] = ('IMPLIES', p[1], p[3])
    
def p_expression_iff(p):
    'expression : expression IFF expression'
    p[0] = ('IFF', p[1], p[3])

def p_expression_ax(p):
    'expression : AX expression'
    p[0] = ('AX', p[2])

def p_expression_ex(p):
    'expression : EX expression'
    p[0] = ('EX', p[2])

def p_expression_af(p):
    'expression : AF expression'
    p[0] = ('AF', p[2])

def p_expression_ef(p):
    'expression : EF expression'
    p[0] = ('EF', p[2])

def p_expression_ag(p):
    'expression : AG expression'
    p[0] = ('AG', p[2])

def p_expression_eg(p):
    'expression : EG expression'
    p[0] = ('EG', p[2])

def p_expression_au(p):
    'expression : A LBRACKET expression U expression RBRACKET'
    p[0] = ('AU', p[3], p[5])

def p_expression_eu(p):
    'expression : E LBRACKET expression U expression RBRACKET'
    p[0] = ('EU', p[3], p[5])

def p_expression_paren(p):
    'expression : LPAREN expression RPAREN'
    p[0] = p[2]

def p_error(p):
    if p:
        raise SyntaxError("Syntax error at token '%s'" % p.value)
    else:
        raise SyntaxError("Syntax error at EOF")

# Build the lexer and parser
lexer = lex.lex()
parser = yacc.yacc()
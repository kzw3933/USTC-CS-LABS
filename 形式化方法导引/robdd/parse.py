import ply.lex as lex
import ply.yacc as yacc

# 定义词法分析器
tokens = ('VARIABLE', 'CONSTANT', 'LPAREN', 'RPAREN', 'NOT', 'AND', 'OR', 'IMPLIES', 'IFF')
t_VARIABLE = r'[a-zA-Z_][a-zA-Z0-9_]*'
t_CONSTANT = r'[01]'
t_LPAREN = r'\('
t_RPAREN = r'\)'
t_NOT = r'~'
t_AND = r'\&'
t_OR = r'\|'
t_IMPLIES = r'->'
t_IFF = r'<->'
t_ignore = ' \t\n'

def t_error(t):
    print("Illegal character '%s'" % t.value[0])
    t.lexer.skip(1)

# 定义逻辑符号的优先级和结合性
precedence = (
    ('left', 'IFF'),
    ('left', 'IMPLIES'),
    ('left', 'OR'),
    ('left', 'AND'),
    ('right', 'NOT'),
)

# 定义语法分析器
def p_expression_variable(p):
    'expression : VARIABLE'
    p[0] = ('VARIABLE', p[1], ('CONSTANT', 1), ('CONSTANT', 0))

def p_expression_constant(p):
    'expression : CONSTANT'
    p[0] = ('CONSTANT', p[1])

def p_expression_paren(p):
    'expression : LPAREN expression RPAREN'
    p[0] = p[2]

def p_expression_not(p):
    'expression : NOT expression'
    p[0] = ('IMPLIES', p[2], ('CONSTANT', 0))
    
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

def p_error(p):
    print("Syntax error in input!")

# 构建词法分析器和语法分析器
lexer = lex.lex()
parser = yacc.yacc()

# --------------------------------------------------------------------------------------------------------
from IR.Search.lex import tokens
import ply.yacc as yacc
import numpy as np
from IR.Corpus.corpus import Corpus


corpus = Corpus(ctype="movie", preload=True)

def andMerge(andList):
    notItems = []
    andItems = []

    for item in andList :
        if item[1] == 0:
            notItems.append(item)
        else:
            andItems.append(item)


    if len(andItems) == 0:
        return [{},1]
    try:
        andItems.sort(key=lambda x: len(x[0]))
    except KeyError:
        print("\033[31m" + "input keywords can't found in system" + "\033[0m")

    andRes = [{},1]
    andRes[0] = andItems[0][0]
    andRes[1] = andItems[0][1]
    for notItem in notItems:
        for item in notItem[0]:
            if andRes[0].__contains__(item):
                andRes[0].pop(item)

    # 合并
    first = 0

    for andItem in andItems:
        if first == 0:
            first = 1
            continue
        else:
            tmp = [{}, 1]
            i,j = 0,0
            list1 = list(andRes[0])
            list2 = list(andItem[0])
            len1 = len(list1)
            len2 = len(list2)
            interval1 = int(np.sqrt(len1)) # list1的跳表指针间隔---根号下表长
            interval2 = int(np.sqrt(len2)) # list2的跳表指针间隔

            while (i < len1) & (j < len2):
                if list1[i] == list2[j]:
                    tmp[0][list1[i]] = andRes[0][list1[i]] + andItem[0][list1[i]]  # 遇见相同元素合并，且词频相加
                    print('+')
                    i = i + 1
                    j = j + 1
                elif list1[i] < list2[j]:
                    if (i % interval1 == 0) and (i+interval1 < len1) and (list1[i+interval1] <= list2[j]):# i+interval1为跳表指针指向位置
                        while (i % interval1 == 0) and (i+interval1 < len1) and (list1[i+interval1] <= list2[j]):
                            i = i + interval1
                    else:
                        i = i + 1
                else: # list1[i] > list2[j]
                    if (j % interval2 == 0) and (j+interval2 < len2) and (list2[j+interval2] <= list2[j]):# i+interval1为跳表指针指向位置
                        while (j % interval2 == 0) and (j+interval2 < len2) and (list2[j+interval2] <= list2[j]):
                            j = j + interval2
                    else:
                        j = j + 1

            andRes = tmp
            print(andRes)

    return andRes


def orMerge(orList):
    try:
        orList.sort(key=lambda x: len(x[0]))
    except KeyError:
        print("\033[31m" + "input keywords can't found in system" + "\033[0m")

    first = 0
    orRes = [{},1]
    for orItem in orList:
        if orItem[1] == 0:
            print('error')
            return [{},1]

        if first == 0:
            first = 1
            orRes = orItem
        else:
            for item in orItem[0]:
                try:
                    num = orRes[0][item]
                except:
                    num = 0
                orRes[0][item] = num + orItem[0][item]  # 词频相加

    return orRes


def p_expr(p):
    'EXPR : ORLIST'
    p[0] = orMerge(p[1])
    # print(p[0])

def p_orList(p):
    '''
     ORLIST : ORLIST OR ANDLIST
        | ANDLIST
    '''
    if len(p) == 2:
        p[0] = []
        p[0].append(andMerge(p[1]))
    else:
        p[0] = p[1]
        p[0].append(andMerge(p[3]))

def p_andList(p):
    '''
     ANDLIST : ANDLIST AND LIST
        | LIST
    '''
    if len(p) == 2:
        p[0] = []
        p[0].append(p[1])
    else:
        p[0] = p[1]
        p[0].append(p[3])

def p_list(p):
    '''
     LIST : LPAREN ORLIST RPAREN
        | NOT LPAREN ORLIST RPAREN
        | NAME
        | NOT NAME
    '''
    if len(p) == 2:
        ReverseTable = {}
        ii = dict(sorted(corpus.invert_indice[corpus.dictionary[p[1]][0]].items(), key=lambda d: d[0], reverse=False))

        for item in ii:
            ReverseTable[item] = corpus.invert_indice[corpus.dictionary[p[1]][0]][item]
        p[0] = [ReverseTable,1]

    elif len(p) == 3:
        ReverseTable = {}
        ii = dict(sorted(corpus.invert_indice[corpus.dictionary[p[2]][0]].items(), key=lambda d: d[0], reverse=False))

        for item in ii:
            ReverseTable[item] = corpus.invert_indice[corpus.dictionary[p[2]][0]][item]
        p[0] = [ReverseTable,0]
    elif len(p) == 4:
        p[0] = orMerge(p[2])
    else :
        p[0] = orMerge(p[3])
        p[0][1] = 0


    # Error rule for syntax errors
def p_error(p):
    print("Syntax error in input!")



    # Build the parser
parser = yacc.yacc()

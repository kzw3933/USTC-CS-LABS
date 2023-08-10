


if __name__ == '__main__':
    with open('tmp.md') as f:
        lines = [line for line in f.readlines()]
        lines = lines[2:]
        (precision5E,precision10E,recall5E, recall10E, ndcg5E, ndcg10E) = ([], [], [], [], [], [])
        (precision5R,precision10R,recall5R, recall10R, ndcg5R, ndcg10R) = ([], [], [], [], [], [])
        i = 0
        for line in lines:
            i += 1    
            line = [i for i in line.strip().split('|') if i][2:] 
            if i%2 == 0:
                precision5R.append(float(line[0].strip()))
                precision10R.append(float(line[3].strip()))
                recall5R.append(float(line[1].strip()))
                recall10R.append(float(line[4].strip()))
                ndcg5R.append(float(line[2].strip()))
                ndcg10R.append(float(line[5].strip()))
            else:              
                precision5E.append(float(line[0].strip()))
                precision10E.append(float(line[3].strip()))
                recall5E.append(float(line[1].strip()))
                recall10E.append(float(line[4].strip()))
                ndcg5E.append(float(line[2].strip()))
                ndcg10E.append(float(line[5].strip()))
        with open('tmp.txt', 'w') as f:
            f.write('precision5R = '+str(precision5R)+'\n')
            f.write('precision10R = '+str(precision10R)+'\n')
            f.write('recall5R = '+str(recall5R)+'\n')
            f.write('recall10R = '+str(recall10R)+'\n')
            f.write('ndcg5R = '+str(ndcg5R)+'\n')
            f.write('ndcg10R = '+str(ndcg10R)+'\n')
            f.write('precision5E = '+str(precision5E)+'\n')
            f.write('precision10E = '+str(precision10E)+'\n')
            f.write('recall5E = '+str(recall5E)+'\n')
            f.write('recall10E = '+str(recall10E)+'\n')
            f.write('ndcg5E = '+str(ndcg5E)+'\n')
            f.write('ndcg10E = '+str(ndcg10E)+'\n')
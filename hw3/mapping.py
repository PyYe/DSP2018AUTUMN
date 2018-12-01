import sys
import codecs

input_file = sys.argv[1]
output_file = sys.argv[2]

dict_Z_B = dict()
with codecs.open(input_file, 'r', encoding='big5-hkscs', errors='ignore') as file:
    for line in file:
        #print (line.split(' '))
        #break
        #line.split(' ')[0]
        for i in line.split(' ')[1].split('/'):
            if dict_Z_B.get(i[0]):
                if (line.split(' ')[0] in dict_Z_B[i[0]])==False: 
                    dict_Z_B[i[0]].append(line.split(' ')[0])
            else:
                dict_Z_B[i[0]] = [line.split(' ')[0]]
        dict_Z_B[line.split(' ')[0]] = [line.split(' ')[0]]

with codecs.open(output_file, 'w', encoding='big5-hkscs', errors='ignore') as file:
    for key in dict_Z_B:
        file.write(key+' ')
        for b in dict_Z_B[key]:
            file.write(b+' ')
        file.write('\n')


import sys
import csv


# Stores the values of STR from dna sequence into a dictionary
dnaDict = {}


def main():
    # Checks proper usage and stores the cmd line arguments
    length = len(sys.argv)
    if length != 3:
        print("Usage: python dna.py data.csv sequence.txt")
    database = sys.argv[1]
    sequence = sys.argv[2]
    
    # Opens dna database and creates a reader
    d = open(database)
    data_reader = csv.DictReader(d)
    
    # Stores the STR names
    headers = data_reader.fieldnames 

    person = ''
    
    # Opens dna sequence and adds STR values to a list
    with open(sequence) as s:
        read_seq = s.read()
        reader_seq = csv.reader(read_seq)
        for STR in headers:
            if STR != 'name':
                dnaDict[STR] = str(counter(read_seq, STR))
                
        # Prints matching name if both lists are the same      
        for row in data_reader:
            name = row['name'] 
            dnaDict['name'] = name
            if row == dnaDict:
                person = row['name']
                print(person)
                break
            
    # Prints if no dna match is found
    if person == '':
        print("No match")
        
        
# Computes the highest repeating STR chain
def counter(dna_seq, STR):
    count = 0
    tmp = 0
    loopcount = len(dna_seq) - 1
    i = 0
    
    while i < loopcount:
        substring = dna_seq[i:i+len(STR)]
        if substring == STR:
            tmp += 1
            i += len(STR)
        else:
            i += 1
            if tmp > count:
                count = tmp
            tmp = 0
    if tmp > count:
        count = tmp
    return count


main()
from cs50 import get_float

def main():
    # Prompt user for change
    while True:
        change = get_float("Change owed: ")
        if change > 0:
            break
    
    change = change * 100  # Convert change to int to avoid float imprecision
    counter = 0  # Counts number of coins
    
    # Checks quarters
    while change >= 25:
        counter += check(25, change)
    
    # Checks dimes
    while change >= 10:
        counter += check(10, change)
    
    # Checks nickles
    while change >= 5:
        counter += check(5, change)
    
    # Checks pennies
    while change >= 1:
        counter += check(1, change)
    
    # Prints coins used
    print(counter)

def check(val):
    change -= val
    return 1

main()
    
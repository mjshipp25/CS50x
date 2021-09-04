from cs50 import get_float


# Prompt user for change
while True:
    change = get_float("Change owed: ")
    if change > 0:
        break
    
change = change * 100  # Convert change to int to avoid float imprecision
counter = 0  # Counts number of coins 

# Checks quarters
while change >= 25:
    counter += 1
    change -= 25

# Checks dimes
while change >= 10:
    counter += 1
    change -= 10

# Checks nickles
while change >= 5:
    counter += 1
    change -= 5

# Checks pennies
while change >= 1:
    counter += 1
    change -= 1

# Prints coins used
print(counter)
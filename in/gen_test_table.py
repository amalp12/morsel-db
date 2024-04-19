import random
import string
import os

def generate_random_name(length=6):
    """Generate a random name of given length."""
    return ''.join(random.choices(string.ascii_letters, k=length))

def generate_random_age():
    """Generate a random age between 1 and 100."""
    return random.randint(1, 100)

def generate_data(num_entries):
    """Generate data with ID, name, and age."""
    data = []
    for i in range(num_entries):
        name = generate_random_name()
        age = generate_random_age()
        data.append((i+1, name, age))
    return data

def save_data_to_file(data, filename):
    """Save data to a file."""
    with open(filename, 'w') as f:
        for entry in data:
            f.write(';'.join(map(str, entry)) + '\n')

if __name__ == "__main__":
    num_entries = int(os.environ['TEST_TABLE_SIZE'])
    print(f'Generating table with size of {num_entries}')
    data = generate_data(num_entries)
    save_data_to_file(data, 'test_table.csv')

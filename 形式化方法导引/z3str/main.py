import os
import subprocess

test_dir = 'tests'  
output_file = 'output.txt'

file = open(output_file, "w")
test_files = sorted(os.listdir(test_dir))

for filename in test_files:
    input_path = os.path.join(test_dir, filename)
    command = '{} {}'.format('/usr/bin/python3', input_path)
    prefix = ">>>>>>>>>>>>>>>>TEST FOR "+filename.strip('.py')+" BEGIN"+"\n"
    res = subprocess.run(command, shell=True, capture_output=True, text=True)
    suffix = ">>>>>>>>>>>>>>>>TEST FOR "+filename.strip('.py')+" END"+"\n\n\n"
    file.write(prefix)
    file.write(res.stdout)
    file.write(suffix)
    
file.close()
import os
import subprocess
import sys

def process_image(image_path, output_path ,output_name):
    command = f'python img2c.py -i {image_path} -o {output_path} --name {output_name}'

    try:
        result = subprocess.run(command, shell=True, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        print(result.stdout)
        if result.stderr:
            print(f"Error: {result.stderr}")
    except subprocess.CalledProcessError as e:
        print(f"Error occurred while processing {image_path}: {e.output}")

def main(argv):
    if not argv:
        current_dir = os.path.dirname(os.path.abspath(__file__))
    else:
        current_dir = argv[0]
    if current_dir[-1] == '\\' or current_dir[-1] == '/':
        current_dir = current_dir[:-1]
    print(current_dir)
    print('\nfile list:')
    for filename in os.listdir(current_dir):
        if filename.lower().endswith(('.png', '.jpg', '.jpeg', '.gif', '.bmp')):
            base_name, file_extension = os.path.splitext(filename)
            image_path = os.path.join(current_dir, filename)
            new_filename = f"{base_name}.c"
            output_path = os.path.join(current_dir, new_filename)
            output_name = f'_{filename.replace(".", "_")}_'
            print('    ',filename,' -> ',output_name)
            process_image(image_path, output_path, output_name)
    print('completed\n')

if __name__ == "__main__":
    main(sys.argv[1:])
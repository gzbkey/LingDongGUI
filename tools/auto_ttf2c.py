import os
import yaml
import subprocess
from pathlib import Path
import sys

def read_font_config(file_path):
    if not os.path.exists(file_path):
        create_default_font_config(file_path)
    with open(file_path, 'r') as file:
        config = yaml.safe_load(file)
    return config

def create_default_font_config(file_path):
    default_config = """arial.ttf:
  - pixelSize: 9
    fontBitWidth: 8
    string: '012345678'
  - pixelSize: 12
    string: '012345678'"""
    with open(file_path, 'w') as file:
        file.write(default_config)

def generate_font_data_for_each_config(font_config, output_dir):
    print('')
    for font_name, configs in font_config.items():
        ttf_path = f"{output_dir}/{font_name}"
        if os.name == 'nt':  # Windows
            ttf_path = f"C:/Windows/Fonts/{font_name}"
        print('['+font_name+']\n    '+ttf_path+'\n')
        for config in configs:
            pixel_size = config['pixelSize']
            font_bit_width = config.get('fontBitWidth', None)
            base_font_name = Path(font_name).stem
            output_c = f"{output_dir}/{base_font_name}_{pixel_size}.c"
            print('    pixelSize:',pixel_size)

            font_txt_path=f'{output_dir}/font.txt'
            if font_bit_width is not None:
                print('    fontBitWidth:',font_bit_width)
            print('')
            with open(font_txt_path, 'w') as file:
                pass

            with open(font_txt_path, 'a') as file:
                file.write(config['string'] + '\n')
            command_args = [
               'python', f"{output_dir}/ttf2c.py", '-i', ttf_path, '-t', font_txt_path, '-o', output_c,
               '-p', str(pixel_size), '-n', f'{base_font_name}_{pixel_size}'
            ]
            if font_bit_width is not None:
               command_args.extend(['-s', str(font_bit_width)])
            subprocess.run(command_args)

def main(argv):
    if not argv:
        output_dir = os.path.dirname(os.path.abspath(__file__))
    else:
        output_dir = argv[0]
    if output_dir[-1] == '\\' or output_dir[-1] == '/':
        output_dir = output_dir[:-1]

    font_config = read_font_config(f'{output_dir}/font.yml',)

    font_txt_path=f"{output_dir}/font.txt"
    if not os.path.exists(font_txt_path):
        with open(font_txt_path, 'w') as file:
            pass

    generate_font_data_for_each_config(font_config,output_dir)

if __name__ == "__main__":
    main(sys.argv[1:])
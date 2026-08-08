import os
import yaml
import subprocess
import argparse
from pathlib import Path
import sys

def read_font_yaml(file_path):
    if not os.path.exists(file_path):
        create_default_font_config(file_path)
    with open(file_path, 'r', encoding='utf-8') as file:
        yamlData = yaml.safe_load(file)
    return yamlData

def create_default_font_config(file_path):
    default_config = """- font:
    family: Arial
    size: 12
    bold: false
    italic: false
    underline: false
    strikeOut: false
    path: arial.ttf
    codeName: Arial
    bitWidth: 8
    text: " !\\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\\\]^_`abcdefghijklmnopqrstuvwxyz{|}~±"
    index: 0
- font:
    family: Arial
    size: 16
    bold: false
    italic: false
    underline: false
    strikeOut: false
    path: arial.ttf
    codeName: Arial
    text: '012345678'
    index: 0
"""
    with open(file_path, 'w', encoding='utf-8') as file:
        file.write(default_config)

header_content_start = f"""#ifndef __UI_FONTS_H__
#define __UI_FONTS_H__

#ifdef __cplusplus
extern "C" {{
#endif

#include "arm_2d_helper.h"
"""

header_content_end = f"""
#ifdef __cplusplus
}}
#endif

#endif
"""

def generate_font_data_for_each_config(fontYaml, output_dir):
    print('')
    header_content=""
    generated_bin_files = []
    bin_info_list = []
    current_xip_offset = 0
    for config in fontYaml:
        font_config = config.get('font', {})
        font_index = font_config.get('index', 0)
        ttf_path = font_config.get('path', 'Default.ttf')
        pixel_size = font_config.get('size', 12)
        font_bit_width = font_config.get('bitWidth', None)
        text = font_config.get('text', " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~±")
        text = str(text)
        base_font_name = font_config.get('codeName', 'Default')
        bold = font_config.get('bold', False)
        italic = font_config.get('italic', False)
        underline = font_config.get('underline', False)
        strikeOut = font_config.get('strikeOut', False)

        font_name = ttf_path

        if not os.path.isfile(ttf_path):
            ttf_path = f"{output_dir}/{font_name}"
            if not os.path.exists(ttf_path):
                if os.name == 'nt':  # Windows
                    ttf_path = f"C:/Windows/Fonts/{font_name}"
                    if not os.path.exists(ttf_path):
                        ttf_path = f"{Path.home()}/AppData/Local/Microsoft/Windows/Fonts/{font_name}"

        print('['+base_font_name+']\n    '+ttf_path)

        base_font_name = base_font_name.replace('-', '_')
        base_font_name = base_font_name.replace(' ', '_')
        base_font_name = base_font_name.replace('.', '_')

        if bold:
            base_font_name += '_Bold'
        if italic:
            base_font_name += '_Italic'
        if underline:
            base_font_name += '_Underline'
        if strikeOut:
            base_font_name += '_StrikeOut'
        output_c = f"{output_dir}/{base_font_name}_{pixel_size}.c"
        print('    size:',pixel_size)

        font_txt_path=f'{output_dir}/font.txt'
        if font_bit_width is not None:
            print('    bitWidth:',font_bit_width)
        print('')
        with open(font_txt_path, 'w', encoding='utf-8') as file:
            pass

        with open(font_txt_path, 'a', encoding='utf-8') as file:
            file.write(text + '\n')
        
        thisPyPath = os.path.dirname(os.path.abspath(__file__))
        command_args = [
            sys.executable, f"{thisPyPath}/ttfConvert.py", '-i', ttf_path, '-t', font_txt_path, '-o', output_c,
            '-p', str(pixel_size), '-n', f'{base_font_name}_{pixel_size}',
            '--xip-offset', f'0x{current_xip_offset:08X}'
        ]
        if font_bit_width is not None:
            command_args.extend(['-s', str(font_bit_width)])

        if ttf_path.lower().endswith('.ttc'):
            command_args.extend(['--index', str(font_index)])
        subprocess.run(command_args)

        bins_this_config = []
        if font_bit_width is not None:
            bin_file = f"{output_dir}/{base_font_name}_{pixel_size}_A{font_bit_width}.bin"
            if os.path.exists(bin_file):
                bins_this_config.append(bin_file)
                generated_bin_files.append(bin_file)
                bin_info_list.append({
                    'name': f"{base_font_name}_{pixel_size}_A{font_bit_width}",
                    'file': bin_file
                })
        else:
            for i in range(4):
                bin_file = f"{output_dir}/{base_font_name}_{pixel_size}_A{2**i}.bin"
                if os.path.exists(bin_file):
                    bins_this_config.append(bin_file)
                    generated_bin_files.append(bin_file)
                    bin_info_list.append({
                        'name': f"{base_font_name}_{pixel_size}_A{2**i}",
                        'file': bin_file
                    })

        for bin_file in bins_this_config:
            bin_size = os.path.getsize(bin_file)
            current_xip_offset += (bin_size + 3) & ~3

        if font_bit_width is not None:
            header_content += f"""
extern const
struct {{
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
}} ARM_2D_FONT_{base_font_name}_{str(pixel_size)}_A{font_bit_width};

#define FONT_{base_font_name.upper()}_{str(pixel_size)}          (arm_2d_font_t*)&ARM_2D_FONT_{base_font_name}_{str(pixel_size)}_A{font_bit_width}
"""
        else:
            for i in range(4):
                header_content += f"""
extern const
struct {{
    implement(arm_2d_user_font_t);
    arm_2d_char_idx_t tUTF8Table;
}} ARM_2D_FONT_{base_font_name}_{str(pixel_size)}_A{2**i};

#define FONT_{base_font_name.upper()}_{str(pixel_size)}_A{2**i}          (arm_2d_font_t*)&ARM_2D_FONT_{base_font_name}_{str(pixel_size)}_A{2**i}
"""

    

    with open(f"{output_dir}/uiFonts.h", 'w', encoding='utf-8') as file:
        file.write(header_content_start)
        file.flush()
        file.write(header_content)
        file.flush()
        file.write(header_content_end)
        file.flush()

    merge_bin_files(output_dir, bin_info_list)

def merge_bin_files(output_dir, bin_info_list):
    if not bin_info_list:
        return
        
    merged_bin_path = f"{output_dir}/fonts.bin"
    offset_info_path = f"{output_dir}/fonts.txt"
    
    current_offset = 0
    offset_info = []
    
    with open(merged_bin_path, 'wb') as merged_file:
        for bin_info in bin_info_list:
            bin_file = bin_info['file']
            if os.path.exists(bin_file):
                font_name_upper = bin_info['name'].upper()
                offset_info.append(f"{font_name_upper}:0x{current_offset:08X}")

                with open(bin_file, 'rb') as f:
                    data = f.read()
                    merged_file.write(data)

                    data_len = len(data)
                    aligned_size = (data_len + 3) & ~3
                    padding = aligned_size - data_len
                    if padding > 0:
                        merged_file.write(bytes([0] * padding))

                    current_offset += aligned_size

    with open(offset_info_path, 'w') as offset_file:
        for info in offset_info:
            offset_file.write(info + "\n")


def main(argv):
    parser = argparse.ArgumentParser(description='Auto TTF to C converter')
    parser.add_argument('output_dir', nargs='?', default=None, help='Output directory')
    args = parser.parse_args(argv)

    if args.output_dir is None:
        output_dir = os.path.dirname(os.path.abspath(__file__))
    else:
        output_dir = args.output_dir
    if output_dir[-1] in ('\\', '/'):
        output_dir = output_dir[:-1]

    fontYamlData = read_font_yaml(f'{output_dir}/font.yaml')

    if isinstance(fontYamlData, dict):
        fontYamlData = fontYamlData.get('fonts', [])
    elif not isinstance(fontYamlData, list):
        fontYamlData = []

    font_txt_path=f"{output_dir}/font.txt"
    if not os.path.exists(font_txt_path):
        with open(font_txt_path, 'w', encoding='utf-8') as file:
            pass

    generate_font_data_for_each_config(fontYamlData, output_dir)
    os.remove(font_txt_path)

if __name__ == "__main__":
    main(sys.argv[1:])

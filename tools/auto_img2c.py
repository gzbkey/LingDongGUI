import os
import yaml
import subprocess
import sys
from PIL import Image
import shutil
import argparse

def process_image(output_dir, image_path, output_path, output_name, depth, xip_offset=0):
    if depth == 8:
        format_arg = 'gray8'
    elif depth == 16:
        format_arg = 'rgb565'
    elif depth == 32:
        format_arg = 'rgb32'
    else:
        format_arg = 'all'

    command = [
        sys.executable,
        os.path.join(output_dir, 'imgConvert.py'),
        '-i', image_path,
        '-o', output_path,
        '--name', output_name,
        '--format', format_arg,
        '--xip-offset', f'0x{xip_offset:08X}'
    ]

    try:
        result = subprocess.run(
            command,
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        print(result.stdout)
        if result.stderr:
            print(f"Error: {result.stderr}")
    except subprocess.CalledProcessError as e:
        print(f"Error occurred while processing {image_path}: {e.output}")

header_content_start = """#ifndef __UI_IMAGES_H__
#define __UI_IMAGES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_2d.h"
#include "ldConfig.h"
"""

header_content_end = """
#ifdef __cplusplus
}
#endif

#endif
"""

def generate_image_data_for_each_config(imageYaml, output_dir, xip_offset_start=0):
    header_parts = []
    imagePathList = []
    imageDepthList = []
    copied_image_paths = set()
    for image_entry in imageYaml:
            image_info = image_entry["image"]
            image_path = os.path.abspath(image_info.get("path"))
            depth = image_info.get("depth", 0)
            imageDepthList.append(depth)
            dest_path = os.path.abspath(os.path.join(output_dir, os.path.basename(image_path)))
            imagePathList.append(dest_path)
            if dest_path != image_path:
                os.makedirs(os.path.dirname(dest_path), exist_ok=True)
                shutil.copyfile(image_path, dest_path)
                copied_image_paths.add(dest_path)

    print(output_dir)
    print('\nfile list:')

    thisPyDirPath = os.path.dirname(os.path.abspath(__file__))

    bin_files = []
    bin_offsets = {}
    current_offset = 0
    current_xip_offset = xip_offset_start

    for filename, depth in zip(imagePathList, imageDepthList):
        filename = os.path.basename(filename)
        if filename.lower().endswith(('.png', '.jpg', '.jpeg', '.gif', '.bmp')):
            base_name, _ = os.path.splitext(filename)
            image_path = os.path.join(output_dir, filename)
            output_name = f'_{filename.replace(".", "_")}_'
            text_file_name = filename.replace(".", "_")
            print('    ', filename, ' -> ', text_file_name)

            with Image.open(image_path) as img:
                imgW, imgH = img.size
                has_alpha = img.mode == 'RGBA'

            new_filename = f"{base_name}.c"
            output_path = os.path.join(output_dir, new_filename)
            process_image(thisPyDirPath, image_path, output_path, output_name, depth, current_xip_offset)

            bin_filename = f"{base_name}.bin"
            bin_filepath = os.path.join(output_dir, bin_filename)
            if os.path.exists(bin_filepath):
                bin_files.append((bin_filename, bin_filepath))
                bin_offsets[bin_filename] = current_offset
                file_size = os.path.getsize(bin_filepath)
                current_offset += file_size
                current_xip_offset += file_size

            header_parts.append(f"\n// {filename} < {imgW}x{imgH} >")

            header_parts.append(f"""
#if LD_CFG_COLOR_DEPTH == 8
extern const arm_2d_tile_t c_tile_{text_file_name}_GRAY8;
#define IMAGE_{text_file_name.upper()}          (arm_2d_tile_t*)&c_tile_{text_file_name}_GRAY8
#elif LD_CFG_COLOR_DEPTH == 16
extern const arm_2d_tile_t c_tile_{text_file_name}_RGB565;
#define IMAGE_{text_file_name.upper()}          (arm_2d_tile_t*)&c_tile_{text_file_name}_RGB565
#else""")

            if has_alpha:
                header_parts.append(f"""
extern const arm_2d_tile_t c_tile_{text_file_name}_CCCA8888;
#define IMAGE_{text_file_name.upper()}          (arm_2d_tile_t*)&c_tile_{text_file_name}_CCCA8888
#endif""")
            else:
                header_parts.append(f"""
extern const arm_2d_tile_t c_tile_{text_file_name}_CCCN888;
#define IMAGE_{text_file_name.upper()}          (arm_2d_tile_t*)&c_tile_{text_file_name}_CCCN888
#endif""")

            if depth == 0:
                header_parts.append(f"""
extern const arm_2d_tile_t c_tile_{text_file_name}_A1Mask;
#define IMAGE_{text_file_name.upper()}_A1_MASK   (arm_2d_tile_t*)&c_tile_{text_file_name}_A1Mask
extern const arm_2d_tile_t c_tile_{text_file_name}_A2Mask;
#define IMAGE_{text_file_name.upper()}_A2_MASK   (arm_2d_tile_t*)&c_tile_{text_file_name}_A2Mask
extern const arm_2d_tile_t c_tile_{text_file_name}_A4Mask;
#define IMAGE_{text_file_name.upper()}_A4_MASK   (arm_2d_tile_t*)&c_tile_{text_file_name}_A4Mask""")

            if has_alpha:
                header_parts.append(f"""
extern const arm_2d_tile_t c_tile_{text_file_name}_Mask;
#define IMAGE_{text_file_name.upper()}_MASK     (arm_2d_tile_t*)&c_tile_{text_file_name}_Mask""")
            else:
                header_parts.append(f"""
#define IMAGE_{text_file_name.upper()}_MASK     NULL""")
            header_parts.append("\n")

    for imagePath in imagePathList:
        if os.path.exists(imagePath) and imagePath in copied_image_paths:
            os.remove(imagePath)

    images_bin_path = os.path.join(output_dir, "images.bin")
    with open(images_bin_path, 'wb') as images_bin:
        for bin_filename, bin_filepath in bin_files:
            with open(bin_filepath, 'rb') as f:
                data = f.read()
                images_bin.write(data)

    images_txt_path = os.path.join(output_dir, "images.txt")
    combined_offsets = {}
    if os.path.exists(images_txt_path):
        with open(images_txt_path, 'r') as f:
            for line in f:
                if ':' in line:
                    name, offset_str = line.strip().split(':', 1)
                    combined_offsets[name] = int(offset_str, 16)

    for bin_filename, base_offset in bin_offsets.items():
        txt_filename = os.path.splitext(bin_filename)[0] + ".txt"
        txt_filepath = os.path.join(output_dir, txt_filename)
        if os.path.exists(txt_filepath):
            with open(txt_filepath, 'r') as f:
                for line in f:
                    if ':' in line:
                        name, offset_str = line.strip().split(':', 1)
                        combined_offsets[name] = base_offset + int(offset_str, 16)

    with open(images_txt_path, 'w') as f:
        for name, offset in sorted(combined_offsets.items(), key=lambda x: x[1]):
            if offset>=0xFFFFFFFF:
                f.write(f"#define IMAGE_{name}_MASK NULL\n")
            else:
                f.write(f"{name}:0x{offset:08x}\n")
    
    full_header = header_content_start + ''.join(header_parts) + header_content_end
    with open(f"{output_dir}/uiImages.h", 'w') as file:
        file.write(full_header)

def main(argv):
    parser = argparse.ArgumentParser(description='Auto image to C converter')
    parser.add_argument('output_dir', nargs='?', default=None, help='Output directory')
    parser.add_argument('--xip-offset-start', type=lambda x: int(x, 0), default=0, help='XIP offset start for merged images.bin')
    args = parser.parse_args(argv)

    if args.output_dir is None:
        output_dir = os.path.dirname(os.path.abspath(__file__))
    else:
        output_dir = args.output_dir

    if output_dir[-1] in ('\\', '/'):
        output_dir = output_dir[:-1]

    image_yaml_path = os.path.join(output_dir, "image.yaml")

    if not os.path.exists(image_yaml_path):
        print("image.yaml not found")
        image_entries = []
        for filename in os.listdir(output_dir):
            if filename.lower().endswith(('.png', '.jpg', '.jpeg', '.gif', '.bmp')):
                full_path = os.path.abspath(os.path.join(output_dir, filename))
                image_entries.append({"image": {"path": full_path, "depth": 16}})

        if image_entries:
            with open(image_yaml_path, 'w') as file:
                yaml.safe_dump(image_entries, file)

    with open(image_yaml_path, 'r') as file:
        imageYamlData = yaml.safe_load(file)
        generate_image_data_for_each_config(imageYamlData, output_dir, args.xip_offset_start)

if __name__ == "__main__":
    main(sys.argv[1:])

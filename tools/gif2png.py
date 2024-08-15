import os
import sys
from PIL import Image

def is_image_opaque(image):
    for x in range(image.width):
        for y in range(image.height):
            if image.getpixel((x, y))[3] != 255:
                return False
    return True

def frames_convert(current_dir, filename, skip_interval):
    base_name, file_extension = os.path.splitext(filename)
    image_path = os.path.join(current_dir, filename)
    save_name = ''
    
    img = Image.open(image_path)
    width, height = img.size
    frame_count = img.n_frames
    skip_count = 0
    for i in range(frame_count):
        if skip_interval != 0:
            if (i+1) % skip_interval == 0:
                skip_count=skip_count+1

    new_img = Image.new('RGBA', (width * (frame_count-skip_count), height))
    skip_temp = 0
    for i in range(frame_count):
        # 跳帧处理
        if skip_interval != 0:
            if (i+1) % skip_interval == 0:
                skip_temp=skip_temp+1
                continue
        img.seek(i)
        new_img.paste(img, ((i-skip_temp)*width, 0))
    if is_image_opaque(new_img):
        save_name=base_name + '_frames.bmp'
        output_path = os.path.join(current_dir, save_name)
        new_img.save(output_path, format='BMP')
    else:
        save_name=base_name + '_frames.png'
        output_path = os.path.join(current_dir, save_name)
        new_img.save(output_path, format='PNG')
    print('    ',filename,'->',save_name)
def main(argv):
    if not argv:
        current_dir = os.path.dirname(os.path.abspath(__file__))
    else:
        current_dir = argv[0]
    if current_dir[-1] == '\\' or current_dir[-1] == '/':
        current_dir = current_dir[:-1]

    frame_skip_interval = input('Please enter the frame skipping interval [2 - 100]:\n')
    if frame_skip_interval == '':
        frame_skip_interval = '0'
    
    print('')
    print('target:',current_dir)
    print('')
    header_content=""
    for filename in os.listdir(current_dir):
        if filename.lower().endswith(('.gif', '.apng', '.webp')):
            print('converting')
            frames_convert(current_dir, filename,int(frame_skip_interval))
    print('completed\n')

if __name__ == "__main__":
    main(sys.argv[1:])

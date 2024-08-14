import os
import sys
from PIL import Image

def is_image_opaque(image):
    # 检查所有像素的透明度
    for x in range(image.width):
        for y in range(image.height):
            if image.getpixel((x, y))[3] != 255:
                # 如果找到透明度不是255的像素，图片包含透明部分
                return False
    # 所有像素的透明度都是255，图片全为不透明
    return True

def frames_convert(current_dir, filename):
    base_name, file_extension = os.path.splitext(filename)
    image_path = os.path.join(current_dir, filename)
    
    img = Image.open(image_path)
    width, height = img.size
    frame_count = img.n_frames
    new_img = Image.new('RGBA', (width * frame_count, height))
    for i in range(frame_count):
        img.seek(i)
        new_img.paste(img, (i*width, 0))
    if is_image_opaque(new_img):
        output_path = os.path.join(current_dir, base_name + '_frames.bmp')
        new_img.save(output_path, format='BMP')
    else:
        output_path = os.path.join(current_dir, base_name + '_frames.png')
        new_img.save(output_path, format='PNG')
def main(argv):
    if not argv:
        current_dir = os.path.dirname(os.path.abspath(__file__))
    else:
        current_dir = argv[0]
    if current_dir[-1] == '\\' or current_dir[-1] == '/':
        current_dir = current_dir[:-1]
    print(current_dir)
    print('\nfile list:')
    header_content=""
    for filename in os.listdir(current_dir):
        if filename.lower().endswith(('.gif', '.apng', '.webp')):
            frames_convert(current_dir, filename)
            print(filename)

if __name__ == "__main__":
    main(sys.argv[1:])

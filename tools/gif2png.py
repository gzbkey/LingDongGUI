import os
import sys
from PIL import Image

def frames_to_png(image_path, output_path):
    img = Image.open(image_path)
    width, height = img.size
    frame_count = img.n_frames
    new_img = Image.new('RGBA', (width * frame_count, height))
    for i in range(frame_count):
        img.seek(i)
        new_img.paste(img, (i*width, 0))
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
            base_name, file_extension = os.path.splitext(filename)
            image_path = os.path.join(current_dir, filename)
            output_path = os.path.join(current_dir, base_name + '_frames.png')
            frames_to_png(image_path, output_path)

if __name__ == "__main__":
    main(sys.argv[1:])

from PIL import Image
from PIL import ImageDraw
import random

TILE_COUNT = 12
TILE_SIZE = 64

img = Image.new( 'RGB', (TILE_SIZE, TILE_COUNT * TILE_SIZE), "black")
draw = ImageDraw.Draw(img)

for i in range(0, TILE_COUNT):
    draw.rectangle([(0, i * TILE_SIZE), (TILE_SIZE, (i + 1) * TILE_SIZE)], fill=(random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)))

img.show()
img.save('tiles.bmp')
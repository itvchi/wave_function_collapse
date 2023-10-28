from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

TILE_COUNT = 12
TILE_SIZE = 64

img = Image.new( 'RGB', (TILE_SIZE, TILE_COUNT * TILE_SIZE), "black")
draw = ImageDraw.Draw(img)
font=ImageFont.truetype("Ubuntu-R.ttf", int(TILE_SIZE*0.8))

for i in range(0, TILE_COUNT):
    _, _, w, h = draw.textbbox((0, 0), str(i + 1), font=font)
    draw.text(((TILE_SIZE-w)/2, (i * TILE_SIZE) + (TILE_SIZE-h)/2), str(i + 1), font=font)

img.show()
img.save('entropy.bmp')
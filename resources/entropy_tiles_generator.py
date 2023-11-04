from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

TILE_COUNT = 12
ENTROPY = TILE_COUNT + 1
TILE_SIZE = 64

img = Image.new( 'RGB', (TILE_SIZE, ENTROPY * TILE_SIZE), "black")
draw = ImageDraw.Draw(img)
font=ImageFont.truetype("Ubuntu-R.ttf", int(TILE_SIZE*0.8))

for i in range(0, ENTROPY):
    _, _, w, h = draw.textbbox((0, 0), str(i), font=font)
    draw.text(((TILE_SIZE-w)/2, (i * TILE_SIZE) + (TILE_SIZE-h)/2), str(i), font=font)

img.show()
img.save('entropy.bmp')